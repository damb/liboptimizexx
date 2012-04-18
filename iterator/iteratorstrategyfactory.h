/*! \file iteratorstrategyfactory.h
 * \brief Declaration of a factory class for parameter space iterator
 * strategies.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 12/04/2012
 * 
 * Purpose:  Declaration of a factory class for parameter space iterator
 * strategies.
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
#include <optimizexx/iterator/iterationmemento.h>
#include <optimizexx/iterator/compositeiterator.h>
#include <optimizexx/iterator/forwarditerator.h>
#include <optimizexx/iterator/forwardnodeiterator.h>
#include <optimizexx/iterator/forwardgriditerator.h>
#include <optimizexx/iterator/reverseiterator.h>
#include <optimizexx/iterator/reversenodeiterator.h>
#include <optimizexx/iterator/reversegriditerator.h>
#include <optimizexx/iterator/nulliterator.h>

#ifndef _OPTIMIZEXX_ITERATORFACTORY_H_
#define _OPTIMIZEXX_ITERATORFACTORY_H_

namespace optimize
{
  // forward declarations
  template <typename Ctype, typename CresultData> class GridComponent;

  namespace iterator
  {

    /* ===================================================================== */
    /*
     * \brief iterator strategy factory
     *
     * Class providing a factory method for parameter space iterator
     * strategies.\n
     * Decouples parameter space iterator strategies from the grid
     * representation.\n
     *
     * Idea got from
     * <a href="http://www.freiesmagazin.de/freiesMagazin-2012-04">\c
     * freiesMagazin</a>.
     *
     * \ingroup group_iterator
     */
    template <typename Ctype, typename CresultData>
    class IteratorStrategyFactory
    {
      public:
        /*!
         * parameterized factory function for an composite iterator strategy
         *
         * \param iter_type type of iterator
         * \param iter_mode mode of iteration
         * \param grid_comp root parameter space component
         */
        std::unique_ptr<CompositeIterator<Ctype, CresultData>>
          makeIteratorStrategy(EiteratorType iter_type,
              EiterationMode iter_mode, 
              GridComponent<Ctype, CresultData>* grid_comp);

      private:
        /*!
         * parameterized helper factory function for an iteration mode
         *
         * \param iter_mode pointer to mode of iteration
         */
        std::unique_ptr<IterationMemento<Ctype, CresultData>>
          makeIterationMemento(EiterationMode iter_mode);

    }; // class template IteratorStrategyFactory

    /* ===================================================================== */
    template <typename Ctype, typename CresultData>
    std::unique_ptr<CompositeIterator<Ctype, CresultData>>
    IteratorStrategyFactory<Ctype, CresultData>::makeIteratorStrategy(
        EiteratorType iter_type, EiterationMode iter_mode,
        GridComponent<Ctype, CresultData>* grid_comp)
    {
      std::unique_ptr<IterationMemento<Ctype, CresultData>> mode =
        makeIterationMemento(iter_mode);

      typedef typename std::unique_ptr<CompositeIterator<Ctype, CresultData>>
        TstrategyPtr;

      if (EiteratorType::ForwardIter == iter_type)
      {
        return TstrategyPtr(new ForwardIterator<Ctype, CresultData>(
              grid_comp, std::move(mode)));
      } else
      if (EiteratorType::ForwardNodeIter == iter_type)
      {
        return TstrategyPtr(new ForwardNodeIterator<Ctype, CresultData>(
              grid_comp, std::move(mode)));
      } else
      if (EiteratorType::ForwardGridIter == iter_type)
      {
        return TstrategyPtr(new ForwardGridIterator<Ctype, CresultData>(
              grid_comp, std::move(mode)));
      } else
      if (EiteratorType::ReverseIter == iter_type)
      {
        return TstrategyPtr(new ReverseIterator<Ctype, CresultData>(
              grid_comp, std::move(mode)));
      } else
      if (EiteratorType::ReverseNodeIter == iter_type)
      {
        return TstrategyPtr(new ReverseNodeIterator<Ctype, CresultData>(
              grid_comp, std::move(mode)));
      } else
      if (EiteratorType::ReverseGridIter == iter_type)
      {
        return TstrategyPtr(new ReverseGridIterator<Ctype, CresultData>(
              grid_comp, std::move(mode)));
      } else
      {
        return TstrategyPtr(new NullIterator<Ctype, CresultData>(grid_comp));
      }

    } // function IteratorStrategyFactory<Ctype, CresultData>::makeIterator

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    std::unique_ptr<IterationMemento<Ctype, CresultData>>
    IteratorStrategyFactory<Ctype, CresultData>::makeIterationMemento(
        EiterationMode iter_mode)
    {
      if (EiterationMode::PreOrder == iter_mode)
      {
        return std::unique_ptr<PreIterationMemento<Ctype, CresultData>>(
            new PreIterationMemento<Ctype, CresultData>);
      } else
      {
        return std::unique_ptr<PostIterationMemento<Ctype, CresultData>>(
            new PostIterationMemento<Ctype, CresultData>);
      }
    } // function IteratorStrategyFactory<Ctype, CresultData>::
    //makeIterationMemento

  } // namespace iterator

} // namespace optimize

#endif // include guard

/* ----- END OF iteratorstrategyfactory.h  ----- */
