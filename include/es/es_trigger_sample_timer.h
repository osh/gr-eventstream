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
#ifndef INCLUDED_EVENTSTREAM_TRIGGER_SAMPLE_TIMER_H
#define INCLUDED_EVENTSTREAM_TRIGGER_SAMPLE_TIMER_H

#include <gnuradio/sync_block.h>
#include <pmt/pmt.h>
#include <es/es_queue.h>
#include <es/es_trigger.h>

using namespace pmt;

class es_trigger_sample_timer;

typedef boost::shared_ptr<es_trigger_sample_timer> es_trigger_sample_timer_sptr;

es_trigger_sample_timer_sptr es_make_trigger_sample_timer ( int itemsize, int period, int shift, int sched_dist, int event_length);

class es_trigger_sample_timer : public es_trigger
{
private:
  friend es_trigger_sample_timer_sptr es_make_trigger_sample_timer (int itemsize, int period, int shift, int sched_dist, int event_length);

  es_trigger_sample_timer (int itemsize, int period, int shift, int sched_dist, int event_length);  

 public:
  ~es_trigger_sample_timer ();	// public destructor
  int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);

  int d_period;
  int d_itemsize;
  int d_shift;
  int d_sched_dist;
  pmt_t d_evt_type;
  int d_evt_len;
  uint64_t d_evt_time;
  uint64_t d_time;
  bool d_enabled;

};

#endif /* INCLUDED_EVENTSTREAM_SQUARE2_FF_H */
