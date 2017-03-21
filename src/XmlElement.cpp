/*
 * $Id: XmlElement.cpp 1792 2009-09-03 10:05:05Z antti $
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

#include "XmlElement.h"
#include "XMLParserConstants.h"
#include "ConversionUtils.h"
#include "XmlVisitor.h"

namespace org
{
namespace ajj
{

/** Default constructor, no implementation. */
EXPORT_C CXmlElement::CXmlElement()
	{
	}


/**
 * Destructor.
 */
EXPORT_C CXmlElement::~CXmlElement()
	{
	delete iName;
	delete iValue;
	iAttributes.ResetAndDestroy();
	iChildren.ResetAndDestroy();
	}

/** Factory method for creating elements with name and value, 8bit version.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewL(const TDesC8 & aName, const TDesC8 & aValue)
	{
	CXmlElement * self = CXmlElement::NewLC(aName, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/** Factory method for creating elements with namespace, name and value, 8bit version.
 * @param aNameSpace The namespace for the element.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewL(const TDesC8 & aNameSpace, const TDesC8 & aName, const TDesC8 & aValue)
	{
	CXmlElement * self = CXmlElement::NewLC(aNameSpace, aName, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/**
 * Factory method for creating elements with name and value, 8bit version.
 * Leaves the object to the cleanup stack.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewLC(const TDesC8 & aName, const TDesC8 & aValue)
	{
	CXmlElement * self = new (ELeave) CXmlElement();
	CleanupStack::PushL(self);
	self->ConstructL(aName, aValue);
	return self;
	}

/**
 * Factory method for creating elements with name and value, 8bit version.
 * Leaves the object to the cleanup stack.
 * @param aNameSpace The namespace of the element.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewLC(const TDesC8 & aNameSpace, const TDesC8 & aName, const TDesC8 & aValue)
	{
	CXmlElement * self = new (ELeave) CXmlElement();
	CleanupStack::PushL(self);
	self->ConstructL(aNameSpace, aName, aValue);
	return self;
	}

/**
 * Factory method for creating elements with name and value.
 * @param aNameSpace The namespace for the element.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewL(const TDesC & aNameSpace, const TDesC & aName, const TDesC & aValue)
	{
	CXmlElement * self = CXmlElement::NewLC(aNameSpace, aName, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/**
 * Factory method for creating elements with name and value.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewL(const TDesC & aName, const TDesC & aValue)
	{
	CXmlElement * self = CXmlElement::NewLC(aName, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/**
 * Factory method for creating elements with name and value.
 * Leaves the object to the cleanup stack.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewLC(const TDesC & aName, const TDesC & aValue)
	{
	CXmlElement * self = new (ELeave) CXmlElement();
	CleanupStack::PushL(self);
	self->ConstructL(aName, aValue);
	return self;
	}

/**
 * Factory method for creating elements with name and value.
 * Leaves the object to the cleanup stack.
 * @param aNameSpace The namespace for the element.
 * @param aName The name for the element.
 * @param aValue The value for the element.
 * @returns A new XML element object.
 */
EXPORT_C CXmlElement * CXmlElement::NewLC(const TDesC & aNameSpace, const TDesC & aName, const TDesC & aValue)
	{
	CXmlElement * self = new (ELeave) CXmlElement();
	CleanupStack::PushL(self);
	self->ConstructL(aNameSpace, aName, aValue);
	return self;
	}


/**
 * Second phase constructor. Leaves if cannot allocate memory
 * for the value member variable.
 * @param aName The name for the object.
 * @param aValue The value of the element.
 */
void CXmlElement::ConstructL(const TDesC & aName, const TDesC & aValue)
	{
	iName = aName.AllocL();
	iValue = aValue.AllocL();
	}

/**
 * Second phase constructor. Leaves if cannot allocate memory
 * for the value member variable.
 * @param aNameSpace The namespace for the object.
 * @param aName The name for the object.
 * @param aValue The value of the element.
 */
void CXmlElement::ConstructL(const TDesC & aNameSpace, const TDesC & aName, const TDesC & aValue)
	{
	SetNameSpace(aNameSpace);
	iName = aName.AllocL();
	iValue = aValue.AllocL();
	}


/**
 * Second phase constructor, 8bit version. Leaves if cannot allocate memory
 * for the value member variable.
 * @param aName The name for the object.
 * @param aValue The value of the element.
 */
void CXmlElement::ConstructL(const TDesC8 & aName, const TDesC8 & aValue)
	{
	SetNameL(aName);
	SetValueL(aValue);
	}

/**
 * Second phase constructor, 8bit version. Leaves if cannot allocate memory
 * for the value member variable.
 * @param aNameSpace The namespace for the object.
 * @param aName The name for the object.
 * @param aValue The value of the element.
 */
void CXmlElement::ConstructL(const TDesC8 & aNameSpace, const TDesC8 & aName, const TDesC8 & aValue)
	{
	SetNameSpace(aNameSpace);
	SetNameL(aName);
	SetValueL(aValue);
	}

/**
 * Query the namespace of the XML element
 * @returns The value for the namespace.
 */
EXPORT_C const TDesC & CXmlElement::NameSpace() const
	{
	return iNameSpace;
	}

/**
 * Query the name of the element.
 * @returns The name of the element.
 */
EXPORT_C const TDesC & CXmlElement::Name() const
	{
	if (iName)
		{
		return *iName;
		}
	return KNullDesC;
	}

/**
 * Query the value of the element.
 * @returns The value of the element.
 */
EXPORT_C const TDesC & CXmlElement::Value() const
	{
	if (iValue)
		{
		return *iValue;
		}
	return KNullDesC;
	}

/**
 * Sets the XML namespace for the element.
 * @param aNameSpace The namespace.
 */
EXPORT_C void CXmlElement::SetNameSpace(const TDesC & aNameSpace)
	{
	iNameSpace.Des().Copy(aNameSpace.Left(KMaxXmlNameSpaceLength));
	}

/**
 * Sets the XML namespace for the element, 8bit version.
 * @param aNameSpace The namespace.
 */
EXPORT_C void CXmlElement::SetNameSpace(const TDesC8 & aNameSpace)
	{
	iNameSpace.Des().Copy(aNameSpace.Left(KMaxXmlNameSpaceLength));
	}

/**
 * Sets the XML name for the element.
 * @param aName The element name.
 */
EXPORT_C void CXmlElement::SetNameL(const TDesC & aName)
	{
	delete iName;
	iName = 0;
	if (aName.Length() > 0)
		{
		iName = aName.AllocL();
		}
	}

/**
 * Sets the XML name for the element, 8bit version.
 * @param aName The element name.
 */
EXPORT_C void CXmlElement::SetNameL(const TDesC8 & aName)
	{
	delete iName;
	iName = 0;
	if (aName.Length() > 0)
		{
		iName = HBufC::NewL(aName.Length());
		TPtr ptr(iName->Des());
		ptr.Copy(aName);
		}
	}

/**
 * Sets the flag on that tells that the value in the object
 * is CDATA. Used in exporting the object into XML to surround
 * the data with the <!CDATA[ .. elements.
 * @param aIsCData ETrue, if data is CDATA.
 */
EXPORT_C void CXmlElement::SetValueIsCData(TBool aIsCData)
	{
	iValueIsCData = aIsCData;
	}

/**
 * Query if the value in the object is CDATA. Used in
 * exporting the object into XML to surround
 * the data with the <!CDATA[ .. elements.
 * @returns ETrue, if data is CDATA.
 */
EXPORT_C TBool CXmlElement::ValueIsCData() const
	{
	return iValueIsCData;
	}

/**
 * Sets the XML value for the element.
 * Leaves if cannot allocate the value member variable.
 * @param aValue The element value.
 */
EXPORT_C void CXmlElement::SetValueL(const TDesC & aValue)
	{
	delete iValue;
	iValue = 0;
	if (aValue.Length() > 0)
		{
		iValue = aValue.AllocL();
		Trim();
		}
	}

/**
 * Sets the XML value for the element, 8bit version.
 * Leaves if cannot allocate the value member variable.
 * @param aValue The element value.
 */
EXPORT_C void CXmlElement::SetValueL(const TDesC8 & aValue)
	{
	delete iValue;
	iValue = 0;
	if (aValue.Length() > 0)
		{
		iValue = HBufC::NewL(aValue.Length());
		TPtr ptr(iValue->Des());
		ConversionUtils::AppendToUnicodeBufferDecodedL(aValue, ptr);
		Trim();
		}
	}

/**
 * Adds to the XML value for the element.
 * Leaves if cannot reallocate the value member variable.
 * @param aValue The element value addition.
 */
EXPORT_C void CXmlElement::AddToValueL(const TDesC & aValue)
	{
	HBufC8 * tmp = HBufC8::NewLC(aValue.Length());
	TPtr8 ptr(tmp->Des());
	ptr.Copy(aValue);
	AddToValueL(*tmp);
	CleanupStack::PopAndDestroy();
	}

/**
 * Adds to the XML value for the element, 8bit version.
 * Leaves if cannot reallocate the value member variable.
 * @param aValue The element value addition.
 */
EXPORT_C void CXmlElement::AddToValueL(const TDesC8 & aValue)
	{
	TInt newSize = 0;
	if (iValue)
		{
		newSize = iValue->Length();
		}
	newSize += aValue.Length(); 
	iValue = iValue->ReAllocL(newSize);

	TPtr ptr(iValue->Des());
	ConversionUtils::AppendToUnicodeBufferDecodedL(aValue, ptr);

	Trim();
	}

/**
 * Trims extra whitespace from the data and removes the CDATA
 * elements from the content. Reallocates the variable to
 * release extra memory no longer needed. Also checks if data
 * contains markup data and if yes, sets this data to be CDATA
 * using SetValueIsCData(). You can check if the value is CDATA
 * by calling ValueIsCData(). When exporting to UTF-8, CXmlElement
 * pads the data with <!CDATA[ and ]]> at the end.
 */
void CXmlElement::Trim()
	{
	TPtr ptr = iValue->Des();
	
	TInt offset = ptr.Find(KCDataStart);
	if (offset >= KErrNone)
		{
		ptr.Delete(offset, KCDataStart().Length());
		SetValueIsCData(ETrue);
		}
	offset = ptr.Find(KCDataEnd);
	if (offset >= KErrNone)
		{
		ptr.Delete(offset, KCDataEnd().Length());
		}
	iValue = iValue->ReAlloc(iValue->Length());
	}

/** Queries how many child elements the XML element has.
 * @returns The count of child objects
 */
EXPORT_C TInt CXmlElement::ChildCount() const
	{
	return iChildren.Count();
	}

/**
 * Gives the nth child object of this element.
 * Panics if index is out of bounds.
 * @param aChild Index to the child object.
 * @returns The child XML element.
 */
EXPORT_C CXmlElement * CXmlElement::Child(TInt aChild)
	{
	return iChildren[aChild];
	}

/**
 * Gives the nth child object of this element, const version.
 * Panics if index is out of bounds.
 * @param aChild Index to the child object.
 * @returns The child XML element.
 */
EXPORT_C const CXmlElement * CXmlElement::Child(TInt aChild) const
	{
	return iChildren[aChild];
	}

/**
 * Adds a new child element to this XML element.
 * Leaves if the array cannot be extended to hold the new element.
 * @param aElement The child element.
 */
EXPORT_C void CXmlElement::AddElementL(const CXmlElement * aElement)
	{
	iChildren.AppendL(aElement);
	}

/**
 * Retrieves an XML element by name, const version.
 * If this element has the name, returns this, otherwise searches
 * for the child elements with the name.
 * @param aNameSpace The namespace of the element to find.
 * @param aName The element's name to find.
 * @returns XML element, 0 if not found.
 */
EXPORT_C const CXmlElement * CXmlElement::Element(const TDesC & aNameSpace, const TDesC & aName) const
	{
	if (aNameSpace == iNameSpace && aName == *iName)
		{
		return this;
		}
	else 
		{
		CXmlElement * tmp = 0;
		for (TInt counter = 0; counter < iChildren.Count(); counter++)
			{
			tmp = iChildren[counter]->Element(aNameSpace, aName);
			if (tmp != 0)
				return tmp;
			}
		}
	return 0;
	}

/**
 * Retrieves an XML element by name, const version.
 * If this element has the name, returns this, otherwise searches
 * for the child elements with the name.
 * @param aNameSpace The namespace of the element to find.
 * @param aName The element's name to find.
 * @returns XML element, 0 if not found.
 */
EXPORT_C CXmlElement * CXmlElement::Element(const TDesC & aNameSpace, const TDesC & aName)
	{
	if (aNameSpace == iNameSpace && aName == *iName)
		{
		return this;
		}
	else 
		{
		CXmlElement * tmp = 0;
		for (TInt counter = 0; counter < iChildren.Count(); counter++)
			{
			tmp = iChildren[counter]->Element(aNameSpace, aName);
			if (tmp != 0)
				return tmp;
			}
		}
	return 0;
	}

/** 
 * Queries the count of attributes in the element.
 * @returns The count of attributes.
 */
EXPORT_C TInt CXmlElement::AttributeCount() const
	{
	return iAttributes.Count();
	}

/**
 * Get the specific attribute of the element.
 * Panics if index is out of bounds.
 * @param aIndex The attribute index.
 * @returns The attribute.
 */
EXPORT_C CKeyValue * CXmlElement::Attribute(TInt aIndex)
	{
	return iAttributes[aIndex];
	}

/**
 * Get the specific attribute of the element, const version.
 * Panics if index is out of bounds.
 * @param aIndex The attribute index.
 * @returns The attribute.
 */
EXPORT_C const CKeyValue * CXmlElement::Attribute(TInt aIndex) const
	{
	return iAttributes[aIndex];
	}

/**
 * Get the specific attribute of the element by name.
 * Returns 0 if no attribute can be found from this element
 * or from the children of this element.
 * @param aNameSpace The namespace of the attribute's key.
 * @param aKey The attribute's key.
 * @returns The attribute.
 */
EXPORT_C CKeyValue * CXmlElement::Attribute(const TDesC & aNameSpace, const TDesC & aKey)
	{
	TInt counter;
	for (counter = 0; counter < iAttributes.Count(); counter++)
		{
		if (iAttributes[counter]->Key() == aKey)
			return iAttributes[counter];
		}
	// Attribute wasn't here, let'c check if some child has it.
	for (counter = 0; counter < iChildren.Count(); counter++)
		{
		CKeyValue * tmp = iChildren[counter]->Attribute(aNameSpace, aKey);
		if (tmp != 0)
			return tmp;
		}
	return 0;
	}

/**
 * Get the specific attribute of the element by name, const version.
 * Returns 0 if no attribute can be found from this element
 * or from the children of this element.
 * @param aNameSpace The namespace of the attribute's key.
 * @param aKey The attribute's key.
 * @returns The attribute.
 */
EXPORT_C const CKeyValue * CXmlElement::Attribute(const TDesC & aNameSpace, const TDesC & aKey) const
	{
	TInt counter;
	for (counter = 0; counter < iAttributes.Count(); counter++)
		{
		if (iAttributes[counter]->NameSpace() == aNameSpace && iAttributes[counter]->Key() == aKey)
			return iAttributes[counter];
		}
	// Attribute wasn't here, let'c check if some child has it.
	for (counter = 0; counter < iChildren.Count(); counter++)
		{
		const CKeyValue * tmp = iChildren[counter]->Attribute(aNameSpace, aKey);
		if (tmp != 0)
			return tmp;
		}
	return 0;
	}

/**
 * Retrieves the value of the attribute from this element, or from
 * the child elements' attributes if one is not found in this element.
 * @param aNameSpace The namespace of the attribute's key.
 * @param aKey The key to be searched.
 * @returns The Value of the attribute, KNullDesC if not found.
 */
EXPORT_C const TDesC & CXmlElement::AttributeKeyValue(const TDesC & aNameSpace, const TDesC & aKey) const
	{
	TInt counter;
	for (counter = 0; counter < iAttributes.Count(); counter++)
		{
		if (iAttributes[counter]->NameSpace() == aNameSpace && iAttributes[counter]->Key() == aKey)
			return iAttributes[counter]->Value();
		}
	// Attribute wasn't here, let'c check if some child has it.
	for (counter = 0; counter < iChildren.Count(); counter++)
		{
		const CKeyValue * tmp = iChildren[counter]->Attribute(aNameSpace, aKey);
		if (tmp != 0)
			return tmp->Value();
		}
	return KNullDesC;
	}

/**
 * Adds a new attribute to this element. Ownership of the
 * key value is transferred to xml element object.
 * @param aKeyValue The new attribute.
 */
EXPORT_C void CXmlElement::AddAttributeL(CKeyValue * aKeyValue)
	{
	iAttributes.AppendL(aKeyValue);
	}

/**
 * Adds the attributes from the array to this object.
 * Ownership is transferred to this xml element object.
 * The parameter array is therefore resetted.
 * @param aKeyValues The array containing the attributes to add.
 */
EXPORT_C void CXmlElement::AddAttributesL(RKeyValuePairs & aKeyValues)
	{
	TInt counter;
	TInt count = aKeyValues.Count();
	for (counter = 0; counter < count; counter++)
		{
		iAttributes.AppendL(aKeyValues[counter]);
		}
	aKeyValues.Reset();
	}

/**
 * Get the parent element of this element.
 * @returns The parent, 0 if has no parent.
 */
EXPORT_C CXmlElement * CXmlElement::Parent()
	{
	return iParent;
	}

/**
 * Get the parent element of this element, const version.
 * @returns The parent, 0 if has no parent.
 */
EXPORT_C const CXmlElement * CXmlElement::Parent() const
	{
	return iParent;
	}

/**
 * Sets the parent of this element.
 * @param aParent The parent.
 */
EXPORT_C void CXmlElement::SetParent(CXmlElement * aParent)
	{
	iParent = aParent;
	}

/** Calculates the approximate size for a descriptor that
 * can hold the contents of a CXmlElement object and it's
 * attributes as well as child elements exported as a text.
 * @see CKeyValue::GetAsText
 * @see CXmlElement::GetAsText
 * @returns The length required to hold data of this object as text.
 */
EXPORT_C TInt CXmlElement::ApproximateTextLength() const
	{
	TInt counter;
	TInt length = (iNameSpace.Length() + 1) * 2;
	length += iName->Length() * 2;
	if (iValue)
		{
		length += iValue->Length();
		}
	if (ValueIsCData())
		{
		length += 12;
		}
	counter = iAttributes.Count();
	length += counter;
	for (--counter; counter >= 0; --counter)
		{
		length += iAttributes[counter]->ApproximateTextLength();
		}
	counter = iChildren.Count();
	length += counter;
	for (--counter; counter >= 0; --counter)
		{
		length += iChildren[counter]->ApproximateTextLength();
		}
	length += 10; // <, >, /
	return length;
	}

/**
 * Exports the contents of the CXmlElement object, it's attributes
 * and child objects as text to a 8 bit descriptor. Used when saving
 * to a file or sending the data as XML over the network. There must
 * be enough room in the buffer to hold the data, else panics.
 * @param aBuffer The buffer to hold the data. 
 */
EXPORT_C void CXmlElement::GetAsTextL(TDes8 & aBuffer)
	{
	TInt counter;
	TInt count;
	TBool done = EFalse;
	
	aBuffer.Append(KCharLessThan);  	// <
	if (iNameSpace.Length() > 0)
		{
		aBuffer.Append(iNameSpace);
		aBuffer.Append(KCharColon);		// <atom:
		}
	aBuffer.Append(*iName);
	count = iAttributes.Count();
	if (count > 0)
		{
		aBuffer.Append(KCharSpace);    // <atom:element_
		for (counter = 0; counter < count; ++counter)
			{
			iAttributes[counter]->GetAsTextL(aBuffer);  		// <atom:element key="value"
			if (counter < count-1) // last to go after this.
				{
				aBuffer.Append(KCharSpace);					// <atom:element key="value"_
				}
			}
		}
	count = iChildren.Count();
	if (count == 0 && !iValue)	// no children, no value
		{
		aBuffer.Append(KCharSpace); 		// <atom:element key="value"_
		aBuffer.Append(KCharSlash); 		// <atom:element key="value" /
		aBuffer.Append(KCharGreaterThan);	// <atom:element key="value" />
		done = ETrue; //return;  // We are done here.
		}
	
	if (!done)
		{
		aBuffer.Append(KCharGreaterThan); 		// <atom:element key="value">
		if (iValue)  // ?? can there be both children and value ??
			{
			// <atom:element key="value">This is the value here
			if (ValueIsCData())
				{
				aBuffer.Append(KCDataStart8);
				ConversionUtils::AppendToUtf8BufferL(*iValue, aBuffer);
				aBuffer.Append(KCDataEnd8);
				}
			else
				{
				ConversionUtils::AppendToUtf8BufferEncodedL(*iValue, aBuffer);
				}
			}
		if (count > 0)			// has childen
			{
			for (counter = 0; counter < count; ++counter)
				{
				iChildren[counter]->GetAsTextL(aBuffer);   // <atom:element key="value"><subelement>value</subelement>
				}
			}
		aBuffer.Append(KCharLessThan);  	// <atom:element key="value">This is value<
		aBuffer.Append(KCharSlash);  		// <atom:element key="value">This is value</
		if (iNameSpace.Length() > 0)
			{
			// <atom:element key="value">This is value</atom
			aBuffer.Append(iNameSpace);
			aBuffer.Append(KCharColon);		// <atom:element key="value">This is value</atom:
			}
		// <atom:element key="value">This is value</atom:element
		aBuffer.Append(*iName);
		aBuffer.Append(KCharGreaterThan);	// <atom:element key="value">This is value</atom:element>
		}
	}

/**
 * Accepts a visitor to visit this object.
 * @param aVisitor The visitor.
 */
EXPORT_C void CXmlElement::AcceptL(MXmlVisitor & aVisitor)
	{
	aVisitor.VisitL(*this);
	if (!aVisitor.VisitorNavigates())
		{
		TInt counter = 0;
		for (; counter < iAttributes.Count(); counter++)
			{
			iAttributes[counter]->AcceptL(aVisitor);
			}
		counter = 0;
		for (; counter < iChildren.Count(); counter++)
			{
			iChildren[counter]->AcceptL(aVisitor);
			}
		}
	}

} // ajj
} // org
