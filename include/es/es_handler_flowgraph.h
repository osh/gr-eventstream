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

#ifndef EVENTSTREAM_HANDLER_FLOWGRAPH_H
#define EVENTSTREAM_HANDLER_FLOWGRAPH_H

#include <pmt/pmt.h>
#include <gnuradio/block.h>



using namespace pmt;

#include <es/es_handler.h>
#include <es/es_pyhandler_def.h>
#include <boost/lockfree/queue.hpp>

class es_handler_flowgraph : public es_handler {
    public:

        es_handler_flowgraph( es_pyhandler_def* _pyhd, int pool_size );   
        ~es_handler_flowgraph();
        void handler(pmt_t msg, gr_vector_void_star buf);
   
        es_pyhandler_def* pyhd;

        std::vector< es_pyhandler* >  d_pyhandlers;
        boost::lockfree::queue<es_pyhandler*> d_free_pyhandlers;

};

es_handler_sptr es_make_handler_flowgraph(es_pyhandler_def* _pyhd, int pool_size);

#endif


