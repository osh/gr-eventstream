#include <es/es_event_acceptor.h>
#include <es/es_trigger.h>
#include <gnuradio/block_registry.h>
#include <stdio.h>

#define DEBUG_ES_SINK 0

void es_event_acceptor::schedule_event(pmt::pmt_t m){

    if(DEBUG_ES_SINK)
        std::cout << "es_sink: msg in " << pmt::write_string(m) << std::endl;

    // if we are a secondary on a thread group don't add anything ...
    if(d_group.enabled() && !d_group.primary())
    {
        d_n_secondary++;
        return;
    }

    if(pmt::is_pair(m) && (pmt::eqv(pmt::car(m), pmt::mp("ES_REGISTER_HANDLER")))){
        // if this is a registration message, register the handlers
        add_handlers(pmt::cdr(m));
    } else if(pmt::is_pair(m) && (pmt::eqv(pmt::car(m), pmt::mp("ES_OBTAIN_QUEUE")))){
        // set up a direct shared pointer to the event_queue in the trigger block
        //  this way we dont have non-deterministic behavior of messages in flight not in the queue yet
        es_trigger* t = boost::any_cast<es_trigger*>( pmt::any_ref(pmt::cdr(m)) );
        t->event_queue = event_queue;
        } else {

        // otherwise assume it is an event we are scheduling
        if(is_event(m)){
            event_queue->add_event(m);
            d_n_scheduled++;
        } else {
            // perform secondary check to see if it is a PDU we can work with

            if(pmt::is_pair(m) &&
                (pmt::is_null(pmt::car(m)) || pmt::is_dict(pmt::car(m))) ){
                    // we have a PDUish thing

                    pmt::pmt_t etype = pmt::dict_ref(pmt::car(m),pmt::mp("event_type"), pmt::mp("pdu_event"));
                    uint64_t time = pmt::to_uint64(pmt::dict_ref(pmt::car(m),pmt::mp("event_time"), pmt::from_uint64(0UL)));
                    uint64_t len;
                    size_t itemsize;
                    pmt::pmt_t buf_list;

                    if(pmt::is_uniform_vector(pmt::cdr(m))){    // often a source event
                        len = pmt::length(pmt::cdr(m));         // get vector length
                        itemsize = pmt::uniform_vector_itemsize(pmt::cdr(m));   // vector itemsize
                        // create the event and register the vector with it
                        pmt::pmt_t buf = pmt::make_blob( pmt::blob_data(pmt::cdr(m)) , len*itemsize );
                        buf_list = pmt::list_add(pmt::PMT_NIL, buf);
                    } else if(pmt::is_null(pmt::cdr(m))) {      // often a sink event
                        // this must be a sink event (metadata pdu with null vector)
                        len = pmt::to_uint64(pmt::dict_ref(pmt::car(m),pmt::mp("event_length"), pmt::from_uint64(0UL)));
                    } else {
                        printf("es_event_acceptor received an almost-pdu! discarding!\n");
                        return;
                    }

                    // create the event
                    pmt::pmt_t evt = event_create( etype, time, len );

                    //register buffer if we have one
                    if(buf_list.get())
                        evt = event_args_add(evt, pmt::mp("vector"), buf_list);

                    // copy any other keys in by default
                    pmt::pmt_t keys = pmt::dict_keys(pmt::car(m));
                    while(pmt::is_pair(keys)){
                        // skip known keys
                        if(pmt::eqv(keys,pmt::mp("event_time")) || pmt::eqv(keys,pmt::mp("event_length")) || pmt::eqv(keys,pmt::mp("event_type")))
                            continue;
                        evt = event_args_add(evt, pmt::car(keys), pmt::dict_ref(pmt::car(m),pmt::car(keys),pmt::PMT_NIL));
                        keys = pmt::cdr(keys);
                        }

                    // add to the queue
                    event_queue->add_event(evt);
                    d_n_scheduled++;
                } else {
                    printf("es_event_acceptor received non event! discarding!\n");
                }
            }
        }
    //printf("schedule_event done.\n");
    }

// register a new event handler based on a recieved registration message
void es_event_acceptor::add_handlers(pmt::pmt_t h){
//    printf("adding handlers ... \n");
    pmt::pmt_t event_type = pmt::car(h);
//    std::cout << "event_type: " << event_type << "\n";

    event_queue->register_event_type(event_type);

    // work through handler list
    pmt::pmt_t targets = pmt::cdr(h);
    while( !pmt::eqv(targets, pmt::PMT_NIL) ){
        // extract target pmts
        pmt::pmt_t block = pmt::car(pmt::car(targets));
        pmt::pmt_t port =  pmt::cdr(pmt::car(targets));
//        std::cout << "target = " << block << ":" << port << "\n";

        // bind the handler
        event_queue->bind_handler(event_type, global_block_registry.block_lookup(block));

        // iterate down target list
        targets = pmt::cdr(targets);
        }

    }
