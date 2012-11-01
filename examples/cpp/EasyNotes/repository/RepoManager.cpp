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
 * @file RepoManager.cpp
 * @author Emma Tresanszki.
 */

#include <MAFS/File.h>
#include <MAUtil/String.h>

#include "RepoManager.h"
#include "../engine/GeneralSettings.h"

/**
 * Constructor.
 */
RepoManager::RepoManager()
{
	// Initialize notes list.
	mNotes = new MAUtil::Vector<NoteGroup*>();

	char localPath[BUF_MAX];
	maGetSystemProperty("mosync.path.local", localPath, BUF_MAX);

	mSettingsFilePath = MAUtil::String(localPath);
	mSettingsFilePath.append("/", 1);
	mSettingsFilePath.append(SETTINGS_FILE, 12);

	mNotesFilePath = MAUtil::String(localPath);
	mNotesFilePath.append("/", 1);
	mNotesFilePath.append(NOTES_FILE, 9);

//	mNotesContent = new MAUtil::String(BUF_MAX);
	mNotesContent = new MAUtil::String( "\0", BUF_MAX);

	checkFiles();
	readSettings();
	readNotes();
}

/**
 * Get the notes list, after it was read from the file.
 * @return The notes list.
 */
MAUtil::Vector<NoteGroup*>* RepoManager::getNotes()
{
	return mNotes;
}

/**
 * Check if the files exist, and if not create empty ones.
 */
void RepoManager::checkFiles()
{
	// Open the two files.
	mSettingsFile = maFileOpen(mSettingsFilePath.c_str(), MA_ACCESS_READ_WRITE);
	mNotesFile = maFileOpen(mNotesFilePath.c_str(), MA_ACCESS_READ_WRITE);

	if ( 0 == maFileExists(mSettingsFile) )
	{
		maFileCreate(mSettingsFile);
	}
	if ( 0 == maFileExists(mNotesFile) )
	{
		maFileCreate(mNotesFile);
	}

	maFileClose(mSettingsFile);
	maFileClose(mNotesFile);
}

/**
 * Add a new note to the end of the file.
 * @param note Teh note object.
 */
void RepoManager::addNote(NoteGroup* note)
{
	//mNotes.insert(mNotesContent->size(), note);
	mNotes->add(note);

	writeNote(note);
}

/**
 * Remove all the notes from the file and from the
 * internal storage.
 */
void RepoManager::removeAllNotes()
{
	mNotesFile = maFileOpen(
			mNotesFilePath.c_str(), MA_ACCESS_READ_WRITE);
	maFileTruncate(mNotesFile, 0);
	maFileClose(mNotesFile);

	// Clear content string.
	mNotesContent->clear();

	// Clear notes map.
	mNotes->clear();
}

/**
 * Remove all the settings data from the file.
 * Remember to apply default settings to the settings screen.
 */
void RepoManager::removeSettings()
{
	mSettingsFile = maFileOpen(
			mSettingsFilePath.c_str(), MA_ACCESS_READ_WRITE);
	maFileTruncate(mSettingsFile, 0);
	maFileClose(mSettingsFile);
}

/**
 * Destructor.
 */
RepoManager::~RepoManager()
{
	mNotes->clear();
	if (mNotesContent)
	{
		delete mNotesContent;
		mNotesContent = NULL;
	}
}

/**
 * Read the application settings.
 * The existence of the file is checked at all times.
 * All data is read at once, since it will always have a fixed size.
 * Structure:
 * 	int fontSize;int fontColor;int modifiedDate;int createdDate;
 */
void RepoManager::readSettings()
{
	// Read file, and if not empty parse it.
	mSettingsFile = maFileOpen(
			mSettingsFilePath.c_str(), MA_ACCESS_READ_WRITE);

	if(0 == maFileExists(mSettingsFile))
	{
		maFileClose(mSettingsFile);
		GeneralSettings::getInstance()->initialize(
				FONT_MEDIUM_RES_LOW, BLUE, false, true, false);
		return;
	}

	// Get the file size.
	int size = maFileSize(mSettingsFile);
    // Read the file data.
	char data[BUF_MAX];
	int readRes = maFileRead(mSettingsFile, &data, size);
	//memset(&data, 0, size);

	if ( readRes < 0 || strlen(data) <= 0 )
	{
		GeneralSettings::getInstance()->initialize(
				FONT_MEDIUM_RES_LOW, BLUE, false, true, false);
		maFileClose(mSettingsFile);
		return;
	}
//	else if(strlen(data) <= 0)
//	{
//		GeneralSettings::getInstance()->initialize(
//				FONT_MEDIUM_RES_LOW, BLUE, false, true, false);
//	}
	else
	{
		MAUtil::String content = MAUtil::String("");
		content.append(data, strlen(data));

		int offset = 0;
		int separatorPos = 0;
		MAUtil::String fieldValue = "";

		separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
		fieldValue = content.substr(offset, separatorPos-offset);
		offset = separatorPos + 1;
		GeneralSettings::getInstance()->setFontSize(MAUtil::stringToInteger(fieldValue));

		separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
		fieldValue = content.substr(offset, separatorPos-offset);
		offset = separatorPos+ 1;
		GeneralSettings::getInstance()->setFontColor(getColorValue(fieldValue));

		separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
		fieldValue = content.substr(offset, separatorPos-offset);
		offset = separatorPos+ 1;
		if ( MAUtil::stringToInteger(fieldValue) )
	    {
			GeneralSettings::getInstance()->setDisplayDateModified(true);
	    }
	    else
	    {
			GeneralSettings::getInstance()->setDisplayDateModified(false);
	    }

		separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
		fieldValue = content.substr(offset, separatorPos-offset);
		offset = separatorPos+ 1;
		if ( MAUtil::stringToInteger(fieldValue) )
	    {
			GeneralSettings::getInstance()->setDisplayDateCreated(true);
	    }
	    else
	    {
			GeneralSettings::getInstance()->setDisplayDateCreated(false);
	    }

		separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
		fieldValue = content.substr(offset, separatorPos-offset);
		offset = separatorPos+ 1;
		if ( MAUtil::stringToInteger(fieldValue) )
	    {
			GeneralSettings::getInstance()->setDisplayCount(true);
	    }
	    else
	    {
			GeneralSettings::getInstance()->setDisplayCount(false);
	    }
	}

	maFileClose(mSettingsFile);
}

/**
 * Read next field from the settings data.
 * @param content The settings data buffer.
 * @param offset The starting position for parsing the buffer.
 * It is always updated with the last search position.
 * @return The setting value.
 */
MAUtil::String RepoManager::readNextSetting(
		const MAUtil::String content, int offset)
{
	int separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
	if (separatorPos == -1 )
		// Broken file. Empty it.
		maFileTruncate(mSettingsFile, 0);

	MAUtil::String fieldValue =
			content.substr(offset, separatorPos-offset);

	offset = separatorPos + 1;
	return fieldValue;
}

/**
 * Read all notes - if any, and store them to the local list.
 */
void RepoManager::readNotes()
{
	// Read file, and if not empty parse it.
	mNotesFile = maFileOpen(
			mNotesFilePath.c_str(), MA_ACCESS_READ_WRITE);

	// Get the file size.
	int size = maFileSize(mNotesFile);
    // Read the file data.
	char data[BUF_MAX];
	int readRes = maFileRead(mNotesFile, &data, size);
	memset(&data, 0, BUF_MAX);

	if ( readRes >= 0 && strlen(data) > 0 )
	{
		// TODO parse chunks.
	//	if ( size > BUF_MAX ))

		//MAUtil::String notesData = "";
	//	notesData.append(data, strlen(data));
		mNotesContent->append(data, strlen(data));

		int offset = 0;
		while (offset != -1)
		{
			//mNotes.add( readNextNote(notesData, offset) );
			mNotes->add(readNextNote(*mNotesContent,offset));
	//		mNotes.insert(offset, readNextNote(notesData, offset));
		}
	}

	maFileClose(mNotesFile);
}

/**
 * Read next note from the notes file.
 * @param content The notes data buffer.
 * @param offset The starting position for parsing the buffer.
 * it is always updated with the last search position.
 * @return The new NoteGroup object.
 */
NoteGroup* RepoManager::readNextNote(const MAUtil::String content, int offset)
{
	int oldOffset = offset;

	int separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
	if (separatorPos == -1 )
		// Broken file. Empty it.
		maFileTruncate(mNotesFile, 0);

	MAUtil::String dateCreated =
			content.substr(offset, separatorPos-offset);

	offset = separatorPos + 1;
	separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
	MAUtil::String noteTitle =
			content.substr(offset, separatorPos-offset);

	offset = separatorPos + 1;
	separatorPos = content.find(FILE_FIELD_SEPARATOR, offset);
	MAUtil::String dateModified =
			content.substr(offset, separatorPos-offset);

	offset = separatorPos + 1;
	separatorPos = content.find(FILE_NOTES_SEPARATOR, offset);
	MAUtil::String noteContent =
			content.substr(offset, separatorPos-offset);

	NoteGroup* newNote = new NoteGroup(noteTitle, noteContent, dateModified, dateCreated);
	newNote->setFileOffset(oldOffset);

	offset = separatorPos + 1; // will be the offset to start searching for the next note.
	return newNote;
}

/**
 * Rewrite all the settings.
 */
void RepoManager::writeSettings()
{
	mSettingsFile = maFileOpen(
			mSettingsFilePath.c_str(), MA_ACCESS_READ_WRITE);

	MAUtil::String settingsData;
	MAUtil::String fontSize = MAUtil::integerToString(
					GeneralSettings::getInstance()->getFontSize());
	settingsData.append( fontSize.c_str(), fontSize.size());
	settingsData.append(FILE_FIELD_SEPARATOR, 1);
	MAUtil::String fontColor = getColorString(
			GeneralSettings::getInstance()->getFontColor() );
	settingsData.append(fontColor.c_str(), 8);//fontColor.size());
	settingsData.append(FILE_FIELD_SEPARATOR, 1);
	MAUtil::String dateModified =
			MAUtil::integerToString(GeneralSettings::getInstance()->isDisplayDateModifiedEnabled());
	settingsData.append(dateModified.c_str(), dateModified.size());
	settingsData.append(FILE_FIELD_SEPARATOR, 1);
	MAUtil::String dateCreated =
			MAUtil::integerToString(GeneralSettings::getInstance()->isDisplayDateCreatedEnabled());
	settingsData.append(dateCreated.c_str(), dateCreated.size());
	settingsData.append(FILE_FIELD_SEPARATOR, 1);
	MAUtil::String countEnabled =
			MAUtil::integerToString(GeneralSettings::getInstance()->isDisplayCountEnabled());
	settingsData.append(countEnabled.c_str(), countEnabled.size());
	settingsData.append(FILE_FIELD_SEPARATOR, 1);

	maFileTruncate(mSettingsFile, 0);
//	maFileClose(mSettingsFile);
//	mSettingsFile = maFileOpen(
//			mSettingsFilePath.c_str(), MA_ACCESS_READ_WRITE);
	maFileSeek(mSettingsFile, 0, SEEK_END);
	int res = maFileWrite(
			mSettingsFile, settingsData.c_str(), settingsData.length());
	maFileClose(mSettingsFile);
}

/**
 * Rewrite all the notes.
 * @param notes The notes list.
 */
void RepoManager::writeNotes(MAUtil::Vector<NoteGroup*>* notes)
{
	mNotesFile = maFileOpen(
			mNotesFilePath.c_str(), MA_ACCESS_READ_WRITE);
	maFileSeek(mNotesFile, 0, SEEK_END);

	MAUtil::String content;
	for(int i=0; i < notes->size(); i++)
	{
		MAUtil::String note = (*notes)[i]->getDateCreated() + FILE_FIELD_SEPARATOR +
				(*notes)[i]->getDateModified() + FILE_FIELD_SEPARATOR +
				(*notes)[i]->getTitle() + FILE_FIELD_SEPARATOR +
				(*notes)[i]->getContent() + FILE_NOTES_SEPARATOR;
		content.append(note.c_str(), note.size());
	}

	maFileTruncate(mNotesFile, 0);
	maFileSeek(mNotesFile, 0 , SEEK_END);
	int res = maFileWrite(
			mNotesFile, content.c_str(), sizeof(content.c_str()));

	maFileClose(mNotesFile);
}

/**
 * Write a new note to the end of the file.
 * Also, store it in the notes list.
 * @param note The new NoteGroup object.
 */
void RepoManager::writeNote(NoteGroup*  note)
{
	note->setFileOffset(mNotesContent->length());

	mNotesFile = maFileOpen(
			mNotesFilePath.c_str(), MA_ACCESS_READ_WRITE);

	// TODO update the first line with the new count.

	maFileSeek(mNotesFile, 0, SEEK_END);
	MAUtil::String noteString;
	noteString.append(note->getDateCreated().c_str(), note->getDateCreated().size());
	noteString.append(FILE_FIELD_SEPARATOR, 1);
	noteString.append(note->getDateModified().c_str(), note->getDateModified().size());
	noteString.append(FILE_FIELD_SEPARATOR, 1);
	noteString.append(note->getTitle().c_str(), note->getTitle().size());
	noteString.append(FILE_FIELD_SEPARATOR, 1);
	noteString.append(note->getContent().c_str(), note->getContent().size());
	noteString.append(FILE_NOTES_SEPARATOR, 1);

//	mNoteStrings.add(noteString);
	mNotesContent->append(noteString.c_str(), noteString.length());

	int res = maFileWrite(
			mNotesFile, noteString.c_str(), sizeof(noteString.c_str()));
	maFileClose(mNotesFile);
}
