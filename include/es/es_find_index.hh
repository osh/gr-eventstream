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

#ifndef ES_FIND_INDEX_HH
#define ES_FIND_INDEX_HH

#include <vector>
#include <algorithm>
#include <limits>

/**
  @brief Provide insertion sort algorithms.

  This is a base class and child classes must derived from it in order to
  properly use this class. This class provides search algorithms for searching
  through a sorted vector. Search methods provided by this class return the
  index at which the compared item might be inserted in order to maintain the
  sort on the vector. This class does not perform the insertion.

  Derived classes must provide a static compare_static() method that is used by
  this class to perform comparisons for decision making within the algorithms.

  This class uses the Curiously Recurring Template Pattern (CRTP) in an attempt
  to avoid costly function pointers.

  @tparam D The type of the derived class.
  @tparam T The type of values stored in the vector to search.
  @tparam T2 The type of the value to compare against an item in the vector.
  Defaults to T unless otherwise specified.
 */
template <typename D, typename T, typename T2 = T>
class index_search_base
{
  public:

    enum search_styles
    {
      SEARCH_FORWARD,
      SEARCH_REVERSE,
      SEARCH_BINARY
    };
    /** @brief Pointer to a sorted vector. */
    std::vector<T>* vptr;

    /**
     * @brief Constructor.
     *
     * @param [in] vec Reference to a sorted vector on which search algorithms
     * are to be performed.
     */
    index_search_base(std::vector<T>& vec):
      vptr(&vec){};

    /**
     * @brief Compare a value from the sorted vector against a comparison value.
     *
     * @param [in] vval Reference to a vector value.
     *
     * @param [in] cval Reference to a comparison value.
     *
     * @return bool True if cval > vval, false otherwise. The details of this
     * comparison are provided by the deriving class' compare_static() method.
     * This is necessary since type T and T2 may not be the same.
     */
    static bool compare_base(const T& vval, const T2& cval)
    {
      /* Compare the two input values using a method implemented by the deriving
       * class.
       */
      return D::compare_static(vval, cval);
    }

    /**
     * @brief Find index based on a forward searching algorithm.
     *
     * Search forward starting at the first index and searching linearly
     * through the vector until either an appropriate insertion index or the
     * end of the vector is found.
     *
     * @param [in] cval Value to test for vector insertion position.
     *
     * @return Index at which cval may be inserted into vector to maintain sort.
     */
    size_t find_forward(const T2& cval)
    {
      size_t idx = 0, sz = vptr->size();
      for (idx = 0; idx < sz; idx++)
      {
        if (!compare_base((*vptr)[idx], cval))
        {
          break;
        }
      }
      return idx;
    }

    /**
     * @brief Find index based on a reverse searching algorithm.
     *
     * Search backward starting at the last index and searching linearly
     * through the vector until either an appropriate insertion index or the
     * start of the vector is found.
     *
     * @param [in] cval Value to test for vector insertion position.
     *
     * @return Index at which cval may be inserted into vector to maintain sort.
     */
    size_t find_reverse(const T2& cval)
    {
      size_t sz = vptr->size(), idx = 0;

      // If nothing is in the vector then the insertion index must be 0.
      if (sz == 0)
      {
        return 0;
      }

      // Note: Unsigned types are used for indexing so the loop must be bounded
      // by max(size_t).
      // If idx == 0, then idx - 1 == max(size_t). This is equivalent to
      // decrementing idx "below" zero.
      size_t max = std::numeric_limits<size_t>::max();
      for (idx = sz - 1; idx != max; idx--)
      {
        if (compare_base((*vptr)[idx], cval))
        {
          break;
        }
      }

      // If we get here and idx == max(size_t), incrementing that value by 1
      // will return an index of 0. This indicates that the above loop was
      // exhausted.
      return idx + 1;
    }

    /**
     * @brief Find index based on a reverse searching algorithm.
     *
     * Search backward starting at the last item and searching linearly
     * through the vector until either an appropriate insertion index or the
     * start of the vector is found. This algorithm uses pointers to vector
     * items for tracking progress through the search (vs. indices or
     * iterators).
     *
     * @param [in] cval Value to test for vector insertion position.
     *
     * @return Index at which cval may be inserted into vector to maintain sort.
     */
    size_t find_reverse_ptr(const T2& cval)
    {
      size_t sz = vptr->size();//, idx = 0;

      // If nothing is in the vector then the insertion index must be 0.
      if (sz == 0)
      {
        return 0;
      }

      T* start = &(*vptr)[0];
      T* curr = &(*vptr)[sz - 1];
      for (; curr >= start; curr--)
      {
        if (compare_base(*curr, cval))
        {
          break;
        }
      }

      return (size_t)(curr - start + 1);
    }

    /**
     * @brief Find index based on a binary searching algorithm.
     *
     * Search using a binary pattern starting at the first item and searching
     * through the vector until either an appropriate insertion index or the
     * end of the search pattern is reached.
     *
     * C++ standard library algorithms are used to enable this search.
     *
     * @param [in] cval Value to test for vector insertion position.
     *
     * @return Index at which cval may be inserted into vector to maintain sort.
     */
    size_t find_binary(const T2& cval)
    {
      typename std::vector<T>::iterator low;
      low = std::lower_bound(
          vptr->begin(),
          vptr->end(),
          cval,
          index_search_base::compare_base);
      return low - vptr->begin();
    }

    bool find(const T2& cval, int search_style)
    {
      switch (search_style)
      {
          case SEARCH_BINARY:
              return find_binary(cval);
          case SEARCH_REVERSE:
              return find_reverse(cval);
          case SEARCH_FORWARD:
              return find_forward(cval);
          default:
              return find_forward(cval);
      }
    }
};

/**
 * @brief Provide a direct comparison implemention for index_search_base.
 *
 * This class derives from index_search_base and provides a default
 * implementation for the case when T and T2 index_search_base template
 * parameters are equal. In other words, the type of the value stored in the
 * sorted vector and the type of the value to be compared are equivalent and
 * their types provide basic conditional operator implementions.
 *
 * One of the template parameters (D) to the index_search_base template is the
 * type of the deriving class. When this class inherits from index_search_base
 * it must provide this class as the derived type. This is part of the Curiously
 * Recurring Template Pattern (CRTP).
 *
 * @tparam T The type of the values to be compared and the type of the values
 * in the sorted vector.
 */
template<typename T>
class index_search_direct:
    public index_search_base<index_search_direct<T>, T, T>
{
  public:
  /**
   * @brief Compare vval and cval.
   *
   * @param [in] vval Reference to a vector value.
   *
   * @param [in] cval Reference to a comparison value.
   *
   * @return True if cval > vval, else false.
   */
  static bool compare_static(const T& vval, const T& cval)
  {
    return cval > vval;
  }

  /**
   * @brief Constructor.
   *
   * @param [in] vec Reference to sorted vector.
   */
  index_search_direct(std::vector<T>& vec):
    index_search_base<
      index_search_direct<T>, T, T>(vec){};
};



#endif //ES_FIND_INDEX_HH
