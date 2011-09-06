#ifndef EVENTSTREAM_HANDLER_H
#define EVENTSTREAM_HANDLER_H

#include <gruel/pmt.h>
#include <gruel/msg_accepter.h>
#include <gr_block.h>

using namespace pmt;


class es_handler;

pmt_t make_handler_pmt( es_handler* h );

gruel::msg_accepter_sptr make_ma_sptr( es_handler* h );

class es_handler : public gruel::msg_accepter {
    public:
        void post(pmt_t msg);
        gr_vector_void_star get_buffer_ptr(pmt_t buffer_pmt);
        virtual void handler(pmt_t msg, gr_vector_void_star buf);
        ~es_handler();
};

#endif


