#!/usr/bin/env python

from gnuradio import gr;
import es;
import threading;
import time;
from gruel import pmt;
import sys;

class es_trigger_keyboard(threading.Thread):
    def __init__(self, queue, evt_type, evt_length):
        threading.Thread.__init__(self);
        import Tkinter as tk
        self.d_type = evt_type;
        self.key_sym = pmt.pmt_intern("key");
        self.queue = queue;
        self.evt_length = evt_length;
        try:
            self.queue.register_event_type( pmt.pmt_intern( self.d_type ) );
        except:
            print "queue must be a valid es.queue !"
            sys.exit(-1);

    def run(self):
        print "running";
        root = tk.Tk();
        frame = tk.Frame(root,width=100,height=100);
        frame.bind("<Key>", self.keypress);
        frame.pack();
        frame.focus_set();
        root.mainloop();       
        
    def keypress(self, event):
        x = event.char;
        print "KEYPRESS!: %s"%(x);    
        key_evt = es.event_create(self.d_type, 0, self.evt_length);
        key_evt = es.event_args_add( key_evt, self.key_sym, pmt.pmt_intern(str(x)) );
        self.queue.add_event( key_evt );


if __name__ == '__main__':
    queue = es.queue();
    queue.set_early_behavior(1);
    a = es_trigger_keyboard(queue, "TYPE1");
    a.start();
    print "Start returned waiting for main thread to join ... "
    a.join();


