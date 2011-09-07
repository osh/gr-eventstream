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
/* -*- c++ -*- */

%include "gnuradio.i"
%include "typemaps.i"
%include "std_string.i"
%include "std_vector.i"

namespace std {
   %template(StrVector) vector<std::string>;
};

%{
#include <es/es_vector_source.hh>
%}


// ----------------------------------------------------------------
GR_SWIG_BLOCK_MAGIC(es,vector_source);
es_vector_source_sptr es_make_vector_source ( const std::vector<int> &io_def );
class es_vector_source : public gr_sync_block
{
private:
  es_vector_source(const std::vector<int> &io_def );
public:
/*  void set_data(std::vector<const void*>  &data, int n_items, bool dealloc);  */
  void set_data(std::vector<std::string>  &data, int n_items);
  void rewind();
  void clear();
  gr_vector_const_void_star data();
};
// ----------------------------------------------------------------


