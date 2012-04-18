/*! \file nulliterator.h
 * \brief Declaration of a degenerated iterator class.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 12/04/2012
 * 
 * Purpose: Declaration of a degenerated iterator class.
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

#ifndef _OPTIMIZEXX_NULLITERATOR_H_
#define _OPTIMIZEXX_NULLITERATOR_H_

namespace optimize
{

  namespace iterator
  {
    /* ===================================================================== */
    /*!
     * Degenerated iterator with the property that NullIterator::isDone always
     * returns true. The advantage of this approach is that boundary conditions
     * are much easier to handle. See also GoF p.262.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class NullIterator : public iterator::CompositeIterator<Ctype, CresultData>
    {
      public:
        typedef typename 
          CompositeIterator<Ctype, CresultData>::Tcomp_ptr Tcomp_ptr;
        typedef typename 
          CompositeIterator<Ctype, CresultData>::TstrategyPtr TstrategyPtr;

      public:
        //! constructor
        NullIterator(Tcomp_ptr root) : Mcomp_ptr(root) { }
        //! destructor
        virtual ~NullIterator() { }
        /*!
         * Because NullIterator is a degenerated iterator always will return
         * true.
         *
         * \return true
         */
        virtual bool isDone() const { return true; }
        /*!
         * Always will return the component which is the NullIterator's
         * originator.
         * 
         * \return The NullIterator's originator.
         */
        virtual Tcomp_ptr currentItem() const { return Mcomp_ptr; }
        /*! 
         * perform deep copy of an iterator strategy\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to the deep copy of iteratior strategy
         */
        virtual TstrategyPtr clone() const
        {
          return TstrategyPtr(new NullIterator(*this));
        }

      private:
        //! shared pointer to the component the pointer is pointing to
        Tcomp_ptr Mcomp_ptr;

    }; // class template NullIterator

    /* ===================================================================== */

  } // namespace iterator

} // namespace optimize

#endif // include guard

/* ----- END OF nulliterator.h  ----- */
