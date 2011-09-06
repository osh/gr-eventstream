#ifndef INCLUDED_EVENTSTREAM_SOURCE_H
#define INCLUDED_EVENTSTREAM_SOURCE_H

#include <gr_sync_block.h>
#include <gruel/pmt.h>
#include <es/es_queue.h>

class es_source;
using namespace pmt;

typedef boost::shared_ptr<es_source> es_source_sptr;

es_source_sptr es_make_source (pmt_t arb, es_queue_sptr queue, gr_io_signature_sptr out_sig);

class es_source : public gr_sync_block
{
private:
  friend es_source_sptr es_make_source (pmt_t arb, es_queue_sptr queue, gr_io_signature_sptr out_sig);

  es_source (pmt_t arb, es_queue_sptr queue, gr_io_signature_sptr out_sig);  	// private constructor

 // TODO: implement internal write-ahead buffer for over-size block writes

 public:
  ~es_source ();	// public destructor

  int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);

  void set_max(unsigned long long maxlen);

  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_maxlen;
  unsigned long long d_time;
  unsigned int d_history;
};

#endif /* INCLUDED_EVENTSTREAM_SQUARE2_FF_H */
