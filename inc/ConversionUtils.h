#ifndef __CONVERSIONUTILS_H_
#define __CONVERSIONUTILS_H_

/*
 * $Id: ConversionUtils.h 1791 2009-09-03 10:04:41Z antti $
 *
 * Created 2008/05/28 by Antti Juustila
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

#include <e32base.h>

namespace org
{
namespace ajj
{

/** Utility class for converting between Unicode and UTF-8 characters and
 * adding the converted string to another descriptor.
 * @version $Revision: 1791 $
 * @author antti
 */
class ConversionUtils 
	{
public:
	static void AppendToUtf8BufferL(const TDesC & aThingToAdd, TDes8 & aWhereToAdd);
	static void AppendToUtf8BufferEncodedL(const TDesC & aThingToAdd, TDes8 & aWhereToAdd);
	static void AppendToUnicodeBufferL(const TDesC8 & aThingToAdd, TDes & aWhereToAdd);
	static void AppendToUnicodeBufferDecodedL(const TDesC8 & aThingToAdd, TDes & aWhereToAdd);
	};

} // ajj
} // org

#endif /*__CONVERSIONUTILS_H_*/
