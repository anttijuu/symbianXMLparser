/*
 * $Id: XMLParserDllMain.cpp 252 2008-06-04 15:13:07Z antti $
 *
 * Created 2008/03/15 by Antti Juustila
 *
 * Version: LGPL 3
 *
 * This file is part of XMLParser dll.
 *
 * XMLParser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * XMLParser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XMLParser.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2008 University of Oulu, Antti Juustila (antti)
 */

//  Include Files  
#include <e32std.h>		 // GLDEF_C
#include "XMLParser.pan"	   // panic codes


//  Global Functions

GLDEF_C void Panic(TXMLParserPanic aPanic)
// Panics the thread with given panic code
	{
	User::Panic(_L("org:ajj:XMLParse"), aPanic);
	}


//  Exported Functions

#ifndef EKA2 // for EKA1 only
EXPORT_C TInt E32Dll(TDllReason /*aReason*/)
// Called when the DLL is loaded and unloaded. Note: have to define
// epoccalldllentrypoints in MMP file to get this called in THUMB.
	{
	return KErrNone;
	}
#endif


