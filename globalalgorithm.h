/*! \file globalalgorithm.h
 * \brief Abstract interface for a global algorithm. (declaration)
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/02/2012
 * 
 * Purpose: Abstract interface for a global algorithm. (declaration)  
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
#include <optimizexx/builder.h>
#include <optimizexx/gridcomponent.h>
#include <optimizexx/parameter.h>
#include <optimizexx/application.h>
 
#ifndef _GLOBALALGORITHM_H_
#define _GLOBALALGORITHM_H_

namespace optimize
{

  /* ======================================================================= */
  //! \defgroup group_global_algos global algorithm modul
  /*!
   * Abstract base class for an global algorithm which uses a discrete 
   * grid/parameter space to compute its results. The parameter space mechanism
   * is completely hidden for users of a global algorithm.\n
   *
   * To build a parameter space up the builder design pattern is in use
   * (GoF p.97) so that this class defines the interface for all concrete
   * clients (concrete global algorithms) in the builder design pattern.\n
   *
   * Additionally each global algorithm makes use of the visitor design pattern
   * (GoF p.315) which applies an application to the parameter space. So to
   * extract the result data of the grid either use your own result data
   * parameter space extractor application which must be inherited of the class
   * template ParameterSpaceVisitor and applied once again after applying the
   * application to the parameter space with the \c execute function or just
   * use one of a CompositeIterators provided by liboptimizexx which might be
   * more convenient and easier to handle.\n
   *
   * \ingroup group_global_algos
   */
  template <typename Ctype, typename CresultData>
  class GlobalAlgorithm
  {
    public:
      /*! Construct a parameter space.
       * Note that the builder design pattern is in use (GoF p.97).
       */
      virtual void constructParameterSpace() = 0;
      /*!
       * Note that the visitor design pattern is in use (GoF p.315). The
       * agorithm acts as a client in this case.
       *
       * \param v An application applied to the grid.
       */
      virtual void execute(ParameterSpaceVisitor<Ctype, CresultData>& v) = 0;
      //! destructor
      virtual ~GlobalAlgorithm() { }
      //! add a parameter or rather add an additional component to the grid
      void addParameter(Parameter<Ctype> const* param);
      //! query function for parameters
      std::vector<Parameter<Ctype> const*> const& getParameter() const;
      /*!
       * query function for parameter space
       *
       * \return Reference to a constant parameter space.
       */
      GridComponent<Ctype, CresultData> const& getParameterSpace() const;
      /*!
       * query function for the parameter space builder
       *
       * \return Reference to a constant parameter space builder.
       */
      ParameterSpaceBuilder<Ctype, CresultData> const&
        getParameterSpaceBuilder() const;

    protected:
      //! constructor
      GlobalAlgorithm(GridComponent<Ctype, CresultData>* parameterspace,
          ParameterSpaceBuilder<Ctype, CresultData>* builder) : 
          MparameterSpace(parameterspace), MparameterSpaceBuilder(builder)
      { }
         

      //! constructor
      GlobalAlgorithm(GridComponent<Ctype, CresultData>* parameterspace, 
          ParameterSpaceBuilder<Ctype, CresultData>* builder,
          std::vector<Parameter<Ctype> const*> parameters) :
          MparameterSpace(parameterspace), MparameterSpaceBuilder(builder),
          Mparameters(parameters) 
      { }

    protected:
      //! Pointer to the parameter space
      GridComponent<Ctype, CresultData>* MparameterSpace;
      //! Pointer to a parameter space builder
      ParameterSpaceBuilder<Ctype, CresultData>* MparameterSpaceBuilder;
      /*!
       * The parameters of which the parameter space builder will put up the
       * grid.
       */
      std::vector<Parameter<Ctype> const*> Mparameters;

  }; // class template GlobalAlgorithm

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  void GlobalAlgorithm<Ctype, CresultData>::addParameter(
      Parameter<Ctype> const* param) 
  {
    OPTIMIZE_assert(param->isValid(), "Invalid parameter.");
    Mparameters.push_back(param);
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  std::vector<Parameter<Ctype> const*> const& 
  GlobalAlgorithm<Ctype, CresultData>::getParameter() const
  {
    return Mparameters;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  GridComponent<Ctype, CresultData> const& 
  GlobalAlgorithm<Ctype, CresultData>::getParameterSpace() const
  {
    return *MparameterSpace;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ParameterSpaceBuilder<Ctype, CresultData> const&
  GlobalAlgorithm<Ctype, CresultData>::getParameterSpaceBuilder() const
  {
    return *MparameterSpaceBuilder;
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif

/* ----- END OF globalalgorithm.h  ----- */
