#ifndef __KEYVALUE_H_
#define __KEYVALUE_H_

/*
 * $Id: KeyValue.h 1155 2009-03-12 18:15:36Z antti $
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

class MXmlVisitor;

/**
 * Defines key-value -pairs, used in XML parsing.
 * Stores the key-value pairs as 16bit descriptors, but accepts
 * also 8 bit descriptors as parameters (from xml files).
 * @author Antti Juustila
 * @version $Revision: 1155 $
 */
class CKeyValue : public CBase
{
public:
	IMPORT_C CKeyValue();
	IMPORT_C static CKeyValue * NewL(const TDesC & aKey, const TDesC & aValue);
	IMPORT_C static CKeyValue * NewLC(const TDesC & aKey, const TDesC & aValue);
	IMPORT_C static CKeyValue * NewL(const TDesC & aNameSpace, const TDesC & aKey, const TDesC & aValue);
	IMPORT_C static CKeyValue * NewLC(const TDesC & aNameSpace, const TDesC & aKey, const TDesC & aValue);

	IMPORT_C static CKeyValue * NewL(const TDesC8 & aKey, const TDesC8 & aValue);
	IMPORT_C static CKeyValue * NewLC(const TDesC8 & aKey, const TDesC8 & aValue);
	IMPORT_C static CKeyValue * NewL(const TDesC8 & aNameSpace, const TDesC8 & aKey, const TDesC8 & aValue);
	IMPORT_C static CKeyValue * NewLC(const TDesC8 & aNameSpace, const TDesC8 & aKey, const TDesC8 & aValue);
	
	IMPORT_C virtual ~CKeyValue();

	IMPORT_C const TDesC & NameSpace() const;
	IMPORT_C const TDesC & Key() const;
	IMPORT_C const TDesC & Value() const;

	IMPORT_C void SetNameSpaceL(const TDesC & aNameSpace);
	IMPORT_C void SetNameSpaceL(const TDesC8 & aNameSpace);
	IMPORT_C void SetKeyL(const TDesC & aKey);
	IMPORT_C void SetValueL(const TDesC & aValue);
	IMPORT_C void SetKeyL(const TDesC8 & aKey);
	IMPORT_C void SetValueL(const TDesC8 & aValue);
	
	IMPORT_C void GetAsTextL(TDes8 & aBuffer);

	static TInt Compare(const CKeyValue & aFirst, const CKeyValue & aSecond);
	
	TInt ApproximateTextLength() const;

	IMPORT_C virtual void AcceptL(MXmlVisitor & aVisitor);
	
private:
	void ConstructL(const TDesC & aNameSpace, const TDesC & aKey, const TDesC & aValue);
	void ConstructL(const TDesC8 & aKey, const TDesC8 & aValue);
	void ConstructL(const TDesC & aKey, const TDesC & aValue);
	void ConstructL(const TDesC8 & aNameSpace, const TDesC8 & aKey, const TDesC8 & aValue);
	
private:
	/** The namespace of the key. */
	HBufC							*iNameSpace;
	/** The value of the key. */
	HBufC					 		*iKey;
	/** The value. */
	HBufC 						*iValue;
};

/** A typedef to easier handling of key value pair arrays. */
typedef RPointerArray<CKeyValue> RKeyValuePairs;

} // ajj
} // org

#endif /*KEYVALUE_H_*/
