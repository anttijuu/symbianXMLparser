#ifndef __XMLPARSERCONSTANTS_H
#define __XMLPARSERCONSTANTS_H

/*
 * $Id: XMLParserConstants.h 1171 2009-03-13 13:51:18Z antti $
 *
 * Created 2008/06/04 by Antti Juustila
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


_LIT8(KXMLHeaderWithUTF8Encoding, "<?xml version=\"1.0\" encoding=\"utf-8\"?>");

_LIT(KCDataStart, "<![CDATA[");
_LIT(KCDataEnd, "]]>");
_LIT8(KCDataStart8, "<![CDATA[");
_LIT8(KCDataEnd8, "]]>");
_LIT8(KXmlNs, "xmlns");

_LIT(KLTReference, "&lt;");
_LIT(KGTReference, "&gt;");
_LIT(KAmpersandReference, "&amp;");
_LIT(KPercentReference, "&#37;");

_LIT8(KLTReference8, "&lt;");
_LIT8(KGTReference8, "&gt;");
_LIT8(KAmpersandReference8, "&amp;");
_LIT8(KPercentReference8, "&#37;");

_LIT8(KCharEquals8, "=");
//const TText8 KCharLessThan8 = '<';
_LIT8(KCharLessThan8, "<");
_LIT8(KCharGreaterThan8, ">");
_LIT8(KCharSlash8, "/");
_LIT8(KCharQuote8, "\"");
_LIT8(KCharSpace8, " ");
_LIT8(KCharColon8, ":");
_LIT8(KCharAmpersand8, "&");
_LIT8(KCharPercent8, "%");

_LIT(KCharEquals, "=");
_LIT(KCharLessThan, "<");
_LIT(KCharGreaterThan, ">");
_LIT(KCharSlash, "/");
_LIT(KCharQuote, "\"");
_LIT(KCharSpace, " ");
_LIT(KCharColon, ":");
_LIT(KCharAmpersand, "&");
_LIT(KCharPercent, "%");

#endif

