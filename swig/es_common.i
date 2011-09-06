


pmt_t es_make_arbiter();

class es {
    public:

        // global tuple type for es events
        static pmt_t type_es_event;

        // event type values within tuple
//        static pmt_t event_vector_created;

        // common hash keys
        static pmt_t event_type;
        static pmt_t event_time;
        static pmt_t event_length;
        static pmt_t event_buffer;

        // common event types
        static pmt_t event_type_1;
        static pmt_t event_type_gen_vector_f;
        static pmt_t event_type_gen_vector_c;
        static pmt_t event_type_gen_vector_b;

};



bool is_event( pmt_t event );
void event_print( pmt_t event );
pmt_t event_create( pmt_t es_event_type, unsigned long long time, unsigned long long max_len );
pmt_t event_create( std::string es_event_type, unsigned long long time, unsigned long long max_len );
pmt_t event_field( pmt_t event, pmt_t field );

std::string event_type( pmt_t event );
bool event_type_compare( pmt_t event, pmt_t evt_type );
uint64_t event_time( pmt_t event );
uint64_t event_length( pmt_t event );


pmt_t event_args_add( pmt_t evt, pmt_t arg_key, pmt_t arg_val );

pmt_t eh_pair_event( pmt_t eh_pair );
pmt_t eh_pair_handler( pmt_t eh_pair );

void register_buffer( pmt_t event, gr_vector_void_star buf );


