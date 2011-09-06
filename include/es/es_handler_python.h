
#ifndef EVENTSTREAM_HANDLER_PYTHON_H
#define EVENTSTREAM_HANDLER_PYTHON_H

#include <gruel/pmt.h>
#include <gr_block.h>

using namespace pmt;

#include <es/es_handler.h>

class es_handler_python : public es_handler {
    public:
        es_handler_insert_vector();   
        void handler( pmt_t msg, gr_vector_void_star buf );
};


#endif

