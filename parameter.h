/*! \file parameter.h
 * \brief Declaration and implementation of a parameter space parameter.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/02/2012
 * 
 * Purpose: Declaration and implementation of a parameter space parameter.  
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
 
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

#ifndef _PARAMETER_H_
#define _PARAMETER_H_

namespace optimize
{
  /* ======================================================================= */
  /*!
   * Parameter for the parameter space. The template parameter might be used to
   * specify either an integer, single precision floating point, or double
   * precision floating point parameter.
   *
   * Default is double precision.
   */
  template <typename Ctype=double>
  class Parameter
  {
    public:
      /*!
       * constructor
       *
       * \param start start value of parameter
       * \param end end value of parameter
       * \param delta parameter interval
       * \param id parameter name
       * \param unit physical unit of paramter
       */
      Parameter(Ctype start, Ctype end, Ctype delta, std::string const id="", 
          std::string const unit="") : Mstart(start), Mend(end), Mdelta(delta),
          Mid(id), Munit(unit)
      { }
      /*!
       * constructor
       *
       * \param param parameter string
       * \param sep separator
       */
      Parameter(std::string const& param, const char sep=' ');
      //! destructor
      virtual ~Parameter() { }
      //! query function for start value of parameter
      Ctype const& getStart() const { return Mstart; }
      //! query function for end value of parameter
      Ctype const& getEnd() const { return Mend; }
      //! query function for delta value / parameter interval of parameter 
      Ctype const& getDelta() const { return Mdelta; }
      //! query function for parameter id
      std::string const& getId() const { return Mid; }
      //! query function for parameter unit
      std::string const& getUnit() const { return Munit; }
      //! check if a parameter is valid
      bool isValid() const;

    protected:
      /*!
       * read parameter from a parameter string using a sepatator character
       *
       * \param sep seperator
       */
      void readFromString(std::string const& param, const char sep=' ');

    private:
      //! start value
      Ctype Mstart;
      //! end value
      Ctype Mend;
      //! delta interval
      Ctype Mdelta;
      //! id of parameter
      std::string Mid;
      //! physical unit of the parameter
      std::string Munit;

  }; // class template Parameter

  /* ======================================================================= */
  template <typename Ctype>
  Parameter<Ctype>::Parameter(std::string const& param, const char sep) :
      Mid(""), Munit("")
  {
    readFromString(param, sep);
  }

  /* ----------------------------------------------------------------------- */
  template <typename Ctype>
  bool Parameter<Ctype>::isValid() const
  {
    if (Mstart == Mend) { return false; }
    if (((Mstart > 0 && Mend > 0) || (Mstart < 0 && Mend < 0)) &&
        Mdelta > fabs(fabs(Mend) - fabs(Mstart)))
    {
      return false;
    } else
    if (Mdelta > (fabs(Mend) + fabs(Mstart))) { return false; }

    return true;
  } // function Parameter<Ctype>::isValid()

  /* ----------------------------------------------------------------------- */
  template <typename Ctype>
  void Parameter<Ctype>::readFromString(std::string const& param,
      const char sep)
  {
    std::string str(param); 
    std::vector<std::string> tokens;
    while (str.find(sep) != std::string::npos)
    {
      tokens.push_back(str.substr(0,str.find(sep)));
      str = str.substr(str.find(sep));
    }
    tokens.push_back(str);

    std::istringstream iss(tokens[0]);
    iss >> Mstart;
    iss.str(std::string()); iss.clear();
    iss.str(tokens[1]);
    iss >> Mend;
    iss.str(std::string()); iss.clear();
    iss.str(tokens[2]);
    iss >> Mdelta;
    if (tokens.size() > 3)
    {
      Mid = tokens[3];
    }
    if (tokens.size() > 4)
    {
      Munit = tokens[4];
    }
  } // function Parameter<Ctype>::readFromString()

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif

/* ----- END OF parameter.h  ----- */
