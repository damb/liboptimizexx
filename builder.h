/*! \file builder.h
 * \brief Builder to build up a parameter space.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/02/2012
 * 
 * Purpose: Builder to build up a parameter space.
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

#include <vector>
#include <optimizexx/gridcomponent.h>
#include <optimizexx/parameter.h>
 
#ifndef _OPTIMIZEXX_BUILDER_H_
#define _OPTIMIZEXX_BUILDER_H_

namespace optimize
{
  /* ======================================================================= */
  //! \defgroup group_builder builder modul
  /*!
   * Abstract base builder class template for parameter space builders.
   * Note, that here the builder design pattern is in use (GoF p.97).\n
   * 
   * Additionally the builder design pattern is in use (GoF p.151). The abstract
   * class template optimize::ParameterSpaceBuilder corresponds to \c
   * Abstraction in GoF. There is a slightly difference because the \c
   * Abstraction here does not hold a pointer to an \c Implementor. Instead the
   * vector of optimize::Parameter pointers will be passed when calling the
   * corresponding functions.
   *
   * \ingroup group_builder
   */
  template <typename Ctype, typename CresultData>
  class ParameterSpaceBuilder
  {
    public:
      //! A component of a parameter space.
      typedef typename std::vector<Ctype> Tcomponent;

    public:
      /*!
       * query function for order of parameters the builder will generate the
       * parameter space grid
       *
       * \param number of dimensions of the parameter space
       * \return vector containing the order
       */
      virtual std::vector<int> getParameterOrder(size_t const dims)
        const = 0;
      //! Create an instance of a parameter space.
      virtual void buildParameterSpace() { }
      /*!
       * Function to build a grid.
       * Does nothing by default.
       *
       * \param parameters parameters for the parameter space.
       */
      virtual void buildGrid(std::vector<Parameter<Ctype> const*> const&
          parameters)
      { }
      /*!
       * Function to build a subgrid.
       * Does nothing by default.
       *
       * \param parameters parameters for the parameter space.
       */
      virtual void buildSubGrid(
          std::vector<Parameter<Ctype> const*> const& parameters)
      { }
      //! destructor
      virtual ~ParameterSpaceBuilder() { delete MparameterSpace; }
      /*!
       * Query function for the built product which in this case is a parameter
       * space.
       *
       * \return 0
       */
      virtual GridComponent<Ctype,CresultData>* getParameterSpace() { return 0; }

    protected:
      //! constructor
      ParameterSpaceBuilder(
          GridComponent<Ctype,CresultData>* parameterspace) : 
          MparameterSpace(parameterspace)
      { }

    protected:
      //! Pointer to the constructed parameter space.
      GridComponent<Ctype, CresultData>* MparameterSpace;

  }; // class ParameterSpaceBuilder 

} // namespace optimize

#endif // include guard

/* ----- END OF builder.h  ----- */
