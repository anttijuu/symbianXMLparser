/*
 * $Id: ConversionUtils.cpp 1804 2009-09-07 10:03:52Z antti $
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

#include <utf.h>

#include "ConversionUtils.h"
#include "XMLParserConstants.h"

namespace org
{
namespace ajj
{


/** Converts an Unicode string to a UTF-8 string, appending the
 * generated UTF-8 string to an existing 8 bit descriptor.
 * There must be enough room in the descriptor to add, otherwise a
 * panic will occur.
 * @param aThingToAdd The 16 bit descriptor to convert.
 * @param aWhereToAdd The 8 bit descriptor to add the UTF-8 converted string.
 */
void ConversionUtils::AppendToUtf8BufferL(const TDesC & aThingToAdd, TDes8 & aWhereToAdd)
	{
	HBufC8 * converted = 0;
	converted = CnvUtfConverter::ConvertFromUnicodeToUtf8L(aThingToAdd);
	if (converted != 0)
		{
		aWhereToAdd.Append(*converted);
		}
	delete converted;
	
//	TInt currLen = aWhereToAdd.Length();
//	TInt toAddLen = aThingToAdd.Length();
//	TInt totalLen = currLen+toAddLen;
//	aWhereToAdd.SetLength(currLen+toAddLen);
//	TPtr8 ptr(aWhereToAdd.MidTPtr(currLen));
//	TInt result = CnvUtfConverter::ConvertFromUnicodeToUtf8(ptr, aThingToAdd);
//	while (result > 0)
//		{
//		currLen = aWhereToAdd.Length();
//		aWhereToAdd.SetLength(currLen+result+1);
//		TPtr8 ptr(aWhereToAdd.MidTPtr(currLen));
////		ptr.FillZ();
//		result = CnvUtfConverter::ConvertFromUnicodeToUtf8(ptr, aThingToAdd.Right(result));
//		}
//	aWhereToAdd.SetLength(currLen+toAddLen);
	}

/**
 * Appends a 16 bit descriptor to a 8 bit one, encoding chars not allowed
 * in XML content. There must be enough room in the receiving descriptor,
 * otherwise a panic will occur.
 * @param aThingToAdd The text to add.
 * @param aWhereToAdd The buffer to add the text to. 
 */
void ConversionUtils::AppendToUtf8BufferEncodedL(const TDesC & aThingToAdd, TDes8 & aWhereToAdd)
	{
	const TInt bufLen = aThingToAdd.Length();
	HBufC * encoded = HBufC::NewL(bufLen*2);
	TPtr ptr(encoded->Des());
	for (TInt counter = 0; counter < bufLen; ++counter)
		{
		if (aThingToAdd[counter] == KCharLessThan()[0])
			{
			ptr.Append(KLTReference);
			}
		else if (aThingToAdd[counter] == KCharGreaterThan()[0])
			{
			ptr.Append(KGTReference);
			}
		else if (aThingToAdd[counter] == KCharAmpersand()[0])
			{
			ptr.Append(KAmpersandReference);
			}
		else if (aThingToAdd[counter] == KCharPercent()[0])
			{
			ptr.Append(KPercentReference);
			break;
			}
		else
			{
			ptr.Append(aThingToAdd[counter]);
			} // End if
		}
	ConversionUtils::AppendToUtf8BufferL(*encoded, aWhereToAdd);
	delete encoded;
	}

/** Converts an 8 bit UTF-8 string to an unicode string, appending the
 * generated unicode string to an existing 16 bit descriptor.
 * There must be enough room in the descriptor to add, otherwise a
 * panic will occur.
 * @param aThingToAdd The 8 bit descriptor to convert.
 * @param aWhereToAdd The 16 bit descriptor to add the Unicode converted string.
 */
void ConversionUtils::AppendToUnicodeBufferL(const TDesC8 & aThingToAdd, TDes & aWhereToAdd)
	{
	HBufC * converted = 0;
	converted = CnvUtfConverter::ConvertToUnicodeFromUtf8L(aThingToAdd);
	if (converted != 0)
		{
		aWhereToAdd.Append(*converted);
		}
	delete converted;
//	TInt currLen = aWhereToAdd.Length();
//	aWhereToAdd.SetLength(currLen+aThingToAdd.Length());
//	TPtr ptr(aWhereToAdd.MidTPtr(currLen));
//	CnvUtfConverter::ConvertToUnicodeFromUtf8(ptr, aThingToAdd);
	}

/** Converts an 8 bit UTF-8 string to an unicode string, appending and
 * decoding the  generated unicode string to an existing 16 bit descriptor.
 * There must be enough room in the descriptor to add, otherwise a
 * panic will occur.
 * @param aThingToAdd The 8 bit descriptor to convert.
 * @param aWhereToAdd The 16 bit descriptor to add the Unicode converted string.
 */
void ConversionUtils::AppendToUnicodeBufferDecodedL(const TDesC8 & aThingToAdd, TDes & aWhereToAdd)
	{
	const TInt bufLen = aThingToAdd.Length();
	HBufC8 * decoded = HBufC8::NewL(bufLen*2);
	TPtr8 ptr(decoded->Des());
	ptr.Copy(aThingToAdd);
	
	const TBufC8<10> replaceables[] = { KLTReference8(), KGTReference8(), KAmpersandReference8(), KPercentReference8() };
	const TBufC8<10> replaceWiths[] = { KCharLessThan8(), KCharGreaterThan8(), KCharAmpersand8(), KCharPercent8() };
	const TInt KReplaceableCount = 4;
	TInt index = 0;
	for (TInt counter = 0; counter < KReplaceableCount; counter++)
		{
		const TDesC8 & toReplace = replaceables[counter];
		const TDesC8 & replaceWith = replaceWiths[counter];
		do
			{
			index = ptr.Find(toReplace);
			if (index >= KErrNone)
				{
				ptr.Replace(index, toReplace.Length(), replaceWith);
				}
			} while (index >= KErrNone);
		}

	ConversionUtils::AppendToUnicodeBufferL(*decoded, aWhereToAdd);
	delete decoded;
	}


} // ajj
} // org
