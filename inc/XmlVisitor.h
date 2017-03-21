#ifndef __XMLVISITOR_H_
#define __XMLVISITOR_H_

/*
 * $Id:  $
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

class CXmlDocument;
class CXmlElement;
class CKeyValue;

/**
 * Defines an interface for a visitor, visiting XML structures.
 * @author Antti Juustila
 * @version $Revision: $
 */
class MXmlVisitor
{
public:
	MXmlVisitor() {};
	virtual ~MXmlVisitor() {};
	
	/** Pure virtual visit method, implemented in subclasses.
	 * @param aDocument Object to visit.
	 */
	virtual void VisitL(CXmlDocument & aDocument) = 0;
	/** Pure virtual visit method, implemented in subclasses.
	 * @param aElement Object to visit.
	 */
	virtual void VisitL(CXmlElement & aElement) = 0;
	/** Pure virtual visit method, implemented in subclasses.
	 * @param aKeyValue Object to visit.
	 */
	virtual void VisitL(CKeyValue & aKeyValue) = 0;
	/** Pure virtual visit method, implemented in subclasses.
	 * @returns Returns ETrue, if visitor takes care of navigating the object structure.
	 */
	virtual TBool VisitorNavigates() const = 0;
};

} // ajj
} // org

#endif /*__XMLVISITOR_H_*/
