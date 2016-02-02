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
#ifndef INCLUDED_EVENTSTREAM_DISTRIBUTOR_H
#define INCLUDED_EVENTSTREAM_DISTRIBUTOR_H

#include <gnuradio/sync_block.h>
#include <pmt/pmt.h>

class es_distributor;
using namespace pmt;

typedef boost::shared_ptr<es_distributor> es_distributor_sptr;

/**
 * @brief Create a new instance of es_distributor.
 *
 * @return es_distributor_sptr (A boost shared_ptr).
 */
es_distributor_sptr es_make_distributor (
  gr_vector_int iosig, int num_out_ports, bool separate_registration = false);

/**
 * @brief (Event) Message distributor.
 *
 * This GNU Radio block is designed to distribute events to Eventstream sink
 * blocks in a multi-sink scenario. This block is designed specifically to
 * distribute event messages but could be made to distribute generic messages.
 *
 * Event Message Distributor Scenario
 * Since we must distinguish between event registration messages and standard
 * event messages this block is not a generic message distributor when the
 * separate_registration boolean sent to the constructor is false. The input
 * message port that accepts event related messages distributes those messages
 * to output message ports of this block as a one-to-one relationship between
 * input and output ports with the output message port selected at random. If a
 * registration event message is found, then the input to output relationship
 * changes to a one-to-many in that the input registration message is output to
 * all output message ports of this block.
 *
 * Generic Message Distributor Scenario
 * If the separate_registration boolean sent to the constructor is true, then
 * this block will get an extra input message port dedicated to forwarding all
 * input messages on that message port to all output message ports satisfying
 * the one-to-many relationship. The original input message port remains and is
 * dedicated to the one-to-one relationship where the output message port is
 * selected at random. In this scenario this block becomes a generic message
 * distributor as no input messages are parsed or tested in any way. The only
 * thing that determines how they are distributed is the input message port on
 * which they arrive.
 */
class es_distributor: public gr::sync_block
{
private:
  friend es_distributor_sptr es_make_distributor (
    gr_vector_int iosig,
    int num_out_ports,
    bool separate_registration);
  /** @brief Private constructor
   *
   * @param [in] iosig Vector whose length indicates number of ports and values
   *   represent the size of each item on each port.
   * @param [in] num_out_ports Number of output message ports to create.
   * @param [in] separate_registration Boolean indicating if an additional input
   *   message port should be created to accept messages that should be
   *   distributed to all output ports regardless of their content.
   */
  es_distributor (
    gr_vector_int iosig, int num_sinks, bool separate_registration = false);
  /** @brief Current sample time. */
  uint64_t d_time;
  /** @brief Number of output message ports. */
  int d_num_out_ports;
  /** @brief Number of messages that have been distributed. */
  uint64_t d_num_events_distributed;
  /** @brief Number of messages that were registrations. */
  uint64_t d_num_events_registered;
  /** @brief List of output message port names. */
  std::vector<std::string> d_out_ports;
  /** @brief Add extra input message port specifically for registrations. */
  bool d_separate_registration;

  /** @brief Helper to convert int to string. */
  std::string int_to_string(int value);

 public:
  /** @brief Destructor. */
  ~es_distributor();
  /** @brief Work method called by the GNU Radio framework. */
  int work(
    int noutput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items
    );
  /** @brief Distribute message to a random output port.
   *
   * If the input message is an event registration message then it will be
   * handled as if it were sent to dist_msg_all(...).
   *
   * @param [in] msg PMT message to be distributed to (at least one) output
   *   port.
   */
  void dist_msg_random(pmt_t msg);
  /** @brief Distribute messages to all output ports.
   *
   * @param [in] msg PMT message to be distributed to all output ports.
   */
  void dist_msg_all(pmt_t msg);
  /** @brief Number of registrations events distributed. */
  uint64_t nevents_registered();
  /** @brief Number of non-registration events distributed. */
  uint64_t nevents_distributed();
};
#endif /* INCLUDED_EVENTSTREAM_DISTRIBUTOR_H */
