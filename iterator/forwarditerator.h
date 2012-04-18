/*! \file forwarditerator.h
 * \brief Declaration of forward parameter space iterators.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 01/04/2012
 * 
 * Purpose: Declaration of forward parameter space iterators.  
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
 * 01/04/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <memory>
#include <optimizexx/iterator/compositeiterator.h>
#include <optimizexx/iterator/iterationmemento.h>
#include <optimizexx/iterator/iterationstate.h>

#ifndef _OPTIMIZEXX_FORWARDITERATOR_H_
#define _OPTIMIZEXX_FORWARDITERATOR_H_

namespace optimize
{

  namespace iterator
  {

    /* ===================================================================== */
    /*!
     * Concrete iterator strategy to traverse the entire parameter space
     * including grids and nodes (post- and pre-ordered depending on 
     * optimizexx::iterator::IterationMemento).\n
     *
     * Note, that here the Strategy design pattern is in use (GoF p.315).
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class ForwardIterator : public CompositeIterator<Ctype, CresultData>
    {
      public: 
        typedef CompositeIterator<Ctype, CresultData> Tbase;
        typedef typename Tbase::Tcomp_ptr Tcomp_ptr;
        typedef typename Tbase::TstrategyPtr TstrategyPtr;

      public:
        /*!
         * constructor
         *
         * \param root Pointer to the composite creating the iterator.
         * \param iter_mode_ptr Mode/memento of the iteration.
         */
        ForwardIterator(Tcomp_ptr root,
            typename std::unique_ptr<IterationMemento<Ctype, CresultData>>
            iter_mode_ptr) : Mcomponent(root), MisDone(false),
            MiterMemento(std::move(iter_mode_ptr))
        { }
        /*!
         * copy constructor
         *
         * \param rhs argument to copy
         */
        ForwardIterator(ForwardIterator<Ctype, CresultData> const& rhs);
        //! destructor
        virtual ~ForwardIterator() { }
        //! set iterator to first element in parameter space composite (grid)
        virtual void first();
        //! set iterator to last element in parameter space composite (grid)
        virtual void back();
        //! go to the next item
        virtual void next();
        //! query function if iteration is finished
        virtual bool isDone() const { return MisDone; }
        //! query function for current item
        virtual Tcomp_ptr currentItem() const
        {
          return *MiterMemento->getCurrentIterator();
        }
        /*! 
         * perform deep copy of an iterator strategy\n
         * Notice that here the
         * <a href="http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/
         * Virtual_Constructor"> Virtual Constructor idiom</a> had been
         * applied.
         *
         * \return unique pointer to the deep copy of iterator strategy
         */
        virtual TstrategyPtr clone() const
        {
          return TstrategyPtr(new ForwardIterator(*this));
        }

      protected:
        //! pointer to the root element of the iteration
        Tcomp_ptr Mcomponent;
        //! variable to save wether iteration is finished
        bool MisDone;
        /*!
         * As suggested at GoF p.271 here the memento design pattern is in use
         * to capture the state of an iteration within a IterationMemento. The
         * iterator stores the memento internally. The functionalism of pre- or
         * rather post-order iteration is delegated to the memento.
         */
        typename std::unique_ptr<IterationMemento<Ctype, CresultData>>
          MiterMemento;

    }; // class template ForwardIterator

    /* ===================================================================== */
    template <typename Ctype, typename CresultData>
    ForwardIterator<Ctype, CresultData>::ForwardIterator(
        ForwardIterator<Ctype, CresultData> const& rhs) :
      Mcomponent(rhs.Mcomponent), MisDone(rhs.MisDone),
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
      MiterMemento(nullptr)
#else
      MiterMemento(0)
#endif
    {
      // copy of iteration memento is a deep copy
      this->MiterMemento = rhs.MiterMemento->clone();
    } // copy constructor

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ForwardIterator<Ctype, CresultData>::first()
    {
      MiterMemento->reset();
      if (Mcomponent->begin() != Mcomponent->end())
      {
        MisDone = false;
        typename std::unique_ptr<ForwardIterationState<Ctype, CresultData>>
          state(new ForwardIterationState<Ctype, CresultData>(
          Mcomponent->begin(), Mcomponent->end()));
        MiterMemento->pushState(std::move(state));
      } else
      {
        MisDone = true;
      }
    } // function ForwardIterator<Ctype, CresultData>::first

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ForwardIterator<Ctype, CresultData>::back()
    {
      // not really effective but must be done as follows to guarantee
      // correct behaviour both for pre- and post-order iterators
      first();

      if (! MisDone)
      {
        while (true)
        {
          if (! (*MiterMemento->getCurrentIterator())->createIterator(
                ForwardIter).isDone())
          {
            Tcomp_ptr comp_ptr = *MiterMemento->getCurrentIterator();
            typename std::unique_ptr<ForwardIterationState<Ctype, CresultData>>
              state(new ForwardIterationState<Ctype, CresultData>(
              comp_ptr->begin(), comp_ptr->end()));
            MiterMemento->pushState(std::move(state));
          } else
          {
            MiterMemento->next();
            while (! MiterMemento->iterationIsBack() &&
                MiterMemento->iterationStateIsEnd())
            {
              MiterMemento->popState();
              if (! MiterMemento->iterationIsBack()) { MiterMemento->next(); }
            }
            if (MiterMemento->iterationIsBack()) { break; }
          }
        }
      }
    } // function ForwardIterator<Ctype, CresultData>::back

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ForwardIterator<Ctype, CresultData>::next()
    {
      if (! (*MiterMemento->getCurrentIterator())->createIterator(
            ForwardIter).isDone())
      {
        Tcomp_ptr comp_ptr = *MiterMemento->getCurrentIterator();
        typename std::unique_ptr<ForwardIterationState<Ctype, CresultData>>
          state(new ForwardIterationState<Ctype, CresultData>(
          comp_ptr->begin(), comp_ptr->end()));
        MiterMemento->pushState(std::move(state));
      } else
      {
        MiterMemento->next();
          
        while (! MiterMemento->empty() && MiterMemento->iterationStateIsEnd())
        {
          MiterMemento->popState();
          if (! MiterMemento->empty()) { MiterMemento->next(); }
        }
        if (MiterMemento->empty()) { MisDone = true; }
      }
    } // function ForwardIterator<Ctype, CresultData>::next

    /* --------------------------------------------------------------------- */

  } // namespace iterator

} // namespace optimize

#endif // include guard

/* ----- END OF forwarditerator.h  ----- */
