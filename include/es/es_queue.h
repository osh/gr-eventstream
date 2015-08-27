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

#ifndef EVENTSTREAM_QUEUE_H
#define EVENTSTREAM_QUEUE_H

#include <gnuradio/block.h>
#include <boost/thread/mutex.hpp>
#include <boost/bimap.hpp>
#include <boost/function.hpp>

class es_queue;
typedef boost::shared_ptr<es_queue> es_queue_sptr;


#include <es/es_eh_pair.hh>
#include <es/es_event.h>
#include <es/es_handler.h>
#include <es/es_common.h>

enum es_queue_early_behaviors {
            DISCARD,
            BALK,
            ASAP
};


es_queue_sptr es_make_queue(
    enum es_queue_early_behaviors = DISCARD,
    enum es_search_behaviors sb = SEARCH_BINARY);

class es_eh_queue;

class es_queue {


    public:

        std::vector<gr::basic_block_sptr> d_hvec;
        es_queue(
            enum es_queue_early_behaviors = DISCARD,
            enum es_search_behaviors = SEARCH_BINARY);
        int add_event(pmt_t evt);
        void print_queue(bool already_locked = false);
        int fetch_next_event(unsigned long long min, unsigned long long max, es_eh_pair **eh);
        int fetch_next_event2(unsigned long long min, unsigned long long max, es_eh_pair **eh);

        //int fetch_next_event(unsigned long long min, unsigned long long max, pmt_t &eh);
        //int fetch_next_event2(unsigned long long min, unsigned long long max, pmt_t &eh);


        void bind_handler(std::string type, gr::basic_block_sptr handler);
        void bind_handler(std::string type, es_handler* handler);
        void bind_handler(pmt_t type, gr::basic_block_sptr handler);

        void protect_handler(es_handler_sptr h){ protected_handler.push_back(h); }

        int register_event_type(std::string type);
        int register_event_type(pmt_t type);

        int d_early_behavior;
        uint64_t d_num_asap, d_num_discarded, d_num_events_added, d_num_events_removed;
        uint64_t d_event_time, d_num_soon;
        int length();

        // set behavior when an item exists before the requested region (BALK or ASAP)
        void set_early_behavior(enum es_queue_early_behaviors);

        // set a callback to be called when an eh pair is added
        void set_append_callback( boost::function<bool (es_eh_pair**)> _cb){
            cb_list.push_back(_cb);
            }


        bool empty(){ return event_queue.empty(); }
        uint64_t min_time(){ return empty()?0: ::event_time(event_queue[0]->event); }

    private:
        std::vector<es_eh_pair*> event_queue;
        pmt_t bindings;
        boost::mutex queue_lock;

        std::vector< es_handler_sptr > protected_handler;
        std::vector< boost::function< bool (es_eh_pair**) > > cb_list;

        /**
         * @brief Configuration variable for selecting an insertion sort algorithm.
         */
        es_search_behaviors d_search_behavior;
        int find_index(uint64_t evt_time);
        size_t find_forward(const uint64_t evt_time);
        size_t find_reverse(const uint64_t evt_time);
        size_t find_binary(const uint64_t evt_time);
};




#endif
