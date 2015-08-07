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

GR_SWIG_BLOCK_MAGIC(es,trigger_edge_f);


es_trigger_edge_f_sptr es_make_trigger_edge_f (float thresh, int length, int lookback, int itemsize, int guard);

class es_trigger_edge_f : public es_trigger
{
private:
  es_trigger_edge_f (float thresh, int length, int lookback, int itemsize, int guard);   // private constructor

 public:
  void set_thresh(float thresh);

  float d_thresh;
  float d_lastval;
};




