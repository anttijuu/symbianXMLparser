/*
 * $Id: XMLParser.cpp 1176 2009-03-16 09:29:51Z antti $
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

//  Include Files  
#include <bautils.h>
#include <xml\attribute.h>
#include <xml\parserfeature.h>

#include "XMLParser.h"	// CXMLParser
#include "XmlDocument.h"
#include "XMLParserConstants.h"

#ifdef USE_DEBUGLOGGER
#include "DebugLogger.h"
#endif


#include "XMLParser.pan"	  	// panic codes

//  Member Functions

namespace org
{
namespace ajj
{

/**
 * Creates a new XML parser.
 * @returns The XML parser object.
 */
EXPORT_C CXmlParser* CXmlParser::NewL(MXmlParserObserver & aObserver)
	{
	CXmlParser* self = new (ELeave) CXmlParser(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/** Default constructor, initializes base class and member variables. */
CXmlParser::CXmlParser(MXmlParserObserver & aObserver)
: CActive(CActive::EPriorityLow), iObserver(aObserver), iBytesToParseInStep(2048), iIsParsing(EFalse), iDoSynchronously(EFalse)
	{
	}

/** 2nd phase constructor.
 * Connects to the file server and creates the XML parser.
 */
void CXmlParser::ConstructL()
	{
	_LIT8(KMIMETextXml, "text/xml");
	User::LeaveIfError(iFs.Connect());
	iXmlParser = Xml::CParser::NewL(KMIMETextXml, *this);
#ifdef USE_DEBUGLOGGER
	iLogger = oy::tol::CDebugLogger::Instance();
#endif
	CActiveScheduler::Add(this);
	}
/**
 * Desctructor for the parser.
 * Closes file server session, deletes the XML buffer and XML parser.
 * Does not delete the XML objects created by the parser.
 * Client is responsible for retrieving the placemarks by calling
 * GetPlacemarksL and then destroying the objects from the container
 * when no longer needed.
 */
EXPORT_C CXmlParser::~CXmlParser()
	{
	Cancel();
	iFs.Close();
	delete iFileBuffer;
	using namespace Xml;
	delete iXmlParser;
	iElements.Reset(); // Client takes ownership of objects!!
	iElements.Close();
	iXmlNameSpaces.Reset(); // Objects moved to the topmost CXmlElement when done parsing.
	iXmlNameSpaces.Close();
	}

/**
 * Query the version number of the parser.
*/
EXPORT_C TVersion CXmlParser::Version() const
	{
	// Version number of example API
	const TInt KMajor = 1;
	const TInt KMinor = 0;
	const TInt KBuild = 0;
	return TVersion(KMajor, KMinor, KBuild);
	}

/**
 * Parsers a XML file, synchronously. Use only for small files.
 * @todo If using large files, read and parse in pieces.
 * @param aFileName The file containing the XML.
 * @returns KErrNone if all went well.
 */
EXPORT_C TInt CXmlParser::ParseXmlFileSyncL(const TDesC & aFileName)
	{
	iDoSynchronously = ETrue;
	ParseXmlFileL(aFileName);
	iDoSynchronously = EFalse;
	return iError;
	}


/**
 * Parsers a XML file, asyncronously.
 * @todo If using large files, read and parse in pieces.
 * @param aFileName The file containing the XML.
 * @returns KErrNone if all went well.
 */
EXPORT_C TInt CXmlParser::ParseXmlFileL(const TDesC & aFileName)
	{
	if (!BaflUtils::FileExists(iFs, aFileName))
		{
#ifdef USE_DEBUGLOGGER
		iLogger->Write(oy::tol::KLogLevelHigh, _L("File parsing: file not found."));
#endif
		User::Leave(KErrNotFound);
		}

	RFile file;
	User::LeaveIfError(file.Open(iFs, aFileName, EFileRead));
	CleanupClosePushL(file);
	TInt size;
	User::LeaveIfError(file.Size(size));
	delete iFileBuffer;
	iFileBuffer = 0;

#ifdef USE_DEBUGLOGGER
	_LIT(KMsg, "Allocating %d bytes for file");
	iLogger->Write(oy::tol::KLogLevelHigh, KMsg, size);
#endif
	iFileBuffer = HBufC8::NewL(size);
	TPtr8 ptr(iFileBuffer->Des());
//TODO: read file in pieces if file size > some constant max size.
	file.Read(ptr);
#ifdef USE_DEBUGLOGGER
	iLogger->Write(oy::tol::KLogLevelHigh, _L("Start parsing now, xml in memory..."));
#endif
	iError = ParseXmlBufferL(*iFileBuffer);
#ifdef USE_DEBUGLOGGER
	if (iDoSynchronously)
		{
		iLogger->Write(oy::tol::KLogLevelHigh, _L("...xml parse done."));
		}
	else
		{
		iLogger->Write(oy::tol::KLogLevelHigh, _L("...xml parse started."));
		}
#endif
	CleanupStack::PopAndDestroy(); // file
	return iError;
	}

/**
 * Parsers a XML buffer, syncronously.
 * @param aBuffer The buffer containing the XML.
 * @returns KErrNone if all went well.
 */
EXPORT_C TInt CXmlParser::ParseXmlBufferSyncL(const TDesC8 & aBuffer)
	{
	iDoSynchronously = ETrue;
	ParseXmlBufferL(aBuffer);
	iDoSynchronously = EFalse;
	return iError;
	}


/**
 * Parses a buffer containing XML. Does the parsing either synchronously or 
 * asynchronously, depending on which method was used to initiate the parsing.
 * In async parsing, client cannot get the content immediately
 * after calling this method. Client must implement the MXMLParserObserver interface
 * and get the content from parser as the interface methods tell so.
 * @param aBuffer The buffer containing the XML data.
 * @returns KErrNone if parsing started successfully.
 */
EXPORT_C TInt CXmlParser::ParseXmlBufferL(const TDesC8 & aBuffer)
	{
	Cancel();
#ifdef USE_DEBUGLOGGER
	iLogger->Write(oy::tol::KLogLevelHigh, _L("ParseXMLBufferL start"));
#endif
	iError = 0;
	iElements.Reset();
	iXmlNameSpaces.Reset();
	iCurrentElement = 0;

	iCurrentParseIndex = 0;
	iXmlString.Set(aBuffer);
	
	iXmlParser->ParseBeginL();
	TInt result = iXmlParser->EnableFeature(Xml::EReportNamespaces);
	result = iXmlParser->EnableFeature(Xml::EReportNamespacePrefixes);
	result = iXmlParser->EnableFeature(Xml::EReportNamespaceMapping);
	result = iXmlParser->EnableFeature(Xml::ESendFullContentInOneChunk);
	
	// Start async parsing.
	if (!iDoSynchronously)
		{
		ParseNextFragmentL();
#ifdef USE_DEBUGLOGGER
		iLogger->Write(oy::tol::KLogLevelHigh, _L("Started parsing first fragment."));
#endif	
		}
	else
		{
		do
			{
			ParseNextFragmentL();
#ifdef USE_DEBUGLOGGER
			iLogger->Write(oy::tol::KLogLevelHigh, _L("parsing fragments...."));
#endif	
			} while (iIsParsing);
		}
	return iError;
	}

/**
 * Parses the next fragment of the XML.
 * Parses a piece of the XML and sets the active object active.
 * When RunL is called, it calls this function again. If there's
 * nothing left to parse, the object is not set active, but
 * ParsingEndedL is called.
 */
void CXmlParser::ParseNextFragmentL()
	{
#ifdef USE_DEBUGLOGGER
	iLogger->Write(oy::tol::KLogLevelMedium, _L("In ParseNextFragmentL"));
#endif
	TInt length = Min(iBytesToParseInStep, iXmlString.Length()-iCurrentParseIndex);
	if (iCurrentParseIndex < iXmlString.Length() && length > 0)
		{
		TPtrC8 ptr(iXmlString.Mid(iCurrentParseIndex, length));
#ifdef USE_DEBUGLOGGER
		iLogger->Write(oy::tol::KLogLevelDetails, ptr);
#endif
		iXmlParser->ParseL(ptr);
		iCurrentParseIndex += length;
		iIsParsing = ETrue;
		if (!iDoSynchronously)
			{
			SetActive();			
			TRequestStatus* status = &iStatus;
			User::RequestComplete(status, KErrNone);
			}
		}
	else
		{
		ParsingEndedL();
		}
	}

/** Called when the parsing has ended. Calls Xml::CParser::ParseEndL
 * and notifies the observer.
 */
void CXmlParser::ParsingEndedL()
	{
#ifdef USE_DEBUGLOGGER
	iLogger->Write(oy::tol::KLogLevelMedium, _L("In ParsingEndedL"));
#endif
	iXmlParser->ParseEndL();
	iIsParsing = EFalse;
	iObserver.ParsingFinishedL(KErrNone);
	}

/**
 * After parsing, use this method to get the results to the client side.
 * Ownership of the result is with the client, so you must call this method
 * and destroy the results when no longer needed. When calling this method,
 * the placemarks are also removed from the parser's container to save memory.
 * @param aArray The array where to place the parsed placemarks.
 */
EXPORT_C void CXmlParser::GetElementsL(RXmlElementArray & aArray)
	{
	while (iElements.Count() > 0)
		{
		aArray.AppendL(iElements[0]);
		iElements.Remove(0);
		}
	}

/**
 * After parsing, use this method to get the results to the client side.
 * Ownership of the result is with the client, so you must call this method
 * and destroy the results when no longer needed. When calling this method,
 * the elements are also removed from the parser's container to save memory.
 * @param aDocument The XML document where to place the parsed elements.
 */
EXPORT_C void CXmlParser::GetElementsL(CXmlDocument & aDocument)
	{
	aDocument.AddElementsL(iElements);
	}


/** See Symbian XML parser doc on this method. */
void CXmlParser::OnStartDocumentL(
		const Xml::RDocumentParameters& /*aDocParam*/,
		TInt aErrorCode)
	{
	iIsParsing = ETrue;
#ifdef USE_DEBUGLOGGER
	_LIT(KMsg, "OnStartDocumentL, ErrorCode: %d");
	iLogger->Write(oy::tol::KLogLevelHigh, KMsg, aErrorCode);
#endif
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnEndDocumentL(TInt aErrorCode)
	{
	iIsParsing = EFalse;
	if (iElements.Count() > 0)
		{
		// Add the namespace definitions to the topmost/first
		// xml element. XML elemet removes the pointers from iXmlNameSpaces.
		iElements[0]->AddAttributesL(iXmlNameSpaces);
		}
#ifdef USE_DEBUGLOGGER
	_LIT(KMsg, "OnEndDocumentL, error: %d");
	iLogger->Write(oy::tol::KLogLevelHigh, KMsg, aErrorCode);
#endif
	}

/**
 * Creates and adds a namespace definition as a key value pair object into an
 * array. When parsing ends, this array contains the namespace definitions found
 * in the XML. These are then added to the topmost CXmlElement as attributes.
 * @param aUri The namespace URI, e.g. xmlns:atom="http://www.w3.org/2005/Atom".
 * @param aPrefix The namespace prefixm e.g. atom.
 */
void CXmlParser::AddToNameSpacesListL(const TDesC8 & aUri, const TDesC8 & aPrefix)
	{
	// For example, xmlns:atom="http://www.w3.org/2005/Atom"
	// xmlns and atom are here the namespace qualifier, which are together
	// used in the search. If one like this cannot be found, adds to the array
	TLinearOrder<CKeyValue> order(CKeyValue::Compare);
	
	CKeyValue * tmp = CKeyValue::NewLC(aPrefix, aUri);
	tmp->SetNameSpaceL(KXmlNs);

	if (iXmlNameSpaces.FindInOrder(tmp, order) == KErrNotFound)
		{
		// Is not in the array, so insert there.
		if (iXmlNameSpaces.InsertInOrder(tmp, order) == KErrNone)
			{
			// Namespace qualifier is now in the array and we are happy.
			CleanupStack::Pop(); // tmp
			}
		else
			{
			// Coudn't insert, destroy.
			CleanupStack::PopAndDestroy(); // tmp
			}
		}
	else // Is already in the array
		{
		CleanupStack::PopAndDestroy(); // tmp
		}
	}


/** See Symbian XML parser doc on this method. */
void CXmlParser::OnStartElementL(const Xml::RTagInfo& aElement,
		const Xml::RAttributeArray& aAttributes, TInt aErrorCode)
	{
	TPtrC8 uri = aElement.Uri().DesC();
	TPtrC8 prefix = aElement.Prefix().DesC();
	TPtrC8 localName = aElement.LocalName().DesC();
	
#ifdef USE_DEBUGLOGGER
	_LIT(KMsg, " OnStartElementL: (error: %d)");
	iLogger->Write(oy::tol::KLogLevelDetails, KMsg, aErrorCode);
	iLogger->Write(oy::tol::KLogLevelDetails, prefix);
	iLogger->Write(oy::tol::KLogLevelDetails, localName);
	iLogger->Write(oy::tol::KLogLevelDetails, uri);
#endif
	
	CXmlElement * newElement = new (ELeave) CXmlElement;
	CleanupStack::PushL(newElement);
	if (iCurrentElement)
		{
		newElement->SetParent(iCurrentElement);
		iCurrentElement->AddElementL(newElement);
		CleanupStack::Pop(); // newElement
		iCurrentElement = newElement;
		}
	else
		{
		iCurrentElement = newElement;
		CleanupStack::Pop(); // newElement
		}
	newElement->SetNameSpace(prefix);
	newElement->SetNameL(localName);
	for (TInt counter = 0; counter < aAttributes.Count(); ++counter)
		{
		const Xml::RAttribute & attr = aAttributes[counter];
		CKeyValue * keyValue = CKeyValue::NewLC(attr.Attribute().LocalName().DesC(), attr.Value().DesC());
		TPtrC8 namesp = attr.Attribute().Prefix().DesC();
		TPtrC8 value = attr.Value().DesC();
		if (namesp.Length() > 0)
			{
			keyValue->SetNameSpaceL(namesp);
			}
		newElement->AddAttributeL(keyValue);
		CleanupStack::Pop(); // keyValue
#ifdef USE_DEBUGLOGGER
		_LIT(KMsg, " Attribute %d: %S:%S=%S, (error: %d)");
		iLogger->Write(oy::tol::KLogLevelDetails, KMsg, counter, &keyValue->NameSpace(), &keyValue->Key(), &keyValue->Value(), aErrorCode);
#endif
		}
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnEndElementL(const Xml::RTagInfo& /*aElement*/, TInt aErrorCode)
	{
	if (iCurrentElement)
		{
		// If current element has no parent, it is a topmost element
		// and will be put on the elements array.
		if (!iCurrentElement->Parent())
			{
			iElements.AppendL(iCurrentElement);
			}
		iCurrentElement = iCurrentElement->Parent();
		}

#ifdef USE_DEBUGLOGGER
	_LIT(KMsg, " OnEndElementL ErrorCode: %d");
	iLogger->Write(oy::tol::KLogLevelDetails, KMsg, aErrorCode);
#endif
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnContentL(const TDesC8& aBytes, TInt aErrorCode)
	{
	if (aBytes.Length() > 0)
		{
		HBufC8 * tmp = aBytes.AllocLC();
		TPtr8 ptr(tmp->Des());
#ifdef USE_DEBUGLOGGER
		_LIT(KMsg, "OnContentL error: %d");
		iLogger->Write(oy::tol::KLogLevelDetails, KMsg, aErrorCode);
		iLogger->Write(oy::tol::KLogLevelDetails, *tmp);
#endif

		if (iCurrentElement)
			{
			if (iCurrentElement == iPreviousElement)
				{
				iCurrentElement->AddToValueL(*tmp);
				}
			else
				{
				iCurrentElement->SetValueL(*tmp);
				iPreviousElement = iCurrentElement;
				}
			}
		CleanupStack::PopAndDestroy(); // tmp
		}
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnStartPrefixMappingL(const RString& aPrefix,
		const RString& aUri, TInt /*aErrorCode*/)
	{
#ifdef USE_DEBUGLOGGER
	_LIT8(KMsg, "OnStartPrefixMapping: Prefix: %S Uri: %S");
	TBuf8<200> msg;
	msg.Format(KMsg, &aPrefix.DesC(), &aUri.DesC());
	iLogger->Write(oy::tol::KLogLevelDetails, msg);
#endif
	AddToNameSpacesListL(aUri.DesC(), aPrefix.DesC());
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnEndPrefixMappingL(const RString& aPrefix,
		TInt /*aErrorCode*/)
	{
#ifdef USE_DEBUGLOGGER
	_LIT8(KMsg, "OnEndPrefixMapping: %S");
	TBuf8<200> msg;
	msg.Format(KMsg, &aPrefix.DesC());
	iLogger->Write(oy::tol::KLogLevelDetails, msg);
#endif
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt /*aErrorCode*/)
	{
#ifdef USE_DEBUGLOGGER
	_LIT8(KMsg, "OnIgnorableWhiteSpaceL: %S");
	TBuf8<200> msg;
	msg.Format(KMsg, &aBytes);
	iLogger->Write(oy::tol::KLogLevelDetails, msg);
#endif
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnSkippedEntityL(const RString& aName, TInt /*aErrorCode*/)
	{
#ifdef USE_DEBUGLOGGER
	_LIT8(KMsg, "OnSkippedEntityL: %S");
	TBuf8<200> msg;
	msg.Format(KMsg, &aName.DesC());
	iLogger->Write(oy::tol::KLogLevelDetails, msg);
#endif
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnProcessingInstructionL(
		const TDesC8& /*aTarget*/, const TDesC8& /*aData*/, TInt /*aErrorCode*/)
	{
	}

/** See Symbian XML parser doc on this method. */
void CXmlParser::OnError(TInt aErrorCode)
	{
#ifdef USE_DEBUGLOGGER
	_LIT(KMsg, "*** OnError ErrorCode: %d");
	iLogger->Write(oy::tol::KLogLevelDetails, KMsg, aErrorCode);
#endif
	
	iError = aErrorCode;
	if ( iError != KErrNone)
		{
		Cancel();
		iIsParsing = EFalse;
		iObserver.ParsingFinishedL(aErrorCode);
		}
	}

/** See Symbian XML parser doc on this method. */
TAny* CXmlParser::GetExtendedInterface(const TInt32 /*aUid*/)
	{
	return 0;
	}


/** Cancels an outstanding request, in this case does nothing. */
void CXmlParser::DoCancel()
	{
	iIsParsing = EFalse;
#ifdef USE_DEBUGLOGGER
	iLogger->Write(oy::tol::KLogLevelDetails, _L("In CXmlParser::DoCancel."));
#endif
	}

/**
 * Called after parsing a piece of XML.
 * Notifies the observer and starts parsing another piece.
 */
void CXmlParser::RunL()
	{
#ifdef USE_DEBUGLOGGER
	iLogger->Write(oy::tol::KLogLevelDetails, _L("In CXmlParser::RunL."));
#endif
	// Notify observer of new content.
	iObserver.FragmentParsedL();
	// Parse the next fragment of XML.
	if (iIsParsing)
		{
		ParseNextFragmentL();
		}
	}

/**
 * Called when RunL leaves. In this case just
 * return the error variable. This will result to a Panic.
 * @param aError The error code
 * @returns The error code.
 */
TInt CXmlParser::RunError(TInt aError)
	{
	if (aError != KErrNone)
		{
#ifdef USE_DEBUGLOGGER
		iLogger->Write(oy::tol::KLogLevelHigh, _L("In CXmlParser::RunError: %d"), aError);
#endif		
		}
	return KErrNone;
	}

} // org
} // ajj

// EOF
