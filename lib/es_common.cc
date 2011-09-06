
#include <es/es_common.h>

using namespace pmt;

#include <stdio.h>
#include <gr_basic_block.h>

pmt_t es_make_arbiter(){
    
    pmt_t p = pmt_make_dict();
       
    return p;


}

// top level tuple type for es_events
pmt_t es::type_es_event( pmt_intern("type_es_event") );

// common dict keys
pmt_t es::event_type( pmt_intern("es::event_type") );
pmt_t es::event_time( pmt_intern("es::event_time") );
pmt_t es::event_length( pmt_intern("es::event_length") );
pmt_t es::event_buffer( pmt_intern("es::event_buffer") );

// common es_event_type vals, can be expanded elsewhere in add on modules
pmt_t es::event_type_1( pmt_intern("es::event_type_1") );
pmt_t es::event_type_gen_vector_f( pmt_intern("es::event_type_gen_vector_f") );
pmt_t es::event_type_gen_vector_c( pmt_intern("es::event_type_gen_vector_c") );
pmt_t es::event_type_gen_vector_b( pmt_intern("es::event_type_gen_vector_b") );
pmt_t es::event_type_gen_vector( pmt_intern("es::event_type_gen_vector") );

pmt_t event_create( std::string event_type, unsigned long long time, unsigned long long length ){
    return event_create( pmt_intern( event_type ), time, length );
}

pmt_t event_create( pmt_t event_type, unsigned long long time, unsigned long long length ){
    pmt_t hash = pmt_make_dict();
    hash = pmt_dict_add( hash, es::event_type, event_type );
    hash = pmt_dict_add( hash, es::event_time, pmt_from_uint64(time) );
    hash = pmt_dict_add( hash, es::event_length, pmt_from_uint64(length) );
    
    return pmt_make_tuple( es::type_es_event, hash );
}

pmt_t event_args_add( pmt_t evt, pmt_t arg_key, pmt_t arg_val ){
    pmt_t msg_head = pmt_tuple_ref(evt, 0);
    pmt_t msg_hash = pmt_tuple_ref(evt, 1);
    msg_hash = pmt_dict_add( msg_hash, arg_key, arg_val );
    return pmt_make_tuple( msg_head, msg_hash );
}

std::string event_type( pmt_t event ){
    assert(is_event( event ) );
    pmt_t msg_hash = pmt_tuple_ref(event, 1);
    return pmt_symbol_to_string( pmt_dict_ref( msg_hash, es::event_type, PMT_NIL ) );
}

bool event_type_compare( pmt_t event, pmt_t evt_type ){
    pmt_t type1 = event_field( event, es::event_type );
    pmt_t type2 = evt_type;
    return pmt_eqv( type1, type2 );
}

void event_print( pmt_t event ){
    assert(is_event(event));
    pmt_t msg_hash = pmt_tuple_ref(event, 1);
    printf(" *************************************************\n");
    printf(" * PRINTING EVENTSTREAM EVENT.\n");

    pmt_t keys = pmt_dict_keys( msg_hash );
    printf(" * hash size = %d\n", (int)pmt_length(keys) );
    for(int i=0; i<pmt_length(keys); i++){
        pmt_t key = pmt_nth(i, keys );
        pmt_t val = pmt_dict_ref( msg_hash, key, PMT_NIL );
        printf(" * %s  \t=> %s\n", pmt_symbol_to_string(key).c_str(), pmt_write_string(val).c_str());
    }
    printf(" *************************************************\n");
}

bool is_event( pmt_t event ){
    assert( pmt_is_tuple( event) );

    // get the tuple values out
    pmt_t msg_type = pmt_tuple_ref(event, 0);
    pmt_t msg_hash = pmt_tuple_ref(event, 1);

    // make sure the first tuple val identifies us as an es_event
    assert(pmt_eq( msg_type, es::type_es_event) );
    return true;
}

pmt_t event_field( pmt_t event, pmt_t field ){
    assert(is_event(event));
    pmt_t msg_hash = pmt_tuple_ref(event, 1);
    pmt_t val = pmt_dict_ref( msg_hash, field, PMT_NIL );
//    printf("event_field(%s) got val. <-- %s\n", pmt_write_string(field).c_str(), pmt_write_string(val).c_str() );
    if( pmt_eq( PMT_NIL, val ) ){
        printf("event_field(%s) got val. <-- %s\n", pmt_write_string(field).c_str(), pmt_write_string(val).c_str() );
        event_print(event);
    }
    assert( !pmt_eq( PMT_NIL, val ) );
    return val;
}

uint64_t event_time( pmt_t event ){
    return pmt_to_uint64(event_field( event, es::event_time ));
}

uint64_t event_length( pmt_t event ){
    return pmt_to_uint64(event_field( event, es::event_length ));
}

pmt_t eh_pair_event( pmt_t eh_pair ){
    return pmt_tuple_ref(eh_pair, 0);
}

pmt_t eh_pair_handler( pmt_t eh_pair ){
    return pmt_tuple_ref(eh_pair, 1);
}


pmt_t register_buffer( pmt_t event, pmt_t bufs ){
    assert(is_event(event));
    return event_args_add( event, es::event_buffer, bufs );
}

pmt_t register_buffer( pmt_t event, gr_vector_void_star buf ){
    assert(is_event(event));
    pmt_t msg_hash = pmt_tuple_ref(event, 1);

    pmt_t buf0 = pmt_make_any( boost::any( buf[0] ) );
    pmt_t buflist = pmt_list1( buf0 );
    for(int i=1; i<buf.size(); i++){
        buflist = pmt_list_add(buflist, pmt_make_any( boost::any( buf[i] ) ) );
    }

//    boost::any v1(buf);
//    msg_hash = pmt_dict_add( msg_hash, es::event_buffer, pmt_make_any( v1 ) );
    msg_hash = pmt_dict_add( msg_hash, es::event_buffer, buflist );

    printf("register_buffer returning.\n");
    return pmt_make_tuple( pmt_tuple_ref(event,0), msg_hash );
}

pmt_t register_buffer( pmt_t event, gr_vector_const_void_star buf ){
    gr_vector_void_star* a = (gr_vector_void_star*) &buf;
    return register_buffer( event, *a );
}

