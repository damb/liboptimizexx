/*! \file gridsearchtest.cc
 * \brief Test gridsearch algorithm.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 13/03/2012
 * 
 * Purpose: Test gridsearch algorithm.
 *
 * See also: 
 *    http://en.wikipedia.org/wiki/Grid_search
 *    http://en.wikipedia.org/wiki/Brute-force_search
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
 * 13/03/2012   V0.1    Daniel Armbruster
 * 09/04/2012   V0.2    Both multithreading and single threading test.
 * 25/04/2012   V0.3    Make use of smart pointers and C++0x.
 * 
 * ============================================================================
 */
 

#include <iostream>
#include <vector>
#include <memory>
#include <optimizexx/parameter.h>
#include <optimizexx/standardbuilder.h>
#include <optimizexx/application.h>
#include <optimizexx/globalalgorithms/gridsearch.h>

namespace opt = optimize;

typedef double TcoordType;
typedef double TresultType;

//! simple application calculating the sum of the parameters
class Sum : public opt::ParameterSpaceVisitor<TcoordType, TresultType>
{
  public:
    /*! Visit function for a grid.
     * Does nothing by default.
     * Since a grid has no coordinates the body of this function is empty.
     *
     * \param grid Grid to be visited.
     */
    virtual void operator()(opt::Grid<TcoordType, TresultType>* grid) { }
      //! Visit function / application for a node.
      /*!
       * Calculates the sum of the coordinate data or rather the parameters
       *
       * \param node Node to be visited.
       */
    virtual void operator()(opt::Node<TcoordType, TresultType>* node)
    {
      std::vector<TcoordType> const& params = node->getCoordinates();
      TresultType result = 0;
      for (auto cit(params.cbegin()); cit != params.cend(); ++cit)
      {
        result += *cit;
      }
      node->setResultData(result);
    }
}; // class Sum

int main()
{
  // create parameters
  std::shared_ptr<opt::Parameter<TcoordType> const> param1( 
    new opt::StandardParameter<TcoordType>("param1",0,1.,0.25));
  std::shared_ptr<opt::Parameter<TcoordType> const> param2( 
    new opt::StandardParameter<TcoordType>("param2",-1,1.,0.5));
  std::shared_ptr<opt::Parameter<TcoordType> const> param3( 
    new opt::StandardParameter<TcoordType>("param3",-1,1.,0.05));
  
  std::vector<std::shared_ptr<opt::Parameter<TcoordType> const>> params;
  params.push_back(param1);
  params.push_back(param2);
  params.push_back(param3);

  // create parameter space builder
  std::unique_ptr<opt::ParameterSpaceBuilder<TcoordType, TresultType>> builder(
    new opt::StandardParameterSpaceBuilder<TcoordType, TresultType>);

  // create gridsearch algorithm (multiple threads)
  std::unique_ptr<opt::GridSearch<TcoordType, TresultType>> gridsearch(
    new opt::GridSearch<TcoordType, TresultType>(std::move(builder), params));

  gridsearch->constructParameterSpace();

  // create application
  Sum app;

  std::cout << "-----------------------\n"
    << "Multiple threads in use\n"
    << "-----------------------" << std::endl;

  gridsearch->execute(app);

  // print results using a node iterator
  opt::Iterator<TcoordType, TresultType> it(
      gridsearch->getParameterSpace().createIterator(opt::ForwardNodeIter));

  for (it.first(); !it.isDone(); ++it)
  {
    std::vector<TcoordType> const& c = (*it)->getCoordinates();
    for (auto cit(c.cbegin()); cit != c.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << (*it)->getResultData() << std::endl;
  }

  builder.reset(
      new opt::StandardParameterSpaceBuilder<TcoordType, TresultType>);
  // single threading test
  gridsearch.reset(new opt::GridSearch<TcoordType, TresultType>(
      std::move(builder), params, false));

  gridsearch->constructParameterSpace();

  std::cout << "--------------------\n"
    << "Single thread in use\n"
    << "--------------------" << std::endl;

  gridsearch->execute(app);
  for (it.first(); !it.isDone(); ++it)
  {
    std::vector<TcoordType> const& c = (*it)->getCoordinates();
    for (auto cit(c.cbegin()); cit != c.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << (*it)->getResultData() << std::endl;
  }

  return 0;
} // function main

/* ----- END OF gridsearchtest.cc  ----- */
