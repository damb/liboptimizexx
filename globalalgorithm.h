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
 * 20/02/2012  V0.1   Daniel Armbruster
 * 25/04/2012  V0.2   Make use of smart pointers and C++0x.
 * 
 * ============================================================================
 */

#include <vector>
#include <memory>
#include <optimizexx/builder.h>
#include <optimizexx/gridcomponent.h>
#include <optimizexx/parameter.h>
#include <optimizexx/application.h>
 
#ifndef _OPTIMIZEXX_GLOBALALGORITHM_H_
#define _OPTIMIZEXX_GLOBALALGORITHM_H_

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
      void addParameter(typename 
          std::shared_ptr<Parameter<Ctype> const> param);
      /*!
       * query function for parameter space
       *
       * \return Reference to a constant parameter space.
       */
      GridComponent<Ctype, CresultData> const& getParameterSpace() const;
      /*!
       * query function for the parameter space builder\n
       * This function especially is util for users to add parameters in the
       * correct order so that actually a concrete instance of
       * optimize::ParameterSpaceVisitor is able to handle the parameter space
       * coordinates appropriately.
       *
       * \return Reference to a constant parameter space builder.
       */
      ParameterSpaceBuilder<Ctype, CresultData> const&
        getParameterSpaceBuilder() const;
      /*!
       * query function for dimension of parameter space
       *
       * This function will return the dimension of the base parameter space,
       * independently how many dimensions possibly available subgrids possess.
       *
       * \return dimension of base parameter space
       */
      size_t getParameterSpaceDimensions() const { return Mparameters.size(); }

    protected:
      //! constructor
      GlobalAlgorithm(
          std::unique_ptr<GridComponent<Ctype, CresultData>> parameterspace,
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder) : 
          MparameterSpace(std::move(parameterspace)),
          MparameterSpaceBuilder(std::move(builder))
      { }
         

      //! constructor
      GlobalAlgorithm(
          std::unique_ptr<GridComponent<Ctype, CresultData>> parameterspace, 
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder,
          std::vector<std::shared_ptr<Parameter<Ctype> const>> parameters) :
          MparameterSpace(std::move(parameterspace)),
          MparameterSpaceBuilder(std::move(builder)),
          Mparameters(parameters) 
      { 
        for (auto cit(Mparameters.cbegin()); cit != Mparameters.cend(); ++cit)
        {
          OPTIMIZE_assert((*cit)->isValid(), "Invalid parameter.");
        }
      }

#if __GNUC__ >= 4 && __GNUC_MINOR__ <= 5
      //! constructor
      GlobalAlgorithm(        
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder) : 
          MparameterSpace(0),
          MparameterSpaceBuilder(std::move(builder))
      { }
         

      //! constructor
      GlobalAlgorithm(
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder,
          std::vector<std::shared_ptr<Parameter<Ctype> const>> parameters) :
          MparameterSpace(0),
          MparameterSpaceBuilder(std::move(builder)),
          Mparameters(parameters) 
      { 
        for (auto cit(Mparameters.cbegin()); cit != Mparameters.cend(); ++cit)
        {
          OPTIMIZE_assert((*cit)->isValid(), "Invalid parameter.");
        }
      }

#endif

    protected:
      //! Pointer to the parameter space
      std::unique_ptr<GridComponent<Ctype, CresultData>> MparameterSpace;
      //! Pointer to a parameter space builder
      std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>>
        MparameterSpaceBuilder;
      /*!
       * The parameters of which the parameter space builder will put up the
       * grid.
       */
      std::vector<std::shared_ptr<Parameter<Ctype> const>> Mparameters;

  }; // class template GlobalAlgorithm

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  void GlobalAlgorithm<Ctype, CresultData>::addParameter(
      typename std::shared_ptr<Parameter<Ctype> const> param) 
  {
    OPTIMIZE_assert(param->isValid(), "Invalid parameter.");
    Mparameters.push_back(param);
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

#endif // include guard

/* ----- END OF globalalgorithm.h  ----- */
