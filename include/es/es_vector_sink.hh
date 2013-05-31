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
#ifndef ES_VECTOR_SINK_H
#define ES_VECTOR_SINK_H

#include <gnuradio/block.h>
#include <gnuradio/sync_block.h>

class es_vector_sink;

typedef boost::shared_ptr<es_vector_sink> es_vector_sink_sptr;

es_vector_sink_sptr es_make_vector_sink( const gr_vector_int &io_def );

class es_vector_sink : public gr::sync_block
{
private:
    friend es_vector_sink_sptr es_make_vector_sink (const gr_vector_int &io_def );
    es_vector_sink ( const gr_vector_int &io_def  );

    uint64_t d_nitems;
    gr_vector_int d_io_def;
    std::vector< std::vector< char > > d_data;
   
public:
    ~es_vector_sink ();  
    int work (int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items);
    void resize(uint64_t n_items);
    void clear();
    std::vector<std::string> strvec();
    gr_vector_const_void_star data();
    int data_len_items();
    gr_vector_int shape(){ return d_io_def; }
   
};

#endif 

