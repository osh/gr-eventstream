/* -*- c++ -*- */

%include "gnuradio.i"			// the common stuff
%include "swig/pmt_swig.i"

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
#include "es/es_handler_flowgraph.h"
#include "es/es_trigger.h"
#include "es/es_trigger_edge_f.h"
%}

%include "es_event.i"
%include "es_queue.i"
%include "es_source.i"
%include "es_sink.i"
%include "es_common.i"
%include "es_handler.i"
%include "es_handlers.i"
%include "es_trigger.i"
%include "es_trigger_edge_f.i"
%include "es_vector_source.i"



