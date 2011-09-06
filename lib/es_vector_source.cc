#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <es/es_vector_source.hh>
#include <gr_io_signature.h>

#include <stdio.h>

es_vector_source_sptr
es_make_vector_source (const gr_vector_int &io_def )
{
    return es_vector_source_sptr (new es_vector_source (io_def ));
}

es_vector_source::es_vector_source ( const gr_vector_int &io_def  ) 
    : gr_sync_block ("vector_source",
        gr_make_io_signature(0, 0, 0 ),
        gr_make_io_signaturev(1, io_def.size(), io_def )),
        d_io_def(io_def),
        d_data( gr_vector_const_void_star(io_def.size()) )
{
    printf("io_def.size() = %d\n", io_def.size() );
    for(int i=0; i<io_def.size(); i++){
        printf("io_def[%d] = %d\n", i, io_def[i] );
    }
    clear();
}

es_vector_source::~es_vector_source () 
{
    // d_data is owned externally
}


int es_vector_source::work (int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
        int num_coppied = std::min( (uint64_t)noutput_items, items_remain());
        printf("num_coppied = %llu\n", num_coppied);
        if(num_coppied > 0){
            for(int i=0; i<output_items.size(); i++){
                assert(d_data[i] != NULL);
                memcpy(output_items[i], d_data[i] + (d_io_def[i])*d_item_offset, num_coppied*d_io_def[i]);
            }
        }
        return num_coppied;
}

void es_vector_source::clear(){
    d_nitems = 0;
    for(int i=0; i<d_io_def.size();i++){ 
        d_data[i] = (const void*) NULL;
    }
    rewind();
}

void es_vector_source::rewind(){
    d_item_offset = 0;
}

/*void es_vector_source::set_data(gr_vector_const_void_star &data, int n_items){
    d_data = data;
    d_nitems = n_items;
    rewind();
}*/

void es_vector_source::set_data(std::vector<char*> data, int n_items){
    printf("SET_DATA CALLED WITH STR!\n");
    //d_data = data;
    //d_nitems = n_items;
    //rewind();
}

gr_vector_const_void_star es_vector_source::data(){
    return d_data;
}

