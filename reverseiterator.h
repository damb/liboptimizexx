/*! \file reverseiterator.h
 * \brief Declaration of reverse parameter space iterators.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 01/04/2012
 * 
 * Purpose: Declaration of reverse parameter space iterators.
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
 * 01/04/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <optimizexx/iterator.h>

#ifndef _OPTIMIZEXX_REVERSEITERATOR_H_
#define _OPTIMIZEXX_REVERSEITERATOR_H_

namespace optimize
{

  /* ======================================================================= */
  /*!
   * Iterator to traverse the entire parameter space including grids and nodes
   * (post- and pre-ordered depending an).
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class ReverseIterator : public CompositeIterator<Ctype, CresultData>
  {
    public: 
      typedef CompositeIterator<Ctype, CresultData> Tbase;
      typedef typename Tbase::Tcomp_ptr Tcomp_ptr;

    public:
      /*!
       * constructor
       *
       * \param root Pointer to the composite creating the iterator.
       * \param type Type of the iteration.
       */
      ReverseIterator(Tcomp_ptr root, EiterationType type=PostOrder) :
        Mcomponent(root), MisDone(false), MiterMemento(0), MiterationType(type)
      {
        MiterMemento = Tbase::setIterationType(type);
      }
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      ReverseIterator(ReverseIterator<Ctype, CresultData> const& rhs) : 
        Mcomponent(rhs.Mcomponent), MisDone(rhs.MisDone),
        MiterMemento(0), MiterationType(rhs.MiterationType)
      { 
        MiterMemento = Tbase::setIterationType(rhs.MiterationType);
      }
      //! destructor
      virtual ~ReverseIterator() { delete MiterMemento; }
      //! set iterator to first element in parameter space composite (grid)
      virtual void first();
      //! go to the next item
      virtual void next();
      //! query function if iteration is finished
      virtual bool isDone() const { return MisDone; }
      //! query function for current item
      virtual Tcomp_ptr currentItem() const
      {
        return *MiterMemento->getCurrentIterator();
      }
      //! assignment operator
      virtual ReverseIterator<Ctype,CresultData>&  operator=(
          ReverseIterator<Ctype, CresultData> const& rhs);

    protected:
      //! constructor
      ReverseIterator(Tcomp_ptr root, bool done=false,
          EiterationType type=PostOrder) : Mcomponent(root), MisDone(done),
          MiterMemento(0), MiterationType(type)
      { 
        MiterMemento = Tbase::setIterationType(type);
      }

    protected:
      //! pointer to the root element of the iteration
      Tcomp_ptr Mcomponent;
      //! variable to save wether iteration is finished
      bool MisDone;
      /*!
       * As suggested at GoF p.271 here the memento design pattern is in use to
       * capture the state of an iteration within a IteratorMemento. The
       * iterator stores the memento internally. The functionalism of pre- or
       * rather post-order iteration is delegated to the memento.
       */
      IteratorMemento<Ctype, CresultData>* MiterMemento;
      //! type of iteration
      EiterationType MiterationType;

  }; // class template ReverseIterator

  /* ======================================================================= */
  /*!
   * Iterator to traverse the parameter space only iterating over grids.
   *
   * \todo Not tested yet.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class ReverseGridIterator : public ReverseIterator<Ctype, CresultData>
  {
    public: 
      typedef ReverseIterator<Ctype, CresultData> Tbase;
      typedef typename Tbase::Tcomp_ptr Tcomp_ptr;

    public:
      //! constructor
      ReverseGridIterator(Tcomp_ptr root, EiterationType type=PostOrder);
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      ReverseGridIterator(ReverseGridIterator<Ctype, CresultData> const& rhs) : 
        Tbase(rhs.Mcomponent, rhs.MisDone, rhs.MiterationType)
      { }
      //! set iterator to first grid in parameter space composite (grid)
      virtual void first();
      //! go to the next grid
      virtual void next();
      //! query function if iteration is finished
      virtual bool isDone() const { return Tbase::MisDone; }
      //! query function for current grid
      virtual Tcomp_ptr currentItem() const
      {
        return *Tbase::MiterMemento->getCurrentIterator();
      }

  }; // class template ReverseGridIterator

  /* ======================================================================= */
  /*!
   * Iterator to traverse the parameter space only iterating over nodes.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class ReverseNodeIterator : public ReverseIterator<Ctype, CresultData>
  {
    public: 
      typedef ReverseIterator<Ctype, CresultData> Tbase;
      typedef typename Tbase::Tcomp_ptr Tcomp_ptr;

    public:
      //! constructor
      ReverseNodeIterator(Tcomp_ptr root, EiterationType type=PostOrder);
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      ReverseNodeIterator(ReverseNodeIterator<Ctype, CresultData> const& rhs) : 
        Tbase(rhs.Mcomponent, rhs.MisDone, rhs.MiterationType)
      { }
      //! set iterator to first node in parameter space composite (grid)
      virtual void first();
      //! set iterator to last node in parameter space composite (grid)
      virtual void last();
      //! go to the next node
      virtual void next();
      //! query function if iteration is finished
      virtual bool isDone() const { return Tbase::MisDone; }
      //! query function for current node
      virtual Tcomp_ptr currentItem() const
      {
        return *Tbase::MiterMemento->getCurrentIterator();
      }
      //! equal to operator
      virtual bool operator==(
          ReverseNodeIterator<Ctype, CresultData> const& rhs);
      //! unequal to operator
      virtual bool operator!=(
          ReverseNodeIterator<Ctype, CresultData> const& rhs);
      //! dereference operator
      virtual GridComponent<Ctype, CresultData>* operator*();
      //! pre-incrementation operator
      virtual ReverseNodeIterator<Ctype, CresultData>& operator++();
      //! post-increment operator
      virtual ReverseNodeIterator<Ctype, CresultData> operator++(int);

  }; // class template ReverseNodeIterator

  /* ======================================================================= */
  // function definitions of class ReverseIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ReverseIterator<Ctype, CresultData>::first()
  {
    MiterMemento->reset();
    if (Mcomponent->rbegin() != Mcomponent->rend())
    {
      MisDone = false;
      IterationState<Ctype, CresultData>* state =
        new ReverseIterationState<Ctype, CresultData>(
        Mcomponent->rbegin(), Mcomponent->rend());
      MiterMemento->pushState(*state);
    } else
    {
      MisDone = true;
    }
  } // function ReverseIterator<Ctype, CresultData>::first

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ReverseIterator<Ctype, CresultData>::next()
  {
    if (! (*MiterMemento->getCurrentIterator())->createIterator(
          Iter)->isDone())
    {
      Tcomp_ptr comp_ptr = *MiterMemento->getCurrentIterator();
      IterationState<Ctype, CresultData>* state =
        new ReverseIterationState<Ctype, CresultData>(
        comp_ptr->rbegin(), comp_ptr->rend());
      MiterMemento->pushState(*state);
    } else
    {
      MiterMemento->next();
        
      while (! MiterMemento->empty() && MiterMemento->getState()->isEnd())
      {
        MiterMemento->popState();
        if (! MiterMemento->empty()) { MiterMemento->next(); }
      }
      if (MiterMemento->empty()) { MisDone = true; }
    }
  } // function ReverseIterator<Ctype, CresultData>::next

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ReverseIterator<Ctype,CresultData>&  
  ReverseIterator<Ctype, CresultData>::operator=(
          ReverseIterator<Ctype, CresultData> const& rhs)
  {
    if (this != &rhs)
    {
      Mcomponent = rhs.Mcomponent;
      MisDone = rhs.MisDone;
      delete MiterMemento;
      MiterMemento = Tbase::setIterationType(rhs.MiterationType);
      MiterationType = rhs.MiterationType;
    }
    return *this;
  } // function ReverseIterator<Ctype, CresultData>::operator=

  /* ======================================================================= */
  // function definitions of class ReverseGridIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ReverseGridIterator<Ctype, CresultData>::ReverseGridIterator(
      Tcomp_ptr root, EiterationType type) : 
      Tbase(root, false, type)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ReverseGridIterator<Ctype, CresultData>::first()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->rbegin() != Tbase::Mcomponent->rend())
    {
      Tbase::MisDone = false;
      IterationState<Ctype, CresultData>* state =
        new ReverseIterationState<Ctype, CresultData>(
        Tbase::Mcomponent->rbegin(), Tbase::Mcomponent->rend());
      Tbase::MiterMemento->pushState(*state);
      next();
    } else
    {
      Tbase::MisDone = true;
    }
  } // function ReverseGridIterator<Ctype, CresultData>::first()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ReverseGridIterator<Ctype, CresultData>::next()
  {
    if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
          GridIter)->isDone())
    {
      Tcomp_ptr comp_ptr = *Tbase::MiterMemento->getCurrentIterator();
      IterationState<Ctype, CresultData>* state =
        new ReverseIterationState<Ctype, CresultData>(
        comp_ptr->rbegin(), comp_ptr->rend());
      Tbase::MiterMemento->pushState(*state);
    } else
    {
      Tbase::MiterMemento->next();
        
      while (! Tbase::MiterMemento->empty() && 
          Tbase::MiterMemento->getState()->isEnd())
      {
        Tbase::MiterMemento->popState();
        if (! Tbase::MiterMemento->empty()) { Tbase::MiterMemento->next(); }
      }
      if (Tbase::MiterMemento->empty())
      {
        Tbase::MisDone = true;
      }
    }
    while (!Tbase::MisDone && 
        (*Tbase::MiterMemento->getCurrentIterator())->getComponentType() !=
        GridComponent<Ctype, CresultData>::Composite)
    {
      Tbase::MiterMemento->next();
    }
  } // function ReverseGridIterator<Ctype, CresultData>::next

  /* ======================================================================= */
  // function definitions of class ReverseNodeIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ReverseNodeIterator<Ctype, CresultData>::ReverseNodeIterator(
      typename Tbase::Tcomp_ptr root, EiterationType type) : 
      Tbase(root, false, type)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ReverseNodeIterator<Ctype, CresultData>::first()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->rbegin() != Tbase::Mcomponent->rend())
    {
      Tbase::MisDone = false;
      IterationState<Ctype, CresultData>* state =
        new ReverseIterationState<Ctype, CresultData>(
        Tbase::Mcomponent->rbegin(), Tbase::Mcomponent->rend());
      Tbase::MiterMemento->pushState(*state);
      while (!Tbase::MisDone && 
          (*Tbase::MiterMemento->getCurrentIterator())->getComponentType() !=
          GridComponent<Ctype, CresultData>::Leaf)
      {
        next();
      }
    } else
    {
      Tbase::MisDone = true;
    }
  } // function ReverseNodeIterator<Ctype, CresultData>::first()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ReverseNodeIterator<Ctype, CresultData>::last()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->rbegin() != Tbase::Mcomponent->rend())
    {
      Tbase::MisDone = false;
      IterationState<Ctype, CresultData>* state =
        new ReverseIterationState<Ctype, CresultData>(
        Tbase::Mcomponent->rbegin(), Tbase::Mcomponent->rend());
      Tbase::MiterMemento->pushState(*state);
      // wind forward
      while (!Tbase::MisDone)
      {
        next();
      }
    } else
    {
      Tbase::MisDone = true;
    }
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ReverseNodeIterator<Ctype, CresultData>::next()
  {
    if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
          NodeIter)->isDone())
    {
      Tcomp_ptr comp_ptr = *Tbase::MiterMemento->getCurrentIterator();
      IterationState<Ctype, CresultData>* state =
        new ReverseIterationState<Ctype, CresultData>(
       comp_ptr->rbegin(), comp_ptr->rend());
      Tbase::MiterMemento->pushState(*state);
    } else
    {
      Tbase::MiterMemento->next();
        
      while (! Tbase::MiterMemento->empty() && 
          Tbase::MiterMemento->getState()->isEnd())
      {
        Tbase::MiterMemento->popState();
        if (! Tbase::MiterMemento->empty()) { Tbase::MiterMemento->next(); }
      }

      if (Tbase::MiterMemento->empty())
      {
        Tbase::MisDone = true;
      }
    }

    while (!Tbase::MisDone && 
        (*Tbase::MiterMemento->getCurrentIterator())->getComponentType() !=
        GridComponent<Ctype, CresultData>::Leaf)
    {
      Tbase::MiterMemento->next();
    }
  } // function ReverseNodeIterator<Ctype, CresultData>::next

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  bool ReverseNodeIterator<Ctype, CresultData>::operator==(
      ReverseNodeIterator<Ctype, CresultData> const& rhs)
  {
    return Tbase::MiterMemento->getCurrentIterator() == 
      rhs.MiterMemento->getCurrentIterator();
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  bool ReverseNodeIterator<Ctype, CresultData>::operator!=(
      ReverseNodeIterator<Ctype, CresultData> const& rhs)
  {
    return !(*this == rhs);
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ReverseNodeIterator<Ctype, CresultData>& 
  ReverseNodeIterator<Ctype, CresultData>::operator++()
  {
    next();
    return *this;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ReverseNodeIterator<Ctype, CresultData> 
  ReverseNodeIterator<Ctype, CresultData>::operator++(int)
  {
    ReverseNodeIterator<Ctype, CresultData> tmp(*this);
    operator++();
    return tmp;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  GridComponent<Ctype, CresultData>*  
  ReverseNodeIterator<Ctype, CresultData>::operator*()
  {
    return currentItem();
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard


/* ----- END OF reverseiterator.h  ----- */
