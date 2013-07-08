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

enum es_queue_early_behaviors {
            DISCARD,
            BALK,
            ASAP
};

es_queue_sptr es_make_queue(enum es_queue_early_behaviors = DISCARD);

class es_eh_queue;

class es_queue {
 
   
    public:


        es_queue(enum es_queue_early_behaviors = DISCARD);
        int add_event(pmt_t evt);
        void print_queue(bool already_locked = false);
        int fetch_next_event(unsigned long long min, unsigned long long max, es_eh_pair **eh);
        int fetch_next_event2(unsigned long long min, unsigned long long max, es_eh_pair **eh);

        //int fetch_next_event(unsigned long long min, unsigned long long max, pmt_t &eh);
        //int fetch_next_event2(unsigned long long min, unsigned long long max, pmt_t &eh);

              
        void bind_handler(std::string type, gr::basic_block_sptr handler);
        void bind_handler(pmt_t type, gr::basic_block_sptr handler);

        void protect_handler(es_handler_sptr h){ protected_handler.push_back(h); }

        int register_event_type(std::string type);
        int register_event_type(pmt_t type);

        int d_early_behavior;
        int length();

        // set behavior when an item exists before the requested region (BALK or ASAP)
        void set_early_behavior(enum es_queue_early_behaviors);

        // set a callback to be called when an eh pair is added
        void set_append_callback( boost::function<bool (es_eh_pair**)> _cb){
            cb_list.push_back(_cb);
            }

    private:
        std::vector<es_eh_pair*> event_queue;
        pmt_t bindings;
        boost::mutex queue_lock;

        std::vector< es_handler_sptr > protected_handler;
        std::vector< boost::function< bool (es_eh_pair**) > > cb_list;

};


#endif
