/*! \file iterator.h
 * \brief Declarations of various parameter space iterators.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/02/2012
 * 
 * Purpose: Declarations of various parameter space iterators. 
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

#include <optimizexx/memento.h>
#include <optimizexx/error.h>
 
#ifndef _ITERATOR_H_
#define _ITERATOR_H_

namespace optimize
{
  // forward declarations
  template <typename Ctype, typename CresultData> class GridComponent;
  template <typename Ctype, typename CresultData> class CompositeIterator;
  
  //! \defgroup group_iterator parameter space iterator
  /*!
   * Iterator Ids for parameterized factory method (GoF p.111). 
   *
   * \sa GridComponent<Ctype, CresultData>::createIterator
   * \sa Grid<Ctype, CresultData>::createIterator
   *
   * \ingroup group_iterator
   */
  enum EiteratorId
  {
    Iter,     //!< usual iterator
    GridIter, //!< only grid iterator
    NodeIter, //!< only node iterator
    NullIter  //!< degenerated iterator
  }; // enum EiteratorId

  //! Type of iteration.
  enum EiterationType
  {
    PreOrder, //!< Pre-oder iteration
    PostOrder //!< Post-order iteration
  }; // enum EiterationType

  /* ======================================================================= */
  /*!
   * Advances the iterator \a iter by \a n elements.
   *
   * \todo Not tested yet.
   *
   * \param iter Iterator to be advanced.
   * \param n Number of elements to be advanced. If n is equal to or greater
   * than the number of children in the parameter space grid than \a iter will
   * point to the last element.
   */
  template <typename CcompositeIterator>
  void advance(CcompositeIterator& iter, unsigned int n)
  {
    while (n > 0 && ! iter.isDone())
    {
      iter.next();
      --n;
    }
  }

  /* ----------------------------------------------------------------------- */
  /*!
   * Calculates the number of elements between \a iter_first and \a iter_last.
   *
   * \todo Not tested yet.
   *
   * \param iter_first Iterator pointing to the initial element.
   * \param iter_last Iterator pointing to the final element. This must be
   * reachable from \a iter_first.
   */
  template <typename CcompositeIterator>
  unsigned int distance(CcompositeIterator& iter_first,
      CcompositeIterator& iter_last)
  {
    CcompositeIterator& iter(iter_first);

    unsigned int retval = 0;
    while (iter != iter_last && ! iter.isDone())
    {
      ++retval;
      iter.next(); 
    }

    if (iter != iter_last)
    {
      retval = 0;
      iter.first(); 
      while (iter != iter_last)
      {
        ++retval;
        iter.next(); 
      }
    }
    return retval;
  }

  /* ======================================================================= */
  /*!
   * Proxy for a composite iterator to make sure iterators get deleted.
   * Implementation of suggestion in GoF p.266.
   *
   * Can be treated just like a pointer to an iterator.
   *
   * \todo Not tested yet.
   */
  template <typename Ctype, typename CresultData>
  class IteratorPtr
  {
    public:
      //! constructor
      IteratorPtr(CompositeIterator<Ctype, CresultData>* i) : Miterator(i) { }
      //! destructor
      ~IteratorPtr() { delete Miterator; }
      CompositeIterator<Ctype, CresultData>* operator->() { return Miterator; }
      CompositeIterator<Ctype, CresultData>& operator*() { return *Miterator; }

    private:
      //! composite iterator to handle  
      CompositeIterator<Ctype, CresultData>* Miterator;

    private:
        //! disallow copy to avoid multiple deletions
        IteratorPtr(IteratorPtr<Ctype, CresultData> const&);
        //! disallow assignment to avoid multiple deletions
        IteratorPtr& operator=(IteratorPtr<Ctype, CresultData> const&);

  }; // class IteratorPtr

  /* ======================================================================= */
  /*!
   * Abstract base class template for parameter space iterators.
   * Note that here the iterator design pattern is in use (GoF p.257).
   * Iterators are created using the Grid::createIterator factory method
   * (GoF p.107). To traverse the recursivly aggregated parameter space the
   * iterators (concrete originators) are able to save their internal iteration
   * states making use of the memento design pattern (GoF p.283).\n
   *
   * IMPORTANT NOTE: Usually the memento's caretaker is different from the
   * memento's originater, but in my case the iterator both is caretaker and
   * originator. This problem could be solved using the IteratorPtr approach
   * (GoF p.266).\n
   *
   * Note that the functionalism of how the post-/pre-order iteration is
   * delegated to 
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class CompositeIterator
  {
    public:
      typedef GridComponent<Ctype, CresultData>* Tcomp_ptr;
      typedef typename GridComponent<Ctype,CresultData>::Titer Titer;

    public:
      virtual void first() { OPTIMIZE_illegal; }
      virtual void last() { OPTIMIZE_illegal; }
      virtual void next()  { OPTIMIZE_illegal; }
      virtual bool isDone() const = 0;
      /*!
       * Note that here returning a pointer is necessary because otherwise there
       * couldn't be made any use of the polymorvic functionalism the parameter
       * space composite is providing.\n
       *
       * IMPORTANT NOTE: For simpler syntax provide the same function returning
       * a reference or even better overload the corresponding operators.
       *
       * \return a pointer to the current grid component
       */
      virtual Tcomp_ptr currentItem() const = 0;
      //! destructor
      virtual ~CompositeIterator() { delete MiterMemento; }
      //! pre-increment operator
      virtual CompositeIterator<Ctype, CresultData>& operator++() 
      { 
        OPTIMIZE_illegal;
      }
      //! equal to operator
      virtual bool operator==(
          CompositeIterator<Ctype, CresultData> const& rhs) 
      {
        OPTIMIZE_illegal;
      }
      //! unequal to operator
      virtual bool operator!=(
          CompositeIterator<Ctype, CresultData> const& rhs) 
      {
        OPTIMIZE_illegal;
      }
      //! dereference operator
      virtual GridComponent<Ctype, CresultData>* operator*()
      {
        OPTIMIZE_illegal;
      }
      //! assignment operator
      virtual CompositeIterator<Ctype,CresultData>&  operator=(
          CompositeIterator<Ctype, CresultData> const& rhs);

    protected:
      //! constructor
      CompositeIterator(Tcomp_ptr root, bool done=false,
          EiterationType type=PostOrder) : Mcomponent(root), MisDone(done),
          MiterationType(type)
      { 
        setIterationType(type);
      }
      /*!
       * Parameterized factory method to create the correct iteration memento.
       * Factory method design pattern in use (GoF p.107).
       */
      void setIterationType(EiterationType type);

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

  }; // class CompositeIterator

  /* ======================================================================= */
  /*!
   * Iterator to traverse the entire parameter space including grids and nodes
   * (post- and pre-ordered depending an).
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class Iterator : public CompositeIterator<Ctype, CresultData>
  {
    public: 
      typedef CompositeIterator<Ctype, CresultData> Tbase;

    public:
      /*!
       * constructor
       *
       * \param root Pointer to the composite creating the iterator.
       * \param type Type of the iteration.
       */
      Iterator(typename Tbase::Tcomp_ptr root, EiterationType type=PostOrder);
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      Iterator(Iterator<Ctype, CresultData> const& rhs) : 
        Tbase(rhs.Mcomponent, rhs.MisDone, rhs.MiterationType)
      { }
      //! set iterator to first element in parameter space composite (grid)
      virtual void first();
      //! go to the next item
      virtual void next();
      //! query function if iteration is finished
      virtual bool isDone() const { return Tbase::MisDone; }
      //! query function for current item
      virtual typename Tbase::Tcomp_ptr currentItem() const
      {
        return *Tbase::MiterMemento->getCurrentIterator();
      }

  }; // class Iterator

  /* ======================================================================= */
  /*!
   * Iterator to traverse the parameter space only iterating over grids.
   *
   * \todo Not tested yet.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class GridIterator : public CompositeIterator<Ctype, CresultData>
  {
    public: 
      typedef CompositeIterator<Ctype, CresultData> Tbase;

    public:
      //! constructor
      GridIterator(typename Tbase::Tcomp_ptr root,
          EiterationType type=PostOrder);
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      GridIterator(GridIterator<Ctype, CresultData> const& rhs) : 
        Tbase(rhs.Mcomponent, rhs.MisDone, rhs.MiterationType)
      { }
      //! set iterator to first grid in parameter space composite (grid)
      virtual void first();
      //! go to the next grid
      virtual void next();
      //! query function if iteration is finished
      virtual bool isDone() const { return Tbase::MisDone; }
      //! query function for current grid
      virtual typename Tbase::Tcomp_ptr currentItem() const
      {
        return *Tbase::MiterMemento->getCurrentIterator();
      }

  }; // class GridIterator

  /* ======================================================================= */
  /*!
   * Iterator to traverse the parameter space only iterating over nodes.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class NodeIterator : public CompositeIterator<Ctype, CresultData>
  {
    public: 
      typedef CompositeIterator<Ctype, CresultData> Tbase;

    public:
      //! constructor
      NodeIterator(typename Tbase::Tcomp_ptr root,
          EiterationType type=PostOrder);
      /*!
       * copy constructor
       * 
       * \param rhs argument to copy
       */
      NodeIterator(NodeIterator<Ctype, CresultData> const& rhs) : 
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
      virtual typename Tbase::Tcomp_ptr currentItem() const
      {
        return *Tbase::MiterMemento->getCurrentIterator();
      }
      //! equal to operator
      virtual bool operator==(
          NodeIterator<Ctype, CresultData> const& rhs);
      //! unequal to operator
      virtual bool operator!=(
          NodeIterator<Ctype, CresultData> const& rhs);
      //! dereference operator
      virtual GridComponent<Ctype, CresultData>* operator*();
      //! pre-incrementation operator
      virtual NodeIterator<Ctype, CresultData>& operator++();
      //! post-increment operator
      virtual NodeIterator<Ctype, CresultData> operator++(int);

  }; // class NodeIterator

  /* ======================================================================= */
  /*!
   * Degenerated iterator with the property that NullIterator::isDone always
   * returns true. The advantage of this approach is that boundary conditions
   * are much easier to handle. See also GoF p.262.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class NullIterator : public CompositeIterator<Ctype, CresultData>
  {
    public: 
      typedef CompositeIterator<Ctype, CresultData> Tbase;

    public:
      //! constructor
      NullIterator(typename Tbase::Tcomp_ptr root) : Tbase(root, true) { }
      /*!
       * Because NullIterator is a degenerated iterator always will return
       * true.
       *
       * \return true
       */
      virtual bool isDone() const { return true; }
      /*!
       * Always will return the component which is the NullIterator's
       * originator.
       * 
       * \return The NullIterator's originator.
       */
      virtual typename Tbase::Tcomp_ptr currentItem() const 
      { 
        return Tbase::Mcomponent;
      }

  }; // class NullIterator

  /* ======================================================================= */
  // function definitions of class CompositeIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  CompositeIterator<Ctype,CresultData>&  
  CompositeIterator<Ctype, CresultData>::operator=(
          CompositeIterator<Ctype, CresultData> const& rhs)
  {
    if (this != &rhs)
    {
      Mcomponent = rhs.Mcomponent;
      MisDone = rhs.MisDone;
      delete MiterMemento;
      this->setIterationType(rhs.MiterationType);
      MiterationType = rhs.MiterationType;
    }
    return *this;
  } // function CompositeIterator<Ctype, CresultData>::operator=()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void CompositeIterator<Ctype, CresultData>::setIterationType(
      EiterationType type)
  {
    //! up to now only post-ordered iteration is implemented.
    if (PostOrder == type) 
    { 
      MiterMemento = new PostIterationMemento<Ctype, CresultData>;
    } else
    if (PreOrder == type) 
    { 
      MiterMemento = new PreIterationMemento<Ctype, CresultData>;
    }
    else { OPTIMIZE_abort("Illegal iteration type."); }
  } // function CompositeIterator<Ctype, CresultData>::setIterationType

  /* ======================================================================= */
  // function definitions of class Iterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  Iterator<Ctype, CresultData>::Iterator(typename Tbase::Tcomp_ptr root,
      EiterationType type) : Tbase(root, false, type)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Iterator<Ctype, CresultData>::first()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
    {
      Tbase::MisDone = false;
      Tbase::MiterMemento->pushState(IterationState<Ctype, CresultData>(
            Tbase::Mcomponent->begin(), Tbase::Mcomponent->end()));
    } else
    {
      Tbase::MisDone = true;
    }
  } // function Iterator<Ctype, CresultData>::first

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void Iterator<Ctype, CresultData>::next()
  {
    if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
          Iter)->isDone())
    {
      GridComponent<Ctype, CresultData>* comp_ptr = 
          *Tbase::MiterMemento->getCurrentIterator();
      Tbase::MiterMemento->pushState(
          IterationState<Ctype, CresultData>(comp_ptr->begin(),
          comp_ptr->end()));
    } else
    {
      Tbase::MiterMemento->next();
        
      while (! Tbase::MiterMemento->empty() && 
        Tbase::MiterMemento->getState().isEnd())
      {
        Tbase::MiterMemento->popState();
        if (! Tbase::MiterMemento->empty())
        {
          Tbase::MiterMemento->next();
        }
      }
      if (Tbase::MiterMemento->empty())
      {
        Tbase::MisDone = true;
      }
    }
  } // function Iterator<Ctype, CresultData>::next

  /* ======================================================================= */
  // function definitions of class GridIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  GridIterator<Ctype, CresultData>::GridIterator(
      typename Tbase::Tcomp_ptr root, EiterationType type) : 
      Tbase(root, false, type)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void GridIterator<Ctype, CresultData>::first()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
    {
      Tbase::MisDone = false;
      Tbase::MiterMemento->pushState(IterationState<Ctype, CresultData>(
            Tbase::Mcomponent->begin(), Tbase::Mcomponent->end()));
      next();
    } else
    {
      Tbase::MisDone = true;
    }
  } // function GridIterator<Ctype, CresultData>::first()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void GridIterator<Ctype, CresultData>::next()
  {
    if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
          GridIter)->isDone())
    {
      GridComponent<Ctype, CresultData>* comp_ptr = 
          *Tbase::MiterMemento->getCurrentIterator();
      Tbase::MiterMemento->pushState(
          IterationState<Ctype, CresultData>(comp_ptr->begin(),
          comp_ptr->end()));
    } else
    {
      Tbase::MiterMemento->next();
        
      while (! Tbase::MiterMemento->empty() && 
          Tbase::MiterMemento->getState().isEnd())
      {
        Tbase::MiterMemento->popState();
        if (! Tbase::MiterMemento->empty())
        {
          Tbase::MiterMemento->next();
        }
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
  } // function GridIterator<Ctype, CresultData>::next

  /* ======================================================================= */
  // function definitions of class NodeIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  NodeIterator<Ctype, CresultData>::NodeIterator(
      typename Tbase::Tcomp_ptr root, EiterationType type) : 
      Tbase(root, false, type)
  { }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void NodeIterator<Ctype, CresultData>::first()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
    {
      Tbase::MisDone = false;
      Tbase::MiterMemento->pushState(IterationState<Ctype, CresultData>(
            Tbase::Mcomponent->begin(), Tbase::Mcomponent->end()));
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
  } // function NodeIterator<Ctype, CresultData>::first()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void NodeIterator<Ctype, CresultData>::last()
  {
    Tbase::MiterMemento->reset();
    if (Tbase::Mcomponent->begin() != Tbase::Mcomponent->end())
    {
      Tbase::MisDone = false;
      Tbase::MiterMemento->pushState(IterationState<Ctype, CresultData>(
            Tbase::Mcomponent->begin(), Tbase::Mcomponent->end()));
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
  void NodeIterator<Ctype, CresultData>::next()
  {
    if (! (*Tbase::MiterMemento->getCurrentIterator())->createIterator(
          NodeIter)->isDone())
    {
      GridComponent<Ctype, CresultData>* comp_ptr = 
          *Tbase::MiterMemento->getCurrentIterator();
      Tbase::MiterMemento->pushState(
          IterationState<Ctype, CresultData>(comp_ptr->begin(),
          comp_ptr->end()));
    } else
    {
      Tbase::MiterMemento->next();
        
      while (! Tbase::MiterMemento->empty() && 
          Tbase::MiterMemento->getState().isEnd())
      {
        Tbase::MiterMemento->popState();
        if (! Tbase::MiterMemento->empty())
        {
          Tbase::MiterMemento->next();
        }
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
  } // function NodeIterator<Ctype, CresultData>::next

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  bool NodeIterator<Ctype, CresultData>::operator==(
      NodeIterator<Ctype, CresultData> const& rhs)
  {
    return Tbase::MiterMemento->getCurrentIterator() == 
      rhs.MiterMemento->getCurrentIterator();
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  bool NodeIterator<Ctype, CresultData>::operator!=(
      NodeIterator<Ctype, CresultData> const& rhs)
  {
    return !(*this == rhs);
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  NodeIterator<Ctype, CresultData>& 
  NodeIterator<Ctype, CresultData>::operator++()
  {
    next();
    return *this;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  NodeIterator<Ctype, CresultData> 
  NodeIterator<Ctype, CresultData>::operator++(int)
  {
    NodeIterator<Ctype, CresultData> tmp(*this);
    operator++();
    return tmp;
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  GridComponent<Ctype, CresultData>*  
  NodeIterator<Ctype, CresultData>::operator*()
  {
    return currentItem();
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif

/* ----- END OF iterator.h  ----- */
