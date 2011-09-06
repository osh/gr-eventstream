


pmt_t pmt_float_vector( std::vector< float > vec );
pmt_t pmt_complex_vector( std::vector< gr_complex > vec );

pmt_t event_create_gen_vector_f ( unsigned long long time, pmt_t float_list );
pmt_t event_create_gen_vector_c ( unsigned long long time, pmt_t cpx_list );

class es_handler_insert_vector : public es_handler {
    public:
        es_handler_insert_vector();
        void handler( pmt_t msg, gr_vector_void_star buf );
};

class es_handler_print : public es_handler {
    public:
        enum DATATYPE {
                      TYPE_F32,
                      TYPE_C32
            };

        es_handler_print( DATATYPE type );
        void handler( pmt_t msg, gr_vector_void_star buf );
        DATATYPE d_type;
};

class es_handler_flowgraph : public es_handler {
    public:
        enum DATATYPE {
                      TYPE_F32,
                      TYPE_C32
            };

        es_handler_flowgraph( DATATYPE type, gr_block_sptr _hb );
        void handler( pmt_t msg, void* buf );
        DATATYPE d_type;
};






