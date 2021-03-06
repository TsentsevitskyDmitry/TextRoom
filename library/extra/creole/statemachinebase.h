/***************************************************************************
 *   Copyright (C) 2007 by Ian Reinhart Geiser                             * 
 *   geiseri@yahoo.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef STATEMACHINEBASE_H
#define STATEMACHINEBASE_H
class WikiLexer;
class ParserObserverBase;

/**
The baseclass for the parser state machine.

	@author Ian Reinhart Geiser <geiseri@yahoo.com>
*/
class StateMachineBase{
public:
	struct Context {
		Context();
		WikiLexer *lexer;
		ParserObserverBase *observer;
		StateMachineBase *nextState;
	};
	StateMachineBase();
	virtual ~StateMachineBase();
	
	virtual void exec( Context *ctx );
};

#endif
