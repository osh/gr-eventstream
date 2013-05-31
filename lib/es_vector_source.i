/* -*- c++ -*- */

%include "gnuradio.i"
%include "typemaps.i"

%{
#include "es_vector_source.hh"
%}

// ----------------------------------------------------------------
GR_SWIG_BLOCK_MAGIC(es,vector_source);
es_vector_source_sptr es_make_vector_source (int );
class es_vector_source : public gr::sync_block
{
private:
  es_vector_source(int );
public:
  
};
// ----------------------------------------------------------------


