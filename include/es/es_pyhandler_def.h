#ifndef ES_PYHANDLER_DEF
#define ES_PYHANDLER_DEF

#include <gr_top_block.h>

// return values for factory function execution
// returns a descriptor for the flow graph
// contains a top block to run(), as well as 
// a list of sources, and sinks to be accessed
struct es_pydef {
    gr_top_block_sptr tb;
    std::vector< es_vector_source > sources;
    std::vector< gr_block_sptr > sinks;
};

// return values from hook execution
// returns a map of str=>str
struct es_rval {
    std::map<std::string, std::string> map;
};


es_pydef* default_factory_cb( int index, void* _cb_data );

typedef es_pydef* (*FACTORYCBTYPE)(int index, void* _cb_data);

es_rval* default_hook_cb( gr_vector_void_star bufs, pmt_t msg );

typedef es_rval* (*HOOKCBTYPE)( gr_vector_void_star bufs, pmt_t msg );


class es_pyhandler_def {

    public:

        es_pyhandler_def();
        
        
        FACTORYCBTYPE factory_callback;
        void* factory_cb_data;
        
        
        HOOKCBTYPE pre_hook_cb;
        void* pre_hook_data;


        HOOKCBTYPE post_hook_cb;
        void* post_hook_data;

        
        void set_factory_cb(FACTORYCBTYPE fp, void* _cb_data);
        void set_pre_hook_cb(HOOKCBTYPE fp, void* _cb_data);
        void set_post_hook_cb(HOOKCBTYPE fp, void* _cb_data);
        
};


#endif
