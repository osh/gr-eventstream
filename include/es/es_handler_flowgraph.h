
#ifndef EVENTSTREAM_HANDLER_FLOWGRAPH_H
#define EVENTSTREAM_HANDLER_FLOWGRAPH_H

#include <gruel/pmt.h>
#include <gr_block.h>



using namespace pmt;

#include <es/es_handler.h>

class es_handler_flowgraph : public es_handler {
    public:
        enum DATATYPE {   
                      TYPE_F32,
                      TYPE_C32
            };

        es_handler_flowgraph( DATATYPE type, gr_block_sptr _hb );   
        void handler( pmt_t msg, void* buf );
        DATATYPE d_type;

        gr_top_block_sptr       tb;
        gr_block_sptr           hb;
        gr_block_sptr           src;
};


#endif


