GR_SWIG_BLOCK_MAGIC(es,source);


es_source_sptr es_make_source (pmt_t arb, es_queue_sptr queue, gr_io_signature_sptr out_sig);

class es_source : public gr_sync_block
{
public:
  void set_max(unsigned long long maxlen);

private:
  es_source (pmt_t arb, es_queue_sptr queue, gr_io_signature out_sig);
};
