/*
 * $Id: XmlDocument.cpp 1593 2009-08-19 19:35:59Z antti $
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

#include <utf.h>

#include "XmlDocument.h"
#include "XMLParserConstants.h"
#include "XmlVisitor.h"

namespace org
{
namespace ajj
{

/** Two phase constructor for the class.
 * @param aArray An array of XML elements this document will hold.
 * @param aOwnsElements ETrue, if owns the elements and destroys them in the destructor.
 * @returns A new CXmlDocument object.
 */
EXPORT_C CXmlDocument* CXmlDocument::NewL(RXmlElementArray & aArray, TBool aOwnsElements)
	{
	CXmlDocument * me = new (ELeave) CXmlDocument(aOwnsElements);
	CleanupStack::PushL(me);
	me->ConstructL(aArray);
	CleanupStack::Pop();
	return me;
	}

/** A C++ constructor for the class.
 * @param aOwnsElements ETrue, if owns the elements and destroys them in the destructor.
 */
EXPORT_C CXmlDocument::CXmlDocument(TBool aOwnsElements) : iOwnsElements(aOwnsElements)
	{
	}

/** Second phase constructor for the class. Moves elements from the aArray 
 * to the internal array of this class.
 * @param aArray An array of XML elements this document will hold.
 */
void CXmlDocument::ConstructL(RXmlElementArray & aArray)
	{
	AddElementsL(aArray);
	}

/**
 * Desctructor for this class. Calls Reset()
 */
EXPORT_C CXmlDocument::~CXmlDocument()
	{
	Reset();
	}

/**
 * Resets the document object by resetting the CXmlElement array.
 * If the document owns the array elements, will destroy them, otherwise
 * just empties the array.
 */
EXPORT_C void CXmlDocument::Reset()
	{
	if (iOwnsElements)
		{
		iElements.ResetAndDestroy();
		}
	else
		{
		iElements.Reset();
		}
	}


/**
 * Extracts the elements from the document to the aArray. Note that
 * the elements are removed from the document and the caller is responsible
 * for destroying the CXmlElements when no longer needed. Use Elements()
 * methods to get access to the XML elements without removing them from
 * the document.
 * @param aArray The array where elements are moved to.
 */
EXPORT_C void CXmlDocument::GetElementsL(RXmlElementArray & aArray)
	{
	while (iElements.Count() > 0)
		{
		aArray.AppendL(iElements[0]);
		iElements.Remove(0);
		}
	}

/** Adds new elements to this document and removes them from the aArray.
 * @param aArray Array holding the elements to add.
 */
EXPORT_C void CXmlDocument::AddElementsL(RXmlElementArray & aArray)
	{
	while (aArray.Count() > 0)
		{
		iElements.AppendL(aArray[0]);
		aArray.Remove(0);
		}
	}

/** 
 * Adds a singel CXmlElement to the array of elements in this document.
 * @param aElement The element to add.
 */
EXPORT_C void CXmlDocument::AddElementL(const CXmlElement * aElement)
	{
	iElements.AppendL(aElement);
	}

/**
 * Get a reference to the array of XML elements in the document.
 * Use this if you do not want to move the elements away from the 
 * document using GetElementsL.
 * @returns The array of XML elements.
 */
EXPORT_C RXmlElementArray & CXmlDocument::Elements()
	{
	return iElements;
	}

/**
 * Get a reference to the array of XML elements in the document, const
 * version.
 * Use this if you do not want to move the elements away from the 
 * document using GetElementsL.
 * @returns The array of XML elements.
 */
EXPORT_C const RXmlElementArray & CXmlDocument::Elements() const
	{
	return iElements;
	}

/**
 * Exports the XML document and it's elements to a file.
 * @param aFileName The file to export the data to.
 */
EXPORT_C void CXmlDocument::ExportToFileL(const TDesC & aFileName) const
	{
	TInt counter;
	TInt count = iElements.Count();
	
	RFs fsSession;
	RFile file;
	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL(fsSession);
	User::LeaveIfError(file.Replace(fsSession, aFileName, EFileWrite));
	CleanupClosePushL(file);
	
	User::LeaveIfError(file.Write(KXMLHeaderWithUTF8Encoding));
	// Save one element at a time, so memory overhead is not so big.
	// Even though this means more allocations and deallocations.
	for (counter = 0; counter < count; counter++)
		{
		TInt bufSize = iElements[counter]->ApproximateTextLength();
		bufSize *= 2;
		HBufC8 * buf = HBufC8::NewLC(bufSize);
		TPtr8 ptr(buf->Des());
		iElements[counter]->GetAsTextL(ptr);
		User::LeaveIfError(file.Write(*buf));
		CleanupStack::PopAndDestroy(); // buf
		}
	CleanupStack::PopAndDestroy(2); // file, fsSession
	}


/**
 * Exports the XML document and it's elements to a file using an existng
 * RFs session object.
 * @param aFs The file server session.
 * @param aFileName The file to export the data to.
 */
EXPORT_C void CXmlDocument::ExportToFileL(RFs & aFs, const TDesC & aFileName) const
	{
	TInt counter;
	TInt count = iElements.Count();
	
	RFile file;
	User::LeaveIfError(file.Replace(aFs, aFileName, EFileWrite));
	CleanupClosePushL(file);
	
	User::LeaveIfError(file.Write(KXMLHeaderWithUTF8Encoding));
	// Save one element at a time, so memory overhead is not so big.
	// Even though this means more allocations and deallocations.
	for (counter = 0; counter < count; counter++)
		{
		TInt bufSize = iElements[counter]->ApproximateTextLength();
		bufSize *= 2;
		HBufC8 * buf = HBufC8::NewLC(bufSize);
		TPtr8 ptr(buf->Des());
		iElements[counter]->GetAsTextL(ptr);
		User::LeaveIfError(file.Write(*buf));
		CleanupStack::PopAndDestroy(); // buf
		}
	CleanupStack::PopAndDestroy(); // file
	}

/**
 * Exports the XML document and it's elements into a 8 bit descriptor.
 * Uses the XCmlElement::ApproximateTextLength to calculate how much memory
 * is needed, then allocates the memory and exports the elements into this
 * buffer. For large structure, there might not be enough memory for this.
 * @returns Dynamically allocated descritor holding the XML as text.
 */ 
EXPORT_C HBufC8 * CXmlDocument::ExportToUtf8L() const
	{
	HBufC8 * buf = ExportToUtf8LC();
	CleanupStack::Pop(); // the buf at stack
	return buf;
	}

/**
 * Exports the XML document and it's elements into a 8 bit descriptor left 
 * in the cleanup stack. Caller must take care of Pop/PopAndDestroy.
 * Uses the XCmlElement::ApproximateTextLength to calculate how much memory
 * is needed, then allocates the memory and exports the elements into this
 * buffer. For large structure, there might not be enough memory for this.
 * @returns Dynamically allocated descritor holding the XML as text.
 */ 
EXPORT_C HBufC8 * CXmlDocument::ExportToUtf8LC() const
	{
	TInt bufSize = 100;
	TInt counter;
	const TInt count = iElements.Count();
	for (counter = 0; counter < count; counter++)
		{
		bufSize += iElements[0]->ApproximateTextLength();
		}
	HBufC8 * buf = HBufC8::NewLC(bufSize);
	TPtr8 ptr(buf->Des());
	ptr.Append(KXMLHeaderWithUTF8Encoding);
	for (counter = 0; counter < count; counter++)
		{
		iElements[0]->GetAsTextL(ptr);
		}
	return buf;
	}

/**
 * Accepts a visitor to visit this object.
 * @param aVisitor The visitor.
 */
EXPORT_C void CXmlDocument::AcceptL(MXmlVisitor & aVisitor)
	{
	aVisitor.VisitL(*this);
	if (!aVisitor.VisitorNavigates())
		{
		TInt counter = 0;
		for (; counter < iElements.Count(); counter++)
			{
			iElements[counter]->AcceptL(aVisitor);
			}
		}
	}

} // ajj
} // org
