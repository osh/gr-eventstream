#include <es/es_event_acceptor.h>
#include <gnuradio/block_registry.h>

void es_event_acceptor::schedule_event(pmt::pmt_t m){
//    printf("es_event_acceptor::schedule_event() called with .,.. \n");
//    pmt::print(m);
    if(pmt::is_pair(m) && (pmt::eqv(pmt::car(m), pmt::mp("ES_REGISTER_HANDLER")))){
        // if this is a registration message, register the handlers
        add_handlers(pmt::cdr(m));
        } else {
        // otherwise assume it is an event we are scheduling
        event_queue->add_event(m);
        }
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

