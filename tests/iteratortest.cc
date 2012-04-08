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
 * 13/03/2012  V0.1  Daniel Armbruster
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
  opt::Parameter<TcoordType> const* param1 =
    new opt::StandardParameter<TcoordType>("param1",0,1.,0.5);
  opt::Parameter<TcoordType> const* param2 =
    new opt::StandardParameter<TcoordType>("param2",-1,1.,1.);
  
  std::vector<optimize::Parameter<TcoordType> const*> params;
  params.push_back(param1);
  params.push_back(param2);

  // create parameter space builder
  opt::ParameterSpaceBuilder<TcoordType, TresultType>* builder =
      new opt::StandardParameterSpaceBuilder<TcoordType, TresultType>;

  // build parameter space
  builder->buildParameterSpace();
  builder->buildGrid(params);

  // get parameter space
  opt::GridComponent<TcoordType, TresultType>* parameterSpace = 
    builder->getParameterSpace();

  // test usual iterator
  

  std::cout << "---------------------------------------" << std::endl;
  std::cout << "Testing usual parameter space iterator:" << std::endl;
  std::cout << "---------------------------------------" << std::endl;
  opt::CompositeIterator<TcoordType, TresultType>* it = 
    parameterSpace->createIterator(opt::Iter);

  it->first();

  while (!it->isDone())
  {
    std::vector<std::string> const& c = it->currentItem()->getCoordinateId();
    for (std::vector<std::string>::const_iterator cit(c.begin());
        cit != c.end(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;

    it->next();
  }

  delete it;

  std::cout << "----------------------" << std::endl;
  std::cout << "Testing node iterator:" << std::endl;
  std::cout << "----------------------" << std::endl;
  it = parameterSpace->createIterator(opt::NodeIter);

  it->first();

  while (!it->isDone())
  {
    std::vector<TcoordType> const& c = it->currentItem()->getCoordinates();
    std::vector<std::string> const& ids = it->currentItem()->getCoordinateId();

    for (std::vector<TcoordType>::const_iterator cit(c.begin());
        cit != c.end(); ++cit)
    {
      std::cout << *cit << " ";
    }

    for (std::vector<std::string>::const_iterator cit(ids.begin());
        cit != ids.end(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;

    it->next();
  }

  delete it;

  std::cout << "------------------------------" << std::endl;
  std::cout << "Testing reverse node iterator:" << std::endl;
  std::cout << "------------------------------" << std::endl;
  it = parameterSpace->createIterator(opt::RevNodeIter);

  it->first();

  while (!it->isDone())
  {
    std::vector<TcoordType> const& c = it->currentItem()->getCoordinates();
    std::vector<std::string> const& ids = it->currentItem()->getCoordinateId();

    for (std::vector<TcoordType>::const_iterator cit(c.begin());
        cit != c.end(); ++cit)
    {
      std::cout << *cit << " ";
    }

    for (std::vector<std::string>::const_iterator cit(ids.begin());
        cit != ids.end(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;

    it->next();
  }

  delete it;


  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "Testing node iterator (overloaded operators):" << std::endl;
  std::cout << "---------------------------------------------" << std::endl;
  it = parameterSpace->createIterator(opt::NodeIter);
  
  opt::CompositeIterator<TcoordType, TresultType>* end = 
    parameterSpace->createIterator(opt::NodeIter);
  end->last();


  // TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
  // using operator!=() produces segmenation fault 
  // TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
  // unconvenient but working solution
  // polymorphism does not work anymore if dereferencing the pointer to the
  // iterator -> dynamic cast necessary
/*  for (it->first(); 
      *dynamic_cast<opt::ForwardNodeIterator<TcoordType, TresultType>*>(it)!=
      *dynamic_cast<opt::ForwardNodeIterator<TcoordType, TresultType>*>(end);
      ++(*it))*/
  for (it->first(); !it->isDone(); ++(*it))
  {
    std::vector<TcoordType> const& c = (**it)->getCoordinates();
    std::vector<std::string> const& ids = (**it)->getCoordinateId();

    for (std::vector<TcoordType>::const_iterator cit(c.begin());
        cit != c.end(); ++cit)
    {
      std::cout << *cit << " ";
    }

    for (std::vector<std::string>::const_iterator cit(ids.begin());
        cit != ids.end(); ++cit)
    {
      std::cout << *cit << " ";
    }
    std::cout << std::endl;
  }

  delete end;
  delete it;
  delete builder;
  delete param1;
  delete param2;

  return 0;

} // function main

/* ----- END OF iteratortest.cc  ----- */
