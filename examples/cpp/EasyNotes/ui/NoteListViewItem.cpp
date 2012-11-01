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
 * @file NoteListViewItem.cpp
 * @author Emma Tresanszki.
 */

#include "../Util.h"
#include "../engine/GeneralSettings.h"
#include "NoteListViewItem.h"
#include "MAHeaders.h"

NoteListViewItem::NoteListViewItem(NoteListItemType type)
:ListViewItem()
{
	createEntryUI(type);
}

NoteListViewItem::NoteListViewItem(
		const MAUtil::String title,
		const MAUtil::String dateCreated,
		const MAUtil::String dateModified,
		int displayIndex)
:ListViewItem()
{
	createEntryUI();
	mNoteTitleLabel->setText(title);
	mDisplayDateModifiedLabel->setText(dateModified);
//	mDisplayDateCreatedLabel->setText(dateCreated);
//	mDisplayDateCreatedLabel->setVisible(
//			GeneralSettings::getInstance()->isDisplayDateCreatedEnabled());
	setDisplayIndex(displayIndex);
}

NoteListViewItem::NoteListViewItem(
		const MAUtil::String title, const MAUtil::String preview)
:ListViewItem()
{
	createEntryUI(NOTE_LIST_VIEW_ITEM_HISTORY);
	mNoteTitleLabel->setText(title);
	mNotePreviewLabel->setText(preview);
}

void NoteListViewItem::createEntryUI(NoteListItemType type)
{
	HorizontalLayout* rowLayout = new HorizontalLayout();

	VerticalLayout* leftLayout = new VerticalLayout();
	mNoteTitleLabel = new Label();
	mNoteTitleLabel->setFontColor(
			GeneralSettings::getInstance()->getFontColor());
	mNoteTitleLabel->setFontSize(
			GeneralSettings::getInstance()->getFontSize());
	leftLayout->addChild(mNoteTitleLabel);

	VerticalLayout* rightLayout = new VerticalLayout();
	mDisplayDateModifiedLabel = new Label();
	mDisplayDateModifiedLabel->setFontSize(
			GeneralSettings::getInstance()->getFontSize()-5);
	leftLayout->addChild(mDisplayDateModifiedLabel);

	mNotePreviewLabel = new Label();
	mNotePreviewLabel->setFontSize(
			GeneralSettings::getInstance()->getFontSize()-10);
	leftLayout->addChild(mNotePreviewLabel);

	mRemoveBtn = new Image();// ImageButton();
	mRemoveBtn->setImage(RES_REMOVE_BTN);
	rightLayout->addChild(mRemoveBtn);
	rightLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
//	mDisplayDateCreatedLabel = new Label();
//	mDisplayDateCreatedLabel->setFontSize(
//			GeneralSettings::getInstance()->getFontSize()-5);

	// Label for history type only.
	mHourDateModifiedLabel = new Label();
	mHourDateModifiedLabel->setFontSize(
			GeneralSettings::getInstance()->getFontSize()-5);

//	rightLayout->addChild(mDisplayDateCreatedLabel);
	rightLayout->addChild(mHourDateModifiedLabel);

	rowLayout->addChild(leftLayout);
	rowLayout->addChild(rightLayout);
	this->addChild(rowLayout);

	if (type == NOTE_LIST_VIEW_ITEM_DEFAULT)
	{
		mNotePreviewLabel->setVisible(false);
		mHourDateModifiedLabel->setVisible(false);
	}
	else
	{
		mDisplayDateModifiedLabel->setVisible(false);
//		mDisplayDateCreatedLabel->setVisible(false);
		mRemoveBtn->setVisible(false);
	}
}

/**
 * Update the UI when the arrange type was updated.
 * @param type The new type.
 */
void NoteListViewItem::updateType(NoteListItemType type)
{
	if (type == NOTE_LIST_VIEW_ITEM_DEFAULT)
	{
		mNotePreviewLabel->setVisible(false);
		mHourDateModifiedLabel->setVisible(false);

		mDisplayDateModifiedLabel->setVisible(true);
//		mDisplayDateCreatedLabel->setVisible(true);
		mRemoveBtn->setVisible(true);
	}
	else
	{
		mDisplayDateModifiedLabel->setVisible(false);
//		mDisplayDateCreatedLabel->setVisible(false);
		mRemoveBtn->setVisible(false);

		mNotePreviewLabel->setVisible(true);
		mHourDateModifiedLabel->setVisible(true);
	}
}

NoteListViewItem::~NoteListViewItem()
{
}

void NoteListViewItem::applyFontSize(int fontSize)
{
	mNoteTitleLabel->setFontSize(fontSize);
//	mDisplayDateLabel->setFontSize(fontSize);
//	mDisplayCountLabel->setFontSize(fontSize);
}
void NoteListViewItem::applyFontColor(int fontColor)
{
	mNoteTitleLabel->setFontColor(fontColor);
//	mDisplayDateLabel->setFontColor(fontColor);
//	mDisplayCountLabel->setFontColor(fontColor);
}
