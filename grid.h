/*! \file grid.h
 * \brief Declaration of a parameter space grid.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 29/02/2012
 * 
 * Purpose: Declaration of a parameter space grid.  
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

#include <list>
#include <string>
#include <vector>
#include <ostream>
#include <algorithm>
#include <memory>
#include <optimizexx/gridcomponent.h>

#ifndef _OPTIMIZEXX_GRID_H_
#define _OPTIMIZEXX_GRID_H_

namespace optimize
{

  // forward declaration
  template <typename Ctype, typename CresultData> class ParameterSpaceVisitor;
  template <typename Ctype, typename CresultData> class Iterator;
  template <typename Ctype, typename CresultData> class IteratorStrategyFactory;

  /* ======================================================================= */
  /*!
   * Delcares a grid or rather a parameter space of a global algorithm. Both
   * could contain subgrids and nodes.
   * Composite class of the composite design pattern (GoF p.163).\n
   *
   * IMPORTANT NOTE:\n
   * The solution to add the typdef Titer to class GridComponent isn't really
   * convenient because in case a new composite class would be created storing
   * its children in a different container than a list I would get into
   * trouble.\n
   * \b Solution?
   * 
   *
   * \ingroup group_grid
   */
  template <typename Ctype, typename CresultData>
  class Grid : public GridComponent<Ctype, CresultData>
  {
    public:
      //! Base class.
      typedef GridComponent<Ctype, CresultData> Tbase; 
      //! Pointer to the base class.
      typedef GridComponent<Ctype, CresultData>* Tbase_ptr; 
      //! Usual iterator for children of the grid.
      typedef typename Tbase::Titer Titer;
      //! Reverse iterator for children of the grid.
      typedef typename Tbase::Treverse_iter Treverse_iter;

    public:
      //! constructor
      Grid() : Tbase(0, false) { }
      //! constructor
      Grid(std::vector<std::string> const coordIds) : Tbase(0, false) { }
      //! destructor
      virtual ~Grid();
      /*! 
       * Visitor acceptance function for a parameter space visitor.
       * Note that here the visitor design pattern is in use (GoF p.315)
       *
       * \param v The application which visits the parameter space respectivly
       * the grid.
       */
      virtual void accept(ParameterSpaceVisitor<Ctype, CresultData>& v);
      /*!
       * Overriding parameterized factory method (GoF p. 111)
       * Factory method design pattern in use (GoF p.107)
       *
       * \param id Iterator id
       * \param type Iteration type.
       * \return The corresponding composite iterator (product).
       */
      virtual Iterator<Ctype, CresultData> createIterator(
          EiteratorType iter_type, EiterationMode iter_mode=PostOrder) const;
      /*!
       * Add a gridcomponent to the grid composite.
       * 
       * \param gridcomponent Component which will be added to the grid.
       */
      virtual void add(Tbase_ptr gridcomponent);
      /*!
       * Remove a gridcomponent of the grid composite.
       * 
       * \param gridcomponent Component which will be removed of the grid.
       */
      virtual void remove(Tbase_ptr gridcomponent);
      //! query function for coordinate Ids
      virtual std::vector<std::string> const& getCoordinateId() const;
      //! Set coordinate Ids of the grid
      virtual void setCoordinateId(std::vector<std::string> const);
      //! query function if grid had been successfully computed
      virtual bool isComputed() const { return Tbase::Mcomputed; }
      //! set a flag to mark the grid as computed
      /*!
       * Caching information of the grids' children leads to improved
       * performance.
       * \todo Check if it wouldn't be better to delegate the entire
       * responsibility of setting the Mcomputed flag to the user so
       * setComputed() does not check anything and just sets the Mcomputed
       * variable.
       */
      virtual void setComputed();
      //! query function for the component type of the grid component
      virtual typename Tbase::EcomponentType getComponentType() const 
      { 
        return Tbase::Composite;
      }
      //! Return an iterator pointing to the first child.
      virtual Titer begin() { return Mchildren.begin(); }
      //! Return an iterator pointing to the last child.
      virtual Titer end() { return Mchildren.end(); }
      /*!
       * Returns a reverse iterator referring to the last element of a
       * composite.
       */
      virtual Treverse_iter rbegin() { return Mchildren.rbegin(); }
      /*!
       * Returns a reverse iterator referring to the first element of a
       * composite.
       */
      virtual Treverse_iter rend() { return Mchildren.rend(); }

    private:
      //! Constant iterator for children.
      typedef typename 
        std::list<GridComponent<Ctype, CresultData>* >::const_iterator 
        Tconst_iter;
      //! List to store grids' children.
      std::list<Tbase_ptr> Mchildren;
      /*!
       * Vector to store coordinate ids.
       * Notice, that a grid caches this information for all of its children,
       * which are nodes. In constrast to nodes, subgrids might have their own
       * coordinate ids.
       */
      std::vector<std::string> McoordinateIds;

  }; // class template Grid

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  Grid<Ctype, CresultData>::~Grid()
  {
    for (Titer it(Mchildren.begin()); it != Mchildren.end(); ++it)
    {
      delete *it;
    }
    Mchildren.clear(); 
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Grid<Ctype, CresultData>::accept(
      ParameterSpaceVisitor<Ctype, CresultData>& v)
  {
    for (Titer it(Mchildren.begin()); it != Mchildren.end(); ++it)
    {
      (*it)->accept(v);
    }
    v(this);
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Grid<Ctype, CresultData>::add(Tbase_ptr gridcomponent)
  {
    Titer iter = find(Mchildren.begin(), Mchildren.end(), gridcomponent);
    if (Mchildren.end() == iter)
    {
      gridcomponent->setParent(this);
      Mchildren.push_back(gridcomponent);
      Tbase::Mcomputed = false;
    }
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Grid<Ctype, CresultData>::remove(Tbase_ptr gridcomponent)
  {
    Titer iter = find(Mchildren.begin(), Mchildren.end(), gridcomponent);
    if (Mchildren.end() != iter)
    {
      gridcomponent->setParent(0);
      delete *iter;
      Mchildren.erase(iter);
    }
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  std::vector<std::string> const& 
  Grid<Ctype, CresultData>::getCoordinateId() const
  {
    return McoordinateIds;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Grid<Ctype, CresultData>::setCoordinateId(
      std::vector<std::string> const ids)
  {
    McoordinateIds = ids;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Grid<Ctype, CresultData>::setComputed()
  {
    //! IMPORTANT NOTE: check if checking all children is necessary here!
    for (Tconst_iter cit = Mchildren.begin(); cit != Mchildren.end(); ++cit)
    {
      if (!(*cit)->isComputed()) { Tbase::Mcomputed = false; break; }
    }
    Tbase::Mcomputed = true;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  Iterator<Ctype, CresultData> Grid<Ctype,CresultData>::createIterator(
          EiteratorType iter_type, EiterationMode iter_mode) const
  {
    iterator::IteratorStrategyFactory<Ctype, CresultData> factory;
    
    return Iterator<Ctype, CresultData>(std::move(factory.makeIteratorStrategy(
            iter_type, iter_mode,
            const_cast<Grid<Ctype, CresultData>*>(this))));
  }

  /* ----------------------------------------------------------------------- */


} // namespace optimize

#endif // include guard

/* ----- END OF grid.h  ----- */
