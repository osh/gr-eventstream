/* -*- c++ -*- */

%include "gnuradio.i"
%include "typemaps.i"

%{
#include <es/es_vector_source.hh>
%}


// ----------------------------------------------------------------
GR_SWIG_BLOCK_MAGIC(es,vector_source);
es_vector_source_sptr es_make_vector_source ( const std::vector<int> &io_def );
class es_vector_source : public gr_sync_block
{
private:
  es_vector_source(const std::vector<int> &io_def );
public:
/*  void set_data(std::vector<const void*>  &data, int n_items); */
  void set_data(std::vector<char*>  &data, int n_items);
  void rewind();
  void clear();
  gr_vector_const_void_star data();
};
// ----------------------------------------------------------------


