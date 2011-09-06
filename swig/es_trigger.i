
GR_SWIG_BLOCK_MAGIC(es,trigger);

es_trigger_sptr es_make_trigger (pmt_t arb, es_queue_sptr queue, int itemsize, std::string name);

class es_trigger : public gr_sync_block
{
 public:
  std::vector<pmt_t> event_types;
  pmt_t event_type(int idx);
  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_time;

  // event sizing parameters around the trigger moment
  unsigned int d_length;
  unsigned int d_lookback;
};

