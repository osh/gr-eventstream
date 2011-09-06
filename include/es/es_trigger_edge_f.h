#ifndef INCLUDED_EVENTSTREAM_TRIGGER_RISINGEDGE_F_H
#define INCLUDED_EVENTSTREAM_TRIGGER_RISINGEDGE_F_H

#include <gr_sync_block.h>
#include <gruel/pmt.h>
#include <es/es_queue.h>
#include <es/es_trigger.h>

class es_trigger_edge_f;
using namespace pmt;

typedef boost::shared_ptr<es_trigger_edge_f> es_trigger_edge_f_sptr;

es_trigger_edge_f_sptr es_make_trigger_edge_f (pmt_t arb, es_queue_sptr queue, float thresh, int length, int lookback);

class es_trigger_edge_f : public es_trigger
{
private:
  friend es_trigger_edge_f_sptr es_make_trigger_edge_f (pmt_t arb, es_queue_sptr queue, float thresh, int length, int lookback);

  es_trigger_edge_f (pmt_t arb, es_queue_sptr queue, float thresh, int length, int lookback);  	// private constructor


 public:
  ~es_trigger_edge_f ();	// public destructor
  int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);

  float d_thresh;
  float d_lastval;
};

#endif /* INCLUDED_EVENTSTREAM_SQUARE2_FF_H */
