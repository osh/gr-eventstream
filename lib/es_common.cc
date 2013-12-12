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

#include <es/es_common.h>
#include <gnuradio/io_signature.h>

using namespace pmt;

#include <stdio.h>
#include <gnuradio/basic_block.h>
#include <boost/format.hpp>

pmt_t es_make_arbiter(){
    
    pmt_t p = pmt::make_dict();
       
    return p;


}

// top level tuple type for es_events
pmt_t es::type_es_event( pmt::intern("type_es_event") );

// common dict keys
pmt_t es::event_type( pmt::intern("es::event_type") );
pmt_t es::event_time( pmt::intern("es::event_time") );
pmt_t es::event_length( pmt::intern("es::event_length") );
pmt_t es::event_buffer( pmt::intern("es::event_buffer") );

// common es_event_type vals, can be expanded elsewhere in add on modules
pmt_t es::event_type_1( pmt::intern("es::event_type_1") );
pmt_t es::event_type_gen_vector_f( pmt::intern("es::event_type_gen_vector_f") );
pmt_t es::event_type_gen_vector_c( pmt::intern("es::event_type_gen_vector_c") );
pmt_t es::event_type_gen_vector_b( pmt::intern("es::event_type_gen_vector_b") );
pmt_t es::event_type_gen_vector( pmt::intern("es::event_type_gen_vector") );

pmt_t event_create( std::string event_type, unsigned long long time, unsigned long long length ){
    return event_create( pmt::intern( event_type ), time, length );
}

pmt_t event_create( pmt_t event_type, unsigned long long time, unsigned long long length ){
    pmt_t hash = pmt::make_dict();
    hash = pmt::dict_add( hash, es::event_type, event_type );
    hash = pmt::dict_add( hash, es::event_time, pmt::from_uint64(time) );
    hash = pmt::dict_add( hash, es::event_length, pmt::from_uint64(length) );
    
    return pmt::make_tuple( es::type_es_event, hash );
}

pmt_t event_args_add( pmt_t evt, pmt_t arg_key, pmt_t arg_val ){
    pmt_t msg_head = pmt::tuple_ref(evt, 0);
    pmt_t msg_hash = pmt::tuple_ref(evt, 1);
    msg_hash = pmt::dict_add( msg_hash, arg_key, arg_val );
    return pmt::make_tuple( msg_head, msg_hash );
}

pmt_t event_type_pmt( pmt_t event ){
    assert(is_event( event ) );
    pmt_t msg_hash = pmt::tuple_ref(event, 1);
    return pmt::dict_ref( msg_hash, es::event_type, PMT_NIL );
}

std::string event_type( pmt_t event ){
    assert(is_event( event ) );
    pmt_t msg_hash = pmt::tuple_ref(event, 1);
    return pmt::symbol_to_string( pmt::dict_ref( msg_hash, es::event_type, PMT_NIL ) );
}

bool event_type_compare( pmt_t event, pmt_t evt_type ){
    pmt_t type1 = event_field( event, es::event_type );
    pmt_t type2 = evt_type;
    return pmt::eqv( type1, type2 );
}

void event_print( pmt_t event ){
    assert(is_event(event));
    pmt_t msg_hash = pmt::tuple_ref(event, 1);
    printf(" *************************************************\n");
    printf(" * PRINTING EVENTSTREAM EVENT.\n");

    pmt_t keys = pmt::dict_keys( msg_hash );
    printf(" * hash size = %d\n", (int)pmt::length(keys) );
    for(int i=0; i<pmt::length(keys); i++){
        pmt_t key = pmt::nth(i, keys );
        pmt_t val = pmt::dict_ref( msg_hash, key, PMT_NIL );
        printf(" * %s  \t=> %s\n", pmt::symbol_to_string(key).c_str(), pmt::write_string(val).c_str());
    }
    printf(" *************************************************\n");
}

bool is_event( pmt_t event ){
    if(!pmt::is_tuple(event))
        return false;
//    assert( pmt::is_tuple( event) );

    // get the tuple values out
    pmt_t msg_type = pmt::tuple_ref(event, 0);
    pmt_t msg_hash = pmt::tuple_ref(event, 1);

    // make sure the first tuple val identifies us as an es_event
    if(!pmt::eq(msg_type, es::type_es_event) )
        return false;
//    assert(pmt::eq( msg_type, es::type_es_event) );
    return true;
}

bool event_has_field( pmt_t event, pmt_t field){
    pmt_t msg_hash = pmt::tuple_ref(event, 1);
    return pmt::dict_has_key( msg_hash, field );
}

pmt_t event_field( pmt_t event, pmt_t field ){
    assert(is_event(event));
    pmt_t msg_hash = pmt::tuple_ref(event, 1);
    pmt_t val = pmt::dict_ref( msg_hash, field, PMT_NIL );
//    printf("event_field(%s) got val. <-- %s\n", pmt::write_string(field).c_str(), pmt::write_string(val).c_str() );
    if( pmt::eq( PMT_NIL, val ) ){
//        printf("event_field(%s) got val. <-- %s\n", pmt::write_string(field).c_str(), pmt::write_string(val).c_str() );
        event_print(event);
    }
    if( pmt::eq(PMT_NIL, val) ){
        throw std::runtime_error((boost::format("could not find event field %s on event %s\n")%pmt::write_string(val).c_str()%pmt::write_string(event).c_str()).str());
        }
    assert( !pmt::eq( PMT_NIL, val ) );
    return val;
}

uint64_t event_time( pmt_t event ){
    return pmt::to_uint64(event_field( event, es::event_time ));
}

uint64_t event_length( pmt_t event ){
    return pmt::to_uint64(event_field( event, es::event_length ));
}

pmt_t eh_pair_event( pmt_t eh_pair ){
    return pmt::tuple_ref(eh_pair, 0);
}

pmt_t eh_pair_handler( pmt_t eh_pair ){
    return pmt::tuple_ref(eh_pair, 1);
}


pmt_t register_buffer( pmt_t event, pmt_t bufs ){
    assert(is_event(event));
    return event_args_add( event, es::event_buffer, bufs );
}

pmt_t register_buffer( pmt_t event, gr_vector_void_star buf, gr_vector_int &sig ){
    assert(is_event(event));
    pmt_t msg_hash = pmt::tuple_ref(event, 1);
    pmt_t buflist = pmt::PMT_NIL;

    for(int i=0; i<buf.size(); i++){
        buflist = pmt::list_add(buflist, pmt::init_u8vector( sig[0], (const uint8_t*) buf[i]) );
    }

    msg_hash = pmt::dict_add( msg_hash, es::event_buffer, buflist );
    return pmt::make_tuple( pmt::tuple_ref(event,0), msg_hash );
}

//pmt_t register_buffer( pmt_t event, gr_vector_const_void_star buf, gr_vector_int sig ){
//    gr_vector_void_star* a = (gr_vector_void_star*) &buf;
//    return register_buffer( event, *a, sig);
//}

gr::io_signature::sptr es_make_io_signature( int min, const std::vector<int> &sizes ){
    if(sizes.size() == 0){
        return gr::io_signature::make(0,0,0);
    } else {
        return gr::io_signature::makev(min, sizes.size(), sizes);
    }
}

std::vector<unsigned char> string_to_vector(std::string s){
    std::vector<unsigned char> vec(s.size());
    for(int i=0; i<s.size(); i++){
        vec[i] = s[i];
    }
    return vec;
}


