#
# Copyright 2004,2005,2006,2008,2009,2010 Free Software Foundation, Inc.
# 
# This file is part of GNU Radio
# 
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

include $(top_srcdir)/Makefile.common

# list of programs run by "make check" and "make distcheck"
TESTS = test_all

# ----------------------------------------------------------------
# es C++ library: libgnuradio-es.so
# ----------------------------------------------------------------

# C/C++ headers get installed in ${prefix}/include/$(modname)
modinclude_HEADERS =  

lib_LTLIBRARIES = libgnuradio-es.la

libgnuradio_es_la_SOURCES = \
    es_common.cc  \
    es_eh_pair.cc \
    es_event_loop_thread.cc \
    es_handler.cc  \
    es_handler_print.cc  \
    es_handler_flowgraph.cc  \
    es_queue.cc  \
    es_sink.cc  \
    es_source.cc  \
    es_vector_source.cc \
    es_gen_vector.cc \
    es_handler_insert_vector.cc \
    es_trigger.cc \
    es_trigger_edge_f.cc  
#    es_pyhandler_def.cc

libgnuradio_es_la_LIBADD = \
	$(GNURADIO_CORE_LA)

libgnuradio_es_la_LDFLAGS = \
	$(NO_UNDEFINED)

# ----------------------------------------------------------------
# es C++ QA library: libgnuradio-es-qa.so (not installed)
# ----------------------------------------------------------------

noinst_LTLIBRARIES = libgnuradio-es-qa.la

libgnuradio_es_qa_la_SOURCES = \
	qa_es.cc   \
	qa_es_common.cc \
	qa_es_source.cc    \
	qa_es_sink.cc   \
	qa_es_trigger.cc   

libgnuradio_es_qa_la_LDFLAGS = $(NO_UNDEFINED) -version-info 0:0:0

libgnuradio_es_qa_la_LIBADD = \
	libgnuradio-es.la \
	$(CPPUNIT_LIBS)

# ----------------------------------------------------------------
# headers that don't get installed
# ----------------------------------------------------------------
noinst_HEADERS = \
	qa_es.h 

# ----------------------------------------------------------------
# test program
# ----------------------------------------------------------------
noinst_PROGRAMS = \
	test_all

test_all_SOURCES = test_all.cc
test_all_LDADD   = libgnuradio-es-qa.la
