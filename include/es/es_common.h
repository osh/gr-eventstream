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
#ifndef EVENTSTREAM_COMMON_H
#define EVENTSTREAM_COMMON_H

#include <pmt/pmt.h>
#include <gnuradio/basic_block.h>
using namespace pmt;

#define es_make_arb es_make_arbiter
pmt_t es_make_arbiter();

class es {
    public:

        // global tuple type for es events
        static pmt_t type_es_event;

        // event type values within tuple
//        static pmt_t event_vector_created;

        // common hash keys
        static pmt_t event_type;
        static pmt_t event_time;
        static pmt_t event_length;
        static pmt_t event_buffer;

        // common event types
        static pmt_t event_type_1;
        static pmt_t event_type_gen_vector_f;
        static pmt_t event_type_gen_vector_c;
        static pmt_t event_type_gen_vector_b;
        static pmt_t event_type_gen_vector;

};

enum es_search_styles {
    SEARCH_FORWARD,
    SEARCH_REVERSE,
    SEARCH_BINARY
};

bool is_event( pmt_t event );
void event_print( pmt_t event );
pmt_t event_create( pmt_t es_event_type, unsigned long long time, unsigned long long max_len );
pmt_t event_create( std::string es_event_type, unsigned long long time, unsigned long long max_len );
pmt_t event_field( pmt_t event, pmt_t field );
bool event_has_field( pmt_t event, pmt_t field );

pmt_t event_type_pmt( pmt_t event );
std::string event_type( pmt_t event );
bool event_type_compare( pmt_t event, pmt_t evt_type );
uint64_t event_time( pmt_t event );
uint64_t event_length( pmt_t event );

pmt_t event_args_add( pmt_t evt, pmt_t arg_key, pmt_t arg_val );

pmt_t eh_pair_event( pmt_t eh_pair );
pmt_t eh_pair_handler( pmt_t eh_pair );

pmt_t register_buffer( pmt_t event, pmt_t bufs);
pmt_t register_buffer( pmt_t event, gr_vector_void_star buf, gr_vector_int &sig);
//pmt_t register_buffer( pmt_t event, gr_vector_const_void_star buf, gr_vector_int &sig);


gr::io_signature::sptr es_make_io_signature( int min, const std::vector<int> &sizes );

std::vector<unsigned char> string_to_vector(std::string);

#endif
