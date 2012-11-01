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
 * @file NoteListViewItem.h
 * @author Emma Tresanszki.
 * Custom list view item for display note group info.
 * All labels are customizable.
 *
 * 2 Types:
 * - the default type used for the list arrange type.
 * Custom list view item for display note group info:
 * Note group title, optional date modified label and count label.
 * All labels are customizable.
 * - the type used for history arrange type.
 * Contains the title, a preview of the note and the date modified.
 * For Today section display only the hour modified,
 * for other sections display only the date modified, without the hour.
 */

#ifndef NOTELISTVIEWITEM_H_
#define NOTELISTVIEWITEM_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

/**
 * Custom list view items.
 */
enum NoteListItemType
{
	NOTE_LIST_VIEW_ITEM_DEFAULT = 0,
	NOTE_LIST_VIEW_ITEM_HISTORY
};

using namespace NativeUI;

/**
 * 2 Types:
 * - the default type used for the list arrange type.
 * Custom list view item for display note group info:
 * Note group title, optional date modified label and count label.
 * All labels are customizable.
 * - the type used for history arrange type.
 * Contains the title, a preview of the note and the date modified.
 * For Today section display only the hour modified,
 * for other sections display only the date modified, without the hour.
 */
class NoteListViewItem:
	public ListViewItem
{
public:
		NoteListViewItem(
				NoteListItemType type = NOTE_LIST_VIEW_ITEM_DEFAULT);
		NoteListViewItem(
				const MAUtil::String title,
				const MAUtil::String dateCreated,
				const MAUtil::String dateModified,
				int displayIndex);
		virtual ~NoteListViewItem();
		void createEntryUI(NoteListItemType type = NOTE_LIST_VIEW_ITEM_DEFAULT);

		void applyFontSize(int fontSize);
		void applyFontColor(int fontColor);
		void enableDisplayDateCreated(bool state) { mDisplayDateCreatedLabel->setVisible(state); };
		void enableDisplayDateModified(bool state){ mDisplayDateModifiedLabel->setVisible(state); };
		void updateTitle(const MAUtil::String title){ mNoteTitleLabel->setText(title); };
		void updateDateCreated(const MAUtil::String date) { mDisplayDateCreatedLabel->setText(date);};
		void updateDateModified(const MAUtil::String date) { mDisplayDateModifiedLabel->setText(date);};
		Image* getRemoveBtn(){ return mRemoveBtn; };

		/**
		 * Update the UI when the arrange type was updated.
		 * @param type The new type.
		 */
		void updateType(NoteListItemType type);

		// Methods for the NOTE_LIST_VIEW_ITEM_HISTORY type.
		NoteListViewItem(
				const MAUtil::String title, const MAUtil::String preview);
		void updatePreview(const MAUtil::String preview) { mNotePreviewLabel->setText(preview); };
		void updateHourDateModified(const MAUtil::String date) { mHourDateModifiedLabel->setText(date); };
		void setDisplayIndex(int index) { mDisplayIndex = index; };
		int getDisplayIndex() { return mDisplayIndex; };
private:
		Label* mNoteTitleLabel;
		Label* mDisplayDateCreatedLabel;
		Label* mDisplayDateModifiedLabel;
		Image* mRemoveBtn;
		/**
		 * Store the list view type.
		 */
		bool mIsDefaultType;

		/**
		 * The history type displays a small preview of the note content.
		 */
		Label* mNotePreviewLabel;

		/**
		 * For the history type display the hour or the date modified.
		 */
		Label* mHourDateModifiedLabel;

		/**
		 * Store the display index in the list view.
		 */
		int mDisplayIndex;
};

#endif /* NOTELISTVIEWITEM_H_ */
