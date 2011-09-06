


#include <es/es.h>
//#include <es/es_handler_print.h>
#include <stdio.h>

es_handler_print::es_handler_print( DATATYPE type ){
    d_type = type;
}

//void es_handler_print::handler( pmt_t msg, void* buf ){
void es_handler_print::handler( pmt_t msg, gr_vector_void_star buf ){
    event_print(msg);
    switch(d_type){
        case TYPE_F32:
            {
            printf(" vector_contents = [");
            float* fbuf = (float*) buf[0];
            for(int i=0; i<event_length(msg); i++){
                printf("%f, ", fbuf[i]);
                }
            printf("]\n");
            break;
            }
        case TYPE_C32:
            {
            printf(" vector_contents = [");
            float* fbuf = (float*) buf[0];
            for(int i=0; i<event_length(msg); i++){
                printf("%f, ", fbuf[i]);
                }
            printf("]\n");
            break;
            }
        default:
            printf("unknown vector content type.\n");
        }

}




