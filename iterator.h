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
 
#ifndef _OPTIMIZEXX_ITERATOR_H_
#define _OPTIMIZEXX_ITERATOR_H_

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
    Iter,         //!< usual iterator
    GridIter,     //!< only grid iterator
    NodeIter,     //!< only node iterator
    RevIter,      //!< reverse iterator
    RevGridIter,  //!< only grid reverse iterator
    RevNodeIter,  //!< only node reverse iterator
    NullIter      //!< degenerated iterator
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
   *
   * \note Have a look at http://drdobbs.com/184401406 to improve
   * implementation.
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
   * delegated to the iterator mementos.
   *
   * \ingroup group_iterator
   */
  template <typename Ctype, typename CresultData>
  class CompositeIterator
  {
    public:
      typedef GridComponent<Ctype, CresultData>* Tcomp_ptr;

    public:
      virtual void first() { OPTIMIZE_illegal; }
      virtual void last() { OPTIMIZE_illegal; }
      //! iterate to the next item in the container
      virtual void next()  { OPTIMIZE_illegal; }
      //! query function if the iteration has finished
      virtual bool isDone() const = 0;
      /*!
       * Note that here returning a pointer is necessary because otherwise there
       * couldn't be made any use of the polymorphic functionalism the parameter
       * space composite is providing.\n
       *
       * IMPORTANT NOTE: For simpler syntax provide the same function returning
       * a reference or even better overload the corresponding operators.
       *
       * \return a pointer to the current grid component
       */
      virtual Tcomp_ptr currentItem() const = 0;
      //! destructor
      virtual ~CompositeIterator() { }
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

    protected:
      /*!
       * Parameterized factory method to create the correct iteration memento.
       * Factory method design pattern in use (GoF p.107).
       */
      IteratorMemento<Ctype, CresultData>* setIterationType(
          EiterationType type);

  }; // class template CompositeIterator

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
      typedef typename CompositeIterator<Ctype, CresultData>::Tcomp_ptr
        Tcomp_ptr;

    public:
      //! constructor
      NullIterator(Tcomp_ptr root) : Mcomp_ptr(root) { }
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
      virtual Tcomp_ptr currentItem() const {  return Mcomp_ptr; }

    private:
      Tcomp_ptr Mcomp_ptr;

  }; // class template NullIterator

  /* ======================================================================= */
  // function definitions of class CompositeIterator
  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  IteratorMemento <Ctype, CresultData>*
  CompositeIterator<Ctype, CresultData>::setIterationType(
      EiterationType type)
  {
    IteratorMemento<Ctype, CresultData>* iter_memento = 0;
    //! up to now only post-ordered iteration is implemented.
    if (PostOrder == type) 
    { 
      iter_memento = new PostIterationMemento<Ctype, CresultData>;
    } else
    if (PreOrder == type) 
    { 
      iter_memento = new PreIterationMemento<Ctype, CresultData>;
    }
    else { OPTIMIZE_abort("Illegal iteration type."); }

    return iter_memento;

  } // function CompositeIterator<Ctype, CresultData>::setIterationType

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard

/* ----- END OF iterator.h  ----- */
