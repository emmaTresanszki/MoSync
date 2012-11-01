/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file NotesListener.h
 * @author Emma Tresanszki.
 */

#ifndef NOTESLISTENER_H_
#define NOTESLISTENER_H_

#include <maapi.h>
#include <MAUtil/util.h>
#include <MAUtil/Vector.h>

#include "../engine/NoteGroup.h"

/**
 * Listener class used for updating the UI when changes are made.
 */
class NotesListener
{
public:
	virtual void noteSaved(NoteGroup* note) = 0;
	virtual void noteRemoved() = 0;
	virtual void allNotesRemoved() = 0;
	virtual void settingsUpdated();
	virtual MAUtil::Vector<NoteGroup*>* getNotes();
	virtual NoteGroup& getNote(int index);
	/**
	 * Check if forward navigation is possible.
	 */
	virtual bool hasNextNote();
	/**
	 * Check if backwards navigation is possible.
	 */
	virtual bool hasPreviousNote();
};

#endif /* NOTESLISTENER_H_ */
