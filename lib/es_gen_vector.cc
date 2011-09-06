
#include <es/es.h>
#include <es/es_gen_vector.h>
#include <gr_io_signature.h>

pmt_t event_create_gen_vector_f ( unsigned long long time, pmt_t float_list ){
    assert(pmt_is_f32vector(float_list));
    int len = pmt_length(float_list);
    pmt_t evt = event_create( es::event_type_gen_vector_f, time, (uint64_t)len);
    return event_args_add( evt, pmt_intern("vector"), float_list );
}

pmt_t event_create_gen_vector_c ( unsigned long long time, pmt_t cpx_list ){
    assert(pmt_is_c32vector(cpx_list));
    int len = pmt_length(cpx_list);
    pmt_t evt = event_create( es::event_type_gen_vector_c, time, (uint64_t)len);
    return event_args_add( evt, pmt_intern("vector"), cpx_list );
}

pmt_t event_create_gen_vector_b ( unsigned long long time, pmt_t u8_list, int itemsize ){
    assert(pmt_is_u8vector(u8_list));
    int len = pmt_length(u8_list)/itemsize;
    pmt_t evt = event_create( es::event_type_gen_vector_b, time, (uint64_t)len);
    return event_args_add( evt, pmt_intern("vector"), u8_list );
}


pmt_t event_create_gen_vector( unsigned long long time, pmt_t vectors, gr_io_signature_sptr _sig ){
    
    pmt_t buf0 = pmt_nth( 0, vectors );
    assert(pmt_is_u8vector(buf0));

    gr_io_signature* sig = &(*_sig);
    int len = pmt_length(buf0)/((_sig)->sizeof_stream_item(0));
    
    pmt_t evt = event_create( es::event_type_gen_vector, time, (uint64_t)len);
    //return event_args_add( evt, pmt_intern("vectors"), vectors );
    return event_args_add( evt, pmt_intern("vector"), vectors );
    
}

