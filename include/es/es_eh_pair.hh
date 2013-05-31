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
#ifndef ES_EH_PAIR_HH
#define ES_EH_PAIR_HH

#include <pmt/pmt.h>
#include <gnuradio/block.h>
using namespace pmt;

class es_eh_pair {

    public:
        es_eh_pair(pmt_t event, pmt_t handler);
        pmt_t event;
        pmt_t handler;

        void run();
        unsigned long long time();
        unsigned long long length();
        ~es_eh_pair();

    private:
        es_eh_pair() {};
};      

#endif
