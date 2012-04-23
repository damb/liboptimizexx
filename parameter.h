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
#include <cmath>
#include <optimizexx/error.h>

#ifndef _OPTIMIZEXX_PARAMETER_H_
#define _OPTIMIZEXX_PARAMETER_H_

namespace optimize
{
  /* ======================================================================= */
  /*!
   * Abstract base class template for \a liboptimizexx parameters.
   *
   * Builder design pattern in use (GoF p.151). optimize::Parameter corresponds
   * to the abstract \c Implementor in GoF.\n
   *
   * \note The implementation of \a liboptimizexx does not exclude from creating
   * parameter spaces which are created from both optimize::Standardparameter
   * and optimize::DimensionlessParameter.
   */
  template <typename Ctype=double>
  class Parameter
  {
    public:
      virtual ~Parameter() { }
      //! query function for start value of parameter
      virtual Ctype getStart() const = 0;
      //! query function for end value of parameter
      virtual Ctype getEnd() const = 0;
      //! query function for delta value / parameter interval of parameter 
      virtual Ctype getDelta() const = 0;
      //! query function for the number of samples the parameter contains
      virtual unsigned int getSamples() const = 0;
      //! query function for parameter unit
      virtual std::string getUnit() const = 0;
      //! query function for parameter id
      virtual std::string const& getId() const { return Mid; }
      //! check if a parameter is valid
      virtual bool isValid() const = 0;

    protected:
      /*!
       * constructor
       *
       * \param id Identifier for a parameter
       */
      Parameter(std::string const id="") : Mid(id) { }

    protected:
      //! id of parameter
      std::string Mid;

  }; // class template Parameter

  /* ======================================================================= */
  /*!
   * StandardParameter for the parameter space. The template parameter might
   * be used to specify either an integer, single precision floating point,
   * or double precision floating point parameter.\n
   *
   * Default is double precision.\n
   *
   * Builder design pattern in use (GoF p.151). optimize::StandardParameter
   * corresponds to a concrete implementor in GoF.
   */
  template <typename Ctype=double>
  class StandardParameter : public Parameter<Ctype>
  {
    public:
      //! typedef to base class
      typedef Parameter<Ctype> Tbase;
    
    public:
      /*!
       * constructor
       *
       * \param id parameter name
       * \param start start value of parameter
       * \param end end value of parameter
       * \param delta parameter interval
       * \param unit physical unit of paramter
       */
      StandardParameter(std::string const id, Ctype start, Ctype end,
          Ctype delta, std::string const unit="") : Tbase(id), Mstart(start),
          Mend(end), Mdelta(delta), Munit(unit)
      { }
      //! destructor
      virtual ~StandardParameter() { }
      //! query function for start value of parameter
      virtual Ctype getStart() const { return Mstart; }
      //! query function for end value of parameter
      virtual Ctype getEnd() const { return Mend; }
      //! query function for delta value / parameter interval of parameter 
      virtual Ctype getDelta() const { return Mdelta; }
      //! query function for the number of samples the parameter contains
      virtual unsigned int getSamples() const;
      //! query function for parameter unit
      virtual std::string getUnit() const { return Munit; }
      //! check if a parameter is valid
      virtual bool isValid() const;

    private:
      //! start value
      Ctype Mstart;
      //! end value
      Ctype Mend;
      //! delta interval
      Ctype Mdelta;
      //! physical unit of the parameter
      std::string Munit;

  }; // class template StandardParameter

  /* ======================================================================= */
  /*!
   * Dimensionless parameter to create a dimensionless parameter space.
   * \ņote If a parameter space had been created with a dimensionless parameter
   * the application / visitor traversing the parameter space then has to decide
   * how to interpret the values.\n
   *
   * Builder design pattern in use (GoF p.151).
   * optimize::DimensionlessParameter corresponds to a concrete implementor in
   * GoF.
   */
  class DimensionlessParameter : public Parameter<double>
  {
    public:
      /*!
       * constructor
       *
       * \param id Identifer of the parameter
       * \param samples number of samples
       */
      DimensionlessParameter(std::string const id,
          unsigned int const samples) : Tbase(id), Msamples(samples)
      { }
      //! destructor
      virtual ~DimensionlessParameter() { }
      //! query function for start value of parameter
      virtual double getStart() const { return 0; }
      //! query function for end value of parameter
      virtual double getEnd() const { return 1; }
      //! query function for delta value / parameter interval of parameter 
      virtual double getDelta() const;
      //! query function for the number of samples the parameter contains
      virtual unsigned int getSamples() const { return Msamples; }
      //! query function for parameter unit
      virtual std::string getUnit() const { return ""; }
      //! check if a parameter is valid
      virtual bool isValid() const { return Msamples > 2; }

    private:
      //! typedef for base class
      typedef Parameter<double> Tbase;
      //! number of samples
      unsigned int Msamples;

  }; // class DimensionlessParameter

  /* ======================================================================= */
  // function implementation of class template StandardParameter
  /* ======================================================================= */
  template <typename Ctype>
  unsigned int StandardParameter<Ctype>::getSamples() const
  {
    return static_cast<unsigned int>(ceil(fabs(Mend - Mstart) / Mdelta)+1);
  } // function StandardParameter<Ctype>::getSamples

  /* ----------------------------------------------------------------------- */
  template <typename Ctype>
  bool StandardParameter<Ctype>::isValid() const
  {
    if (Mstart == Mend) { return false; }
    if (((Mstart > 0 && Mend > 0) || (Mstart < 0 && Mend < 0)) &&
        Mdelta > fabs(fabs(Mend) - fabs(Mstart)))
    {
      return false;
    } else
    if (Mdelta > (fabs(Mend) + fabs(Mstart))) { return false; }

    return true;
  } // function StandardParameter<Ctype>::isValid

  /* ----------------------------------------------------------------------- */

} // namespace optimize

#endif // include guard

/* ----- END OF parameter.h  ----- */
