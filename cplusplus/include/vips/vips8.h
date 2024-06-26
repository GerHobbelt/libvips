// Include file to get vips C++ binding

/*

    This file is part of VIPS.
    
    VIPS is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA

 */

/*

    These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

#ifndef VIPS_CPLUSPLUS
#define VIPS_CPLUSPLUS

#include <vips/version.h>

#ifdef HAVE_GLIB_OBJECT_H
#include <glib-object.h>
#endif

/* Define VIPS_CPLUSPLUS_EXPORTS to build a DLL using MSVC.
 */
#ifdef _VIPS_PUBLIC
#  define VIPS_CPLUSPLUS_API _VIPS_PUBLIC
#elif defined(_MSC_VER) && !defined(VIPS_STATICLIB)
#  ifdef VIPS_CPLUSPLUS_EXPORTS
#    define VIPS_CPLUSPLUS_API __declspec(dllexport)
#  else
#    define VIPS_CPLUSPLUS_API __declspec(dllimport)
#  endif
#else
#  define VIPS_CPLUSPLUS_API
#endif

#define VIPS_NAMESPACE_START namespace vips {
#define VIPS_NAMESPACE_END }

#include "VError8.h"
#include "VImage8.h"
#include "VInterpolate8.h"
#include "VRegion8.h"
#include "VConnection8.h"

#endif /*VIPS_CPLUSPLUS*/
