
#ifndef EVENTSTREAM_HANDLER_PRINT_H
#define EVENTSTREAM_HANDLER_PRINT_H

#include <gruel/pmt.h>
#include <gr_block.h>



using namespace pmt;

#include <es/es_handler.h>

class es_handler_print : public es_handler {
    public:
        enum DATATYPE {   
                      TYPE_F32,
                      TYPE_C32
            };

        es_handler_print( DATATYPE type );   
        //void handler( pmt_t msg, void* buf );
        void handler( pmt_t msg, gr_vector_void_star buf );
        DATATYPE d_type;
};


#endif


