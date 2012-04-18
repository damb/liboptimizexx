/*! \file gridcomponent.h
 * \brief Parameter space grid.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/02/2012
 * 
 * Purpose:  Parameter space grid.
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
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <optimizexx/application.h>
#include <optimizexx/parameter.h>
#include <optimizexx/iterator.h>
#include <optimizexx/iterator/iteratorstrategyfactory.h>
#include <optimizexx/error.h>
 
#ifndef _GRIDCOMPONENT_H_
#define _GRIDCOMPONENT_H_

namespace optimize
{
  /* ======================================================================= */
  //! \defgroup group_grid grid modul
  /*! 
   * Abstract base class for a grid component
   * Note that here the Composite design pattern is in use (GoF p.163)
   * Publicity approach chosen so that this class provides the entire
   * interface.
   * By default several functions throw Exceptions at runtime if a function of
   * a component is called which had not been redefined by an inherited grid
   * component.
   *
   * \ingroup group_grid
   */
  template <typename Ctype, typename CresultData>
  class GridComponent
  {
    public:
      //! Types of grid components.
      enum EcomponentType
      {
        Leaf,
        Composite
      };
      //! Usual iterator for the children of a composite.
      typedef typename std::list<GridComponent<Ctype, CresultData>*>::iterator
        Titer;
      //! Constant iterator for the children of a composite.
      typedef typename
        std::list<GridComponent<Ctype, CresultData>*>::const_iterator
        Tconst_iter;
      //! Reverse iterator for children of a composite.
      typedef typename std::list<
        GridComponent<Ctype, CresultData>*>::reverse_iterator Treverse_iter;
      //! Constant reverse iterator for children of a composite.
      typedef typename std::list<
        GridComponent<Ctype, CresultData>*>::const_reverse_iterator
        Tconst_reverse_iter;

    public:
      //! destructor
      virtual ~GridComponent() { }
      /*! 
       * Abstract visitor acceptance function for a parameter space visitor.
       * Note that here the visitor design pattern is in use (GoF p.315)
       *
       * \param v The application which visits the grid component.
       */
      virtual void accept(ParameterSpaceVisitor<Ctype, CresultData>& v) = 0;
      //! Add a grid component to a composite.
      virtual void add(GridComponent<Ctype, CresultData>* gridcomponent);
      //! Remove a grid component from a composite.
      virtual void remove(GridComponent<Ctype, CresultData>* gridcomponent);
      /*!
       * Parameterized factory method (GoF p.111)
       * Factory method design pattern in use (GoF p.107)
       * Creates a NullIterator by default.
       *
       * \param id Iterator id
       * \param type Iteration type.
       * \return The corresponding composite iterator (product).
       */
      virtual Iterator<Ctype, CresultData> createIterator(
          EiteratorType iter_type, EiterationMode iter_mode=PostOrder) const;
      //! Query function for the component type of the grid component.
      /*!
       *  Accutally only necessary for the implementation of the composite
       *  iterators.
       */
      virtual EcomponentType getComponentType() const = 0;
      //! Query function if grid component had been computed.
      virtual bool isComputed() const = 0;
      //! Set a grid component as computed to cache information.
      virtual void setComputed() = 0;
      //! Query function for the grid components' coordinates.
      virtual typename std::vector<Ctype> const& getCoordinates() const;
      /*!
       * Query function for coordinate Ids
       * Note that this information will be stored by the parent composite if
       * demanded from a leaf class.
       */
      virtual std::vector<std::string> const& getCoordinateId() const;
      //! Set coordinate Ids of a grid component
      virtual void setCoordinateId(std::vector<std::string> const ids);
      //! Return result data of the grid component.
      virtual CresultData const& getResultData() const { OPTIMIZE_illegal; }
      //! Set the result data of the grid component.
      virtual void setResultData(CresultData const data) { OPTIMIZE_illegal; }
      //! Returns an iterator pointing to the first child.
      virtual Titer begin() { OPTIMIZE_illegal; }
      //! Returns an iterator pointing to the last child.
      virtual Titer end() { OPTIMIZE_illegal; }
      /*!
       * Returns a reverse iterator referring to the last element of a
       * composite.
       */
      virtual Treverse_iter rbegin() { OPTIMIZE_illegal; }
      /*!
       * Returns a reverse iterator referring to the first element of a
       * composite.
       */
      virtual Treverse_iter rend() { OPTIMIZE_illegal; }

      //! Set the parent of a grid component.
      void setParent(GridComponent<Ctype, CresultData>* p) { Mparent = p; }
      //! Query function for the parent of the grid component.
      GridComponent<Ctype, CresultData>* getParent() const;

    protected:
      //! constructor
      GridComponent(GridComponent<Ctype, CresultData>* parent=0,
          bool computed=false) : Mparent(parent), Mcomputed(computed)
      { }

    protected:
      //! Pointer to the parent grid component.
      GridComponent<Ctype, CresultData>* Mparent;
      /*! 
       * Variable which provides the opportunity mark a grid component as
       * 'visited' by an application.
       */
      bool Mcomputed;

  }; // class GridComponent

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void GridComponent<Ctype, CresultData>::add(
      GridComponent<Ctype, CresultData>* gridcomponent)
  {
    OPTIMIZE_illegal;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void GridComponent<Ctype, CresultData>::remove(
      GridComponent<Ctype, CresultData>* gridcomponent)
  {
    OPTIMIZE_illegal;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  typename std::vector<Ctype> const& 
  GridComponent<Ctype, CresultData>::getCoordinates() const
  {
    OPTIMIZE_illegal;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  std::vector<std::string> const& 
  GridComponent<Ctype, CresultData>::getCoordinateId() const
  {
    OPTIMIZE_illegal;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void GridComponent <Ctype, CresultData>::setCoordinateId(
      std::vector<std::string> const ids)
  {
    OPTIMIZE_illegal;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  GridComponent<Ctype, CresultData>* 
    GridComponent<Ctype, CresultData>::getParent() const
  {
    return Mparent;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  Iterator<Ctype, CresultData> 
  GridComponent<Ctype,CresultData>::createIterator(EiteratorType iter_type,
      EiterationMode iter_mode) const
  {
    iterator::IteratorStrategyFactory<Ctype, CresultData> factory;
    return Iterator<Ctype, CresultData>(std::move(
          factory.makeIteratorStrategy(NullIter, iter_mode, 
          const_cast<GridComponent<Ctype, CresultData>*>(this))));
  }

} // namespace optimize

#endif

/* ----- END OF gridcomponent.h  ----- */
