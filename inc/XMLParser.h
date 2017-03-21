#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

/*
 * $Id: XMLParser.h 1769 2009-09-01 20:41:24Z antti $
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


#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include <f32file.h> // RFs

#include <Xml\ContentHandler.h>
#include <Xml\Attribute.h>
#include <Xml\Parser.h>
#include <Xml\TagInfo.h>
#include <Xml\DocumentParameters.h>

#include "XmlElement.h"


/** Use this macro to enable debuglogging also in
  * urel builds (where _DEBUG is 0).
  * undef if you do not/cannot use the logger, or when doing a GCCE UREL build,
  * since the logger lib is DEBUGLIBRARY in the XMLParser.mmp.
  */
#undef USE_DEBUGLOGGER

#ifdef USE_DEBUGLOGGER
// Forward declaration:
namespace oy {
namespace tol {
class CDebugLogger;
}
}
#endif

namespace org
{
/**
 * Namespace org, used to include my own namespace ajj.
 */

namespace ajj
{

/**
 * Namespace ajj, used to include my own developed stuff.
 * Here I implement my XML parser.
 */


//  Class Definitions
class CXmlDocument;

/**
 * Observer class for getting parsing events.
 * When the parser calls MXmlParserObserver::FragmentParsedL,
 * the client should call CXmlParser::GetElementsL. There, 
 * the already created elements are removed from parser's 
 * array of elements. When the parser calls ParsingFinishedL,
 * the rest of the elements can be fetched, again by calling
 * CXmlParser::GetElementsL.<br />
 * Client can, however, choose to call CXmlParser::GetElementsL 
 * only when responding to ParsingFinishedL, since the parser does
 * not empty the array passed in CXmlParser::GetElementsL.
 * @author Antti Juustila
 * @version $Revision: 1769 $
 */

class MXmlParserObserver
{
public:
	MXmlParserObserver() {};
	virtual ~MXmlParserObserver() {};
	/** Called by the parser when a fragment of a XML has been parsed.
	 * Client can, if wishes, extract the already parsed full CXmlElements
	 * to the client side, or wait until the whole XML has been parsed and
	 * ParsingFinishedL is called. */
	virtual void FragmentParsedL() = 0;
	/** Called by the parser when the whole of XML has been parsed.
	 * Client can now fetch the XML elements by calling GetElementsL. */
	virtual void ParsingFinishedL(TInt aError) = 0;
};

/**
 * Parser for XML content, producing CXmlElement objects in a container.<br />
 * Usage:<br />
 * <ul>
 * <li>Define a client class which implements the MXmlParserObserver interface</li>
 * <li>Create parser using NewL, passing the interface implementor as a parameter</li>
 * <li>Call either ParseXmlFileL or ParseXmlBuffer, depending.</li>
 * <li>As the interface methods FragmentParsedL and/or ParsingFinishedL are called,
 * retrieve the already created objects by....</li>
 * <li>...calling GetElementsL.</li>
 * <li>Use the elements and finally destroy them when no longer needed.</li>
 * </ul>
 * <strong>NOTE</strong>: The client is responsible for destroying the
 * CXmlElement objects after calling GetElementsL from the container provided
 * by the client!<br />
 * <strong>NOTE</strong>: The parser is using the DebugLogger for logging,
 * and client is responsible for initializing the logger first! It is also
 * recommended to set the logging level to KLogLevelMedium since logging
 * slows down the application (if not debugging the logger itself).
 * If you do not use (or cannot use) the logger, change the define from XMLParser.h
 * <code>00047 #define USE_DEBUGLOGGER 1</code> to <code>00047 #undef USE_DEBUGLOGGER</code>
 * and remove the DebugLogger_0X..lib from the MMP file.<br />
 * <strong>NOTE</strong>: The parser does not check if the XML is valid or not,
 * or if the file is even XML or cooking recipes!
 * <strong>NOTE</strong>: The parser needs potentially a lot of heap memory, so use the
 * EPOCHEAPSIZE directive in your client's mmp file to enable growing of heap when necessary.
 * @author Antti Juustila
 * @version $Revision: 1769 $
 * 
 */

class CXmlParser : public CActive, public Xml::MContentHandler
	{
public:
	IMPORT_C static CXmlParser* NewL(MXmlParserObserver & aObserver);
	IMPORT_C ~CXmlParser();

public:
	IMPORT_C TVersion Version() const;
	IMPORT_C TInt ParseXmlBufferL(const TDesC8 & aBuffer);
	IMPORT_C TInt ParseXmlFileL(const TDesC & aFileName);
	IMPORT_C TInt ParseXmlBufferSyncL(const TDesC8 & aBuffer);
	IMPORT_C TInt ParseXmlFileSyncL(const TDesC & aFileName);
	IMPORT_C void GetElementsL(RXmlElementArray & aArray);
	IMPORT_C void GetElementsL(CXmlDocument & aDocument);
	
public:
	// From MContentHandler

	virtual void OnStartDocumentL(
			const Xml::RDocumentParameters& aDocParam,
			TInt aErrorCode);
	virtual void OnEndDocumentL(TInt aErrorCode);
	virtual void
			OnStartElementL(const Xml::RTagInfo& aElement,
					const Xml::RAttributeArray& aAttributes,
					TInt aErrorCode);
	virtual void OnEndElementL(const Xml::RTagInfo& aElement,
			TInt aErrorCode);
	virtual void OnContentL(const TDesC8& aBytes, TInt aErrorCode);
	virtual void OnStartPrefixMappingL(const RString& aPrefix,
			const RString& aUri, TInt aErrorCode);
	virtual void OnEndPrefixMappingL(const RString& aPrefix,
			TInt aErrorCode);
	virtual void OnIgnorableWhiteSpaceL(const TDesC8& aBytes,
			TInt aErrorCode);
	virtual void OnSkippedEntityL(const RString& aName,
			TInt aErrorCode);
	virtual void OnProcessingInstructionL(const TDesC8& aTarget,
			const TDesC8& aData, TInt aErrorCode);
	virtual void OnError(TInt aErrorCode);
	virtual TAny* GetExtendedInterface(const TInt32 aUid);

protected:
	virtual void ParseNextFragmentL();
	virtual void ParsingEndedL();
	
	// From CActive
	virtual void DoCancel();
	virtual void RunL();
	virtual TInt RunError(TInt aError);
	
private:
	CXmlParser(MXmlParserObserver & aObserver);
	void ConstructL();

	void AddToNameSpacesListL(const TDesC8 & aUri, const TDesC8 & aPrefix);

private:
	/** Observer to notify of parsing. */
	MXmlParserObserver & iObserver;
	/** Place to get the XML content when parsing from file. */
	HBufC8 * iFileBuffer;
	/** A pointer descriptor to the KML content, used in parsing. */
	TPtrC8 iXmlString;
	/** File server session for reading KML files. */
	RFs iFs;
	/** The Symbian XML parser. */
	Xml::CParser * iXmlParser;
	/** If something went wrong in parsing, the error code should be here. */
	TInt iError;

	/**
	 * The array of placemark objects parsed from the KML.
	 * Parser does not delete these ever, so client is
	 * responsible of calling GetPlacemarksL and destroying
	 * of the objects.*/
	RXmlElementArray iElements;
	
	/** This array contains the namespace definitions, found from 
	 * the XML while parsing it. The namespace definitions will be added to
	 * the topmost CXmlElement object when the parsing has been finished.
	 */
	RKeyValuePairs iXmlNameSpaces;
	
	/**
	 * The current XML element which is parsed. 
	 */
	CXmlElement * iCurrentElement;
	/** Used when parsing of content is done in pieces. */
	CXmlElement * iPreviousElement;
	/** Current index to the fragment of descriptor under parsing now. */
	TInt		  iCurrentParseIndex;
	/** How big fragment is parsed in one step. */
	const TInt 	  iBytesToParseInStep;
	/** Controls the parsing, enables canceling. */
	TBool		  iIsParsing;
	/** Controls whether to do the parsing synchronously. */
	TBool iDoSynchronously;
#ifdef USE_DEBUGLOGGER
	/** object used for debug logging. */
	oy::tol::CDebugLogger * iLogger;
#endif
	};

} // org
} // ajj


#endif  // __XMLPARSER_H__


