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
 * 16/03/2012   V0.1    Daniel Armbruster
 * 18/04/2012   V0.2    Completely use C++0x.
 * 25/04/2012   V0.3    Make use of smart pointers and C++0x.
 * 
 * ============================================================================
 */

#include <iostream>
#include <memory>
#include <random>
#include <functional>
#include <optimizexx/globalalgorithm.h>
#include <optimizexx/parameter.h>
#include <optimizexx/error.h>
#include <optimizexx/iterator.h>
 
#ifndef _OPTIMIZEXX_MONTECARLO_H_
#define _OPTIMIZEXX_MONTECARLO_H_

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
       * \param builder Pointer to a builder of a parameter space.
       * \param distr type of probability distribution
       */
      MonteCarlo(
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder,
          Edistribution distr=Normal, float const percent=5) : 
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
          Tbase(nullptr, std::move(builder)), Mdistribution(distr),
#else
          Tbase(std::move(builder)), Mdistribution(distr),
#endif
          Mpercentage(percent)
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
      MonteCarlo(
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder,
          std::vector<std::shared_ptr<Parameter<Ctype> const>> const parameters,
          Edistribution distr=Normal, float const percent=5) :
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
          Tbase(nullptr, std::move(builder), parameters), Mdistribution(distr),
#else
          Tbase(std::move(builder), parameters), Mdistribution(distr),
#endif
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
       *
       * \todo Does not work properly yet. To determine the size of the
       * parameter space composite I have to implement the
       * optimize::iterator::ForwardNodeIterator::last function.
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
    OPTIMIZE_assert(Tbase::MparameterSpace, "Missing parameter space.");

    Iterator<Ctype, CresultData> iter(
        Tbase::MparameterSpace->createIterator(ForwardNodeIter));
    iter.first();
    Iterator<Ctype, CresultData> iter_last(
        Tbase::MparameterSpace->createIterator(ForwardNodeIter));
    iter_last.back();

    unsigned int num_elements =
      distance<Iterator<Ctype, CresultData>, Iterator<Ctype, CresultData>>(
          iter, iter_last);

    std::vector<unsigned int> indices((Mpercentage/100.)*num_elements);

    // generate vector with random numbers
    unsigned int mean = num_elements/2;

    std::function<double()> generator;
    std::random_device generate_seed;
    std::mt19937 engine(generate_seed());
    if (UniformInt == Mdistribution)
    {
      std::uniform_int_distribution<unsigned int> ui(0,num_elements);
      generator = std::bind(ui, engine);
    } else
    if (Poisson == Mdistribution)
    {
      std::poisson_distribution<unsigned int> poisson(mean);
      generator = std::bind(poisson, engine);
    } else
    if (Exponential == Mdistribution)
    {
      std::exponential_distribution<float> e(1.0/mean);
      generator = std::bind(e, engine);
    } else
    {
      std::normal_distribution<float> nd(mean, mean/3.);
      generator = std::bind(nd, engine);
    }

    for (size_t i = 0; i < indices.size(); ++i)
    {
      indices[i] = std::round(generator()); 
    }

    for (auto cit(indices.cbegin()); cit != indices.cend(); ++cit)
    {
      iter.first();
      advance<Iterator<Ctype, CresultData>>(iter, *cit);
      (*iter)->accept(v);
    }
  } // function MonteCarlo<Ctype, CresultData>::execute()

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard

/* ----- END OF montecarlo.h  ----- */
