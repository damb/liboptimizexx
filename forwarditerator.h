/*! \file forwarditerator.h
 * \brief Declaration of forward parameter space iterators.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 01/04/2012
 * 
 * Purpose: Declaration of forward parameter space iterators.  
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

#ifndef _OPTIMIZEXX_FORWARDITERATOR_H_
#define _OPTIMIZEXX_FORWARDITERATOR_H_

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
  class ForwardIterator : public CompositeIterator<Ctype, CresultData>
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
      ForwardIterator(Tcomp_ptr root, EiterationType type=PostOrder) :
        Mcomponent(root), MisDone(false), MiterMemento(0), MiterationType(type)
      {
        MiterMemento = Tbase::setIterationType(type);
      }
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      ForwardIterator(ForwardIterator<Ctype, CresultData> const& rhs) : 
        Mcomponent(rhs.Mcomponent), MisDone(rhs.MisDone),
        MiterMemento(0), MiterationType(rhs.MiterationType)
      { 
        MiterMemento = Tbase::setIterationType(rhs.MiterationType);
      }
      //! destructor
      virtual ~ForwardIterator() { delete MiterMemento; }
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
      virtual ForwardIterator<Ctype,CresultData>&  operator=(
          ForwardIterator<Ctype, CresultData> const& rhs);

    protected:
      //! constructor
      ForwardIterator(Tcomp_ptr root, bool done=false,
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

  }; // class template ForwardIterator

  /* ======================================================================= */
  /*!
   * Iterator to traverse the parameter space only iterating over grids.
   *
   * \todo Not tested yet.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class ForwardGridIterator : public ForwardIterator<Ctype, CresultData>
  {
    public: 
      typedef ForwardIterator<Ctype, CresultData> Tbase;
      typedef typename Tbase::Tcomp_ptr Tcomp_ptr;

    public:
      //! constructor
      ForwardGridIterator(Tcomp_ptr root, EiterationType type=PostOrder);
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      ForwardGridIterator(ForwardGridIterator<Ctype, CresultData> const& rhs) : 
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

  }; // class template ForwardGridIterator

  /* ======================================================================= */
  /*!
   * Iterator to traverse the parameter space only iterating over nodes.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class ForwardNodeIterator : public ForwardIterator<Ctype, CresultData>
  {
    public: 
      typedef ForwardIterator<Ctype, CresultData> Tbase;
      typedef typename Tbase::Tcomp_ptr Tcomp_ptr;

    public:
      //! constructor
      ForwardNodeIterator(Tcomp_ptr root, EiterationType type=PostOrder);
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      ForwardNodeIterator(ForwardNodeIterator<Ctype, CresultData> const& rhs) : 
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
          ForwardNodeIterator<Ctype, CresultData> const& rhs);
      //! unequal to operator
      virtual bool operator!=(
          ForwardNodeIterator<Ctype, CresultData> const& rhs);
      //! dereference operator
      virtual GridComponent<Ctype, CresultData>* operator*();
      //! pre-incrementation operator
      virtual ForwardNodeIterator<Ctype, CresultData>& operator++();
      //! post-increment operator
      virtual ForwardNodeIterator<Ctype, CresultData> operator++(int);

  }; // class template ForwardNodeIterator

  /* ======================================================================= */
  // function definitions of class ForwardIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ForwardIterator<Ctype, CresultData>::first()
  {
    MiterMemento->reset();
    if (Mcomponent->begin() != Mcomponent->end())
    {
      MisDone = false;
      ForwardIterationState<Ctype, CresultData>* state =
        new ForwardIterationState<Ctype, CresultData>(
          Mcomponent->begin(), Mcomponent->end());
      MiterMemento->pushState(*state);
    } else
    {
      MisDone = true;
    }
  } // function ForwardIterator<Ctype, CresultData>::first

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ForwardIterator<Ctype, CresultData>::next()
  {
    if (! (*MiterMemento->getCurrentIterator())->createIterator(
          Iter)->isDone())
    {
      Tcomp_ptr comp_ptr = *MiterMemento->getCurrentIterator();
      ForwardIterationState<Ctype, CresultData>* state = 
        new ForwardIterationState<Ctype, CresultData>(
          comp_ptr->begin(), comp_ptr->end());
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
  } // function ForwardIterator<Ctype, CresultData>::next

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ForwardIterator<Ctype,CresultData>&  
  ForwardIterator<Ctype, CresultData>::operator=(
          ForwardIterator<Ctype, CresultData> const& rhs)
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
  } // function ForwardIterator<Ctype, CresultData>::operator=

  /* ======================================================================= */
  // function definitions of class ForwardGridIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ForwardGridIterator<Ctype, CresultData>::ForwardGridIterator(
      Tcomp_ptr root, EiterationType type) : Tbase(root, false, type)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ForwardGridIterator<Ctype, CresultData>::first()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
    {
      Tbase::MisDone = false;
      ForwardIterationState<Ctype, CresultData>* state =
        new ForwardIterationState<Ctype, CresultData>(
            Tbase::Mcomponent->begin(), Tbase::Mcomponent->end());
      Tbase::MiterMemento->pushState(*state);
      next();
    } else
    {
      Tbase::MisDone = true;
    }
  } // function ForwardGridIterator<Ctype, CresultData>::first()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ForwardGridIterator<Ctype, CresultData>::next()
  {
    if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
          GridIter)->isDone())
    {
      GridComponent<Ctype, CresultData>* comp_ptr = 
          *Tbase::MiterMemento->getCurrentIterator();
      ForwardIterationState<Ctype, CresultData>* state =
        new ForwardIterationState<Ctype, CresultData>(
            comp_ptr->begin(), comp_ptr->end());
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
  } // function ForwardGridIterator<Ctype, CresultData>::next

  /* ======================================================================= */
  // function definitions of class ForwardNodeIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ForwardNodeIterator<Ctype, CresultData>::ForwardNodeIterator(
      Tcomp_ptr root, EiterationType type) : Tbase(root, false, type)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ForwardNodeIterator<Ctype, CresultData>::first()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
    {
      Tbase::MisDone = false;
      ForwardIterationState<Ctype, CresultData>* state =
        new ForwardIterationState<Ctype, CresultData>(
            Tbase::Mcomponent->begin(), Tbase::Mcomponent->end());
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
  } // function ForwardNodeIterator<Ctype, CresultData>::first()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void ForwardNodeIterator<Ctype, CresultData>::last()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
    {
      Tbase::MisDone = false;
      ForwardIterationState<Ctype, CresultData>* state =
        new ForwardIterationState<Ctype, CresultData>(
          Tbase::Mcomponent->begin(), Tbase::Mcomponent->end());
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
  void ForwardNodeIterator<Ctype, CresultData>::next()
  {
    if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
          NodeIter)->isDone())
    {
      Tcomp_ptr comp_ptr = *Tbase::MiterMemento->getCurrentIterator();
      ForwardIterationState<Ctype, CresultData>* state =
        new ForwardIterationState<Ctype, CresultData>(
          comp_ptr->begin(), comp_ptr->end());
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
  } // function ForwardNodeIterator<Ctype, CresultData>::next

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  bool ForwardNodeIterator<Ctype, CresultData>::operator==(
      ForwardNodeIterator<Ctype, CresultData> const& rhs)
  {
    return Tbase::MiterMemento->getCurrentIterator() == 
      rhs.MiterMemento->getCurrentIterator();
  }

  /* ----------------------------------------------------------------------- */
  /*
   * \todo produces Segmenation fault
   */
  template <typename Ctype, typename CresultData>
  bool ForwardNodeIterator<Ctype, CresultData>::operator!=(
      ForwardNodeIterator<Ctype, CresultData> const& rhs)
  {
    return !(*this == rhs);
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ForwardNodeIterator<Ctype, CresultData>& 
  ForwardNodeIterator<Ctype, CresultData>::operator++()
  {
    next();
    return *this;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  ForwardNodeIterator<Ctype, CresultData> 
  ForwardNodeIterator<Ctype, CresultData>::operator++(int)
  {
    ForwardNodeIterator<Ctype, CresultData> tmp(*this);
    operator++();
    return tmp;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  GridComponent<Ctype, CresultData>*  
  ForwardNodeIterator<Ctype, CresultData>::operator*()
  {
    return currentItem();
  }

  /* ----------------------------------------------------------------------- */


} // namespace optimize

#endif // include guard

/* ----- END OF forwarditerator.h  ----- */
