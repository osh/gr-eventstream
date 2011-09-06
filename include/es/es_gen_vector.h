#ifndef ES_GEN_VECTOR_F
#define ES_GEN_VECTOR_F

#include <gr_block.h>

pmt_t pmt_float_vector( std::vector< float > );
pmt_t pmt_complex_vector( std::vector< gr_complex > vec );

pmt_t event_create_gen_vector_f ( unsigned long long time, pmt_t float_list );
pmt_t event_create_gen_vector_c ( unsigned long long time, pmt_t cpx_list );
pmt_t event_create_gen_vector_b ( unsigned long long time, pmt_t u8_list, int itemsize );
pmt_t event_create_gen_vector( unsigned long long time, pmt_t vectors, gr_io_signature_sptr _sig );

//pmt_t event_create_gen_vector_b ( unsigned long long time, pmt_t u8_list, int itemsize );


#endif
