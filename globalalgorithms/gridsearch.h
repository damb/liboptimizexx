/*! \file gridsearch.h
 * \brief Grid search algorithm.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 24/02/2012
 * 
 * Purpose: Grid search algorithm.
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
 * 24/02/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */

#include <vector>
#include <optimizexx/globalalgorithm.h>
#include <optimizexx/parameter.h>
#include <optimizexx/error.h>
 
#ifndef _GRIDSEARCH_H_
#define _GRIDSEARCH_H_

namespace optimize
{
  // forward declarations
  template <typename Ctype, typename CresultData> class ParameterSpaceVisitor;
  template <typename Ctype> class Parameter;

  /* ======================================================================= */
  /*!
   * Grid search algorithm.\n
   *
   * See also: <a
   * href="http://en.wikipedia.org/wiki/Grid_search">Wikipedia</a>\n
   *
   * To use this algorithm an application must be specified which is inherited
   * of ParameterSpaceVisitor.
   *
   * \ingroup group_global_algos
   */
  template <typename Ctype, typename CresultData>
  class GridSearch : public GlobalAlgorithm<Ctype, CresultData>
  {
    public:
      typedef GlobalAlgorithm<Ctype, CresultData> Tbase;
      
    public:
      /*!
       * constructor
       *
       * \param parameterspacebuilder Pointer to a builder of a parameter space.
       * Default is a StandardParameterSpaceBuilder.
       */
      GridSearch(ParameterSpaceBuilder<Ctype, CresultData>* builder) :
          Tbase(0, builder)
      { }
      /*!
       * constructor
       *
       * \param parameterspacebuilder Pointer to a builder of a parameter space.
       * Default is a StandardParameterSpaceBuilder.
       * \param parameters STL vector of parameters.
       */
      GridSearch(ParameterSpaceBuilder<Ctype, CresultData>* builder, 
          std::vector<Parameter<Ctype> > const parameters) : 
          Tbase(0, builder, parameters)
      { }
      /*!
       * Construct a parameter space. Before constructing a parameter space for
       * a global algorithm don't forget to make parameters available.
       */
      virtual void constructParameterSpace();
      /*!
       * Apply an application to the parameter space grid.
       *
       * \param v Reference to a ParameterSpaceVisitor or rather application.
       * For further information on how to implement an application for the
       * parameter space \sa ParameterSpace
       */
      virtual void execute(ParameterSpaceVisitor<Ctype, CresultData>& v);

  }; // class template GridSearch

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  void GridSearch<Ctype, CresultData>::constructParameterSpace()
  {
    OPTIMIZE_assert(Tbase::Mparameters.size() != 0, "Missing parameters.");
    Tbase::MparameterSpaceBuilder->buildParameterSpace();
    Tbase::MparameterSpaceBuilder->buildGrid(Tbase::Mparameters);
    Tbase::MparameterSpace = 
      Tbase::MparameterSpaceBuilder->getParameterSpace();
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void GridSearch<Ctype, CresultData>::execute(
      ParameterSpaceVisitor<Ctype, CresultData>& v)
  {
    OPTIMIZE_assert(Tbase::MparameterSpace != 0, "Missing parameter space.");
    // Note, that the visitor design pattern is in use (GoF p.315)
    Tbase::MparameterSpace->accept(v);
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif

/* ----- END OF gridsearch.h  ----- */
