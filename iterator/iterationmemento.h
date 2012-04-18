/*! \file iterationmemento.h
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
#include <memory>
#include <optimizexx/iterator/iterationstate.h>

#ifndef _OPTIMIZEXX_ITERATIONMEMENTO_H_
#define _OPTIMIZEXX_ITERATIONMEMENTO_H_

namespace optimize
{

  namespace iterator
  {
    /* ===================================================================== */
    /*!
     * Abstract base memento class of the memento design pattern (GoF p.283)
     * which is  responsible for saving the iterator's internal states. Stored
     * are instances of IterationState.\n
     * 
     * Note that in this case the memento design pattern is not implemented as
     * suggested in GoF.\n
     * The narrow interface approach is not in use for IteratorMemento because
     * it would lead to adding all different types of Iterator as friends to
     * this memento class (GoF p. 287). Additionally there would be access to
     * the mementos private snapshot because the originator (Iterator) of the
     * memento stores its own state which means that it is its own caretaker,
     * too.\n
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class IterationMemento
    {
      public:
        typedef typename std::unique_ptr<IterationState<Ctype, CresultData>>
          Tstate;
        typedef typename std::unique_ptr<IterationMemento<Ctype, CresultData>>
          TmementoPtr;

      public:
        //! copy constructor
        IterationMemento(IterationMemento<Ctype, CresultData> const& rhs);
        //! destructor
        virtual ~IterationMemento() { }
        /*!
         * add a new iteration state
         *
         * \param state iteration state
         */
        virtual void pushState(Tstate state) = 0;
        //! delete the current state
        virtual void popState();
        /*!
         * query function if last state has finished iteration
         *
         * \return if last iteration state has finished iteration
         */
        virtual bool iterationStateIsEnd() const;
        /*!
         * query function if last iteration has reached last element
         *
         * \return status
         */
        virtual bool iterationIsBack();
        /*!
         * query function for the current iterator
         *
         * \return current iterator
         */
        virtual typename IterationState<Ctype, CresultData>::Titer&
          getCurrentIterator();
        //! proceed with iteration
        virtual void next() { MstateStack.back()->next(); }
        //! query function if iteration state container is empty
        bool empty() const { return MstateStack.empty(); }
        //! empty the memento's stack
        void reset();
        //! assignment operator
        virtual IterationMemento<Ctype, CresultData>& operator=(
            IterationMemento<Ctype, CresultData> const& rhs);
        /*! 
         * perform deep copy of an iteration memento\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied because C++ does neither support virtual copy constructors
         * not virtual constructors.
         *
         * \return unique pointer to the deep copy of iteration momento
         */
        virtual TmementoPtr clone() const = 0;
        /*!
         * virtual constructor for an iteration memento
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied because C++ does neither support virtual copy constructors
         * not virtual constructors.
         *
         * \return unique pointer to constructed iteration momento
         */
        virtual TmementoPtr create() const = 0;

      protected:
        //! constructor
        IterationMemento() { }

      protected:
        /*!
         *  Iteration state stack which basically is a std::deque to make use of
         *  both push/pop - back/front functionalism.
         */
        typename std::deque<Tstate> MstateStack;

    }; // class template IterationMemento

    /* ===================================================================== */
    /*!
     * Memento for post-order iterators.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class PostIterationMemento : public IterationMemento<Ctype, CresultData>
    {
      public:
        typedef iterator::IterationMemento<Ctype, CresultData> Tbase;
        typedef typename Tbase::TmementoPtr TmementoPtr;

      public:
        //! constructor
        PostIterationMemento() { }
        //! copy constructor
        PostIterationMemento(
            PostIterationMemento<Ctype, CresultData> const& rhs) : Tbase(rhs)
        { }
        //! destructor
        virtual ~PostIterationMemento() { }
        /*!
         * add a new iteration state
         *
         * \param state iteration state
         */
        virtual void pushState(typename Tbase::Tstate state);
        /*! 
         * perform deep copy of an iteration memento\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to the deep copy of iteration momento
         */
        virtual TmementoPtr clone() const
        {
          return TmementoPtr(
              new PostIterationMemento<Ctype, CresultData>(*this));
        }
        /*!
         * virtual constructor for an iteration memento
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied because C++ does neither support virtual copy constructors
         * not virtual constructors.
         *
         * \return unique pointer to constructed post iteration momento
         */
        virtual TmementoPtr create() const 
        {
          return TmementoPtr(new PostIterationMemento<Ctype, CresultData>);
        }

    }; // class template PostIterationMemento

    /* ===================================================================== */
    /*!
     * Memento for pre-order iterators.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class PreIterationMemento : public IterationMemento<Ctype, CresultData>
    {
      public:
        typedef IterationMemento<Ctype, CresultData> Tbase;
        typedef typename Tbase::TmementoPtr TmementoPtr;

      public:
        //! constructor
        PreIterationMemento() { }
        //! copy constructor
        PreIterationMemento(
            PreIterationMemento<Ctype, CresultData> const& rhs) : Tbase(rhs)
        { }
        //! destructor
        virtual ~PreIterationMemento() { }
        /*!
         * add a new iteration state
         *
         * \param state iteration state
         */
        virtual void pushState(typename Tbase::Tstate state);
        /*! 
         * perform deep copy of an preiteration memento\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to the deep copy of iteration momento
         */
        virtual TmementoPtr clone() const
        {
          return TmementoPtr(
              new PreIterationMemento<Ctype, CresultData>(*this));
        }
        /*!
         * virtual constructor for an preiteration memento
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied because C++ does neither support virtual copy constructors
         * not virtual constructors.
         *
         * \return unique pointer to constructed pre iteration momento
         */
        virtual TmementoPtr create() const 
        {
          return TmementoPtr(new PreIterationMemento<Ctype, CresultData>);
        }

    }; // class template PreIterationMemento

    /* ===================================================================== */
    // function definitions of IterationMemento
    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    IterationMemento<Ctype, CresultData>::IterationMemento(
    IterationMemento<Ctype, CresultData> const& rhs)
    {
      // deep copy of state stack
      for (typename std::deque<Tstate>::const_iterator cit(
            rhs.MstateStack.begin()); cit != rhs.MstateStack.end(); ++cit)
      {
        this->MstateStack.push_back((*cit)->clone());
      }
    } // copy constructor

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    typename IterationState<Ctype, CresultData>::Titer&
    IterationMemento<Ctype, CresultData>::getCurrentIterator()
    {
      return MstateStack.back()->getIterator();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void IterationMemento<Ctype, CresultData>::popState()
    { 
      MstateStack.pop_back();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    bool IterationMemento<Ctype, CresultData>::iterationStateIsEnd() const
    { 
      return MstateStack.back()->isEnd();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    bool IterationMemento<Ctype, CresultData>::iterationIsBack()
    {
      return 1 == MstateStack.size() && MstateStack.back()->isBack();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void IterationMemento<Ctype, CresultData>::reset()
    {
      MstateStack.clear();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    IterationMemento<Ctype, CresultData>&
    IterationMemento<Ctype, CresultData>::operator=(
        IterationMemento<Ctype, CresultData> const& rhs)
    {
      if (this != &rhs)
      {
        // deep copy of state stack
        for (typename std::deque<Tstate>::const_iterator cit(
              rhs.MstateStack.begin()); cit != rhs.MstateStack.end(); ++cit)
        {
          this->MstateStack.push_back((*cit)->clone());
        }
      }
      return *this;
    }

    /* ===================================================================== */
    // function definitions of PostIterationMemento
    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void PostIterationMemento<Ctype, CresultData>::pushState(typename
        Tbase::Tstate state)
    {
      Tbase::MstateStack.push_front(std::move(state));
    }

    /* ===================================================================== */
    // function definitions of PreIterationMemento
    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void PreIterationMemento<Ctype, CresultData>::pushState(typename
        Tbase::Tstate state)
    {
      Tbase::MstateStack.push_back(std::move(state));
    }

    /* --------------------------------------------------------------------- */

  } // namespace iterator

} // namespace optimize
 
#endif // include guard

/* ----- END OF iterationmemento.h  ----- */
