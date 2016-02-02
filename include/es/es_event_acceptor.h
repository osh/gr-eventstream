#ifndef ES_EVENT_ACCEPTOR_H
#define ES_EVENT_ACCEPTOR_H

#include <gnuradio/sync_block.h>
#include <es/es_queue.h>

class es_event_acceptor : public virtual gr::sync_block {
    public:
        es_queue_group d_group;
        es_queue_sptr event_queue;
        uint64_t d_n_scheduled;
        uint64_t d_n_secondary;

        es_event_acceptor(enum es_queue_early_behaviors eb= DISCARD,
                          enum es_search_behaviors sb = SEARCH_BINARY,
                          std::string tgroup = "") :
            d_group(tgroup,eb,sb)
            {
                // set up queue pointer
                event_queue = d_group.queue();
                d_n_scheduled = d_n_secondary = 0;

                // listen for events
                message_port_register_in(pmt::mp("schedule_event"));
                set_msg_handler(pmt::mp("schedule_event"),
                    boost::bind(&es_event_acceptor::schedule_event, this, _1));
            }

        // handle pmt msgs in...
        void schedule_event(pmt::pmt_t m);

        // register a new event handler based on a recieved registration message
        void add_handlers(pmt::pmt_t h);
        uint64_t nscheduled(){return d_n_scheduled;}
        uint64_t nsecondary(){return d_n_secondary;}
};


#endif
