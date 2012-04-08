/*! \file threadpool.h
 * \brief Declaration of a threadpool class template to provide concurrent
 * computing. 
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 07/04/2012
 * 
 * Purpose:  Declaration of a threadpool class template to provide concurrent
 * computing. 
 *
 * ----
 * This file is part of liboptimize.
 *
 * liboptimize is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liboptimize is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liboptimize.  If not, see <http://www.gnu.org/licenses/>.
 * ----
 * 
 * Copyright (c) 2012 by Daniel Armbruster
 * 
 * REVISIONS and CHANGES 
 * 07/04/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <queue>
#include <boost/thread.hpp>
#include <optimizexx/application.h>
#include <optimizexx/gridcomponent.h>

#ifndef _OPTIMIZEXX_THREADPOOL_H_
#define _OPTIMIZEXX_THREADPOOL_H_

namespace optimize
{
  /*!
   * \brief Namespace which helds classes belonging to concurrent computation.
   *
   * To increase the velocity of computation of \a liboptimizexx results a
   * simple thread pool was implemented making use of the <em>Thread pool
   * design</em> pattern. With means of this pattern parallel calculation of
   * results in a \a liboptimizexx parameter is possible.\n
   *
   * \note Notice that for the implementation of this module the recently
   * introduced thread functionalisms in the C++0x STL library weren't used yet.
   * When compilers (especially \c gcc) will support fully the new standard I'm
   * going to switch completely to the built in modules of STL. Up to now the
   * implementation is done using <a
   * href="http://www.boost.org/doc/libs/release/libs/thread/">Boost's
   * thread library</a>.
   *
   * \defgroup group_thread
   */
  namespace thread
  {

    /* ===================================================================== */
    /*!
     * Declaration of a thread safe task queue. Notice that here the Thread pool
     * design pattern is in use (<a
     * href="http://en.wikipedia.org/wiki/Thread_pool_pattern">Wikipedia</a>).
     * \n
     *
     * Thread safety for the task queue is guaranteed to prevent from deadlocks
     * are other unpleasant side effects. 
     *
     * \ingroup group_thread
     */
    template <typename Ctask>
    class Fifo
    {
      public:
        //! constructor
        Fifo() : MnumPopped(0)
        { }
        //! add a new task to the task queue
        void addTask(Ctask& task);
        //! query function for the next function in the task queue
        Ctask& waitAndPopTask();
        //! query function if task queue is empty
        bool empty() const;
        //! query function for number of popped tasks
        size_t const& getNumPoppedTasks();

      private:
        //! fifo buffer
        std::queue<Ctask> Mqueue;
        //! status variable to save number of popped tasks
        size_t MnumPopped;
        /*!
         * condition variable to guarantee thread safety of class
         */
        boost::condition_variable Mcondition;
        //! mutual exclusion variable to guarantee thread safety
        boost::mutex Mmutex; 

    }; // class template Fifo

    /* ===================================================================== */
    /*!
     * Declaration of a thread pool. Notice that here the thread pool design
     * pattern is in use (<a
     * href="http://en.wikipedia.org/wiki/Thread_pool_pattern">Wikipedia</a>).\n
     *
     * The advantage of this pattern is to reduce the costs which are arised by
     * creating instances of new threads. In contrast to generating new
     * instances of threads a thread pool keeps a defined number of threads
     * which acctually act as \a workers always alive. These workes wake up as
     * soon as the task queue contains tasks that have to be worked off.
     *
     * \ingroup group_thread
     */
    template <typename Ctype, typename CresultData>
    class ThreadPool
    {
      public:
        /* ----------------------------------------------------------------- */
        /*!
         * Member class for a thread pool thread to make handling easier.
         *
         * \ingroup group_thread
         */
        class ThreadHandle
        {
          public:
            //! constructor
            ThreadHandle(bool& active,
                Fifo<GridComponent<Ctype, CresultData>*>& tasks,
                ParameterSpaceVisitor<Ctype, CresultData>& app) :
              Mactive(active), Mtasks(tasks), Mapp(&app)
            { }
            
            //! thread function to be executed
            void operator()()
            {
              while (Mactive) 
              { 
                GridComponent<Ctype, CresultData>* comp_ptr =
                  Mtasks.waitAndPopTask(); 
                if (GridComponent<Ctype, CresultData>::Leaf ==
                    comp_ptr->getComponentType())
                {
                  Mapp->operator()(dynamic_cast<Node<Ctype, CresultData>*>(
                        comp_ptr));
                } else
                {
                  Mapp->operator()(dynamic_cast<Grid<Ctype, CresultData>*>(
                        comp_ptr));
                }
              }
            }

          private:
            //! status if active
            bool& Mactive;
            //! task queue
            Fifo<GridComponent<Ctype, CresultData>*>& Mtasks;
            //! application to execute on tasks
            ParameterSpaceVisitor<Ctype, CresultData>* Mapp;

        }; // class ThreadHandle

      public:
        //! constructor
        ThreadPool(ParameterSpaceVisitor<Ctype, CresultData>& app,
            size_t numThreads = 0) : Mapplication(&app),
          MnumThreads(numThreads), Mactive(false)
        { }

        //! destructor
        ~ThreadPool();
        //! initialize the thread pool
        void initialize();
        //! add a new task
        void addTask(GridComponent<Ctype, CresultData>* task);
        //! stop work of threads in threadpool
        void stop() { Mactive = false; }
        //! query function for number of completed tasks
        size_t const& getCompletedTasksCount();

      private:
        //! application to execute
        ParameterSpaceVisitor<Ctype, CresultData>* Mapplication;
        //! number of threads
        size_t MnumThreads;
        //! status variable
        bool Mactive;
        //! queue of tasks
        Fifo<GridComponent<Ctype, CresultData>*> Mtasks;
        //! thread group
        boost::thread_group Mworkers; 

    }; // class template ThreadPool<Ctype, CresultData>

    /* ===================================================================== */
    // function implementations of class template Fifo
    /* --------------------------------------------------------------------- */
    template <typename Ctask>
    void Fifo<Ctask>::addTask(Ctask& task)
    {
      {
        boost::lock_guard<boost::mutex> lock(Mmutex);
        Mqueue.push(task);
      }
      Mcondition.notify_one();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctask>
    Ctask& Fifo<Ctask>::waitAndPopTask()
    {
      boost::unique_lock<boost::mutex> lock(Mmutex);
      while(Mqueue.empty())
      {
        Mcondition.wait(lock);
      }
      Ctask& retval = Mqueue.front(); 
      Mqueue.pop();
      ++MnumPopped;
      return retval;
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctask>
    bool Fifo<Ctask>::empty() const
    {
      boost::lock_guard<boost::mutex> lock(Mmutex);
      return Mqueue.empty();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctask>
    size_t const& Fifo<Ctask>::getNumPoppedTasks()
    {
      boost::lock_guard<boost::mutex> lock(Mmutex);
      return MnumPopped;
    }

    /* ===================================================================== */
    // function implementations of class template ThreadPool
    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    ThreadPool<Ctype, CresultData>::~ThreadPool()
    {
      Mactive = false;
      //! \todo Not the clean way
      Mworkers.interrupt_all();
      Mworkers.join_all();
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ThreadPool<Ctype, CresultData>::initialize()
    {
      // determine number of threads
      if (0 == MnumThreads)
      {
        MnumThreads = boost::thread::hardware_concurrency();
      }
      // set default value - at least one threadhandle
      if (0 == MnumThreads) { MnumThreads = 1; }

      Mactive = true;
      // create threads
      for (size_t i = 0; i < MnumThreads; ++i)
      {
        Mworkers.create_thread(ThreadHandle(
              Mactive, Mtasks, *Mapplication));
      }
    } // function ThreadPool<Ctype, CresultData>::initialize

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    void ThreadPool<Ctype, CresultData>::addTask(
        GridComponent<Ctype, CresultData>* task)
    {
      Mtasks.addTask(task);
    }

    /* --------------------------------------------------------------------- */
    template <typename Ctype, typename CresultData>
    size_t const& ThreadPool<Ctype, CresultData>::getCompletedTasksCount()
    {
      return Mtasks.getNumPoppedTasks();
    }

    /* --------------------------------------------------------------------- */

  } // namespace thread

} // namespace optimize

#endif // include guard

/* ----- END OF threadpool.h  ----- */
