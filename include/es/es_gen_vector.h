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

#ifndef ES_GEN_VECTOR_F
#define ES_GEN_VECTOR_F

#include <gnuradio/block.h>

pmt_t pmt_float_vector( std::vector< float > );
pmt_t pmt_complex_vector( std::vector< gr_complex > vec );

pmt_t event_create_gen_vector_f ( unsigned long long time, pmt_t float_list );
pmt_t event_create_gen_vector_c ( unsigned long long time, pmt_t cpx_list );
pmt_t event_create_gen_vector_b ( unsigned long long time, pmt_t u8_list, int itemsize );
pmt_t event_create_gen_vector( unsigned long long time, pmt_t vectors, gr::io_signature::sptr _sig );

//pmt_t event_create_gen_vector_b ( unsigned long long time, pmt_t u8_list, int itemsize );


#endif
