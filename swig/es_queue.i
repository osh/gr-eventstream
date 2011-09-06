
GR_SWIG_BLOCK_MAGIC(es,queue);


es_queue_sptr es_make_queue();

class es_queue {
    public:
        es_queue();
        int add_event(pmt_t evt);
        void print();
        int register_event_type(std::string type);
        int register_event_type(pmt_t type);
        void bind_handler(std::string type, pmt_t handler);
        void bind_handler(pmt_t type, pmt_t handler);
};




