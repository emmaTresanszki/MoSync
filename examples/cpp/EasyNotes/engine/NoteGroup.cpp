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
 * @file NoteGroup.cpp
 * @author Emma Tresanszki.
 */

#include "../Util.h"
#include "NoteGroup.h"

NoteGroup::NoteGroup()
{
	mTitle = "";
	mContent = "";
	mDateCreated = getTimeString();
	mDateModified = getTimeString();
	mDisplayIndex = -1;
	mFileOffset = -1;
	mModifiedDate = new tm;
	split_time(maTime(), mModifiedDate);
}

NoteGroup::NoteGroup(
		const MAUtil::String title, MAUtil::String note,
		const MAUtil::String dateModified, const MAUtil::String dateCreated)
{
	mTitle = title;
	mContent = note;
	mDateModified = dateModified;
	mDateCreated = dateCreated;
}

NoteGroup:: ~NoteGroup()
{
	if (mModifiedDate)
	{
		delete mModifiedDate;
		mModifiedDate = NULL;
	}
}

/**
 * Override the assignment operator.
 * @param note The note object.
 * @return  reference to the copied object.
 */
NoteGroup& NoteGroup::operator=(const NoteGroup& obj)
{
	mContent = obj.getContent();
	mDateCreated = obj.getDateCreated();
	mDateModified = obj.getDateModified();
	mDisplayIndex = obj.getDisplayIndex();
	mFileOffset = obj.getFileOffset();
	mTitle = obj.getTitle();

	return *this;
}

void NoteGroup::setDateModified(const MAUtil::String date)
{
	mDateModified = date;
	if ( mModifiedDate )
	{
		delete mModifiedDate;
		mModifiedDate = NULL;
	}
	mModifiedDate = new tm;
	split_time(maTime(), mModifiedDate);
}
