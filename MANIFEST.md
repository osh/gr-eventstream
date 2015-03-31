title: gr-eventstream
brief: The event stream scheduler
tags:
  - scheduler
  - streams
  - bursty
author:
  - Tim O'Shea <tim.oshea753@gmail.com>
copyright_owner:
  - Tim O'Shea <tim.oshea753@gmail.com>
dependencies:
  - gnuradio (>= 3.7.0)
repo: https://github.com/osh/gr-eventstream.git
stable_release: HEAD
icon: https://raw.githubusercontent.com/gnuradio/pybombs/master/img/gr-eventstream.png
---

These blocks are intended to help improve the translation of
streams of data items into scheduled-finite length events
occurring in the data streams in both directions.

* es.sink provides the translation from stream to events
* es.source provides the translation from events to streams

* triggers cause events to be scheduled in es.source
 and es.sink block event queues.

Some examples of specific triggers included are,
    - es_trigger_edge_f: cause an event when float stream rises
                         above a fixed threshold value
    - es_trigger_sample_timer: cause an event to occur on
                    a periodic sample increment in the stream

* handlers consume events and do some ammount of signal
  processing on them

Some example of specific handlers included are,
    - es_handler_print: print the event to STDOUT
    - es_handler_pdu: convert the event to a PDU and send
                        it out a message port
    - es_handler_insert_vector: a source handler which
                populates an event window from a PMT vector
    - es_handler_file: a sink handler which writes event
                contents out to a file
