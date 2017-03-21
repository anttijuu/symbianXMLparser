#ifndef __XMLDOCUMENT_H__
#define __XMLDOCUMENT_H__

/*
 * $Id: XMLDocument.h 720 2008-10-10 17:55:45Z antti $
 *
 * Created 2008/06/02 by Antti Juustila
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


#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include <f32file.h> // RFs

#include "XmlElement.h"

namespace org
{
namespace ajj
{

class MXmlVisitor;

/**
 * This class holds XML elements and document information and can be
 * used to export XML elements into a text file
 * @todo Make the ownership of elements clearer in different methods
 * @todo Add document specific member variables and methods to handle doc specific xml attributes.
 * @author Antti Juustila
 * @version $Revision: 720 $
 * 
 */

class CXmlDocument : public CBase
	{
public:
	IMPORT_C static CXmlDocument* NewL(RXmlElementArray & aArray, TBool aOwnsElements = ETrue);
	IMPORT_C CXmlDocument(TBool aOwnsElements = ETrue);
	IMPORT_C ~CXmlDocument();

public:
	IMPORT_C void GetElementsL(RXmlElementArray & aArray);
	IMPORT_C void AddElementsL(RXmlElementArray & aArray);
	IMPORT_C void AddElementL(const CXmlElement * aElement);
	IMPORT_C void ExportToFileL(const TDesC & aFileName) const;
	IMPORT_C void ExportToFileL(RFs & aFs, const TDesC & aFileName) const;
	IMPORT_C HBufC8 * ExportToUtf8L() const;
	IMPORT_C HBufC8 * ExportToUtf8LC() const;
	IMPORT_C void Reset();
	IMPORT_C RXmlElementArray & Elements();
	IMPORT_C const RXmlElementArray & Elements() const;

	IMPORT_C virtual void AcceptL(MXmlVisitor & aVisitor);
	
private:
	void ConstructL(RXmlElementArray & aArray);

private:
	/**
	 * The array of xml element objects.
	 */
	RXmlElementArray iElements;
	/**
	 * Does the document own the elements or not.
	 */
	TBool iOwnsElements;
	};

} // org
} // ajj


#endif  // __XMLDOCUMENT_H__


