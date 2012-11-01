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
 * @file NoteScreen.cpp
 * @author Emma Tresanszki.
 *
 * This screen contains editable text areas for the note content and title.
 * When Enter/Done is pressed on the Virtual keyboard, the corresponding
 * note object is updated.
 * The NoteScreen displays:
 * - editable title.
 * - editable content.
 * - optional date created ( enabled/disabled via Settings)
 * - optional date modified ( enabled/disabled via Settings)
 * After edit boxes are edited, the note
 * object is updated and the main screen notified.
 * Top button for Back navigation.
 * OptionsMenu for:  Remove.
 */

#include "NoteScreen.h"
#include "../Util.h"
#include "../engine/GeneralSettings.h"

#include "MAHeaders.h"

/**
 * Constructor.
 */
NoteScreen::NoteScreen() :
	Screen(),
	mMainLayout(NULL)
{
	createMainLayout();

	mTitleEdit->addEditBoxListener(this);
	mNoteEditBox->addEditBoxListener(this);
	mBackButton->addButtonListener(this);
	mNoteEditBox->addEditBoxListener(this);

	createConfirmationDialog();
	mYesBtn->addButtonListener(this);
	mNoBtn->addButtonListener(this);

	addScreenListener(this);
	mRemoveOptionsMenu =
			addOptionsMenuItem("Remove", MAW_OPTIONS_MENU_ICON_CONSTANT_DELETE, false);
}

/**
 * Destructor.
 */
NoteScreen::~NoteScreen()
{
	mTitleEdit->removeEditBoxListener(this);
	mNoteEditBox->removeEditBoxListener(this);
	mBackButton->removeButtonListener(this);
	mNoteEditBox->removeEditBoxListener(this);
	mYesBtn->removeButtonListener(this);
	mNoBtn->removeButtonListener(this);
    removeScreenListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void NoteScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(SLATE_GRAY);
	setMainWidget(mMainLayout);

	createScreenHeader();

	mNoteEditBox = new EditBox();
	mNoteEditBox->setPlaceholder("...");
	mNoteEditBox->fillSpaceHorizontally();
	mNoteEditBox->fillSpaceVertically();
	mNoteEditBox->setBackgroundColor(SLATE_GRAY);
	// Make the edit box multi-line.
	mNoteEditBox->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
//	mNoteEditBox->setMaxLines(100);
//	mNoteEditBox->setLinesNumber(100);
//	mNoteEditBox->setMinLines(10);
	mMainLayout->addChild(mNoteEditBox);

	HorizontalLayout* navigationLayout = new HorizontalLayout();
	navigationLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	navigationLayout->wrapContentVertically();
	mPreviousBtn = new ImageButton();
	mPreviousBtn->setImage(RES_PREVIOUS_BTN);
	mNextBtn = new ImageButton();
	mNextBtn->setImage(RES_NEXT_BTN);

	navigationLayout->addChild(mPreviousBtn);
	navigationLayout->addChild(mNextBtn);
	mMainLayout->addChild(navigationLayout);
}

/**
 * Create the remove confirmation dialog.
 */
void NoteScreen::createConfirmationDialog()
{
	mRemoveConfirmationDialog = new Dialog();
	mRemoveConfirmationDialog->setTitle("Warning");
	VerticalLayout* dialogLayout = new VerticalLayout();
	VerticalLayout* infoLayout = new VerticalLayout();
	infoLayout->setHeight(HEADER_HEIGHT);
	infoLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	Label* info = new Label();
	info->setText("  Are you sure you want to remove this note?  ");
	infoLayout->addChild(info);
	HorizontalLayout* buttons = new HorizontalLayout();
	mYesBtn = new Button();
	mYesBtn->setText(" Yes ");
	mYesBtn->fillSpaceHorizontally();
	mYesBtn->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mNoBtn = new Button();
	mNoBtn->setText(" No ");
	mNoBtn->fillSpaceHorizontally();
	mNoBtn->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	buttons->addChild(mYesBtn);
	buttons->addChild(mNoBtn);

	dialogLayout->addChild(infoLayout);
	dialogLayout->addChild(buttons);

	mRemoveConfirmationDialog->setMainWidget(dialogLayout);
}

/**
 * Load the note group and update UI.
 */
void NoteScreen::loadNoteGroup(NoteGroup* note)
{
	mNote = note;

	mLastModifiedDate->setText(note->getDateModified());//getTimeString());
	mCreatedDate->setText(note->getDateCreated());
	mTitleEdit->setText(note->getTitle());
	mNoteEditBox->setText(note->getContent());
	mCreatedDate->setVisible(
			GeneralSettings::getInstance()->isDisplayDateCreatedEnabled());
	mLastModifiedDate->setVisible(
			GeneralSettings::getInstance()->isDisplayDateModifiedEnabled());
	checkNavigation();
}

/**
 * Check if next and previous navigation is possible for the
 * current note.
 */
void NoteScreen::checkNavigation()
{
	if ( !mListener->hasNextNote() )
	{
		mNextBtn->setImage(RES_NEXT_BTN_DISABLED);
		mNextBtn->setEnabled(false);
//		mNextBtn->setVisible(false);
	}
	else
	{
		mNextBtn->setImage(RES_NEXT_BTN);
		mNextBtn->setEnabled(true);
//		mNextBtn->setVisible(true);
	}
	if( !mListener->hasPreviousNote() )
	{
		mPreviousBtn->setImage(RES_PREVIOUS_BTN_DISABLED);
		mPreviousBtn->setEnabled(false);
//		mPreviousBtn->setVisible(false);
	}
	else
	{
		mPreviousBtn->setImage(RES_PREVIOUS_BTN);
		mPreviousBtn->setEnabled(true);
//		mPreviousBtn->setVisible(true);
	}
}

/**
 * Create the header area.
 */
void NoteScreen::createScreenHeader()
{
	HorizontalLayout* topLayout = new HorizontalLayout();
//	topLayout->wrapContentVertically();
	topLayout->setHeight(100);
	topLayout->setBackgroundColor(QUARTZ);
	mBackButton = new Button();
	mBackButton->setFontColor(SLATE_GRAY);
	mBackButton->setFontSize(
			GeneralSettings::getInstance()->getFontSize() * 2);
	mBackButton->setBackgroundColor(QUARTZ);
	mBackButton->setText(" < ");//Back");
	mTitleEdit = new EditBox();
	mTitleEdit->setWidth( (3*getScreenWidth())/4);
	mTitleEdit->setBackgroundColor(SLATE_GRAY);
	mTitleEdit->setFontColor(
			GeneralSettings::getInstance()->getFontColor());
	mTitleEdit->setPlaceholder("Note ...");
	mTitleEdit->setPlaceholderFontColor(QUARTZ);

	topLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	topLayout->addChild(mBackButton);
	topLayout->addChild(mTitleEdit);
	mMainLayout->addChild(topLayout);

	VerticalLayout* lineLayout = new VerticalLayout();
	lineLayout->setBackgroundColor(DARK_WHITE);
	lineLayout->setHeight(5);
	mMainLayout->addChild(lineLayout);

	HorizontalLayout* datesLayout = new HorizontalLayout();
	datesLayout->wrapContentVertically();
	datesLayout->setBackgroundColor(SLATE_GRAY);
	VerticalLayout* dateCreatedLayout = new VerticalLayout();
	dateCreatedLayout->wrapContentVertically();
	dateCreatedLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	mCreatedDate = new Label();
	mCreatedDate->setText("");
	mCreatedDate->setFontColor(QUARTZ);
	dateCreatedLayout->addChild(mCreatedDate);
	VerticalLayout* dateLayout = new VerticalLayout();
	dateLayout->wrapContentVertically();
	dateLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	mLastModifiedDate = new Label();
	mLastModifiedDate->setText("");
	mLastModifiedDate->setFontColor(QUARTZ);
	dateLayout->addChild(mLastModifiedDate);

	datesLayout->addChild(dateCreatedLayout);
	datesLayout->addChild(dateLayout);
	mMainLayout->addChild(datesLayout);
}

/**
 * Empty the UI fields.
 */
void NoteScreen::emptyUIFields()
{
	mLastModifiedDate->setText(getTimeString());
	mCreatedDate->setText(getTimeString());
	mTitleEdit->setText("");
	mNoteEditBox->setText("");
}

/**
 * Apply application settings to the UI controls.
 */
void NoteScreen::applyAppSettings()
{
	mTitleEdit->setFontColor(
			GeneralSettings::getInstance()->getFontColor());
	mTitleEdit->setPropertyInt(
			MAW_LABEL_FONT_SIZE,
			GeneralSettings::getInstance()->getFontSize());
	mNoteEditBox->setFontColor(
			GeneralSettings::getInstance()->getFontColor());
	mNoteEditBox->setPropertyInt(
			MAW_LABEL_FONT_SIZE,
			GeneralSettings::getInstance()->getFontSize());
	mCreatedDate->setVisible(
			GeneralSettings::getInstance()->isDisplayDateCreatedEnabled());
	mLastModifiedDate->setVisible(
			GeneralSettings::getInstance()->isDisplayDateModifiedEnabled());
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void NoteScreen::buttonClicked(Widget* button)
{
	if(mYesBtn == button)
	{
		// Remove this note and go back to main screen.
		mRemoveConfirmationDialog->hide();
		mListener->noteRemoved();
	}
	else if(mNoBtn == button)
	{
		mRemoveConfirmationDialog->hide();
	}
}

/**
 * Clear the current note object's info.
 */
void NoteScreen::clearNoteObj()
{
	mNote->setContent("");
	mNote->setDateCreated(getTimeString());
	mNote->setDateModified(getTimeString());
	mNote->setTitle("");
	mNote->setDisplayIndex(0);
	mNote->setFileOffset(0);
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void NoteScreen::editBoxReturn(EditBox* editBox)
{
	if (mTitleEdit == editBox)
	{
		mLastModifiedDate->setText(getTimeString());
		// Do not hide keyboard when a new line is added.
		// TOdO send new type of event when new line is entered.
//		mTitleEdit->hideKeyboard();
		mNote->setTitle(mTitleEdit->getText());
		// Store the new note title.
		mListener->noteSaved(mNote);

	}
	else if (mNoteEditBox == editBox)
	{
		mLastModifiedDate->setText(getTimeString());
//		mNoteEditBox->hideKeyboard();
		mNote->setContent(mNoteEditBox->getText());
		// Save content and last modified date.
		mListener->noteSaved(mNote);
	}
}

/**
 * This method is called when an edit box loses focus.
 * The virtual keyboard is hidden.
 * Platform: Android and iOS.
 * @param editBox The edit box object that generated the event.
 */
void NoteScreen::editBoxEditingDidEnd(EditBox* editBox)
{
	if (mTitleEdit == editBox)
	{
		mLastModifiedDate->setText(getTimeString());
		mNote->setTitle(mTitleEdit->getText());
		// Store the new note title.
		mListener->noteSaved(mNote);
	}
	else if (mNoteEditBox == editBox)
	{
		mLastModifiedDate->setText(getTimeString());
		mNote->setContent(mNoteEditBox->getText());
		// Save content and last modified date.
		mListener->noteSaved(mNote);
	}
}

/**
 * This method is called when an options menu item is selected.
 * @param index The index on which the item is placed into the
 * OptionsMenu.
 * @param screen The screen that generated the event.
 */
void NoteScreen::optionsMenuItemSelected(Screen* screen, int index)
{
	// If remove was pressed, update current note index.
	if (mRemoveOptionsMenu == index)
	{
		mRemoveConfirmationDialog->show();
	}
}

/**
 * This method is called when the OptionsMenu is being closed
 * (either by the user canceling the menu with the back/menu
 * button, or when an item is selected.
 * @param screen The screen that generated the event.
 */
void NoteScreen::optionsMenuClosed(Screen* screen)
{

}
