#ifndef __XMLPARSER_PAN__
#define __XMLPARSER_PAN__

/*
 * $Id: XMLParser.pan 251 2008-06-04 15:09:25Z antti $
 *
 * Created 2008/05/28 by Antti Juustila
 *
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

enum TXMLParserPanic
	{
	ENullPointer,
	EInvalidXml
	};


//  Function Prototypes
GLREF_C void Panic(TXMLParserPanic aPanic);


#endif  // __XMLPARSER_PAN__


