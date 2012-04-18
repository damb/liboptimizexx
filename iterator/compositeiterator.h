/*! \file compositeiterator.h
 * \brief Declarations of various parameter space iterators.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/02/2012
 * 
 * Purpose: Declarations of various parameter space iterators. 
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
 * 20/02/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */

#include <memory>
#include <optimizexx/error.h>
 
#ifndef _OPTIMIZEXX_COMPOSITEITERATOR_H_
#define _OPTIMIZEXX_COMPOSITEITERATOR_H_

namespace optimize
{
  // forward declarations
  template <typename Ctype, typename CresultData> class GridComponent;

  namespace iterator
  {

    /* ===================================================================== */
    /*!
     * Abstract base class template for parameter space iterators.
     * Note that here the iterator design pattern is in use (GoF p.257).
     * Iterators are created using the Grid::createIterator factory method
     * (GoF p.107). To traverse the recursivly aggregated parameter space the
     * iterators (concrete originators) are able to save their internal
     * iteration states making use of the memento design pattern (GoF p.283).\n
     *
     * To simplify iterator handling parameter space iterators were implemented
     * as concrete strategies using the Strategy design pattern (GoF p.315). The
     * class refers in the GoF sense to the Strategy class declaring an
     * interface for concrete strategies (concrete iterators). The advantage of
     * this approach is that no iterator pointer is necessary anymore so that
     * a \a liboptimizexx iterator just is configured with the appropriately
     * iterator strategy (iterator algorithm).
     *
     * \note Note that the functionalism of how the post-/pre-order iteration
     * was delegated to optimize::iterator::iterator::IterationMemento.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class CompositeIterator
    {
      public:
        typedef GridComponent<Ctype, CresultData>* Tcomp_ptr;
        typedef typename std::unique_ptr<CompositeIterator<Ctype, CresultData>>
          TstrategyPtr;

      public:
        /*!
         * set iterator to first element in parameter space (depending on
         * iterator
         */
        virtual void first() { OPTIMIZE_illegal; }
        //! set iterator to last element in parameter space
        virtual void back() { OPTIMIZE_illegal; }
        //! iterate to the next item in the container
        virtual void next()  { OPTIMIZE_illegal; }
        //! query function if the iteration has finished
        virtual bool isDone() const = 0;
        /*!
         * Note that here returning a pointer is necessary because otherwise
         * there couldn't be made any use of the polymorphic functionalism the
         * parameter space composite is providing.\n
         *
         * IMPORTANT NOTE: For simpler syntax provide the same function
         * returning a reference or even better overload the corresponding
         * operators.
         *
         * \return a pointer to the current grid component
         */
        virtual Tcomp_ptr currentItem() const = 0;
        //! destructor
        virtual ~CompositeIterator() { }
        /*!
         * \brief assignment operator
         * 
         * Implemented as a template method to provide virtual functionalism.
         * See also GoF p.325.
         */
        virtual CompositeIterator<Ctype, CresultData>& operator=(
            CompositeIterator<Ctype, CresultData> const& rhs);
        /*! 
         * perform deep copy of an iterator strategy\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to the deep copy of iteratior strategy
         */
        virtual TstrategyPtr clone() const = 0;

      protected:
        //! constructor
        CompositeIterator() { }

    }; // class template CompositeIterator

    /* ===================================================================== */
    template <typename Ctype, typename CresultData>
    CompositeIterator<Ctype, CresultData>&
      CompositeIterator<Ctype, CresultData>::operator=(
      CompositeIterator<Ctype, CresultData> const& rhs)
    {
      if (this != &rhs) { clone(); }
      return *this;
    }

    /* ===================================================================== */

  } // namespace iterator

} // namespace optimize

#endif // include guard

/* ----- END OF compositeiterator.h  ----- */
