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

GR_SWIG_BLOCK_MAGIC(es,source2);
%include "std_string.i"
%include "std_vector.i"



es_source2_sptr es_make_source2 (pmt_t arb, es_queue_sptr queue, std::vector<int> out_sig, int nthreads);

class es_source2 : public gr::sync_block
{
public:
  void set_max(unsigned long long maxlen);
  unsigned long long time();

private:
  es_source2 (pmt_t arb, es_queue_sptr queue, std::vector<int> out_sig, int nthreads);
};
