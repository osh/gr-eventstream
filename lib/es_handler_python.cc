
#include <es/es.h>
#include <stdio.h>
#include <string.h>


es_pyhandler_def* default_handler(int index, void* clientdata){
    printf("DEFAULT HANDLER CALLED (index = %d)!\n",index);
    es_pyhandler_def* hd = new es_pyhandler_def();
    return hd;
}

es_handler_python::es_handler_python( ){
    printf("new handler px = %x\n", this);
}

//void es_handler_insert_vector::handler( pmt_t msg, void* buf ){
void es_handler_python::handler( pmt_t msg, gr_vector_void_star buf ){


}




