#ifndef __XMLELEMENT_H_
#define __XMLELEMENT_H_

/*
 * $Id: XmlElement.h 1590 2009-08-19 19:34:21Z antti $
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
#include "KeyValue.h"

namespace org
{
namespace ajj
{

class MXmlVisitor;

/** Maximum length of the namespace name. */
const TInt KMaxXmlNameSpaceLength = 20;

class CXmlElement;
/** A typedef for an array of pointers to CXmlElement objects. */
typedef RPointerArray<CXmlElement> RXmlElementArray;

/**
 * An interface for containers containing CXmlElement objects. For 
 * an implementation, see CXmlElement class.
 */
class MXmlElementContainer 
{
public:
// XML Element Child management
	/** Child count of the container.
	 * @returns The count of children. */
	virtual TInt ChildCount() const = 0;
	/** Gets a child from an index.
	 * @param aChild The index of the child.
	 * @returns The XML element in the index. */
	virtual CXmlElement * Child(TInt aChild) = 0;
	/** Gets a child from an index, const version.
	 * @param aChild The index of the child.
	 * @returns The XML element in the index. */
	virtual const CXmlElement * Child(TInt aChild) const = 0;
	/** Adds a child element. 
	 * @param aElement The child to add. */
	virtual void AddElementL(const CXmlElement * aElement) = 0;
	/** Retrieves a child element by key name, const version.
	 * @param aNameSpace The namespace of the element to retrieve.
	 * @param aKey The key name for the element to retrieve.
	 * @returns The XML element with the key value. */
	virtual const CXmlElement * Element(const TDesC & aNameSpace, const TDesC & aKey) const = 0;
	/** Retrieves a child element by key name.
	 * @param aNameSpace The namespace of the element to retrieve.
	 * @param aKey The key name for the element to retrieve.
	 * @returns The XML element with the key value. */
	virtual CXmlElement * Element(const TDesC & aNameSpace, const TDesC & aKey) = 0;
};

/**
 * Defines XML elements. CXmlElement has attributes (CKeyValue pairs) as well
 * as child CXmlElement objects. Each element has a namespace, a name and a value.
 * @todo Change the namespace to a dynamic buffer too, as name and value are.
 * @author Antti Juustila
 * @version $Revision: 1590 $
 */
class CXmlElement : public CBase, public MXmlElementContainer
{
public:
	IMPORT_C CXmlElement();
	
	IMPORT_C virtual ~CXmlElement();

	IMPORT_C static CXmlElement * NewL(const TDesC8 & aName, const TDesC8 & aValue);
	IMPORT_C static CXmlElement * NewL(const TDesC8 & aNameSpace, const TDesC8 & aName, const TDesC8 & aValue);
	IMPORT_C static CXmlElement * NewLC(const TDesC8 & aNameSpace, const TDesC8 & aName, const TDesC8 & aValue);
	IMPORT_C static CXmlElement * NewLC(const TDesC8 & aName, const TDesC8 & aValue);
	
	IMPORT_C static CXmlElement * NewL(const TDesC & aName, const TDesC & aValue);
	IMPORT_C static CXmlElement * NewL(const TDesC & aNameSpace, const TDesC & aName, const TDesC & aValue);
	IMPORT_C static CXmlElement * NewLC(const TDesC & aName, const TDesC & aValue);
	IMPORT_C static CXmlElement * NewLC(const TDesC & aNameSpace, const TDesC & aName, const TDesC & aValue);
	
	IMPORT_C const TDesC & Name() const;
	IMPORT_C const TDesC & NameSpace() const;
	IMPORT_C const TDesC & Value() const;
	
	IMPORT_C void SetNameL(const TDesC & aName);
	IMPORT_C void SetNameL(const TDesC8 & aName);
	IMPORT_C void SetValueL(const TDesC & aValue);
	IMPORT_C void SetValueL(const TDesC8 & aValue);
	IMPORT_C void AddToValueL(const TDesC & aValue);
	IMPORT_C void AddToValueL(const TDesC8 & aValue);
	IMPORT_C void SetNameSpace(const TDesC & aNameSpace);
	IMPORT_C void SetNameSpace(const TDesC8 & aNameSpace);

	IMPORT_C void SetValueIsCData(TBool aIsCData);
	IMPORT_C TBool ValueIsCData() const;
	
	// XML Element Child management
	IMPORT_C TInt ChildCount() const;
	IMPORT_C CXmlElement * Child(TInt aChild);
	IMPORT_C const CXmlElement * Child(TInt aChild) const;
	IMPORT_C void AddElementL(const CXmlElement * aElement);
	IMPORT_C const CXmlElement * Element(const TDesC & aNameSpace, const TDesC & aKey) const;
	IMPORT_C CXmlElement * Element(const TDesC & aNameSpace, const TDesC & aKey);

	// Attribute management
	IMPORT_C TInt AttributeCount() const;
	IMPORT_C CKeyValue * Attribute(TInt aIndex);
	IMPORT_C const CKeyValue * Attribute(TInt aIndex) const;
	IMPORT_C CKeyValue * Attribute(const TDesC & aNameSpace, const TDesC & aKey);
	IMPORT_C const CKeyValue * Attribute(const TDesC & aNameSpace, const TDesC & aKey) const;
	IMPORT_C const TDesC & AttributeKeyValue(const TDesC & aNameSpace, const TDesC & aKey) const;
	IMPORT_C void AddAttributeL(CKeyValue * aKeyValue);
	IMPORT_C void AddAttributesL(RKeyValuePairs & aKeyValues);
	
	IMPORT_C CXmlElement * Parent();
	IMPORT_C const CXmlElement * Parent() const;
	IMPORT_C void SetParent(CXmlElement * aParent);
	
	IMPORT_C void GetAsTextL(TDes8 & aBuffer);
	IMPORT_C TInt ApproximateTextLength() const;
	
	IMPORT_C virtual void AcceptL(MXmlVisitor & aVisitor);
	
private:
	void ConstructL(const TDesC & aName, const TDesC & aValue);
	void ConstructL(const TDesC8 & aName, const TDesC8 & aValue);
	void ConstructL(const TDesC & aNameSpace, const TDesC & aName, const TDesC & aValue);
	void ConstructL(const TDesC8 & aNameSpace, const TDesC8 & aName, const TDesC8 & aValue);
	
	void Trim();
	
private:
	// TODO make iNameSpace HBufC
	/** Namespace of the element. */
	TBufC<KMaxXmlNameSpaceLength>   iNameSpace;
	/** Name of the element. */
	HBufC							*iName;
	/** The value for the element. */
	HBufC							*iValue;
	/** ETrue, if the value is CDATA. */
	TBool							iValueIsCData;
	/** Contains the attributes of the XML element in key-value -pairs. */
	RKeyValuePairs				iAttributes;
	/** Contains the child elements of this XML element. */
	RXmlElementArray			iChildren;
	
	/** Points to the parent element of this object. Used in parsing. */
	CXmlElement					* iParent;
};


} // ajj
} // org

#endif /*__XMLELEMENT_H_*/
