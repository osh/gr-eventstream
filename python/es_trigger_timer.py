#!/usr/bin/env python

from gnuradio import gr;
import es;
import threading;
import time;
import pmt;
import sys;
import time;


class es_trigger_timer(gr.sync_block):
    def __init__(self, evt_type, evt_length, delay, count=-1):
        gr.sync_block.__init__(self,"timer_event",[],[])

        self.message_port_register_out(pmt.intern("timer_event"));
        self.message_port_register_out(pmt.intern("which_stream"));

        self.count_init = count;
        self.d_type = pmt.intern("timer_event");
        self.count = count;
        self.delay = delay;
        self.evt_length = evt_length;
        self.thread = threading.Thread(target=self.run);


    def start(self):
        self.count = self.count_init;
        self.thread.start();
        reg = pmt.cons(pmt.intern("timer_event"), self.message_subscribers(pmt.intern("timer_event")));
        reg_msg = pmt.cons(pmt.intern("ES_REGISTER_HANDLER"), reg);
        print "subscribers: %s"%(str(self.message_subscribers(pmt.intern("timer_event"))));
        print "registration message: %s"%(str(reg_msg))
        self.message_port_pub(pmt.intern("which_stream"), reg_msg);

    def stop(self):
        self.count = 0;
        self.thread.join();

    def run(self):
        while(self.count == -1 or self.count > 0):
            time.sleep(self.delay);
            timer_evt = es.event_create(self.d_type, 0, self.evt_length);
            timer_evt = es.event_args_add( timer_evt, pmt.intern("timer_counter"), pmt.from_long(self.count));
            try:
                self.message_port_pub(pmt.intern("which_stream"), timer_evt);
            except:
                return

            if(self.count > 0):
                self.count = self.count - 1;

    def work(self, input_items, output_items):
        pass



if __name__ == '__main__':
    queue = es.queue();
    queue.set_early_behavior(1);
    a = es_trigger_timer(queue, "TYPE1", 0.1, 10);
    a.start();
    print "Start returned waiting for main thread to join ... "
    a.join();


