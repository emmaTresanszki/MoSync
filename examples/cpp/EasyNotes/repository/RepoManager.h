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
 * @file RepoManager.h
 * @author Emma Tresanszki.
 */


#ifndef REPOMANAGER_H_
#define REPOMANAGER_H_

#include <MAUtil/String.h>

#include "../Util.h"
#include "../engine/NoteGroup.h"

/**
 * All applications settings are stored into a file.
 * All notes are stored into a separate file.
 */
class RepoManager
{
public:
	/**
	 * Constructor.
	 */
	RepoManager();

	/**
	 * Destructor.
	 */
	virtual ~RepoManager();

	/**
	 * Read all settings from the settings file.
	 */
	void readSettings();

	/**
	 * Read all the notes from the notes file.
	 */
	void readNotes();

	/**
	 * Read next field from the settings data.
	 * @param content The settings data buffer.
	 * @param offset The starting position for parsing the buffer.
	 * It is always updated with the last search position.
	 * @return The setting value.
	 */
	MAUtil::String readNextSetting(
			const MAUtil::String content, int offset);

	/**
	 * Read next note from the notes file.
	 * @param content The notes data buffer.
	 * @param offset The starting position for parsing the buffer.
	 * it is always updated with the last search position.
	 * @return The new NoteGroup object.
	 */
	NoteGroup* readNextNote(const MAUtil::String content, int offset);

	/**
	 * Rewrite all the settings to the file.
	 */
	void writeSettings();

	/**
	 * Rewrite all the notes.
	 * @param notes The notes list.
	 */
	void writeNotes(MAUtil::Vector<NoteGroup*>* notes);

	/**
	 * Write a new note to the end of the file.
	 * Also, store it in the notes list.
	 * @param note The new NoteGroup object.
	 */
	void writeNote(NoteGroup*  note);

	/**
	 * Edit an already stored note.
	 * @param note The existing note object.
	 */
//	void editNote(NoteGroup* note);

	/**
	 * Check if the files exist, and if not create empty ones.
	 */
	void checkFiles();

	/**
	 * Add a new note to the end of the file.
	 * @param note The note object.
	 */
	void addNote(NoteGroup* note);

	/**
	 * Remove all the notes from the file and from the
	 * internal storage.
	 */
	void removeAllNotes();

	/**
	 * Remove all the settings data from the file.
	 * Remember to apply default settings to the settings screen.
	 */
	void removeSettings();

	/**
	 * Get the notes list, after it was read from the file.
	 * @return The notes list.
	 */
	MAUtil::Vector<NoteGroup*>* getNotes();
private:

	/**
	 * Handles to the files.
	 */
	MAHandle mSettingsFile;
	MAHandle mNotesFile;
	MAUtil::String mSettingsFilePath;
	MAUtil::String mNotesFilePath;

	/**
	 * The content string of the notes file.
	 */
	MAUtil::String* mNotesContent;

	/**
	 * Keep a map with each NoteGroup object
	 * read from the file, and it's starting
	 * position inside the mNotesContent;
	 */
//	MAUtil::Map<int, NoteGroup*> mNotes;
	/**
	 * Vector with each NoteGroup object read
	 * from the notes file.
	 */
	MAUtil::Vector<NoteGroup*>* mNotes;
};

#endif /* REPOMANAGER_H_ */
