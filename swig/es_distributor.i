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

GR_SWIG_BLOCK_MAGIC(es,distributor);

%include "std_string.i"
%include "std_vector.i"

es_distributor_sptr es_make_distributor (
  std::vector<int> iosig, int num_sinks, bool separate_registration = false);

class es_distributor: public gr::sync_block
{
  es_distributor (
    gr_vector_int iosig, int num_sinks, bool separate_registration = false);   // private constructor

 public:
  ~es_distributor();
  int work(
    int noutput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items
    );
  void dist_msg_random(pmt_t msg);
  void dist_msg_all(pmt_t msg);
  uint64_t nevents_registered();
  uint64_t nevents_distributed();
};
