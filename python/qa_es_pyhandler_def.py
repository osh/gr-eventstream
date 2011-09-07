#!/usr/bin/env python
# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of gr-eventstream
#
# gr-eventstream is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# gr-eventstream is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gr-eventstream; see the file COPYING. If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

from gnuradio import gr;
import es;
from gruel import pmt;



def test_factory_cb(index):
    print "test_factory_cb called with index = %d"%(index);
    r = es.es_pyhandler();
    r.set_b2( {} );
    return r;


def test_hook_cb(val):
    print "TEST HOOK CB Called with %s :: %s"%(type(val), str(val));
    print val.bufs;
    print val.msg;
    print val.handler.pb2();
    r = es.es_hook_rval();
    return r;


ph = es.es_pyhandler_def();

# dont run with default handler,
# this should assert(0)
#ph.run_factory();

ph.set_pyfactory_cb(test_factory_cb);
h = ph.run_factory();

ph.set_pyfactory_cb(test_factory_cb);
ph.run_factory();

ph.set_pyfactory_cb(test_factory_cb);
ph.run_factory();

ph.set_pyfactory_cb(test_factory_cb);
ph.run_factory();

a = ph.empty_vec();
b = pmt.pmt_intern("test");
c = es.stdMapStringBlock();

ph.set_pypre_hook_cb(test_hook_cb);
ph.set_pypost_hook_cb(test_hook_cb);


ph.run_pre_hook(a,b,h);
print "pre hook ran"

ph.run_post_hook(a,b,h);
print "post hook ran"







