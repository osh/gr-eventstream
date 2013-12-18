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

%include "runtime_swig.i"			// the common stuff
%include "pmt_swig.i"

namespace std {
   %template(StrVector) vector<std::string>;
};

%{
#include "es/es_event.h"
#include "es/es_queue.h"
#include "es/es_source.h"
#include "es/es_sink.h"
#include "es/es_common.h"
#include "es/es_gen_vector.h"
#include "es/es_handler.h"
#include "es/es_handler_insert_vector.h"
#include "es/es_handler_print.h"
#include "es/es_handler_file.h"
#include "es/es_handler_pdu.h"
#include "es/es_handler_flowgraph.h"
#include "es/es_trigger.h"
#include "es/es_trigger_edge_f.h"
#include "es/es_trigger_sample_timer.h"
#include "es/es_pyhandler_def.h"
#include "es/es_vector_source.hh"
#include "es/es_vector_sink.hh"
#include "es/es_patterned_interleaver.h"
%}


%include "es/es_handler_pdu.h"
%include "es_handler.i"
%include "es_event.i"
%include "es_source.i"
%include "es_sink.i"
%include "es_common.i"
%include "es_handlers.i"
%include "es_trigger.i"
%include "es_trigger_edge_f.i"
%include "es_trigger_sample_timer.i"
%include "es_vector_source.i"
%include "es_vector_sink.i"
%include "es_pyhandler_def.i"
%include "es_patterned_interleaver.i"
