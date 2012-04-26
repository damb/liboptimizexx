/*! \file standardbuilder.h
 * \brief Declaration of a standard parameter space builder.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 29/02/2012
 * 
 * Purpose: Declaration of a standard parameter space builder.
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
 * 29/02/2012   V0.1    Daniel Armbruster
 * 25/04/2012   V0.2    Make use of smart pointers and C++0x.
 * 
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <cmath>
#include <optimizexx/builder.h>
#include <optimizexx/grid.h>
#include <optimizexx/node.h>
#include <optimizexx/error.h>
 
#ifndef _OPTIMIZEXX_STANDARDBUILDER_H_
#define _OPTIMIZEXX_STANDARDBUILDER_H_

namespace optimize
{
  // forward declarations
  template <typename Ctype, typename CresultData> class GridComponent;
  template <typename Ctype> class Parameter;

  /* ======================================================================= */
  /*!
   * Concrete builder class template for a standard parameter space. Note, that
   * here the builder design pattern is in use (GoF p.97).
   *
   * This builder builds a usual grid only consisting of nodes without the
   * opportunity to add a subgrid to this grid.\n
   *
   * Additionally the builder design pattern is in use (GoF p.151). The abstract
   * class template optimize::ParameterSpaceBuilder corresponds to \c
   * RefinedAbstraction in GoF.
   *
   * \ingroup group_builder
   */
  template<typename Ctype, typename CresultData>
  class StandardParameterSpaceBuilder : 
      public ParameterSpaceBuilder<Ctype, CresultData>
  {
    public:
      typedef ParameterSpaceBuilder<Ctype, CresultData> Tbase;
      typedef typename Tbase::Tcomponent Tcomponent;

    public:
      //! constructor
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
      StandardParameterSpaceBuilder() : Tbase(nullptr)
#else
      StandardParameterSpaceBuilder()
#endif
      { }
      /*!
       * query function for order of parameters the builder will generate the
       * parameter space grid
       *
       * \param dims number of dimensions of the parameter space
       * \return vector containing the order
       */
      virtual std::vector<int> getParameterOrder(size_t const dims) const;
      //! Create an instance of a parameter space.
      virtual void buildParameterSpace();
      /*!
       * Build a \c N dimensional grid where \c N is the size of the parameter
       * vector passed.
       *
       * \param parameters Parameters of the parameter space.
       */
      virtual void buildGrid(
          typename std::vector<std::shared_ptr<Parameter<Ctype> const>> const&
          parameters);
      //! destructor
      virtual ~StandardParameterSpaceBuilder() { }
      /*!
       * Query function for the standard parameter space.
       *
       * \return The parameter space built by the
       * StandardParameterSpaceBuilder.
       */
      virtual typename
        std::unique_ptr<GridComponent<Ctype, CresultData>> getParameterSpace();

  }; // class StandardParameterSpaceBuilder

  /* ======================================================================= */
  template<typename Ctype, typename CresultData> 
  void StandardParameterSpaceBuilder<Ctype, CresultData>::buildParameterSpace()
  {
    Tbase::MparameterSpace.reset(new Grid<Ctype, CresultData>); 
  }

  /* ----------------------------------------------------------------------- */
  template<typename Ctype, typename CresultData> 
  std::vector<int> 
  StandardParameterSpaceBuilder<Ctype, CresultData>::getParameterOrder(
      size_t const dims) const
  {
    std::vector<int> retval(dims);
    for (size_t i = 0; i < dims; ++i) { retval[i] = i; }
    return retval;
  } // function StandardParameterSpaceBuilder::getParameterOrder

  /* ----------------------------------------------------------------------- */
  template<typename Ctype, typename CresultData> 
  void StandardParameterSpaceBuilder<Ctype, CresultData>::buildGrid(
      typename std::vector<std::shared_ptr<Parameter<Ctype> const>> const&
      parameters)
  {
    size_t dimension = parameters.size();      

    // coordinate ids of parameter space
    std::vector<std::string> coordIds;
    coordIds.reserve(dimension);
  for (auto cit(parameters.cbegin()); cit != parameters.cend(); ++cit)
    {
      OPTIMIZE_assert((*cit)->isValid(), "Invalid parameter");
      if (! (*cit)->getId().empty())
      {
        coordIds.push_back((*cit)->getId());
      } else
      {
        coordIds.push_back("Unkown");
      }
    }
    Tbase::MparameterSpace->setCoordinateId(coordIds);

    // generate parameter vectors
    typename std::vector<Tcomponent> components;
    components.reserve(dimension);

    for (auto cit(parameters.cbegin()); cit != parameters.cend(); ++cit)
    {
      // number of samples - round up
      size_t samples = (*cit)->getSamples();

      Tcomponent component;
      component.reserve(samples);
      // generate samples
      Ctype val = (*cit)->getStart();

      for (size_t s = 0; s < samples; ++s)
      {
        component.push_back(val);
        val += (*cit)->getDelta();
      }

      components.push_back(component);
      component.clear();
    }

    // vector of component iterators
    typename std::vector<typename Tcomponent::const_iterator> iterators;
    for (auto cit(components.cbegin()); cit != components.cend(); ++cit)
    {
      typename Tcomponent::const_iterator i(cit->begin());
      iterators.push_back(i);
    }
    // add nodes to grid - generate all possible combinations of coordinates
    while (true)
    {
      // generate node
      while (iterators[0] != components[0].end())
      {
        Tcomponent coordinates;
        for (auto cit(iterators.cbegin()); cit != iterators.cend(); ++cit)
        {
          coordinates.push_back(**cit);
        }

        Tbase::MparameterSpace->add(new Node<Ctype,CresultData>(coordinates));
        ++iterators[0];
      }
      // reset
      iterators[0] = components[0].begin();

      size_t k;
      // update incrementation
      for (k = 1; k < components.size(); ++k)
      {
        ++iterators[k];
        if (iterators[k] != components[k].end()) { break; }
        iterators[k] = components[k].begin();
      }
      if (components.size() == k) { break; }
    }
  } // function StandardParameterSpaceBuilder<Ctype, CresultData>::buildGrid 

  /* ----------------------------------------------------------------------- */
  template<typename Ctype, typename CresultData> 
  typename std::unique_ptr<GridComponent<Ctype,CresultData>>
  StandardParameterSpaceBuilder<Ctype, CresultData>::getParameterSpace()
  {
    return std::move(Tbase::MparameterSpace);
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard

/* ----- END OF standardbuilder.h  ----- */
