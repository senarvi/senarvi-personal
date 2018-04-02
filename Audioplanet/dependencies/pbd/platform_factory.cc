/*
    Copyright (C) 1999 Stephane Conversy

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: platform_factory.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <pbd/platform_factory.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef LINUX_PLATFORM_SUPPORT
#include <pbd/linux_platform.h>
#endif

#ifdef IRIX_PLATFORM_SUPPORT
#include <pbd/irix_platform.h>
#endif

#ifdef SOLARIS_PLATFORM_SUPPORT
#include <pbd/solaris_platform.h>
#endif

/* XXX this shouldn't really be here, but there is no .cc file
   that it belongs in. 
*/

Platform  *Platform::thePlatform = 0;

Platform*
PlatformFactory::create_platform()
{
	#ifdef LINUX_PLATFORM_SUPPORT
	return new LinuxPlatform ();
	#endif

	#ifdef IRIX_PLATFORM_SUPPORT
	return new IrixPlatform ();
	#endif

	#ifdef SOLARIS_PLATFORM_SUPPORT
	return new SolarisPlatform ();
	#endif
	
	return 0;
}
