/*! \file gridsearch.h
 * \brief Grid search algorithm.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 24/02/2012
 * 
 * Purpose: Grid search algorithm.
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
 * 24/02/2012  V0.1   Daniel Armbruster
 * 08/04/2012  V0.2   Make use of the threadpool take advantage of concurrency.
 * 25/04/2012  V0.3   Make use of smart pointers and C++0x.
 * 
 * ============================================================================
 */

#include <vector>
#include <memory>
#include <optimizexx/globalalgorithm.h>
#include <optimizexx/parameter.h>
#include <optimizexx/threadpool.h>
#include <optimizexx/iterator.h>
#include <optimizexx/error.h>
 
#ifndef _OPTIMIZEXX_GRIDSEARCH_H_
#define _OPTIMIZEXX_GRIDSEARCH_H_

namespace optimize
{
  // forward declarations
  template <typename Ctype, typename CresultData> class ParameterSpaceVisitor;
  template <typename Ctype> class Parameter;

  /* ======================================================================= */
  /*!
   * Grid search algorithm.\n
   *
   * See also: <a
   * href="http://en.wikipedia.org/wiki/Grid_search">Wikipedia</a>\n
   *
   * To use this algorithm an application must be specified which is inherited
   * of ParameterSpaceVisitor.
   *
   * \ingroup group_global_algos
   */
  template <typename Ctype, typename CresultData>
  class GridSearch : public GlobalAlgorithm<Ctype, CresultData>
  {
    public:
      typedef GlobalAlgorithm<Ctype, CresultData> Tbase;
      
    public:
      /*!
       * constructor
       *
       * \param parameterspacebuilder Pointer to a builder of a parameter space.
       * Default is a StandardParameterSpaceBuilder.
       * \param num_threads Number of threads the algorithm uses for parallel
       * computation
       */
      GridSearch(
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder,
          size_t num_threads=0) :
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
          Tbase(nullptr, std::move(builder)), MnumThreads(num_threads)
#else
          Tbase(std::move(builder)), MnumThreads(num_threads)
#endif
      { }
      /*!
       * constructor
       *
       * \param parameterspacebuilder Pointer to a builder of a parameter space.
       * Default is a StandardParameterSpaceBuilder.
       * \param parameters STL vector of pointers to parameters.
       * \param num_threads Number of threads the algorithm uses for parallel
       * computation
       */
      GridSearch(
          std::unique_ptr<ParameterSpaceBuilder<Ctype, CresultData>> builder,
          std::vector<std::shared_ptr<Parameter<Ctype> const>> const parameters,
          size_t num_threads=0) :
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
          Tbase(nullptr, std::move(builder), parameters),
          MnumThreads(num_threads)
#else
          Tbase(std::move(builder), parameters), MnumThreads(num_threads)
#endif
      { }
      /*!
       * Construct a parameter space. Before constructing a parameter space for
       * a global algorithm don't forget to make parameters available.
       */
      virtual void constructParameterSpace();
      /*!
       * Apply an application to the parameter space grid. Makes use of the \a
       * liboptimizexx thread pool to increase calculation velocity due to
       * concurrency.
       *
       * \param v Reference to a ParameterSpaceVisitor or rather application.
       * For further information on how to implement an application for the
       * parameter space \sa ParameterSpace
       */
      virtual void execute(ParameterSpaceVisitor<Ctype, CresultData>& v);

    private:
      //! status variable if algorithm uses multiple threads
      size_t MnumThreads;

  }; // class template GridSearch

  /* ======================================================================= */
  template <typename Ctype, typename CresultData>
  void GridSearch<Ctype, CresultData>::constructParameterSpace()
  {
    OPTIMIZE_assert(Tbase::Mparameters.size() != 0, "Missing parameters.");
    Tbase::MparameterSpaceBuilder->buildParameterSpace();
    Tbase::MparameterSpaceBuilder->buildGrid(Tbase::Mparameters);
    Tbase::MparameterSpace = 
      Tbase::MparameterSpaceBuilder->getParameterSpace();
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype, typename CresultData>
  void GridSearch<Ctype, CresultData>::execute(
      ParameterSpaceVisitor<Ctype, CresultData>& v)
  {
    OPTIMIZE_assert(Tbase::MparameterSpace, "Missing parameter space.");

    Iterator<Ctype, CresultData> iter(
        Tbase::MparameterSpace->createIterator(ForwardNodeIter));

    // simple single threading execution
    if (0 == MnumThreads)
    {
      for (iter.first(); !iter.isDone(); ++iter)
      {
        (*iter)->accept(v);
      }
    } else
    {
      size_t num_tasks = 0;

      // create thread pool for parallel computation
      typename thread::ThreadPool<Ctype, CresultData>* pool =
        new typename thread::ThreadPool<Ctype, CresultData>(v, MnumThreads);
      pool->initialize();

      // add tasks (node pointers) to pool task queue
      for (iter.first(); !iter.isDone(); ++iter)
      {
        pool->addTask(*iter);
        ++num_tasks;
      }

      // wait until all threads finished
      while(num_tasks != pool->getCompletedTasksCount()) { }

      delete pool;
    }
  } // function GridSearch<Ctype, CresultData>::execute

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard

/* ----- END OF gridsearch.h  ----- */
