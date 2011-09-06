#ifndef ES_VECTOR_SOURCE_H
#define ES_VECTOR_SOURCE_H

#include <gr_block.h>
#include <gr_sync_block.h>

class es_vector_source;

typedef boost::shared_ptr<es_vector_source> es_vector_source_sptr;

es_vector_source_sptr es_make_vector_source( const gr_vector_int &io_def );

class es_vector_source : public gr_sync_block
{
private:
    friend es_vector_source_sptr es_make_vector_source (const gr_vector_int &io_def );
    es_vector_source ( const gr_vector_int &io_def  );

    gr_vector_int d_io_def;
    gr_vector_const_void_star d_data;
    uint64_t d_nitems;
    uint64_t d_item_offset;
    uint64_t items_remain(){ return d_nitems - d_item_offset; }
    
public:
    ~es_vector_source ();  
    int work (int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items);

//    void set_data(gr_vector_const_void_star &data, int n_items);
    void set_data(std::vector<char*> data, int n_items);
    void rewind();
    void clear();
    gr_vector_const_void_star data();

};

#endif 

