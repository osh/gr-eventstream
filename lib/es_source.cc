/* -*- c++ -*- */
/*
 * Copyright 2017 Tim O'Shea 
 * 
 * This file is part of gr-eventstream 
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <es/es_source.h>
#include <es/es_common.h>
#include <es/es_queue.h>
#include <es/es.h>
#include <es/es_handler_insert_vector.h>
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
#include <stdio.h>
#include <string.h>

#define __STDC_FORMAT_MACROS
//#define DEBUG(x) x
#define DEBUG(x)

/*
 * Create a new instance of es_source and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
es_source_sptr 
es_make_source (gr_vector_int out_sig, int nthreads, enum es_queue_early_behaviors eb, enum es_source_merge_behavior mb)
{
  return es_source_sptr (new es_source (out_sig, nthreads, eb, mb));
}

/*
 * Specify constraints on number of input and output streams.
 * This info is used to construct the input and output signatures
 * (2nd & 3rd args to gr::block's constructor).  The input and
 * output signatures are used by the runtime system to
 * check that a valid number and type of inputs and outputs
 * are connected to this block.  In this case, we accept
 * only 1 input and 1 output.
 */
static const int MIN_IN = 0;	// mininum number of input streams
static const int MAX_IN = 0;	// maximum number of input streams

unsigned long long es_source::time(){
    return d_time;
}

/*
 * The private constructor
 */
es_source::es_source (gr_vector_int out_sig, int nthreads, enum es_queue_early_behaviors eb, enum es_source_merge_behavior mb)
  : gr::sync_block ("es_source",
        gr::io_signature::make (MIN_IN, MAX_IN, 0),
        es_make_io_signature (out_sig.size(), out_sig) ),
    d_merge_mode(mb),
    d_maxlen(ULLONG_MAX),
    d_time(0),
    n_threads(nthreads), // poke this through as a constructor arg
    qq(100), dq(100),
    es_event_acceptor(eb)
{
    // create and dispatch handler threads
    for(int i=0; i<n_threads; i++){
        boost::shared_ptr<es_source_thread> th( new es_source_thread(pmt::PMT_NIL, event_queue, &qq, &lin_mut, &readylist, &qq_cond, out_sig) );
        threadpool.push_back( th );
    }

    // bind event_queue->append() callback function
    boost::function< bool(es_eh_pair**) > f;
    f = std::bind1st(std::mem_fun(&es_source::cb), this);
    event_queue->set_append_callback( f );

    // bind event handler for standard inserters
    event_queue->register_event_type(pmt::mp("pdu_event"));
    ih = es_make_handler_insert_vector();
    event_queue->bind_handler( pmt::mp("pdu_event"), ih->to_basic_block() );

    // message port that tracks the production rate
    // for upstream schedulers
    message_port_register_out(pmt::mp("nproduced"));

    // message port that outputs events with timing info once they are scheduled
    message_port_register_out(pmt::mp("events_scheduled"));
}


// callback bound to the es_queue add_item routine (executed for each new eh pair)
bool es_source::cb(es_eh_pair** eh){
    
    DEBUG(printf("es_source::cb() executing.\n");)

    // pass eh pair to lockfree fifos (out to threads)
    es_eh_pair * tp = *eh;
    qq.push(tp);
    qq_cond.notify_one(); // notify one of the sleeping threads (if any)
    
    // we dont need this event anymore in the main queue
    return false;
}


// set a maximum number of items to produce (otherwise we will run forever and never mark finished)
void es_source::set_max(unsigned long long maxlen){
    d_maxlen = maxlen;
}

/*
 * Our virtual destructor.
 */
es_source::~es_source ()
{
    //shutdown threads
    for(int i=0; i<n_threads; i++){
        threadpool[i]->stop();
    }
}

int 
es_source::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
  char *out = (char *) output_items[0];
  DEBUG(printf("entered work.\n");)
  DEBUG(printf("d_time = %llu, noutput_items = %d\n", d_time, noutput_items);  )
  

  unsigned long long max_time = d_time + noutput_items;
  unsigned long long min_time = d_time;

  // zero the output buffers
  for(int j=0; j<output_items.size();j++){
      int itemsize = d_output_signature->sizeof_stream_item(j);
      memset(output_items[j], 0x00, noutput_items*itemsize);
  }
  
  // acquire the readylist lock
  lin_mut.lock();

  // grab serialized buffers from thread output
  //        copy buffers into work output buffer
  std::vector<pmt_t>::iterator it;
  for(int i = 0; i<readylist.size(); i++){
    DEBUG(printf("readylist[%d] = %p\n", i, readylist[i].get());)
    pmt_t evt = readylist[i];
    
    uint64_t e_time = event_time(evt);
    uint64_t e_length = event_length(evt);

    if(e_time >= d_time + noutput_items){ // event starts after our current buffer area save for later
        break; // if event is in the future do nothing with it (since they are time ordered - we are done here)
    } else { // event starts in our buffer, or in the past

        // remove the event from the queue we are handling it
        readylist.erase(readylist.begin()+i);
        i--;

        // make sure event time is greater than right now 
        if(e_time < d_time){ // if event starts in the past handle the behavior appropriately
            switch(event_queue->d_early_behavior){
                case DISCARD:
                    // discard this event
                    continue; //goto next queued event
                case BALK:
                    // throw an error
                    throw std::runtime_error((boost::format("source event arrived at the source work function late (evt=%lu, time=%llu)!")%e_time%d_time).str());
                case ASAP:
                    // update event time to be as soon as possible
                    evt = event_args_add(evt, pmt::intern("es::event_time") , pmt::from_uint64(d_time));
                    e_time = event_time(evt);
                    break;
                case RNDF:
                    // update event time to be a near future time
                    evt = event_args_add(evt, pmt::intern("es::event_time") , pmt::from_uint64(d_time+rand()%noutput_items));
                    e_time = event_time(evt);
                    break;
                default:
                    std::cout <<  event_queue->d_early_behavior << "\n";
                    throw std::runtime_error("unknown value for event_queue->d_early_behavior");
            }
        }

        // if we reach this point, we will be generating output from this event
        // compute portion of event to output
        int space_avail = d_time + noutput_items - e_time;
        int item_copy = (space_avail >= e_length ? e_length : space_avail); //TODO: this may cause issues for large events

        // compute copy offsets
        int output_offset = 0;
        int input_offset = 0;
        if(e_time >= d_time){
            // event starts at non zero offset in buffer
            output_offset = e_time - d_time;
            DEBUG(printf("output_offset = %d\n", output_offset);)
        } else {
            // event starts in the past (copy only the end region)
            input_offset = d_time - e_time;
            DEBUG(printf("input_offset = %d (e_time = %lu, dtime = %llu)\n", input_offset, e_time, d_time);)
        }

        if( !event_has_field( (evt), es::event_buffer ) ){
            perror("malformed event");
            }

        // buf_list is a pmt_list of pmt_blobs containing buffers for N output ports
        pmt_t buf_list = event_field( (evt), es::event_buffer ); 

        // sanity checking
        if( (!(item_copy <= input_offset+noutput_items)) || (!(input_offset >= 0)) ){
            perror("insane offsets\n");
        }

        // copy to output buffer (iterate over number of output ports)
        for(int j=0; j<output_items.size();j++){
            pmt_t buf = pmt::nth(j, buf_list);           

            // get reference to buffer stored in the event
            const char* ii = (const char*) pmt::blob_data(buf);

            // get reference to the output buffer
            char* oo = (char*) output_items[j];
            int itemsize = d_output_signature->sizeof_stream_item(j);

            // copy our event contents into the output buffer ...
            //   - this now supports several merge modes
            switch(d_merge_mode){
                case MEMCPY: // raw memcpy mode (no overlap merge)
                    memcpy( &oo[output_offset*itemsize], &ii[input_offset*itemsize], item_copy*itemsize );
                    DEBUG(printf("memcpy returned\n");)
                    break;
                case ADD_INT8S: // int8 additive merge mode
                    {
                    int nitem = item_copy*itemsize / sizeof(int8_t);
                    int8_t* out = (int8_t*) &oo[output_offset*itemsize];
                    int8_t* in  = (int8_t*) &ii[input_offset*itemsize];
                    for(int kk=0; kk<nitem; kk++){ out[kk] += in[kk]; }
                    }
                    break;
                case ADD_INT16S: // int16 additive merge mode
                    {
                    int nitem = item_copy*itemsize / sizeof(int16_t);
                    int16_t* out = (int16_t*) &oo[output_offset*itemsize];
                    int16_t* in  = (int16_t*) &ii[input_offset*itemsize];
                    for(int kk=0; kk<nitem; kk++){ out[kk] += in[kk]; }
                    }
                    break;
                case ADD_FLOATS: // float additive merge mode
                    {
                    int nitem = item_copy*itemsize / sizeof(float);
                    float* out = (float*) &oo[output_offset*itemsize];
                    float* in  = (float*) &ii[input_offset*itemsize];
                    for(int kk=0; kk<nitem; kk++){ out[kk] += in[kk]; }
                    }
                    break;
                default:
                    throw std::runtime_error("bad value for d_merge_mode");
            } 
        }

        // copy event dict to scheduled output message port incase someone else cares ¯\_(ツ)_/¯ 
        if(pmt::length(message_subscribers(pmt::mp("events_scheduled")))){
            pmt::pmt_t dict = pmt::tuple_ref(evt,1);
            if(! pmt::eqv(pmt::dict_ref(dict, pmt::mp("es::event_type"), PMT_NIL), pmt::mp("CONTINUATION"))){
                dict = pmt::dict_delete( dict, pmt::intern("es::event_buffer") );
                message_port_pub(pmt::mp("events_scheduled"), dict );
                }
            }

        // if we have leftovers to store (from previous work executions)
        if(e_length > item_copy){

            DEBUG(printf("generating continuation event for next time.\n");)

            // generate a new event to represent the remaining contents which have not yet been output
            pmt_t evt_c = event_create( "CONTINUATION", e_time + item_copy, e_length - item_copy );
            pmt_t outbuf_list = pmt::PMT_NIL;

            // populate the event with remaining buffer contents
            // create a pmt_list with pointers to existing buffers
            // no new mallocing should happen here
            for(int j=0; j<output_items.size();j++){ //iterate over number of output ports
                int itemsize = d_output_signature->sizeof_stream_item(j);

                // get a reference to our blob of interest in the blob list
                pmt_t buf = pmt::nth(j, buf_list);           
                const char* base_srcptr = (const char*) pmt::blob_data(buf);

                // make a new blob pointing to a portion of the old blob
                pmt_t newblob = pmt::make_blob( base_srcptr + itemsize * item_copy, itemsize*(event_length(evt)-item_copy));
                outbuf_list = pmt::list_add( outbuf_list, newblob );
            }
 
            // tag the new buffers onto the event
            DEBUG(printf("register buffer.\n");)
            evt_c = register_buffer( evt_c, outbuf_list );

            // the original buffers are saved to make sure we reserve the pmt_blobs allocation!           
            static const pmt_t ORIG_FLAG(pmt::intern("SAVE_ORIG_BUFS"));
            if(event_has_field(evt, ORIG_FLAG)){
                evt_c = event_args_add( evt_c, ORIG_FLAG, event_field( evt, ORIG_FLAG ) );
            } else {
                evt_c = event_args_add( evt_c, ORIG_FLAG, event_field( evt, es::event_buffer ) );
            }
 
            DEBUG(printf("inserting into readylist (readylist.size() = %lu).\n",readylist.size());)
            // insert the new event into our readylist
            for(int k=0; k<=readylist.size(); k++){
                if(k == readylist.size()){
                    readylist.insert( readylist.begin()+k, evt_c );
                    break;
                } else if( event_time(evt_c) < event_time(readylist[k]) ){
                    readylist.insert( readylist.begin()+k, evt_c );
                    DEBUG(printf("inserting into %d of readylist.\n",k);)
                    break;
                } else {
                    DEBUG(printf("insert failed!\n");)
                }
                
            } // done inserting
        } // done leftover exists conditional
    } // done time range if()
    } // done outer loop over readylist contents


  lin_mut.unlock();
 
  
  // determine number to be produced
  int produced = (d_maxlen < d_time + noutput_items)?d_maxlen - d_time:noutput_items;
  message_port_pub(pmt::mp("nproduced"), pmt::mp(d_time));
  
  // check finished condition for exit
#ifndef _MSC_VER
  if(__builtin_expect(d_maxlen == d_time, false)){ return -1; } 
#endif 
  d_time += produced; // step time ptr along
  return produced;
}
