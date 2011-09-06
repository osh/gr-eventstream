

#ifndef EVENTSTREAM_QUEUE_H
#define EVENTSTREAM_QUEUE_H

#include <gr_block.h>
#include <boost/thread/mutex.hpp>
#include <boost/bimap.hpp>


class es_queue;
typedef boost::shared_ptr<es_queue> es_queue_sptr;

es_queue_sptr es_make_queue();

#include <es/es_eh_pair.hh>
#include <es/es_event.h>
#include <es/es_handler.h>

class es_eh_queue;

class es_queue {
    
    public:
        es_queue();
        int add_event(pmt_t evt);
        void print();
        int fetch_next_event(unsigned long long min, unsigned long long max, es_eh_pair **eh);
        int fetch_next_event2(unsigned long long min, unsigned long long max, es_eh_pair **eh);

        //int fetch_next_event(unsigned long long min, unsigned long long max, pmt_t &eh);
        //int fetch_next_event2(unsigned long long min, unsigned long long max, pmt_t &eh);

              
        void bind_handler(std::string type, pmt_t handler);
        void bind_handler(pmt_t type, pmt_t handler);

        int register_event_type(std::string type);
        int register_event_type(pmt_t type);

    private:
        //std::vector<pmt_t> event_queue;
        std::vector<es_eh_pair*> event_queue;
        std::map< std::string, std::vector<pmt_t> > bindings;       
        boost::mutex queue_lock;
};


#endif
