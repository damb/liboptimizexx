/*! \file node.h
 * \brief Declaration of a parameter space node.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 29/02/2012
 * 
 * Purpose: Declaration of a parameter space node.  
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
 * 29/02/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */

#include <vector>
#include <ostream>
#include <optimizexx/gridcomponent.h>


#ifndef _OPTIMIZEXX_NODE_H_
#define _OPTIMIZEXX_NODE_H_

namespace optimize
{

  // forward declaration
  template <typename Ctype, typename CresultData> class ParameterSpaceVisitor;

  /* ======================================================================= */
  /*!
   * Declares a node of a parameter space. It's coordinates represent the
   * parameters of the parameter space. Additionally holds the result data
   * after an application had visited the node.
   * Leaf class of the composite design pattern (GoF p.163).
   *
   * \ingroup group_grid
   */
  template <typename Ctype, typename CresultData>
  class Node : public GridComponent<Ctype,CresultData>
  {
    public:
      //! Base class.
      typedef GridComponent<Ctype, CresultData> Tbase; 
      //! typedef for coordinates
      typedef typename std::vector<Ctype> Tcoordinates;

    public:
      //! constructor
      /*!
       * \param coordinates The coordinates of the node.
       */
      Node(Tcoordinates const coordinates);
      //! destructor
      virtual ~Node() { }
      /*! 
       * Visitor acceptance function for a parameter space visitor.
       * Note that here the visitor design pattern is in use (GoF p.315)
       *
       * \param v The application which visits the parameter space respectively
       * the node.
       */
      virtual void accept(ParameterSpaceVisitor<Ctype, CresultData>& v);
      //! query function if node results had been computed
      virtual bool isComputed() const { return Tbase::Mcomputed; }
      /*! 
       * Set this flag if the node has been computed and the data had been
       * stored within CresultData.
       */
      virtual void setComputed() { Tbase::Mcomputed = true; }
      //! query function for the type of the grid component
      virtual typename Tbase::EcomponentType getComponentType() const 
      { 
        return Tbase::Leaf;
      }
      //! query function for the nodes' coordinates
      virtual Tcoordinates const& getCoordinates() const
      {
        return Mcoordinates;
      }
      //! query function for coordinate Ids
      virtual std::vector<std::string> const& getCoordinateId() const;
      //! query function for result data
      virtual CresultData const& getResultData() const { return MresultData; }
      //! Set the result data of the grid component.
      virtual void setResultData(CresultData const data) 
      { 
        MresultData = data;
      }

    private:
      //! Coordinates of the node.
      Tcoordinates Mcoordinates;
      //! Template parameter to store the results of the calculation.
      CresultData MresultData;

  }; // class template Node

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  Node<Ctype, CresultData>::Node(Tcoordinates const coordinates) : 
    Tbase(0, false), Mcoordinates(coordinates)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Node<Ctype, CresultData>::accept(
      ParameterSpaceVisitor<Ctype, CresultData>& v)
  {
    v(this);
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  std::vector<std::string> const& 
  Node<Ctype, CresultData>::getCoordinateId() const
  {
    OPTIMIZE_assert(0 != Tbase::Mparent, "No coordinate Ids available.");
    return Tbase::Mparent->getCoordinateId();
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize
 
#endif // include guard

/* ----- END OF node.h  ----- */
