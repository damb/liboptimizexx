/*! \file parameterspacetest.cc
 * \brief Test of parameter space (builder, composite and visitor).
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 12/03/2012
 * 
 * Purpose: Test of parameter space (builder, composite and visitor).
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
 * 12/03/2012   V0.1    Daniel Armbruster
 * 25/04/2012   V0.2    Make use of smart pointers and C++0x.
 * 
 * ============================================================================
 */
 
#include <iostream>
#include <vector>
#include <memory>
#include <optimizexx/parameter.h>
#include <optimizexx/standardbuilder.h>
#include <optimizexx/gridcomponent.h>
#include <optimizexx/application.h>

namespace opt = optimize;

int main ()
{
  typedef double TcoordType;
  typedef int TresultType;

  // create parameters
  std::shared_ptr<opt::Parameter<TcoordType> const> param1( 
    new opt::StandardParameter<TcoordType>("param1",0,10,1.));
  std::shared_ptr<opt::Parameter<TcoordType> const> param2( 
    new opt::StandardParameter<TcoordType>("param2",0,2,0.5));
  std::shared_ptr<opt::Parameter<TcoordType> const> param3( 
    new opt::StandardParameter<TcoordType>("param3",0,1.,0.25));

  std::cout << "param1: start: " << param1->getStart() 
    << " end: " << param1->getEnd() 
    << " delta: " << param1->getDelta() << std::endl;
  std::cout << "param2: start: " << param2->getStart() 
    << " end: " << param2->getEnd() 
    << " delta: " << param2->getDelta() << std::endl;
  std::cout << "param3: start: " << param3->getStart() 
    << " end: " << param3->getEnd() 
    << " delta: " << param3->getDelta() << std::endl;

  // create parameter space builder
  opt::StandardParameterSpaceBuilder<TcoordType, TresultType> builder;

  std::vector<std::shared_ptr<opt::Parameter<TcoordType> const>> params;
  params.push_back(param1);
  params.push_back(param2);
  params.push_back(param3);

  // build parameter space
  builder.buildParameterSpace();
  builder.buildGrid(params);

  // get parameter space
  std::unique_ptr<opt::GridComponent<TcoordType, TresultType>> parameterspace = 
    builder.getParameterSpace();
  
  // create parameter space application
  opt::GridCoordinateDataVisitor<TcoordType, TresultType> app(std::cout);

  // send an application into the grid
  parameterspace->accept(app);

  return 0;

} // function main

/* ----- END OF parameterspacetest.cc  ----- */
