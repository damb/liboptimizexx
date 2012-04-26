/*! \file iteratortest.cc
 * \brief Iterator testing for parameter space of liboptimizexx.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 13/03/2012
 * 
 * Purpose: Iterator testing for parameter space of liboptimizexx.  
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
 * 25/04/2012   V0.2    Make use of smart pointers and C++0x.
 * 
 * ============================================================================
 */

/*
 * IMPORTANT NOTES AND MISSING TESTS:
 * 1. GridIterator test missing
 * 2. All pre-order iterator tests missing
 * 3. IteratorPtr test missing
 */

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <optimizexx/parameter.h>
#include <optimizexx/standardbuilder.h>
#include <optimizexx/gridcomponent.h>
#include <optimizexx/iterator.h>
 
namespace opt = optimize;

int main ()
{
  typedef double TcoordType;
  typedef double TresultType;
  // create parameters
  std::shared_ptr<opt::Parameter<TcoordType> const> param1( 
    new opt::StandardParameter<TcoordType>("param1",0,1.,0.5));
  std::shared_ptr<opt::Parameter<TcoordType> const> param2( 
    new opt::StandardParameter<TcoordType>("param2",-1,1.,1.));
  
  std::vector<std::shared_ptr<optimize::Parameter<TcoordType> const>> params;
  params.push_back(param1);
  params.push_back(param2);

  // create parameter space builder
  std::unique_ptr<opt::ParameterSpaceBuilder<TcoordType, TresultType>> builder(
      new opt::StandardParameterSpaceBuilder<TcoordType, TresultType>);

  // build parameter space
  builder->buildParameterSpace();
  builder->buildGrid(params);

  // get parameter space
  std::unique_ptr<opt::GridComponent<TcoordType, TresultType>> parameterSpace = 
    builder->getParameterSpace();

  // test usual iterator
  

  std::cout << "---------------------------------------" << std::endl;
  std::cout << "Testing usual parameter space iterator:" << std::endl;
  std::cout << "---------------------------------------" << std::endl;
  opt::Iterator<TcoordType, TresultType> it(
      parameterSpace->createIterator(opt::ForwardIter));

  it.first();

  while (!it.isDone())
  {
    std::vector<std::string> const& c = (*it)->getCoordinateId();
    for (std::vector<std::string>::const_iterator cit(c.begin());
        cit != c.end(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;

    it.next();
  }

  std::cout << "----------------------" << std::endl;
  std::cout << "Testing node iterator:" << std::endl;
  std::cout << "----------------------" << std::endl;
  it = parameterSpace->createIterator(opt::ForwardNodeIter);

  it.first();

  while (!it.isDone())
  {
    std::vector<TcoordType> const& c = (*it)->getCoordinates();
    std::vector<std::string> const& ids = (*it)->getCoordinateId();

    for (auto cit(c.cbegin()); cit != c.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }

    for (auto cit(ids.cbegin()); cit != ids.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;

    it.next();
  }

  std::cout << "------------------------------" << std::endl;
  std::cout << "Testing reverse node iterator:" << std::endl;
  std::cout << "------------------------------" << std::endl;
  it = parameterSpace->createIterator(opt::ReverseNodeIter);

  it.first();

  while (!it.isDone())
  {
    std::vector<TcoordType> const& c = (*it)->getCoordinates();
    std::vector<std::string> const& ids = (*it)->getCoordinateId();

    for (auto cit(c.cbegin()); cit != c.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }

    for (auto cit(ids.cbegin()); cit != ids.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;

    it.next();
  }


  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "Testing node iterator (overloaded operators):" << std::endl;
  std::cout << "---------------------------------------------" << std::endl;
  it = parameterSpace->createIterator(opt::ForwardIter);

  opt::Iterator<TcoordType, TresultType> end_it(
      parameterSpace->createIterator(opt::ForwardIter));
  end_it.back();

  for (it.first(); it != end_it; ++it)
  {
    std::vector<TcoordType> const& c = (*it)->getCoordinates();
    std::vector<std::string> const& ids = (*it)->getCoordinateId();

    for (auto cit(c.cbegin()); cit != c.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }

    for (auto cit(ids.cbegin()); cit != ids.cend(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;
  }

  return 0;

} // function main

/* ----- END OF iteratortest.cc  ----- */
