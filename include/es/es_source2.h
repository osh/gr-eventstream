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
#ifndef INCLUDED_EVENTSTREAM_SOURCE2_H
#define INCLUDED_EVENTSTREAM_SOURCE2_H

#include <gnuradio/sync_block.h>
#include <pmt/pmt.h>
#include <es/es_queue.h>
#include <es/es_source_thread.hh>
#include <functional>
#include <boost/function.hpp>
#include <boost/lockfree/queue.hpp>


class es_source2;
using namespace pmt;

typedef boost::shared_ptr<es_source2> es_source2_sptr;

es_source2_sptr es_make_source2 (pmt_t arb, es_queue_sptr queue, gr_vector_int out_sig);

class es_source2 : public gr::sync_block
{
private:
  friend es_source2_sptr es_make_source2 (pmt_t arb, es_queue_sptr queue, gr_vector_int out_sig);

  es_source2 (pmt_t arb, es_queue_sptr queue, gr_vector_int out_sig);  	// private constructor

 // TODO: implement internal write-ahead buffer for over-size block writes

 public:
  ~es_source2 ();	// public destructor

  int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);

  void set_max(unsigned long long maxlen);

  boost::condition qq_cond;

  boost::lockfree::queue<es_eh_pair*> qq;        // work items to start
  boost::lockfree::queue<unsigned long long> dq; // finished time indexes

  boost::mutex lin_mut;
  std::vector<pmt_t> readylist;

  std::vector<boost::shared_ptr<es_source_thread> > threadpool;
//  std::vector<unsigned long long> live_event_times;

  //bool cb(es_eh_pair** eh){ return false; }
  bool cb(es_eh_pair** eh);
 
  int n_threads;   

  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_maxlen;
  unsigned long long d_time;
  unsigned int d_history;

  unsigned long long time();
};

#endif /* INCLUDED_EVENTSTREAM_SQUARE2_FF_H */
