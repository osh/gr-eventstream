

GR_SWIG_BLOCK_MAGIC(es,trigger_edge_f);


es_trigger_edge_f_sptr es_make_trigger_edge_f (pmt_t arb, es_queue_sptr queue, float thresh, int length, int lookback);

class es_trigger_edge_f : public es_trigger
{
private:
  es_trigger_edge_f (pmt_t arb, es_queue_sptr queue, float thresh, int length, int lookback);   // private constructor

 public:
  float d_thresh;
  float d_lastval;
};




