/*
 * $Id: KeyValue.cpp 1593 2009-08-19 19:35:59Z antti $
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

#include <utf.h>


#include "KeyValue.h"
#include "XMLParserConstants.h"
#include "ConversionUtils.h"
#include "XmlVisitor.h"

namespace org
{
namespace ajj
{

/** Default constructor for the class. */
EXPORT_C CKeyValue::CKeyValue()
	{
	}

/** Destructor for the class. */
EXPORT_C CKeyValue::~CKeyValue()
	{
	delete iNameSpace;
	delete iKey;
	delete iValue;
	}


/** Constructs a keyvalue object with key and value.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewL(const TDesC & aKey, const TDesC & aValue)
	{
	CKeyValue * self = CKeyValue::NewLC(aKey, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/** Constructs a keyvalue object with key and value.
 * @param aNameSpace The namespace for the object.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewL(const TDesC & aNameSpace, const TDesC & aKey, const TDesC & aValue)
	{
	CKeyValue * self = CKeyValue::NewLC(aNameSpace, aKey, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/** Constructs a keyvalue object with 8 bit key and value.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewL(const TDesC8 & aKey, const TDesC8 & aValue)
	{
	CKeyValue * self = CKeyValue::NewLC(aKey, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/** Constructs a keyvalue object with 8 bit key and value.
 * @param aNameSpace The key for the object.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewL(const TDesC8 & aNameSpace, const TDesC8 & aKey, const TDesC8 & aValue)
	{
	CKeyValue * self = CKeyValue::NewLC(aNameSpace, aKey, aValue);
	CleanupStack::Pop(); // self
	return self;
	}

/** Constructs a keyvalue object with key and value, leaves on Cleanupstack.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewLC(const TDesC & aKey, const TDesC & aValue)
	{
	CKeyValue * self = new (ELeave) CKeyValue();
	CleanupStack::PushL(self);
	self->ConstructL(aKey, aValue);
	return self;
	}

/** Constructs a keyvalue object with key and value, leaves on Cleanupstack.
 * @param aNameSpace The namespace for the object.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewLC(const TDesC & aNameSpace, const TDesC & aKey, const TDesC & aValue)
	{
	CKeyValue * self = new (ELeave) CKeyValue();
	CleanupStack::PushL(self);
	self->ConstructL(aNameSpace, aKey, aValue);
	return self;
	}

/** Constructs a keyvalue object with 8 bit key and value, leaves on Cleanupstack.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewLC(const TDesC8 & aKey, const TDesC8 & aValue)
	{
	CKeyValue * self = new (ELeave) CKeyValue();
	CleanupStack::PushL(self);
	self->ConstructL(aKey, aValue);
	return self;
	}

/** Constructs a keyvalue object with 8 bit key and value, leaves on Cleanupstack.
 * @param aNameSpace The namespace for the object.
 * @param aKey The key for the object.
 * @param aValue The value. 
 * @returns The keyvalue object.
 */
EXPORT_C CKeyValue * CKeyValue::NewLC(const TDesC8 & aNameSpace, const TDesC8 & aKey, const TDesC8 & aValue)
	{
	CKeyValue * self = new (ELeave) CKeyValue();
	CleanupStack::PushL(self);
	self->ConstructL(aNameSpace, aKey, aValue);
	return self;
	}

/** 2nd phase constructor.
 * @param aKey The key for the object.
 * @param aValue The value.
 */
void CKeyValue::ConstructL(const TDesC & aKey, const TDesC & aValue)
	{
	if (aKey.Length() > 0)
		{
		iKey = aKey.AllocL();
		}
	if (aValue.Length() > 0)
		{
		iValue = aValue.AllocL();
		}
	}

/** 2nd phase constructor.
 * @param aNameSpace The namespace for the object.
 * @param aKey The key for the object.
 * @param aValue The value.
 */
void CKeyValue::ConstructL(const TDesC & aNameSpace, const TDesC & aKey, const TDesC & aValue)
	{
	if (aNameSpace.Length() > 0)
		{
		iNameSpace = aNameSpace.AllocL();
		}
	if (aKey.Length() > 0)
		{
		iKey = aKey.AllocL();
		}
	if (aValue.Length() > 0)
		{
		iValue = aValue.AllocL();
		}
	}

/** 2nd phase constructor.
 * @param aKey The 6 bit key.
 * @param aValue The 8 bit value.
 */
void CKeyValue::ConstructL(const TDesC8 & aKey, const TDesC8 & aValue)
	{
	SetKeyL(aKey);
	SetValueL(aValue);
	}

/** 2nd phase constructor.
 * @param aNameSpace The 6 bit namespace.
 * @param aKey The 6 bit key.
 * @param aValue The 8 bit value.
 */
void CKeyValue::ConstructL(const TDesC8 & aNameSpace, const TDesC8 & aKey, const TDesC8 & aValue)
	{
	SetNameSpaceL(aNameSpace);
	SetKeyL(aKey);
	SetValueL(aValue);
	}

/** Query the namespace of the object.
 * @returns The namespace value.
 */
EXPORT_C const TDesC & CKeyValue::NameSpace() const
	{
	if (iNameSpace)
		{
		return *iNameSpace;
		}
	return KNullDesC;
	}

/** Query the key of the object.
 * @returns The key's value.
 */
EXPORT_C const TDesC & CKeyValue::Key() const
	{
	if (iKey)
		{
		return *iKey;
		}
	return KNullDesC;
	}

/** Query the key of the object.
 * @returns The key's value, KNullDesC if no value.
 */
EXPORT_C const TDesC & CKeyValue::Value() const
	{
	if (iValue)
		{
		return *iValue;
		}
	return KNullDesC;
	}

/**
 * Sets the value of the namespace member variable. If the new
 * name length is zero, current namepace is emptied and new
 * one is null.
 * @param aNameSpace New name of the namespace.
 */
EXPORT_C void CKeyValue::SetNameSpaceL(const TDesC & aNameSpace)
	{
	delete iNameSpace;
	iNameSpace = 0;
	if (aNameSpace.Length() > 0)
		{
		iNameSpace = aNameSpace.AllocL();
		}
	}

/** Sets the key for the object.
 * @param aKey The new key.
 */
EXPORT_C void CKeyValue::SetKeyL(const TDesC & aKey)
	{
	delete iKey;
	iKey = 0;
	if (aKey.Length() > 0)
		{
		iKey = aKey.AllocL();
		}
	}

/** Sets the value for the object.
 * @param aValue The new value.
 */
EXPORT_C void CKeyValue::SetValueL(const TDesC & aValue)
	{
	delete iValue;
	iValue = 0;
	if (aValue.Length() > 0)
		{
		iValue = aValue.AllocL();
		}
	}

/**
 * Sets the value of the namespace member variable, 8 bit version. If the new
 * name length is zero, current namepace is emptied and new
 * one is null.
 * @param aNameSpace New name of the namespace.
 */
EXPORT_C void CKeyValue::SetNameSpaceL(const TDesC8 & aNameSpace)
	{
	delete iNameSpace;
	iNameSpace = 0;
	if (aNameSpace.Length() > 0)
		{
		iNameSpace = HBufC::NewL(aNameSpace.Length());
		TPtr ptr(iNameSpace->Des());
		ptr.Copy(aNameSpace);
		}
	}

/** Sets the key for the object.
 * @param aKey The new key as 8 bit descriptor.
 */
EXPORT_C void CKeyValue::SetKeyL(const TDesC8 & aKey)
	{
	delete iKey;
	iKey = 0;
	if (aKey.Length() > 0)
		{
		iKey = HBufC::NewL(aKey.Length());
		TPtr ptr(iKey->Des());
		ptr.Copy(aKey);
		}
	}

/** Sets the value for the object.
 * @param aValue The new value as 8 bit descriptor.
 */
EXPORT_C void CKeyValue::SetValueL(const TDesC8 & aValue)
	{
	delete iValue;
	iValue = 0;
	if (aValue.Length() > 0)
		{
		iValue = HBufC::NewL(aValue.Length());
		TPtr ptr(iValue->Des());
		ptr.Copy(aValue);
		}
	}

/** Calculates the approximate size for a descriptor that
 * can hold the contents of a CKeyValue object exported
 * as a text.
 * @see CKeyValue::GetAsText
 * @returns The length required to hold data of this object as text.
 */
TInt CKeyValue::ApproximateTextLength() const
	{
	TInt len = 0;
	if (iNameSpace)
		{
		len += iNameSpace->Length()+1;
		}
	if (iKey)
		{
		len += iKey->Length();
		}
	if (iValue)
		{
		len += iValue->Length() + iValue->Length()/10; // Reserve room for escaped chars.
		}
	return len + 4;
	}

/**
 * Exports the contents of the CKeyValue object as text
 * to a 8 bit descriptor. Used when saving to a file or
 * sending the data as XML over the network. There must be enough
 * room in the buffer to hold the data, else panics.
 * @param aBuffer The buffer to hold the data. 
 */
EXPORT_C void CKeyValue::GetAsTextL(TDes8 & aBuffer)
	{
	if (iNameSpace)
		{
		aBuffer.Append(*iNameSpace);
		}
	if (iKey)
		{
		if (iNameSpace)
			{
			aBuffer.Append(KCharColon);
			}
		aBuffer.Append(*iKey);
		}
	aBuffer.Append(KCharEquals);
	aBuffer.Append(KCharQuote);
	if ((iNameSpace || iKey) && iValue)
		{
		ConversionUtils::AppendToUtf8BufferEncodedL(*iValue, aBuffer);
		}
	aBuffer.Append(KCharQuote);
	}

/** Compares two CKeyValues and returns result. Compares the NameSpace values first
 * and if they are equal, compares Keys. Note: if the NameSpace or Key has not been
 * set, empty string is used in the comparison.
 * @param aFirst The first key value pair
 * @param aSecond The second key value pair.
 * @returns -1 if aFirst < aSecond, 0 if equal, 1 if aFirst > aSecond.
 */
TInt CKeyValue::Compare(const CKeyValue & aFirst, const CKeyValue & aSecond)
	{
	TInt result =  aFirst.NameSpace().Compare(aSecond.NameSpace());
	if (result > 0)
		return 1;
	else if (result < 0)
		return -1;
	else // Keys are the same
		{
		result = aFirst.Key().Compare(aSecond.Key());
		if (result > 0)
			return 1;
		if (result < 0)
			return -1;
		}
	return 0; // Objects are equal.
	}

/**
 * Accepts a visitor to visit this object.
 * @param aVisitor The visitor.
 */
EXPORT_C void CKeyValue::AcceptL(MXmlVisitor & aVisitor)
	{
	aVisitor.VisitL(*this);
	}


} // ajj
} // org
