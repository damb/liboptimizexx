/*! \file parameter.cc
 * \brief 
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 30/03/2012
 * 
 * Purpose: 
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
 * 30/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <optimizexx/parameter.h>

namespace optimize
{
  /* ----------------------------------------------------------------------- */
  double DimensionlessParameter::getDelta() const
  {
    return 1./static_cast<double>(Msamples-1); 
  }

  /* ----------------------------------------------------------------------- */

} // namespace optimize

/* ----- END OF parameter.cc  ----- */
