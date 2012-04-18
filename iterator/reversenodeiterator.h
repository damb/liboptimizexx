/*! \file reversenodeiterator.h
 * \brief Declaration of a reverse node parameter space iterator class
 * template.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 12/04/2012
 * 
 * Purpose:  Declaration of a reverse node parameter space iterator class
 * template.
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
#include <optimizexx/iterator/reverseiterator.h>
#include <optimizexx/error.h>
 
#ifndef _OPTIMIZEXX_REVERSENODEITERATOR_H_
#define _OPTIMIZEXX_REVERSENODEITERATOR_H_

namespace optimize
{

  namespace iterator
  {
    // forward declarations
    template <typename Ctype, typename CresultData> class IterationMemento;
    template <typename Ctype, typename CresultData> class
      ReverseIterationState;
    template <typename Ctype, typename CresultData> class ForwardNodeIterator;

    /* ===================================================================== */
    /*!
     * Iterator to traverse backwards the parameter space only iterating over
     * nodes.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class ReverseNodeIterator : public ReverseIterator<Ctype, CresultData>
    {
      public: 
        typedef ReverseIterator<Ctype, CresultData> Tbase;
        typedef typename Tbase::Tcomp_ptr Tcomp_ptr;
        typedef typename Tbase::TstrategyPtr TstrategyPtr;

      public:
        //! constructor
        ReverseNodeIterator(Tcomp_ptr root,
            typename std::unique_ptr<IterationMemento<Ctype, CresultData>>
            iter_mode_ptr) : Tbase(root, std::move(iter_mode_ptr))
        { }
        /*!
         * copy constructor
         * 
         * \param rhs argument to copy
         */
        ReverseNodeIterator(ReverseNodeIterator<Ctype, CresultData> const& rhs)
          : Tbase(rhs)
        { }
        //! destructor
        virtual ~ReverseNodeIterator() { }
        //! set iterator to first node in parameter space composite (grid)
        virtual void first();
        //! set iterator to first node in parameter space composite (grid)
        virtual void back();
        //! go to the next node
        virtual void next();
        //! query function if iteration is finished
        virtual bool isDone() const { return Tbase::MisDone; }
        //! query function for current node
        virtual Tcomp_ptr currentItem() const
        {
          return *Tbase::MiterMemento->getCurrentIterator();
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
          return TstrategyPtr(new ReverseNodeIterator(*this));
        }

    }; // class template ReverseNodeIterator

    /* ===================================================================== */
    template <typename Ctype, typename CresultData>
    void ReverseNodeIterator<Ctype, CresultData>::first()
    {
      Tbase::MiterMemento->reset();
      if (Tbase::Mcomponent->rbegin() != Tbase::Mcomponent->rend())
      {
        Tbase::MisDone = false;
        typename std::unique_ptr<IterationState<Ctype, CresultData>> state(
          new ReverseIterationState<Ctype, CresultData>(
          Tbase::Mcomponent->rbegin(), Tbase::Mcomponent->rend()));
        Tbase::MiterMemento->pushState(std::move(state));
        while (!Tbase::MisDone && 
            (*Tbase::MiterMemento->getCurrentIterator())->getComponentType() !=
            GridComponent<Ctype, CresultData>::Leaf)
        {
          next();
        }
      } else
      {
        Tbase::MisDone = true;
      }
    } // function ReverseNodeIterator<Ctype, CresultData>::first

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ReverseNodeIterator<Ctype, CresultData>::back()
    {
      // not really efficiant but must be done as follows to guarantee
      // correct behaviour both for pre- and post-order iterators
      first();

      ForwardNodeIterator<Ctype, CresultData> iter(Tbase::Mcomponent,
          std::move(Tbase::MiterMemento->create()));
      iter.first();

      while (! Tbase::MisDone && iter.currentItem() != currentItem())
      { 
        next();
      }
    } // function ReverseNodeIterator<Ctype, CresultData>::back

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ReverseNodeIterator<Ctype, CresultData>::next()
    {
      if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
            ReverseNodeIter).isDone())
      {
        Tcomp_ptr comp_ptr = *Tbase::MiterMemento->getCurrentIterator();
        typename std::unique_ptr<IterationState<Ctype, CresultData>> state(
          new ReverseIterationState<Ctype, CresultData>(
         comp_ptr->rbegin(), comp_ptr->rend()));
        Tbase::MiterMemento->pushState(std::move(state));
      } else
      {
        Tbase::MiterMemento->next();
          
        while (! Tbase::MiterMemento->empty() && 
            Tbase::MiterMemento->iterationStateIsEnd())
        {
          Tbase::MiterMemento->popState();
          if (! Tbase::MiterMemento->empty()) { Tbase::MiterMemento->next(); }
        }

        if (Tbase::MiterMemento->empty())
        {
          Tbase::MisDone = true;
        }
      }

      while (!Tbase::MisDone && 
          (*Tbase::MiterMemento->getCurrentIterator())->getComponentType() !=
          GridComponent<Ctype, CresultData>::Leaf)
      {
        Tbase::MiterMemento->next();
      }
    } // function ReverseNodeIterator<Ctype, CresultData>:next

    /* --------------------------------------------------------------------- */

  } // namespace iterator

} // namespace optimize

#endif // include guard

/* ----- END OF reversenodeiterator.h  ----- */
