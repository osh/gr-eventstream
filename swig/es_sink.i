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
GR_SWIG_BLOCK_MAGIC(es,sink);

%include "std_string.i"
%include "std_vector.i"
#%include "gr_top_block.i"

%template(gr_top_block_sptr) boost::shared_ptr<gr_top_block>;

namespace std {
   %template(IntVector) vector<int>;
};


es_sink_sptr es_make_sink (pmt_t arb, es_queue_sptr queue, std::vector<int> insig, int n_threads);

class es_sink : public gr_sync_block
{
  es_sink (pmt_t arb, es_queue_sptr queue, std::vector<int> insig, int n_threads);   // private constructor

  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_time;
  unsigned int d_history;

public:
  void wait_events();
};



