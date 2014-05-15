/* -*- c++ -*- */
/*
 * Copyright 2011 Free Software Foundation, Inc.
 * 
 * This file is part of gr-eventstream
 * 
 * gr-eventstream is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * gr-eventstream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gr-eventstream; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <stdio.h>
#include <es/es_common.h>
#include <es/es_source_thread.hh>


/*
 * Constructor function, sets up parameters
 */
//es_source_thread::es_source_thread(pmt_t _arb, es_queue_sptr _queue, boost::lockfree::queue<es_eh_pair*> *_qq, boost::lockfree::queue<unsigned long long> *_dq, boost::condition *_qq_cond) :
es_source_thread::es_source_thread(pmt_t _arb, es_queue_sptr _queue, boost::lockfree::queue<es_eh_pair*> *_qq, boost::mutex *_lin_mut, std::vector<pmt_t> *_readylist, boost::condition *_qq_cond, gr_vector_int _out_sig) :
    arb(_arb),
    queue(_queue),
    qq(_qq),
    qq_cond(_qq_cond),
    readylist(_readylist),
    lin_mut(_lin_mut),
    finished(false),
    out_sig(_out_sig) // TODO: update out_sig when connections are updated ??
{
    start();
}

/*
 * kick off a thread running the do_work() main loop function
 */
void es_source_thread::start(){
    d_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&es_source_thread::do_work, this)));
}

/* 
 * Shut down all the running threads and join them.
 *   Called by es_sink destructor
 */
void es_source_thread::stop(){
    finished = true;
    qq_cond->notify_all();
    d_thread->join();
}


/*
 *  Main event loop thread work function,
 *    constantly receives and services event/handler pairs
 *    then queues the live_times for deletion.
 */
void es_source_thread::do_work(){

    // used by boost::condition, has no scope outside of this thread, unneccisary
    boost::mutex access;
    boost::mutex::scoped_lock lock(access);

    // run the thread until we are told to shut down
    while(!finished){
        es_eh_pair* eh = NULL;

        // wait for a signal to start looking in the queue (when we run out of events)
        qq_cond->wait(lock);

        // get events to handle as long as they are available
        while( (*qq).pop(eh) ){

            // if BB entered, we have a new eh pair to process ... 
            
            // TODO: replace this segment with pmt_mgr managed pmt_blobs!!
            //          round up to next 2^n size for better pool size hits
            // allocate some buffers (this should be pooled soon)
            int n_items = event_length(eh->event);


            pmt_t buf_list;
            for(int i=0 ; i<out_sig.size(); i++){
                int itemsize = out_sig[i];

//                printf("allocating buffer idx = %d, itemsize = %d, n_items = %d\n", i, itemsize, n_items);
                
                if(zerobuf.size() < itemsize*n_items){
                    zerobuf.resize(itemsize*n_items);
                }
                pmt_t buf = pmt::make_blob(&zerobuf[0],itemsize*n_items);

                if(i==0){
                    buf_list = pmt::list1( buf );
                } else {
                    buf_list = pmt::list_add( buf_list, buf );
                }

            }
    
            // assign buffers to the event for output
            pmt_t event = register_buffer( eh->event, buf_list);
            eh->event = event;

            // run the event/handler pair
            eh->run();

            // grab the mutex over the linear list 
            lin_mut->lock();
            //printf("got lock\n");

            // add buffer into re time ordered list of events
            readylist->push_back( eh->event );
                // source2::work() can not grab the earliest event off this list and memcpy away

            // release mutex lock
            lin_mut->unlock();
            //printf("released lock\n");

            // disabled for now, we need to worry about cleaning up leaks later ...
            // delete the pair, (the event should be ref counted underneath and uneffected)
            //delete eh;

        }
    }
}





