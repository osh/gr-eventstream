/* -*- c++ -*- */
/*
 * Copyright 2011 Free Software Foundation, Inc.
 * 
 * This file is part of gr-eventstream
 * 
 * gr-eventstream is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * gr-eventstream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gr-eventstream; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#ifndef EVENTSTREAM_HANDLER_H
#define EVENTSTREAM_HANDLER_H

#include <gruel/pmt.h>
#include <gruel/msg_accepter.h>
#include <gr_block.h>

using namespace pmt;


class es_handler;

pmt_t make_handler_pmt( es_handler* h );

gruel::msg_accepter_sptr make_ma_sptr( es_handler* h );

typedef boost::shared_ptr<es_handler> es_handler_sptr;

class es_handler : public gruel::msg_accepter {
    public:
        void post(pmt_t msg);
        gr_vector_void_star get_buffer_ptr(pmt_t buffer_pmt);
        virtual void handler(pmt_t msg, gr_vector_void_star buf);
        pmt_t get_ma(){ make_handler_pmt(this); }
        ~es_handler();
};

#endif


