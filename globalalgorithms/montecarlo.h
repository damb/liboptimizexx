/*! \file montecarlo.h
 * \brief Monte Carlo Algorithm.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 16/03/2012
 * 
 * Purpose: Monte Carlo Algorithm.
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
 * 16/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  #include <random>
  #include <functional>
#else
  #include <cstdlib>
  #include <ctime>
#endif

#include <iostream>
#include <optimizexx/globalalgorithm.h>
#include <optimizexx/parameter.h>
#include <optimizexx/error.h>
#include <optimizexx/iterator.h>
 
#ifndef _MONTECARLO_H_
#define _MONTECARLO_H_

namespace optimize
{

  // forward declarations
  template <typename Ctype, typename CresultData> class ParameterSpaceVisitor;
  template <typename Ctype> class Parameter;

  /* ======================================================================= */
  //! distribution types
  enum Edistribution
  {
    UniformInt,
    Poisson,
    Exponential,
    Normal
  }; // enum Edistribution

  /* ======================================================================= */
  /*!
   * Grid search algorithm.\n
   *
   * See also: <a
   * href="http://en.wikipedia.org/wiki/Monte_Carlo_method">Wikipedia</a>\n 
   *
   * To use this algorithm an application must be specified which is inherited
   * of ParameterSpaceVisitor.\n
   * If compiling the library with \a C++11 support there is the opportunity to
   * select the probability distribution the algorithm will use to generate the
   * random numbers will be generated else only calculating values using the
   * normal/gaussian distribution will be provided.
   *
   * \todo Not tested yet.
   * See also: http://www.cplusplus.com/doc/tutorial/typecasting/
   *
   * \ingroup group_global_algos
   */
  template <typename Ctype, typename CresultData>
  class MonteCarlo : public GlobalAlgorithm<Ctype, CresultData>
  {
    public:
      typedef GlobalAlgorithm<Ctype, CresultData> Tbase;

    public:
      /*!
       * constructor
       *
       * \param parameterspacebuilder Pointer to a builder of a parameter space.
       * \param distr type of probability distribution
       */
      MonteCarlo(ParameterSpaceBuilder<Ctype, CresultData>* builder,
          Edistribution distr=Normal, float const percent=5) : 
          Tbase(0, builder), Mdistribution(distr), Mpercentage(percent)
      { 
        OPTIMIZE_assert(Mpercentage > 0 && Mpercentage <= 100.,
            "Illegal value.");
      }
      /*!
       * constructor
       *
       * \param parameterspacebuilder Pointer to a builder of a parameter space.
       * Default is a StandardParameterSpaceBuilder.
       * \param parameters STL vector of parameters.
       * \param distr type of probability distribution
       */
      MonteCarlo(ParameterSpaceBuilder<Ctype, CresultData>* builder, 
          std::vector<Parameter<Ctype> > const parameters,
          Edistribution distr=Normal, float const percent=5) :
          Tbase(0, builder, parameters), Mdistribution(distr),
          Mpercentage(percent)
      { 
        OPTIMIZE_assert(Mpercentage > 0 && Mpercentage <= 100.,
            "Illegal value.");
      }
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

    private:
      //! probability distribution
      Edistribution Mdistribution;
      //! percentage of how many nodes in a grid will be computed
      float Mpercentage;

  }; // class template MonteCarlo
  
  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  void MonteCarlo<Ctype, CresultData>::constructParameterSpace()
  {
    OPTIMIZE_assert(Tbase::Mparameters.size() != 0, "Missing parameters.");
    Tbase::MparameterSpaceBuilder->buildParameterSpace();
    Tbase::MparameterSpaceBuilder->buildGrid(Tbase::Mparameters);
    Tbase::MparameterSpace = 
      Tbase::MparameterSpaceBuilder->getParameterSpace();
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void MonteCarlo<Ctype, CresultData>::execute(
      ParameterSpaceVisitor<Ctype, CresultData>& v)
  {
    OPTIMIZE_assert(Tbase::MparameterSpace != 0, "Missing parameter space.");

    CompositeIterator<Ctype, CresultData>* iter = 
      Tbase::MparameterSpace->createIterator(NodeIter);
    iter->first();
    CompositeIterator<Ctype, CresultData>* iter_last = 
      Tbase::MparameterSpace->createIterator(NodeIter);
    iter_last->last();
    unsigned int num_elements = distance(
        *dynamic_cast<NodeIterator<Ctype, CresultData>*>(iter),
        *dynamic_cast<NodeIterator<Ctype, CresultData>*>(iter_last));
    delete iter_last;

    std::vector<unsigned int> indices((Mpercentage/100.)*num_elements);

    // generate vector with random numbers
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    unsigned int mean = num_elements/2;

    std::function<unsigned int ()> generator;
    std::tr1::mt19937 engine;
    if (UniformInt == Mdistribution)
    {
      std::tr1::uniform_int_distribution<unsigned int> ui(0,num_elements);
      generator = std::bind(ui, engine);
    } else
    if (Poisson == Mdistribution)
    {
      std::tr1::poisson_distribution<unsigned int> poisson(mean);
      generator = std::bind(poisson, engine);
    } else
    if (Exponential == Mdistribution)
    {
      std::tr1::exponential_distribution<unsigned int> e(1.0/mean);
      generator = std::bind(e, engine);
    } else
    {
      std::tr1::normal_distribution<unsigned int> nd(1.0/mean);
      generator = std::bind(ui, engine);
    }

    for (size_t i = 0; i < indices.size(); ++i)
    {
      indices[i] = generator(); 
    }
#else
    srand(time(0));
    for (size_t i = 0; i < indices.size(); ++i)
    {
      indices[i] = rand()%num_elements; 
    }
#endif
    for (std::vector<unsigned int>::const_iterator cit(indices.begin());
        cit != indices.end(); ++cit)
    {
      iter->first();
      advance(*dynamic_cast<NodeIterator<Ctype, CresultData>*>(iter), *cit);
      iter->currentItem()->accept(v);
    }
    delete iter;
  } // function MonteCarlo<Ctype, CresultData>::execute()

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard

/* ----- END OF montecarlo.h  ----- */
