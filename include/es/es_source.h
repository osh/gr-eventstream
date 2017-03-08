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
#ifndef INCLUDED_EVENTSTREAM_SOURCE_H
#define INCLUDED_EVENTSTREAM_SOURCE_H

#include <gnuradio/sync_block.h>
#include <pmt/pmt.h>
#include <es/es_queue.h>
#include <es/es_source_thread.hh>
#include <es/es_event_acceptor.h>
#include <functional>
#include <boost/function.hpp>
#include <boost/lockfree/queue.hpp>

enum es_source_merge_behavior {
  MEMCPY,
  ADD_INT8S,
  ADD_INT16S,
  ADD_FLOATS,
};

class es_source;
using namespace pmt;

typedef boost::shared_ptr<es_source> es_source_sptr;

es_source_sptr es_make_source (gr_vector_int out_sig, int nthreads=1, enum es_queue_early_behaviors = DISCARD, enum es_source_merge_behavior = MEMCPY);

class es_source : public virtual gr::sync_block, public virtual es_event_acceptor
{
private:
  friend es_source_sptr es_make_source (gr_vector_int out_sig, int nthreads, enum es_queue_early_behaviors, enum es_source_merge_behavior);

  es_source (gr_vector_int out_sig, int nthreads=1, enum es_queue_early_behaviors = DISCARD, enum es_source_merge_behavior = MEMCPY);  	// private constructor

  es_handler_sptr ih;

 public:
  ~es_source ();	// public destructor

  int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);

  es_source_merge_behavior d_merge_mode;

  void set_max(unsigned long long maxlen);

  boost::condition qq_cond;

  boost::lockfree::queue<es_eh_pair*> qq;        // work items to start
  boost::lockfree::queue<unsigned long long> dq; // finished time indexes

  boost::mutex lin_mut;
  std::vector<pmt_t> readylist;

  std::vector<boost::shared_ptr<es_source_thread> > threadpool;

  bool cb(es_eh_pair** eh);

  int n_threads;

  unsigned long long d_maxlen;
  unsigned long long d_time;
  unsigned int d_history;

  unsigned long long time();
};

#endif /* INCLUDED_EVENTSTREAM_SQUARE_FF_H */
