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

es_sink_sptr es_make_sink (std::vector<int> insig, int n_threads, int sample_history_in_kilosamples=64, enum es_queue_early_behaviors eb = DISCARD, enum es_search_behaviors sb = SEARCH_BINARY, std::string tgroup = "");

class es_sink : public gr::sync_block
{
  es_sink (std::vector<int> insig, int n_threads, int sample_history_in_kilosamples=64, enum es_queue_early_behaviors eb = DISCARD, enum es_search_behaviors sb = SEARCH_BINARY, std::string tgroup = "");   // private constructor

  es_queue_sptr event_queue;
  unsigned long long d_time;
  unsigned int d_history;

public:
  void wait_events();
};
