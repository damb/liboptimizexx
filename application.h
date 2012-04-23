/*! \file application.h
 * \brief Declarations of parameter space visitor applications.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/02/2012
 * 
 * Purpose: Declarations of parameter space visitor applications. 
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

#include <ostream>

#ifndef _OPTIMIZEXX_APPLICATION_H_
#define _OPTIMIZEXX_APPLICATION_H_

namespace optimize
{
  // forward declaration
  template <typename Ctype, typename CresultData> class Grid;
  template <typename Ctype, typename CresultData> class Node;

  //! \defgroup group_applications applications modul
  /*! \brief Helds all classes, variables and functions of applications which
   * could be applied to a parameter space grid.
   */

  /* ======================================================================= */
  //! Abstract class template for an parameter space application.
  //! \ingroup group_applications
  template <typename Ctype, typename CresultData>
  class ParameterSpaceVisitor
  {
    public:
      //! Abstract visit function for a grid.
      //! \param grid Grid to be visited.
      virtual void operator()(Grid<Ctype, CresultData>* grid) = 0; 
      //! Abstract visit function for a node.
      //! \param grid Node to be visited.
      virtual void operator()(Node<Ctype, CresultData>* node) = 0; 
      //! destructor
      virtual ~ParameterSpaceVisitor() { }
    protected:
      ParameterSpaceVisitor() { }
  }; // class ParameterSpaceVisitor

  /* ======================================================================= */
  //! Visitor to print node coordinates to outputstream.
  /*!
   *  Might be convenient to visualize the grid.
   *  \ingroup group_application
   */
  template <typename Ctype, typename CresultData>
  class GridCoordinateDataVisitor : 
    public ParameterSpaceVisitor<Ctype,CresultData>
  {
    public:
      //! Constructor
      /*! 
       * \param os Outputstream to pass the coordinate data.
       */
      GridCoordinateDataVisitor(std::ostream& os) : Mos(os) { }
      /*! Visit function for a grid.
       * Does nothing by default.
       * Since a grid has no coordinates the body of this function is empty.
       *
       * \param grid Grid to be visited.
       */
      virtual void operator()(Grid<Ctype, CresultData>* grid) { }
      //! Visit function for a node.
      /*!
       * Sends the coordinate data of each node to the output stream passed in
       * the constructor.
       *
       * \param node Node to be visited.
       */
      virtual void operator()(Node<Ctype, CresultData>* node); 
    private:
      std::ostream& Mos;

  }; // class GridCoordinateDataVisitor

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  void GridCoordinateDataVisitor<Ctype,CresultData>::operator()(
      Node<Ctype,CresultData>* node)
  {
    for (auto cit = node->getCoordinates().cbegin();
        cit != node->getCoordinates().cend(); ++cit)
    {
      Mos << *cit << " ";
    }
    Mos << "\n";
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize
 
#endif // include guard

/* ----- END OF application.h  ----- */
