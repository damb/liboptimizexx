/*! \file iterationstate.h
 * \brief Declarations of iteration state related classes.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 31/03/2012
 * 
 * Purpose: Declarations of iteration state related classes.  
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
 * 31/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */

#include <memory>
#include <optimizexx/error.h>

#ifndef _OPTIMIZEXX_ITERATIONSTATE_H_
#define _OPTIMIZEXX_ITERATIONSTATE_H_
 
namespace optimize
{
  // forward declaration
  template <typename Ctype, typename CresultData> class GridComponent;

  /* ======================================================================= */

  namespace iterator
  {
    /* ===================================================================== */
    /*!
     * Abstract base class for iteration states.
     *
     * \todo IMPORTANT NOTE: Due to reasons of an increased transparancy and to
     * have an iterator for any kind of grid composite it would be more
     * convenient to introduce a further template parameter in class
     * optimize::GridComponent for the data structure composites store their
     * children in. 
     *
     * \ingroup group_iterator
     */ 
    template <typename Ctype, typename CresultData>
    class IterationState
    {
      public:
        typedef typename GridComponent<Ctype, CresultData>::Titer Titer;
        typedef typename GridComponent<Ctype, CresultData>::Treverse_iter
          Treverse_iter;
        typedef typename std::unique_ptr<IterationState<Ctype, CresultData>>
          TiterStatePtr; 

      public:
        //! destructor
        virtual ~IterationState() { }
        //! query function for the iterator in the current iteration state
        virtual Titer& getIterator() = 0;
        //! test if iteration is finished
        virtual bool isEnd() const = 0;
        //! test if iteration has reached last element
        virtual bool isBack() = 0;
        //! increment iterator in the current iteration state
        virtual void next() = 0;
        /*!
         * perform a deep copy of the iteration state\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to new iteration state
         */
        virtual TiterStatePtr clone() const = 0;

      protected:
        //! constructor
        IterationState() { }

    }; // class template IterationState

    /* ===================================================================== */
    /*!
     * Provides a data structure to save a forward iteration state. Iteration
     * states are handled by optimize::IterationMemento. Note that any iterator
     * direcly works on the memento.
     * 
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class ForwardIterationState : public IterationState<Ctype, CresultData>
    {
      public:
        //! typedef to base class
        typedef IterationState<Ctype, CresultData> Tbase;
        typedef typename Tbase::Titer Titer;
        typedef typename Tbase::TiterStatePtr TiterStatePtr;

      public:
        //! constructor
        ForwardIterationState(Titer iter, Titer end_iter) : Miter(iter),
          Mend_iter(end_iter)
        { }
        //! destructor
        virtual ~ForwardIterationState() { }
        //! query function for the iterator in the current iteration state
        virtual Titer& getIterator() { return Miter; }
        //! test if iteration is finished
        virtual bool isEnd() const { return Miter == Mend_iter; }
        //! test if iteration has reached last element
        virtual bool isBack() { Titer tmp(Mend_iter); return Miter == --tmp; }
        //! increment iterator in the current iteration state
        virtual void next() { ++Miter; }
        /*!
         * perform a deep copy of the iteration state\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to new iteration state
         */
        virtual TiterStatePtr clone() const
        {
          return TiterStatePtr(
              new ForwardIterationState<Ctype, CresultData>(*this));
        }

      private:
        //! iterator which will be changed
        Titer Miter;
        //! iterator which marks the end of the container
        Titer Mend_iter;

    }; // class template ForwardIterationState

    /* ===================================================================== */
    /*!
     * Provides a data structure to save a reverse iteration state. Iteration
     * states are handled by optimize::IterationMemento. Note that any iterator
     * of the parameter space grid container direcly works on the iteration
     * state.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class ReverseIterationState : public IterationState<Ctype, CresultData>
    {
      public:
        //! typedef to base class
        typedef IterationState<Ctype, CresultData> Tbase;
        typedef typename Tbase::Treverse_iter Treverse_iter;
        typedef typename Tbase::Titer Titer;
        typedef typename Tbase::TiterStatePtr TiterStatePtr;

      public:
        //! constructor
        ReverseIterationState(Treverse_iter iter, Treverse_iter end_iter) :
          Miter(iter), Mend_iter(end_iter)
        { }
        //! destructor
        virtual ~ReverseIterationState() { }
        /*!
         * query function for the iterator in the current iteration state
         * 
         * Makes use of \c base function of STL reverse iterator to return a
         * common STL iterator. See also: http://drdobbs.com/184401406
         *
         * \note
         * \code
         *    return --Miter.base();
         * \endcode
         * might not compile with STL vectors or strings.
         *
         */
        virtual Titer& getIterator() { return --Miter.base(); }
        //! test if iteration is finished
        virtual bool isEnd() const { return Miter == Mend_iter; }
        //! test if iteration has reached last element
        virtual bool isBack();
        //! increment iterator in the current iteration state
        virtual void next() { ++Miter; }
        /*!
         * perform a deep copy of the iteration state\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to new iteration state
         */
        virtual TiterStatePtr clone() const
        {
          return TiterStatePtr(
              new ReverseIterationState<Ctype, CresultData>(*this));
        }

      private:
        //! iterator which will be changed
        Treverse_iter Miter;
        //! iterator which marks the end of the container
        Treverse_iter Mend_iter;

    }; // class template ReverseIterationState

    /* ===================================================================== */
    template <typename Ctype, typename CresultData>
    bool ReverseIterationState<Ctype, CresultData>::isBack()
    { 
      Treverse_iter tmp(Mend_iter); return Miter == --tmp;
    }

    /* --------------------------------------------------------------------- */

  } // namespace iterator

} // namespace optimize

#endif // include guard

/* ----- END OF iterationstate.h  ----- */
