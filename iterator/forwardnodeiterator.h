/*! \file forwardnodeiterator.h
 * \brief Declaration of a class template for an forward node iterator
 * strategy.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 12/04/2012
 * 
 * Purpose:  Declaration of a class template for an forward node iterator
 * strategy.
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
#include <optimizexx/iterator/forwarditerator.h>
#include <optimizexx/error.h>
 
#ifndef _OPTIMIZEXX_FORWARDNODEITERATOR_H_
#define _OPTIMIZEXX_FORWARDNODEITERATOR_H_

namespace optimize
{
  
  namespace iterator
  {

    // forward declarations
    template <typename Ctype, typename CresultData> class IterationMemento;
    template <typename Ctype, typename CresultData> class
      ForwardIterationState;
    template <typename Ctype, typename CresultData> class ReverseNodeIterator;
      

    /* ===================================================================== */
    /*!
     * Iterator to traverse the parameter space only iterating over nodes.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class ForwardNodeIterator : public ForwardIterator<Ctype, CresultData>
    {
      public: 
        typedef ForwardIterator<Ctype, CresultData> Tbase;
        typedef typename Tbase::Tcomp_ptr Tcomp_ptr;
        typedef typename Tbase::TstrategyPtr TstrategyPtr;

      public:
        //! constructor
        ForwardNodeIterator(Tcomp_ptr root,
            typename std::unique_ptr<IterationMemento<Ctype, CresultData>>
            iter_mode_ptr) : Tbase(root, std::move(iter_mode_ptr))
        { }
        /*!
         * copy constructor
         * 
         * \param rhs argument to copy
         */
        ForwardNodeIterator(ForwardNodeIterator<Ctype, CresultData> const& rhs)
          : Tbase(rhs)
        { }
        //! destructor
        virtual ~ForwardNodeIterator() { }
        //! set iterator to first node in parameter space composite (grid)
        virtual void first();
        //! set iterator to last node in parameter space composite (grid)
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
          return TstrategyPtr(new ForwardNodeIterator(*this));
        }

    }; // class template ForwardNodeIterator

    /* ===================================================================== */
    template <typename Ctype, typename CresultData>
    void ForwardNodeIterator<Ctype, CresultData>::first()
    {
      Tbase::MiterMemento->reset();
      if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
      {
        Tbase::MisDone = false;
        typename std::unique_ptr<ForwardIterationState<Ctype, CresultData>>
          state(new ForwardIterationState<Ctype, CresultData>(
          Tbase::Mcomponent->begin(), Tbase::Mcomponent->end()));
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
    } // function ForwardNodeIterator<Ctype, CresultData>::first

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ForwardNodeIterator<Ctype, CresultData>::back()
    {
      // not really efficiant but must be done as follows to guarantee
      // correct behaviour both for pre- and post-order iterators
      first();
      
      ReverseNodeIterator<Ctype, CresultData> rev_iter(Tbase::Mcomponent,
          std::move(Tbase::MiterMemento->create()));
      rev_iter.first();

      while (! Tbase::MisDone && rev_iter.currentItem() != currentItem())
      { 
        next();
      }
    } // function ForwardNodeIterator<Ctype, CresultData>::back

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ForwardNodeIterator<Ctype, CresultData>::next()
    {
      if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
            ForwardNodeIter).isDone())
      {
        Tcomp_ptr comp_ptr = *Tbase::MiterMemento->getCurrentIterator();
        typename std::unique_ptr<ForwardIterationState<Ctype, CresultData>>
          state(new ForwardIterationState<Ctype, CresultData>(
          comp_ptr->begin(), comp_ptr->end()));
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
    } // function ForwardNodeIterator<Ctype, CresultData>::next

    /* --------------------------------------------------------------------- */

  } // namespace iterator

} // namespace optimize

#endif // include guard

/* ----- END OF forwardnodeiterator.h  ----- */
