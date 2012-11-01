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
 * @file MainScreen.h
 * @author Emma Tresanszki.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "NoteScreen.h"
#include "SettingsScreen.h"
#include "NoteListViewItem.h"
#include "../engine/NotesListener.h"
#include "../engine/NoteGroup.h"

using namespace NativeUI;

/**
 * The MainScreen of the application.
 * It contains the list of all note groups,
 * grouped by a category/title.
 * Top buttons for Settings, Arrange by List, or by History.
 * OptionsMenu for: Discard all.
 * Bottom button for adding New note.
 */
class MainScreen:
	public StackScreen,
	public NotesListener,
	public ListViewListener,
	public StackScreenListener,
	public ScreenListener,
	public ButtonListener//ImageButtonListener
{

public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	virtual ~MainScreen();

	/**
	 * Load all the notes.
	 */
	void populateNotesList(MAUtil::Vector<NoteGroup*>* notes);

	// from NotesListener
	virtual void noteSaved(NoteGroup* note);
	virtual void noteRemoved();
	virtual MAUtil::Vector<NoteGroup*>* getNotes();
	virtual void allNotesRemoved();
	virtual void settingsUpdated() {};
	virtual NoteGroup& getNote(int index);

	/**
	 * Check if forward navigation is possible.
	 */
	virtual bool hasNextNote();

	/**
	 * Check if backwards navigation is possible.
	 */
	virtual bool hasPreviousNote();
private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Create and add the header section of the screen.
	 */
	void addScreenHeader();

	/**
	 * Create a note entry based on note information.
	 * @param title
	 * @param dateModified
	 * @param noItems
	 * @return the item.
	 */
	NoteListViewItem* createNoteEntryUI(
			const MAUtil::String title,
			const MAUtil::String dateCreated,
			const MAUtil::String dateModified,
			const MAUtil::String noItems);

	/**
	 * Create a note entry based on a note object.
	 * @param note The NoteGroup object.
	 * @return the item.
	 */
	NoteListViewItem* createNoteEntryUI(NoteGroup* note);

	/**
	 * Update a note entry in the main list.
	 * @param note The modified note object.
	 */
	void updateListViewItem(NoteGroup& note);

	/**
	 * Add Today's section if does not exist already.
	 */
	void createTodaySection();

	/**
	 * Add last week section if does not exist already.
	 */
	void createLastWeekSection();

	/**
	 * Add last month section if does not exist already.
	 */
	void createLastMonthSection();

	/**
	 * Add older section if does not exist already.
	 */
	void createOlderSection();

	/**
	 * Clear the notes before the list is populated
	 * and sorted.
	 */
	void clearList();

	/**
	 * Sort notes based on history.
	 */
	void sortNotes();

	/**
	 * Load all the notes back to List mode.
	 */
	void loadNotes();

	/**
	 * Create confirmation dialog for removing all the notes.
	 */
	void createDeleteDialog();

	/**
	 * Update the count label according to the list.
	 */
	void updateNotesCount();

    /**
     * Apply the selected settings to the notes list.
     */
    void applySettingsOnList();

    /**
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);

	/**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param index The index on which the list view item is positioned.
	 */
	virtual void listViewItemClicked(
		ListView* listView,
		int index);

	/**
	 * This method is called when a segmented/alphabetical list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param listViewSection The ListViewSection object that contains the selected item.
	 * @param listViewItem The ListViewItem object clicked.
	 */
	virtual void segmentedListViewItemClicked(
		ListView* listView,
		ListViewSection* listViewSection,
		ListViewItem* listViewItem);

    /**
     * This method is called when a screen has been popped from a stack
     * screen.
     * @param stackScreen The stack screen object that generated the event.
     * @param fromScreen The screen that was popped from the stack screen.
     * @param toScreen The screen that will be shown.
     */
    virtual void stackScreenScreenPopped(
        StackScreen* stackScreen,
        Screen* fromScreen,
        Screen* toScreen);

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
private:
    /**
     * The main screen.
     */
	Screen* mScreen;

	SettingsScreen* mSettingsScreen;
	NoteScreen* mNoteScreen;

	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * Optional label for counting notes.
	 */
	Label* mCountLabel;

	/**
	 * List for preview of all note groups.
	 */
	ListView* mListView;

	/**
	 * List sections, when the notes are sorted by history.
	 */
	ListViewSection* mTodaySection;
	ListViewSection* mLastWeekSection;
	ListViewSection* mLastMonthSection;
	ListViewSection* mOlderSection;

	ImageButton* mAddNoteBtn;
	ImageButton* mSettingsBtn;

	/**
	 * Buttons for list sort:
	 *  - default sort by date created.
	 *  - history sort with sections.
	 */
	Button* mArrangeListButton;
	Button* mArrangeHistoryButton;
	/**
	 * The current display type:
	 * NOTE_LIST_VIEW_ITEM_DEFAULT or NOTE_LIST_VIEW_ITEM_HISTORY
	 */
	NoteListItemType mDisplayType;

	/**
	 * Warning dialog before notes removal.
	 */
	Dialog* mRemoveDialog;
	Dialog* mRemoveAllDialog;

	MAUtil::Vector<NoteListViewItem*> mNotesItems;
	//todo delete this.
	MAUtil::Vector<ImageButton*> mRemoveButtons;

	/**
	 * The current note index.
	 */
	int mNoteIndex;

	/**
	 * The notes objects.
	 */
	MAUtil::Vector<NoteGroup*>* mNotes;

	/**
	 * Controls for Delete all confirmation dialog.
	 */
	Dialog* mDeleteAllDialog;
	Button* mYesBtn;
	Button* mNoBtn;
	int mDeleteAllOptionsMenu;
};


#endif /* MAINSCREEN_H_ */
