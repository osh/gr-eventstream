


#include <es/es.h>
#include <es/es_handler_flowgraph.h>
#include <stdio.h>

#include <gr_message_source.h>
#include <gr_vector_source_c.h>
#include <gr_top_block.h>

es_handler_flowgraph::es_handler_flowgraph( DATATYPE type, gr_block_sptr _hb ){
    d_type = type;

    tb = gr_make_top_block("autotb");
    hb = _hb;
}

void es_handler_flowgraph::handler( pmt_t msg, void* buf ){
    
    int itemsize = 0;
    switch(d_type){
        case TYPE_F32:
            {
            itemsize = sizeof(float);
            break;
            }
        case TYPE_C32:
            {
            break;
            itemsize = sizeof(gr_complex);
            }
        default:
            printf("unknown vector content type.\n");
            assert(0);
        }
    
    int num_items = event_length(msg);
    int num_bytes = num_items * itemsize;

    src = gr_make_vector_source_c( std::vector<gr_complex>((gr_complex*)buf, ((gr_complex*)buf)+num_items) );
    printf("connecting flowgraph.\n");
    tb->connect( src, 0, hb, 0 );
    tb->run();
    printf("fg finished.\n");
       
}




