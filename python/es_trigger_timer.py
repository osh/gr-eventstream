#!/usr/bin/env python

from gnuradio import gr;
import es;
import threading;
import time;
import pmt;
import sys;
import time;


class es_trigger_timer(threading.Thread):
    def __init__(self, queue, evt_type, evt_length, delay, count=-1):
        threading.Thread.__init__(self);
        self.d_type = evt_type;
        self.queue = queue;
        self.count = count;
        self.delay = delay;
        self.evt_length = evt_length;
        try:
            self.queue.register_event_type( pmt.intern( self.d_type ) );
        except:
            print "queue must be a valid es.queue !"
            sys.exit(-1);

    def run(self):
        while(self.count == -1 or self.count > 0):
            time.sleep(self.delay);
            timer_evt = es.event_create(self.d_type, 0, self.evt_length);
            timer_evt = es.event_args_add( timer_evt, pmt.intern("timer_counter"), pmt.pmt_from_long(self.count));
            self.queue.add_event( timer_evt );

            if(self.count > 0):
                self.count = self.count - 1;




if __name__ == '__main__':
    queue = es.queue();
    queue.set_early_behavior(1);
    a = es_trigger_timer(queue, "TYPE1", 0.1, 10);
    a.start();
    print "Start returned waiting for main thread to join ... "
    a.join();


