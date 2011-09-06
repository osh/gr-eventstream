GR_SWIG_BLOCK_MAGIC(es,sink);


es_sink_sptr es_make_sink (pmt_t arb, es_queue_sptr queue, gr_io_signature_sptr insig, int n_threads);

class es_sink : public gr_sync_block
{
  es_sink (pmt_t arb, es_queue_sptr queue, gr_io_signature_sptr insig, int n_threads);   // private constructor

  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_time;
  unsigned int d_history;
};



