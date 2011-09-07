/* -*- c++ -*- */
/* 
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_ES_PATTERNED_INTERLEAVER_H
#define INCLUDED_ES_PATTERNED_INTERLEAVER_H

#include <gr_block.h>

class es_patterned_interleaver;
typedef boost::shared_ptr<es_patterned_interleaver> es_patterned_interleaver_sptr;

 es_patterned_interleaver_sptr es_make_patterned_interleaver (int items, int itemsize, std::vector<int> pattern);

/*!
 * \brief <+description+>
 *
 */
class  es_patterned_interleaver : public gr_block
{
	friend  es_patterned_interleaver_sptr es_make_patterned_interleaver (int items, int itemsize, std::vector<int> pattern);

	es_patterned_interleaver (int items, int itemsize, std::vector<int> pattern);

 public:
	~es_patterned_interleaver ();


	int general_work (int noutput_items,
        gr_vector_int &ninput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_ES_PATTERNED_INTERLEAVER_H */

