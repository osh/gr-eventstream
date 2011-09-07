#!/usr/bin/env python
import numpy;
import es;
from gnuradio import gr;

def int_list_to_string(l1):
    l2 = "";
    for v in l1:
        l2 = l2 + chr(v);
    return l2;



def mod_bits_qpsk(strbits, syms=[0,1,3,2], pts=[-1,1j,1,-1j]):
    src = gr.vector_source_b(es.string_to_vector(strbits));
    unpack = gr.packed_to_unpacked_bb(1, gr.GR_MSB_FIRST);
    pack = gr.pack_k_bits_bb(2);
    bts = gr.map_bb((syms));
    mapper =  gr.chunks_to_symbols_bc(pts, 1);
    rrc_taps = gr.firdes.root_raised_cosine(1.0, 2.0, 1.0, 0.35, 91);
    interp = gr.interp_fir_filter_ccc(2, (rrc_taps))
    sink = gr.vector_sink_c();
    tb=gr.top_block();
    tb.connect(src,unpack,pack,bts,mapper,interp,sink);
    tb.run();
    return sink.data();


def list_reverse(l1):
    l1 = list(l1);
    l1.reverse();
    return l1;

def list_conj_reverse(l1):
    l1 = list(l1);
    l1.reverse();
    l2 = numpy.array(l1);
    l2.conjugate();
    return l2;



if __name__ == '__main__':
    a = [0x00, 0x01, 0x02, 0x0A, 0xFF];
    b = int_list_to_string(a);
    c = mod_bits_qpsk(b);
    print c;

