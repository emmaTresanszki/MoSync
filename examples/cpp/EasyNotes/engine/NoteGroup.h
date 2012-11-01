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
 * @file NoteGroup.h
 * @author Emma Tresanszki.
 *
 */


#ifndef NOTEGROUP_H_
#define NOTEGROUP_H_

#include <MAUtil/String.h>

/**
 * The NoteGroup class holds all note information:
 *  - title,
 *  - content,
 *  - date created,
 *  - date modified,
 *  - the display index inside the main list.
 */
class NoteGroup
{
public:
	NoteGroup();
	NoteGroup(
			const MAUtil::String title, MAUtil::String note,
			const MAUtil::String dateModified, const MAUtil::String dateCreated);
	virtual ~NoteGroup();

	/**
	 * Override the assignment operator.
	 * @param note The note object.
	 * @return  reference to the copied object.
	 */
	NoteGroup& operator=(const NoteGroup& obj);

	const MAUtil::String getTitle() const{ return mTitle; }
	const MAUtil::String getContent() const{ return mContent; }
	const MAUtil::String getDateCreated() const{ return mDateCreated; }
	const MAUtil::String getDateModified() const{ return mDateModified; }
	tm* getDateModifiedStruct(){ return mModifiedDate; };
	void setTitle(const MAUtil::String title){ mTitle = title; };
	void setContent(const MAUtil::String content){ mContent = content; };
	void setDateModified(const MAUtil::String date);
	void setDateCreated(const MAUtil::String date){ mDateCreated = date; };

	const int getFileOffset() const { return mFileOffset; };
	const int getDisplayIndex() const { return mDisplayIndex; };
	void setDisplayIndex(int index) { mDisplayIndex = index; };
	void setFileOffset(int offset) { mFileOffset = offset; };
private:

	MAUtil::String mTitle;
	MAUtil::String mContent;
	//MAUtil::Vector<MAUtil::String> mNotes; //so we have items count.
	MAUtil::String mDateModified;
	MAUtil::String mDateCreated;
	struct tm * mModifiedDate;

	/**
	 * The note offset into the notes file.
	 * This will be removed.
	 */
	int mFileOffset;

	/**
	 * The display index inside the notes list.
	 */
	int mDisplayIndex;
};

#endif /* NOTEGROUP_H_ */
