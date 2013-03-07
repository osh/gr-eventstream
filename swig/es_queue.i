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

GR_SWIG_BLOCK_MAGIC(es,queue);


es_queue_sptr es_make_queue();

enum es_queue_early_behaviors {
    BALK,
    ASAP
};

class es_queue {
    public:

        es_queue();
        int add_event(pmt_t evt);
        void print();
        int length();
        int register_event_type(std::string type);
        int register_event_type(pmt_t type);
        void bind_handler(std::string type, gr_basic_block_sptr handler);
        void bind_handler(pmt_t type, gr_basic_block_sptr handler);
        void set_early_behavior(enum es_queue_early_behaviors);
        void protect_handler(es_handler_sptr h);
};




