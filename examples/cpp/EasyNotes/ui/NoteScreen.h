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
 * @file NoteScreen.h
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

#ifndef NOTESCREEN_H_
#define NOTESCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "../engine/NotesListener.h"

using namespace NativeUI;

/**
 * The NoteScreen class.
 */
class NoteScreen:
	public Screen,
	public ScreenListener,
	public EditBoxListener,
	public ButtonListener
{

public:
	/**
	 * Constructor.
	 */
	NoteScreen();

	/**
	 * Destructor.
	 */
	virtual ~NoteScreen();

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Create the remove confirmation dialog.
	 */
	void createConfirmationDialog();

	/**
	 * Create the header area.
	 */
	void createScreenHeader();

	/**
	 * Empty the UI fields.
	 */
	void emptyUIFields();

	/**
	 * Apply application settings to the UI controls.
	 */
	void applyAppSettings();

	Button* getBackBtn(){ return mBackButton; };
	ImageButton* getNextBtn(){ return mNextBtn; };
	ImageButton* getPreviousBtn(){ return mPreviousBtn; };

	/**
	 * Add a notes listener.
	 * @param listener
	 */
	void addNotesListener(NotesListener* listener) { mListener = listener; };

	/**
	 * Clear the current note object's info.
	 */
	void clearNoteObj();

	/**
	 * Load the note group and update UI.
	 */
	void loadNoteGroup(NoteGroup* note);

	/**
	 * Get the current note object.
	 * @return The object.
	 */
	NoteGroup* getNoteObj() { return mNote; };

	/**
	 * Check if next and previous navigation is possible for the
	 * current note.
	 */
	void checkNavigation();
private:

    /**
     * This method is called when an options menu item is selected.
     * @param index The index on which the item is placed into the
     * OptionsMenu.
     * @param screen The screen that generated the event.
     */
    virtual void optionsMenuItemSelected(Screen* screen, int index);

    /**
     * This method is called when the OptionsMenu is being closed
     * (either by the user canceling the menu with the back/menu
     * button, or when an item is selected.
     * @param screen The screen that generated the event.
     */
    virtual void optionsMenuClosed(Screen* screen);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);
    /**
     * This method is called when an edit box loses focus.
     * The virtual keyboard is hidden.
     * Platform: Android and iOS.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidEnd(EditBox* editBox);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
	Button* mBackButton;
	Label* mLastModifiedDate;
	Label* mCreatedDate;

	EditBox* mTitleEdit;
	EditBox* mNoteEditBox;

	/**
	 * Navigation buttons.
	 */
	ImageButton* mNextBtn;
	ImageButton* mPreviousBtn;

	/**
	 * Listener that notifies repository when changes are made.
	 */
	NotesListener* mListener;

	/**
	 * The current note object.
	 */
	NoteGroup* mNote;

	/**
	 * Remove options menu for deleting the current note.
	 */
	int mRemoveOptionsMenu;

	/**
	 * The remove note confirmation dialog.
	 */
	Dialog* mRemoveConfirmationDialog;
	Button* mYesBtn;
	Button* mNoBtn;
};

#endif /* NOTESCREEN_H_ */
