/*! \file memento.h
 * \brief Declaration of iterator memento related classes.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 23/02/2012
 * 
 * Purpose: Declaration of iterator memento related classes.
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
 * 23/02/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */

#include <deque>

#ifndef _MEMENTO_H_
#define _MEMENTO_H_

namespace optimize
{
  // forward declaration
  template <typename Ctype, typename CresultData> class Grid;

  /* ======================================================================= */
  /*!
   * Provides a data structure to save an iteration state. Iteration states are
   * handled by IterationMemento. Note that any iterator direcly works on the
   * memento.
   *
   * IMPORTANT NOTE: Due to reasons of an increased transparancy and to have an
   * iterator for any kind of grid composite it would be more convenient to
   * introduce a template parameter for iterators of composites. This would
   * effect especially the line containing
   * 
   * typedef typename Grid<Ctype, CresultData>::Titer Titer;
   * 
   * 
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class IterationState
  {
    public:
      typedef typename Grid<Ctype, CresultData>::Titer Titer;

    public:
      //! constructor
      IterationState(Titer iter, Titer end_iter) :
        Miter(iter), Mend_iter(end_iter)
      { }
      //! query function for the iterator in the current iteration state
      Titer& getIterator() { return Miter; }
      //! test if iteration is finished
      bool isEnd() const { return Miter == Mend_iter; }
      //! increment iterator in the current iteration state
      void next() { ++Miter; }

    private:
      //! begin iterator
      Titer Miter;
      //! end iterator
      Titer Mend_iter;

  }; // class IterationState

  /* ======================================================================= */
  /*!
   * Abstract base memento class of the memento design pattern (GoF p.283)
   * which is  responsible for saving the iterator's internal states. Stored
   * are instances of IterationState.\n
   * 
   * Note that in this case the memento design pattern is not implemented as
   * suggested in GoF.\n
   * The narrow interface approach is not in use for IteratorMemento because it
   * would lead to adding all different types of Iterator as friends to this
   * memento class (GoF p. 287). Additionally there would be access to the
   * mementos private snapshot because the originator (Iterator) of the memento
   * stores its own state which means that it is its own caretaker, too.\n
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class IteratorMemento
  {
    public:
      typedef IterationState<Ctype, CresultData> Tstate;

    public:
      /*!
       * add a new iteration state
       *
       * \param state iteration state
       */
      virtual void pushState(
          IterationState<Ctype, CresultData> const& state) = 0;
      //! delete the current state
      virtual void popState() { MstateStack.pop_back(); }
      /*!
       * query function for the current state
       *
       * \return iteration state
       */
      virtual Tstate const& getState() { return MstateStack.back(); }
      /*!
       * query function for the current iterator
       *
       * \return current iterator
       */
      virtual typename IterationState<Ctype, CresultData>::Titer&
        getCurrentIterator();
      //! proceed with iteration
      virtual void next() { MstateStack.back().next(); }
      //! query function if iteration state container is empty
      virtual bool empty() const { return MstateStack.empty(); }
      //! empty the memento's stack
      void reset() { MstateStack.clear(); }

    protected:
      //! constructor
      IteratorMemento() { }

    protected:
      /*!
       *  Iteration state stack which basically is a std::deque to make use of
       *  both push/pop - back/front functionalism.
       */
      typename std::deque<Tstate> MstateStack;

  }; // class IteratorMemento


  /* ======================================================================= */
  /*!
   * Memento for post-order iterators.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class PostIterationMemento : public IteratorMemento<Ctype, CresultData>
  {
    public:
      typedef IteratorMemento<Ctype, CresultData> Tbase;

    public:
      //! destructor
      virtual ~PostIterationMemento() { }
      /*!
       * add a new iteration state
       *
       * \param state iteration state
       */
      virtual void pushState(IterationState<Ctype, CresultData> const& state);

  }; // class PostIterationMemento

  /* ======================================================================= */
  /*!
   * Memento for pre-order iterators.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class PreIterationMemento : public IteratorMemento<Ctype, CresultData>
  {
    public:
      typedef IteratorMemento<Ctype, CresultData> Tbase;

    public:
      //! destructor
      virtual ~PreIterationMemento() { }
      /*!
       * add a new iteration state
       *
       * \param state iteration state
       */
      virtual void pushState(IterationState<Ctype, CresultData> const& state);

  }; // class PostIterationMemento

  /* ======================================================================= */
  // function definitions of IteratorMemento
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  typename IterationState<Ctype, CresultData>::Titer&
  IteratorMemento<Ctype, CresultData>::getCurrentIterator()
  {
    return MstateStack.back().getIterator();
  }

  /* ======================================================================= */
  // function definitions of PostIterationMemento
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void PostIterationMemento<Ctype, CresultData>::pushState(
      IterationState<Ctype, CresultData> const& state)
  {
    Tbase::MstateStack.push_front(state);
  }

  /* ======================================================================= */
  // function definitions of PreIterationMemento
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void PreIterationMemento<Ctype, CresultData>::pushState(
      IterationState<Ctype, CresultData> const& state)
  {
    Tbase::MstateStack.push_back(state);
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize
 

#endif

/* ----- END OF memento.h  ----- */
