/*! \file error.cc
 * \brief exception class for liboptimizexx (implementation)
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Thomas Forbriger, Daniel Armbruster
 * \date 30/03/2004
 * 
 * exception class for liboptimizexx (implementation)
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
 *  - 07/07/2006   V1.1   added report function
 *  - 06/09/2011   V1.2   introduced report_deprecated
 *  - 27/02/2012          taken from Thomas Forbrigers libraries
 * 
 * ============================================================================
 */
#include <iostream>
#include <optimizexx/error.h>

using std::cerr;
using std::endl;

namespace optimize
{

  //! initialize and instantiate
  bool Exception::Mreport_on_construct=true;

  //! construct from nothing
  Exception::Exception(): 
    Mmessage(0), Mfile(0), Mline(0), Mcondition(0)
    { if (Mreport_on_construct) { report(); } }

  //! construct with message
  Exception::Exception(const char* message):
    Mmessage(message), Mfile(0), Mline(0), Mcondition(0)
    { if (Mreport_on_construct) { report(); } }

  //! construct with message and file info
  Exception::Exception(const char* message,
                       const char* condition):
    Mmessage(message), Mfile(0), Mline(0), Mcondition(condition)
    { if (Mreport_on_construct) { report(); } }

  //! construct with message and file info
  Exception::Exception(const char* message,
                       const char* file,
                       const int& line):
    Mmessage(message), Mfile(file), Mline(line), Mcondition(0)
    { if (Mreport_on_construct) { report(); } }

  //! construct with message and file info and condition
  Exception::Exception(const char* message,
                       const char* file,
                       const int& line,
                       const char* condition):
    Mmessage(message), Mfile(file), Mline(line), Mcondition(condition)
    { if (Mreport_on_construct) { report(); } }
      
  //! switch on
  void Exception::report_on_construct() 
  {
  Mreport_on_construct=true;
  }

  //! switch off
  void Exception::dont_report_on_construct()
  {
  Mreport_on_construct=false;
  }

  //! report
  void Exception::report() const
  {
    base_report();
  }

  //! report
  void Exception::base_report() const
  {
    cerr << "Exception report:" << endl;
    if (Mmessage==0)
    {
      cerr << "  No message" << endl;
    }
    else
    {
      cerr << "  message: " << Mmessage << endl;
    }
    if (Mfile!=0)
    {
      cerr << "  triggered in \"" << Mfile << "\" at line #" << Mline << endl;
    }
    if (Mcondition!=0)
    {
      cerr << "  by violation of condition:" << endl
        << "    \"" << Mcondition << "\"" << endl;
    }
  }

  /*----------------------------------------------------------------------*/

  //! report violation of assertion
  void report_violation(const char* message, 
                        const char* file,
                        const int& line,
                        const char* condition)
  {
    std::cerr << std::endl;
    std::cerr << "VIOLATION of condition: " << condition << std::endl; 
    std::cerr << "* in " << file  << " at line " << line << std::endl; 
    std::cerr << "* message: " << message << std::endl; 
  }

  /*----------------------------------------------------------------------*/

  //! report deprecated function
  void report_deprecated(const char* function,
                         const char* reason)
  {
    std::cerr << "WARNING: program uses deprecated function in "
      << "liboptimizexx\n"
      << "* " << function << std::endl; 
    std::cerr << "* This function should no longer be used because\n"
      << "* " << reason << std::endl;
    std::cerr << "* Please place a ticket at "
      "http://gpitrsvn.gpi.uni-karlsruhe.de:8000/TFSoftware"
      << std::endl;
  }

} // namespace optimize

/* ----- END OF error.cc ----- */
