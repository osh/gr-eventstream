#!/usr/bin/env python

from gnuradio import gr,blocks
import es, pmt, uuid;

class multisink(gr.hier_block2):
    def __init__(self, io_sig=1*[gr.sizeof_gr_complex], nthreads=8, samplehist=1000, eb=0, ss=2, tg=None, nsinks=2, distribute=0):
        # parent constructor
        gr.hier_block2.__init__(self,
            "es_multisink",
            gr.io_signature(len(io_sig), len(io_sig), io_sig[0]),
            gr.io_signature(0,0,0))
        if(tg == None):
            tg = uuid.uuid4();

        #debug = blocks.message_debug()

        # set up ports
        self.message_port_register_hier_in("schedule_event");
        self.message_port_register_hier_out("nconsumed");
        self.message_port_register_hier_out("pdu_event");

        # create sinks
        self.sinks = [];
        for i in range(0, nsinks):
            if distribute:
                gid = '{}_{}'.format(tg, i)
            else:
                gid = tg
            self.sinks.append(
                es.sink(io_sig, nthreads,samplehist,eb,ss,gid)
                )

        if not distribute:
            #self.msg_connect( (self, "schedule_event"), (debug, "print") )
            # Set up hier connections
            for sink in self.sinks:
                #print "es_multisink connecting internal sink: ",sink
                if sink == self.sinks[0]:
                    self.msg_connect( (self, "schedule_event"), (sink, "schedule_event") )
                    self.msg_connect( sink, "nconsumed", self, "nconsumed" )
                self.msg_connect( sink, "pdu_event", self, "pdu_event" )
                self.connect( (self,0), (sink,0) )
        else:
            # Create distributor
            self.dist = es.distributor(io_sig, nsinks)
            #self.dist.set_processor_affinity([79])

            # Connect heir 'schedule_event' input msg port to dist event input
            # msg port.
            self.msg_connect(
                (self, 'schedule_event'),
                (self.dist, 'dist_random'))

            # Connect each heir input streaming port to dist input streaming
            # ports.
            for i in range(len(io_sig)):
                self.connect((self, i), (self.dist, i))

            for i in range(nsinks):
                if nsinks == 1:
                    dist_out_port_name = 'dist_out'
                else:
                    dist_out_port_name = 'dist_out{}'.format(i)
                # For now, there can only be one sink connected to the
                # 'nconsumed' output msg port of the heir block.
                #
                # XXX: What to do about 'nconsumed' output port? Connecting this
                # port to only one of the sinks will result in only that sink
                # generating msg's on this port. The other sink's 'nconsumed'
                # msg's will be ignored. They can't be tied together because
                # each sink generates its own 'nconsumed' number. May need
                # another block to tally the 'nconsumed' msg's from each sink
                # and then output a total 'nconsumed' msg for all sinks.
                if i == 0:
                    self.msg_connect(
                        (self.sinks[i], 'nconsumed'),
                        (self, 'nconsumed'))

                # Connect each output msg port of dist to the input event msg
                # ports of each sink. The number of output msg ports of dist
                # must be equal to the number of sinks.
                self.msg_connect(
                    (self.dist, dist_out_port_name),
                    (self.sinks[i], 'schedule_event'))

                # Connect each sink's output 'pdu_event' msg port to the heir
                # block output 'pdu_event' msg port.
                self.msg_connect(
                    (self.sinks[i], 'pdu_event'),
                    (self, 'pdu_event'))

                # Connect each streaming output port of dist to this sink.
                for j in range(len(io_sig)):
                    self.connect((self.dist, j), (self.sinks[i], j))
