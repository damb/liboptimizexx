/*! \file error.h
 * \brief exception class declaration for liboptimizexx (prototypes)
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Thomas Forbriger, Daniel Armbruster
 * \date 30/03/2004
 * 
 * exception class declaration for liboptimizewxx (prototypes)
 *
 * ----
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * ----
 * 
 * Copyright (c) 2004 by Thomas Forbriger (BFO Schiltach) 
 * Copyright (c) 2012 by Daniel Armbruster (BFO Schiltach) 
 * 
 * REVISIONS and CHANGES 
 *  - 30/03/2004   V1.0   Thomas Forbriger
 *  - 28/04/2006   V1.1   provide explicit virtual destructor
 *  - 07/07/2006   V1.2   provide non-fatal behaviour
 *  - 06/09/2011   V1.3   introduced report_deprecated
 *  - 27/02/2012          taken from Thomas Forbrigers libraries
 * 
 * ============================================================================
 */

#ifndef _OPTIMIZEERROR_H_
#define _OPTIMIZEERROR_H_

#include <iostream>

namespace optimize
{

/*! \defgroup group_error Error handling module
 */

  /*! \brief Base class for exceptions
   *
   * This is an exception base class. It holds some information about the
   * reason for throwing the exception. The information is printed to cerr
   * through function report(). This function may be overloaded by a derived
   * type. But its functionality is still accessible through base_report().
   *
   * The standard behaviour is to print out the message during object
   * initialization. If you don't like this, call dont_report_on_construct().
   *
   * \ingroup group_error
   * \sa OPTIMIZE_Xassert
   * \sa OPTIMIZE_assert
   * \sa OPTIMIZE_abort
   */
  class Exception 
  {
    public:
      //! Creates exception with no explaning comments
      Exception();
      //! Creates an exception with an explanation message
      Exception(const char* message);
      //! Creates an exception with message and failed assertion
      Exception(const char* message, 
                const char* condition);
      //! Create with message, failed assertion, and code position
      Exception(const char* message, 
                const char* file,
                const int& line,
                const char* condition);
      //! Create with message and code position
      Exception(const char* message, 
                const char* file,
                const int& line);
      //! provide explicit virtual destructor
      virtual ~Exception() { }
      //! Screen report
      virtual void report() const;
      //! Issue a screen report on construction of exception
      static void report_on_construct();
      //! Issue NO screen report on construction of exception
      static void dont_report_on_construct();
      //! set report on construct flag
      static void report_on_construct_flag(const bool& flag)
      { Mreport_on_construct=flag; }
      //! return report on construct flag
      static bool report_on_construct_flag() { return(Mreport_on_construct); }
    protected:
      //! Screen report
      void base_report() const;
    private:
      //! Shall we print to cerr at construction time?
      static bool Mreport_on_construct;
      //! pointer to message string
      const char* Mmessage;
      //! pointer to file name string
      const char* Mfile;
      //! pointer to line number in source file
      const int& Mline;
      //! pointer to assertion condition text string
      const char* Mcondition;
  }; // class Exception

/*! \brief report violation of condition
 *
 * \ingroup group_error
 * \param message       message of type char*
 * \param file          name of source code file
 * \param line          source code line number
 * \param condition     assert condition
 */
  void report_violation(const char* message, 
                        const char* file,
                        const int& line,
                        const char* condition);

/*! \brief report deprecation of a function
 *
 * \ingroup group_error
 * \param function      name of deprecated function
 * \param reason        the reason for deprecating the function
 *                      should finish a sentence which started with "because"
 */
  void report_deprecated(const char* function,
                         const char* reason);

} // namespace optimize

/*======================================================================*/
//
// preprocessor macros
// ===================

/*! \brief Check an assertion and report by throwing an exception.
 *
 * \ingroup group_error
 * \param C assert condition
 * \param M message of type char*
 * \param E exception class to throw
 */
#define OPTIMIZE_Xassert(C,M,E) \
  if (!(C)) { throw( E ( M , __FILE__, __LINE__, #C )); }

/*! \brief Check an assertion and report by throwing an exception.
 *
 * \ingroup group_error
 * \param C assert condition
 * \param M message of type char*
 */
#define OPTIMIZE_assert(C,M) OPTIMIZE_Xassert( C , M , optimize::Exception )

/*! \brief Abort and give a message.
 *
 * \ingroup group_error
 * \param M message of type char*
 * \param E exception class to throw
 */
#define OPTIMIZE_abort(M) \
  throw( optimize::Exception ( M , __FILE__, __LINE__ )) 

#define OPTIMIZE_illegal OPTIMIZE_abort("illegal call!")


/*! \brief Check an assertion and report only.
 *
 * \ingroup group_error
 * \param C assert condition
 * \param M message of type char*
 * \param V any values that should be output (comment)
 *          a sequence of values and output operators
 */
#define OPTIMIZE_report_assert(C,M,V) \
  if (!(C)) { \
    optimize::report_violation(M, __FILE__, __LINE__, #C); \
    std::cerr << "* comment: " << V << std::endl; \
    std::cerr << std::endl; \
    std::cerr.flush(); \
  }

/*! \brief Macro to distinguish between fatal and non fatal assertions.
 *
 * \ingroup group_error
 * \param F true for non fatal behaviour
 * \param C assert condition
 * \param M message of type char*
 * \param V any values that should be output (comment)
 *          a sequence of values and output operators
 */
#define OPTIMIZE_nonfatal_assert(F,C,M,V) \
  if (F) { OPTIMIZE_report_assert(C,M,V) } else { OPTIMIZE_assert(C,M) }

#endif // _ERROR_H_ (include guard)

/* ----- END OF error.h ----- */
