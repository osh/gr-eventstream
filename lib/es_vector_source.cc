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
#include <es/es_vector_source.hh>
#include <gr_io_signature.h>

es_vector_source_sptr
es_make_vector_source (const gr_vector_int &io_def )
{
    return es_vector_source_sptr (new es_vector_source (io_def ));
}

es_vector_source::es_vector_source ( const gr_vector_int &io_def  ) 
    : gr_sync_block ("vector_source",
        gr_make_io_signature(0, 0, 0 ),
        es_make_io_signature(0, io_def)),
        d_io_def(io_def),
        d_data( gr_vector_const_void_star(io_def.size()) ),
        d_dealloc(false)
{
    clear();
}

es_vector_source::~es_vector_source () 
{
    clear();
}


int es_vector_source::work (int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
        int num_coppied = std::min( (uint64_t)noutput_items, items_remain());
        if(num_coppied > 0){
            for(int i=0; i<output_items.size(); i++){
                assert(d_data[i] != NULL);
                memcpy(output_items[i], (void*)((uint64_t)d_data[i] + (d_io_def[i])*d_item_offset), num_coppied*d_io_def[i]);
            }
        }
	d_item_offset += num_coppied;

    if((num_coppied==0) && (items_remain() == 0)){ return -1; }
    return num_coppied;
}

void es_vector_source::clear(){
    d_nitems = 0;
    for(int i=0; i<d_io_def.size();i++){ 
        d_data[i] = (const void*) NULL;
        if(d_dealloc){
            delete d_data[i];
        }
    }
    rewind();
    d_dealloc = false;
}

void es_vector_source::rewind(){
    d_item_offset = 0;
}

void es_vector_source::set_data(gr_vector_const_void_star &data, int n_items, bool dealloc){
    clear();
    d_data = data;
    d_nitems = n_items;
    d_dealloc = dealloc;
    rewind();
}

void es_vector_source::set_data(std::vector<std::string> data, int n_items){
    gr_vector_const_void_star d2(data.size());
    for(int i=0; i<data.size(); i++){ 
	void* copy = malloc(n_items * d_io_def[i]);
	memcpy(copy, data[i].c_str(), n_items*d_io_def[i]);
	d2[i] = (const void*) copy;
	}
    set_data(d2, n_items, true);
}

gr_vector_const_void_star es_vector_source::data(){
    return d_data;
}

