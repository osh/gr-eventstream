
#include <stdio.h>
#include <es/es.h>

pmt_t pmt_float_vector( std::vector< float > vec ){
    return pmt_init_f32vector(vec.size(), &vec[0]);
}

pmt_t pmt_complex_vector( std::vector< gr_complex > vec ){
    return pmt_init_c32vector(vec.size(), &vec[0]);
}

gruel::msg_accepter_sptr make_ma_sptr( es_handler* h) {
    gruel::msg_accepter_sptr ptr1( (gruel::msg_accepter*) h);
    return ptr1;
}

pmt_t make_handler_pmt( es_handler* h ){
    return pmt_make_msg_accepter(make_ma_sptr(h));
    }

void es_handler::post(pmt_t msg){
    handler( msg, get_buffer_ptr( event_field( msg, es::event_buffer ) ) );
}

es_handler::~es_handler(){
//    printf("es_handler destructor running (%x).\n",this);
}

//void* es_handler::get_buffer_ptr(pmt_t buffer_arg){
gr_vector_void_star es_handler::get_buffer_ptr(pmt_t buffer_arg){
    printf("(es_handler::get_buffer_ptr() ... getting buffer .\n");

//    assert(pmt_is_list(buffer_arg));

    int nvec = pmt_length(buffer_arg);
    gr_vector_void_star outvec(nvec);
    for(int i=0; i<nvec; i++){
        pmt_t list_nth = pmt_nth(i,buffer_arg);
        if(pmt_is_any(list_nth)){
            boost::any ptr = pmt_any_ref( list_nth );
            outvec[i] = boost::any_cast<void*>(ptr);
        } else if(pmt_is_u8vector( list_nth ) ) {
            size_t buf_len = 0; // this is set by return, input val not used?
            const uint8_t* dataptr = pmt_u8vector_elements(list_nth, buf_len);
            outvec[i] = (void*) dataptr;
        } else {
            printf("unknown pmt type in event buffer!!\n");
            assert(0);
        }
//        boost::any ptr = pmt_any_ref(pmt_nth(i, buffer_arg));
//        outvec[i] = boost::any_cast<void*>(ptr);
    }
    return outvec;

    //boost::any ptr = pmt_any_ref(buffer_arg);
    //return boost::any_cast<void*>(ptr);
    //return boost::any_cast<gr_vector_void_star>(ptr);
}

//void es_handler::handler(pmt_t msg, void* buf){
void es_handler::handler(pmt_t msg, gr_vector_void_star buf){
    printf("es_handler::%x\n",this);
}   




