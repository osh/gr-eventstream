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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <es/es_common.h>
#include <es/es_vector_sink.hh>
#include <gnuradio/io_signature.h>

es_vector_sink_sptr
es_make_vector_sink (const gr_vector_int &io_def )
{
    return es_vector_sink_sptr (new es_vector_sink (io_def ));
}

es_vector_sink::es_vector_sink ( const gr_vector_int &io_def  ) 
    : gr::sync_block ("vector_sink",
        es_make_io_signature(0, io_def ),
        gr::io_signature::make(0, 0, 0 )),
        d_io_def(io_def)
{
    for(int i=0; i<io_def.size(); i++){
        d_data.push_back( std::vector<char>() );
    }
    clear();
}

es_vector_sink::~es_vector_sink () 
{
}


int es_vector_sink::work (int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
        if(d_nitems + noutput_items > (d_data[0].size()/d_io_def[0]) ){
            resize(d_nitems + noutput_items);
        }
        for(int i=0; i<input_items.size(); i++){
            memcpy(&d_data[i][d_nitems*d_io_def[i]], input_items[i], noutput_items*d_io_def[i]);
        }
        d_nitems += noutput_items;
        return noutput_items;
}

void es_vector_sink::clear(){
    for(int i=0; i<d_io_def.size(); i++){
        d_data[i].clear();
    }
    d_nitems = 0;
}

void es_vector_sink::resize(uint64_t n_items){
    assert( n_items >= d_nitems );
    for(int i=0; i<d_io_def.size(); i++){
        d_data[i].resize( d_io_def[i] * n_items );
    }
}

gr_vector_const_void_star es_vector_sink::data(){
    gr_vector_const_void_star pvec(d_io_def.size());
    for(int i=0; i<d_io_def.size(); i++){
        pvec[i] = (const void*) &d_data[i][0];
    }
    return pvec;
}

int es_vector_sink::data_len_items(){
    return d_nitems;
}


std::vector<std::string> es_vector_sink::strvec(){
    std::vector<std::string> strvec;
    for(int i=0; i<d_data.size(); i++){
        strvec.push_back( std::string( (const char*) &d_data[i][0], (size_t) d_data[i].size() ) );
    }
    return strvec;
}

