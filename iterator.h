/*! \file iterator.h
 * \brief Declare an iterator class template
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 12/04/2012
 * 
 * Purpose: Declare an iterator class template.
 *
 * ----
 * This file is part of liboptimizexx.
 *
 * liboptimizexx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liboptimizexx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liboptimizexx.  If not, see <http://www.gnu.org/licenses/>.
 * ----
 * 
 * Copyright (c) 2012 by Daniel Armbruster
 * 
 * REVISIONS and CHANGES 
 * 12/04/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <memory>
#include <optimizexx/iterator/compositeiterator.h>

#ifndef _OPTIMIZEXX_ITERATOR_H_
#define _OPTIMIZEXX_ITERATOR_H_

namespace optimize
{

  // forward declaration
  template <typename Ctype, typename CresultData> class GridComponent;
  template <typename Ctype, typename CresultData> class Iterator;

  /* ======================================================================= */
  //! \defgroup group_iterator Parameter Space iterator module
  /*!
   * Iterator types for parameterized factory method (GoF p.111). 
   *
   * \ingroup group_iterator
   */
  enum EiteratorType
  {
    ForwardIter,          //!< usual iterator
    ForwardGridIter,      //!< only grid iterator
    ForwardNodeIter,      //!< only node iterator
    ReverseIter,          //!< reverse iterator
    ReverseGridIter,      //!< only grid reverse iterator
    ReverseNodeIter,      //!< only node reverse iterator
    NullIter              //!< degenerated iterator
  }; // enum EiteratorType

  /*!
   * Mode of iteration.
   *
   * \ingroup group_iterator
   */
  enum EiterationMode
  {
    PreOrder, //!< Pre-oder iteration
    PostOrder //!< Post-order iteration
  }; // enum EiterationMode

  /* ======================================================================= */
  /*!
   * Advances the iterator \c iter by \c n elements.
   *
   * \todo Not tested yet.
   *
   * \param iter Iterator to be advanced.
   * \param n Number of elements to be advanced. If n is equal to or greater
   * than the number of children in the parameter space grid than \c iter will
   * point to the last element.
   *
   * \ingroup group_iterator
   */
  template <typename Citerator>
  void advance(Citerator& iter, size_t n)
  {
    while (! iter.isDone() && n > 0)
    {
      --n;
      iter.next();
    }
  } // function template advance

  /* ----------------------------------------------------------------------- */
  /*!
   * Calculates the number of elements between \c iter_first and \c iter_last.
   *
   * \param iter_first Iterator pointing to the initial element.
   * \param iter_last Iterator pointing to the final element. This must be
   * reachable from \c iter_first.
   *
   * \note In contrast to the STL distance function the function uses
   * repeatedly the \c next() function of the composite iterator.
   *
   * \return number of elements between \c iter_first and \c iter_last
   *
   * \ingroup group_iterator
   *
   * \note Have a look at http://drdobbs.com/184401406 to improve
   * implementation.
   */
  template <typename Citer_first, typename Citer_last>
  size_t distance(Citer_first const& iter_first, Citer_last const& iter_last)
  {
    Citer_first iter(iter_first);

    size_t retval = 0;
    std::cout << iter.isDone() << std::endl;
    std::cout << iter_last.isDone() << std::endl;
    std::cout << *iter << std::endl;
    while (! iter.isDone() && iter != iter_last)
    {
      ++retval;
      iter.next(); 
    }
    if (iter != iter_last)
    {
      iter.first(); 
      while (iter != iter_last) { ++retval; iter.next(); }
    }
    return retval;
  } // function distance

  /* ======================================================================= */
  /*!
   * \brief Parameter space iterator.
   *
   * Notice, that here the Strategy design pattern is in use (GoF p.315). The
   * main advantage of using this approach is encapsulating an iterator strategy
   * so that clients using a parameter space iterator are able to avoid iterator
   * pointers actually. This class corresponds to the \a Context class of the
   * Strategy design pattern in GoF.
   *
   * \todo How to avoid the fact that the 
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class Iterator
  {
    public:
      typedef GridComponent<Ctype, CresultData>* Tcomp_ptr;
      typedef typename std::unique_ptr<
          iterator::CompositeIterator<Ctype, CresultData>> Tstrategy;

    public:
      //! constructor
      Iterator(Tstrategy iter_strategy) : Miter(std::move(iter_strategy))
      { }
      //! copy constructor
      Iterator(Iterator<Ctype, CresultData> const& rhs);
      //! assignment operator
      Iterator<Ctype, CresultData>& operator=(
          Iterator<Ctype, CresultData> const& rhs);
      //! set iterator to first element (depending on strategy)
      void first() { Miter->first(); }
      //! set iterator to last element
      void back() { Miter->back(); }
      //! query function if iterator has reached last element
      bool isDone() const { return Miter->isDone(); }
      //! iterate to next element
      void next() { Miter->next(); }
      //! query function for current item iterator is pointing to
      Tcomp_ptr currentItem() { return Miter->currentItem(); }
      //! pre-increment operator
      Iterator<Ctype, CresultData>& operator++();
      //! post-increment operator
      Iterator<Ctype, CresultData> operator++(int);
      //! equal to operator
      bool operator==(Iterator<Ctype, CresultData> const& rhs) const;
      //! unequal to operator
      bool operator!=(Iterator<Ctype, CresultData> const& rhs) const;
      //! dereference operator
      Tcomp_ptr operator*() { return Miter->currentItem(); }

    private:
      //! pointer to a concrete iterator strategy
      typename std::unique_ptr<iterator::CompositeIterator<Ctype, CresultData>>
        Miter;

  }; // class template Iterator

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  Iterator<Ctype, CresultData>::Iterator(
      Iterator<Ctype, CresultData> const& rhs)
  {
    this->Miter = rhs.Miter->clone();
  } // copy constructor

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  Iterator<Ctype, CresultData>& Iterator<Ctype, CresultData>::operator=(
      Iterator<Ctype, CresultData> const& rhs)
  {
    if (this != &rhs)
    {
      this->Miter = rhs.Miter->clone();
    }
    return *this;
  } // function Iterator<Ctype, CresultData>::operator=

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  Iterator<Ctype, CresultData>& Iterator<Ctype, CresultData>::operator++()
  { 
    Miter->next(); return *this; 
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  Iterator<Ctype, CresultData> Iterator<Ctype, CresultData>::operator++(int)
  {
    Iterator<Ctype, CresultData> tmp(*this);
    operator++();
    return tmp;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  bool Iterator<Ctype, CresultData>::operator==(
      Iterator<Ctype, CresultData> const& rhs) const
  {
    return Miter->currentItem() == rhs.Miter->currentItem();
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  bool Iterator<Ctype, CresultData>::operator!=(
      Iterator<Ctype, CresultData> const& rhs) const
  {
    return !(*this == rhs);
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard

/* ----- END OF iterator.h  ----- */
