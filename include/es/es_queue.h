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


#include <es/es_find_index.hh>
#include <es/es_eh_pair.hh>
#include <es/es_event.h>
#include <es/es_handler.h>
#include <es/es_common.h>

enum es_queue_early_behaviors {
            DISCARD,
            BALK,
            ASAP
};

template<typename T, typename T2> class index_search_es_queue;

/**
 * @brief Provide an es_queue comparison implemention for index_search_base.
 *
 * This class derives from index_search_base and provides a specialized
 * implementation for the case when T and T2 index_search_base template
 * parameters are of types es_eh_pair* and uint64_t respectively. In other
 * words, the type of the value stored in the sorted vector is es_eh_pair* and
 * the type of the value to be compared is uint64_t. Since the types are not
 * equal the index_search_direct implementation cannot be used.
 */
template<>
class index_search_es_queue<es_eh_pair*, uint64_t>:
    public index_search_base<
        index_search_es_queue<es_eh_pair*, uint64_t>, es_eh_pair*, uint64_t>
{
  public:
  typedef es_eh_pair* pair_ptr;
  /**
   * @brief Compare vval and cval.
   *
   * @param [in] vval Reference to a vector value.
   *
   * @param [in] cval Reference to a comparison value.
   *
   * @return True if cval > vval->time(), else false.
   */
  static bool compare_static(const pair_ptr& vval, const uint64_t& cval)
  {
    return cval > vval->time();
  }

  /**
   * @brief Constructor.
   *
   * @param [in] vec Reference to sorted vector.
   */
  index_search_es_queue(std::vector<pair_ptr>& vec):
    index_search_base<
        index_search_es_queue<pair_ptr, uint64_t>,
        pair_ptr,
        uint64_t>(vec)
  {};
};

es_queue_sptr es_make_queue(
    enum es_queue_early_behaviors = DISCARD,
    enum es_search_styles ss = SEARCH_FORWARD);

class es_eh_queue;

class es_queue {


    public:

        std::vector<gr::basic_block_sptr> d_hvec;
        es_queue(
            enum es_queue_early_behaviors = DISCARD,
            enum es_search_styles = SEARCH_FORWARD);
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


        es_search_styles d_search_style;
        int find_index(uint64_t evt_time);

        index_search_es_queue<es_eh_pair*, uint64_t> d_idx_srch;
};




#endif
