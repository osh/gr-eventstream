#ifndef INCLUDED_EVENTSTREAM_TRIGGER_H
#define INCLUDED_EVENTSTREAM_TRIGGER_H

#include <gr_sync_block.h>
#include <gruel/pmt.h>
#include <es/es_queue.h>
#include <string>

class es_trigger;
using namespace pmt;

typedef boost::shared_ptr<es_trigger> es_trigger_sptr;

es_trigger_sptr es_make_trigger (pmt_t arb, es_queue_sptr queue, int itemsize, std::string type);

class es_trigger : public gr_sync_block
{
private:
  friend es_trigger_sptr es_make_trigger (pmt_t arb, es_queue_sptr queue, int itemsize, std::string type);

 protected:
  es_trigger (pmt_t arb, es_queue_sptr queue, int itemsize, std::string type);  	// private constructor


 public:
  ~es_trigger ();	// public destructor

  virtual int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);
    
  std::vector<pmt_t> event_types;
  pmt_t event_type(int idx);
  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_time;

  // event sizing parameters around the trigger moment
  unsigned int d_length;
  unsigned int d_lookback;
};

#endif /* INCLUDED_EVENTSTREAM_SQUARE2_FF_H */
