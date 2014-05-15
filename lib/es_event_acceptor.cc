#include <es/es_event_acceptor.h>
#include <gnuradio/block_registry.h>
#include <stdio.h>

void es_event_acceptor::schedule_event(pmt::pmt_t m){
    //printf("schedule_event.\n");
//    printf("es_event_acceptor::schedule_event() called with .,.. \n");
//    pmt::print(m);
    if(pmt::is_pair(m) && (pmt::eqv(pmt::car(m), pmt::mp("ES_REGISTER_HANDLER")))){
        // if this is a registration message, register the handlers
        add_handlers(pmt::cdr(m));
        } else {
        // otherwise assume it is an event we are scheduling
        if(is_event(m)){
            event_queue->add_event(m);
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

