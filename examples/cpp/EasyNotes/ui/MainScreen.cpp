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
 * @file MainScreen.cpp
 * @author Emma Tresanszki.
 */

#include "MainScreen.h"
#include "../Util.h"
#include "../engine/GeneralSettings.h"
#include <MAFS/File.h>

#include "MAHeaders.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	StackScreen(),
	mMainLayout(NULL),
	mListView(NULL),
	mAddNoteBtn(NULL),
	mSettingsBtn(NULL)
{
	createMainLayout();
	mArrangeListButton->addButtonListener(this);
	mArrangeHistoryButton->addButtonListener(this);
	mAddNoteBtn->addButtonListener(this);
	mSettingsBtn->addButtonListener(this);
	mListView->addListViewListener(this);
	addStackScreenListener(this);

	/**
	 * Create the confirmation dialog UI.
	 */
	createDeleteDialog();
	mYesBtn->addButtonListener(this);
	mNoBtn->addButtonListener(this);

	// Create the other screens of the application.
	mNoteScreen = new NoteScreen();
	mNoteScreen->addNotesListener(this);
	mNoteScreen->getBackBtn()->addButtonListener(this);
	mNoteScreen->getPreviousBtn()->addButtonListener(this);
	mNoteScreen->getNextBtn()->addButtonListener(this);

	mSettingsScreen = new SettingsScreen();
	mSettingsScreen->getBackBtn()->addButtonListener(this);

	// Workaround: Set this to false, so that when the VK from notes
	// screen received a return key, it does not pop the stack.
//	this->enableBackButton(false);
	mNoteIndex = -1;
	mDisplayType = NOTE_LIST_VIEW_ITEM_DEFAULT;
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mNotes->clear();
	mNotesItems.clear();

	mScreen->removeScreenListener(this);
	mArrangeListButton->removeButtonListener(this);
	mArrangeHistoryButton->removeButtonListener(this);
	mAddNoteBtn->removeButtonListener(NULL);
	mSettingsBtn->removeButtonListener(NULL);
    mListView->removeListViewListener(this);

    removeStackScreenListener(this);
    removeScreenListener(this);

    mYesBtn->removeButtonListener(this);
    mNoBtn->removeButtonListener(this);
//    for(int i=0; i < mNotesItems.size(); i++)
//    {
//    	mNotesItems[i]->getRemoveBtn()->removeButtonListener(this);
//    }

    mNoteScreen->getBackBtn()->removeButtonListener(this);
    mNoteScreen->getPreviousBtn()->removeButtonListener(this);
    mNoteScreen->getNextBtn()->removeButtonListener(this);

    delete mNoteScreen;
    delete mSettingsScreen;
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create the top screen in the stack.
	mScreen = new Screen();

	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(SLATE_GRAY);
	mScreen->setMainWidget(mMainLayout);

	addScreenHeader();

	// Create the buttons layout for sorting the notes.
	HorizontalLayout* buttonsLayout = new HorizontalLayout();
	buttonsLayout->wrapContentVertically();
	buttonsLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	buttonsLayout->setBackgroundColor(QUARTZ);
	mArrangeListButton = new Button();
	mArrangeListButton->setBackgroundColor(QUARTZ);
	mArrangeListButton->setText("LIST");
	mArrangeListButton->setFontSize(
			GeneralSettings::getInstance()->getFontSize()/2);
	mArrangeListButton->setFontColor(RED);
	mArrangeHistoryButton = new Button();
	mArrangeHistoryButton->setBackgroundColor(QUARTZ);
	mArrangeHistoryButton->setText("HISTORY");
	mArrangeHistoryButton->setFontSize(
			GeneralSettings::getInstance()->getFontSize()/2);
	mArrangeHistoryButton->setFontColor(BLACK);
	buttonsLayout->addChild(mArrangeListButton);
	buttonsLayout->addChild(mArrangeHistoryButton);
	mMainLayout->addChild(buttonsLayout);

	// Add the label for notes counting.
	VerticalLayout* countLayout = new VerticalLayout();
	countLayout->setBackgroundColor(SLATE_GRAY);
	countLayout->wrapContentVertically();
	countLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	mCountLabel = new Label();
	updateNotesCount();
	mCountLabel->setVisible(
			GeneralSettings::getInstance()->isDisplayCountEnabled());
	mCountLabel->setFontColor(QUARTZ);
	countLayout->addChild(mCountLabel);
	mMainLayout->addChild(countLayout);

	// Create list with default font settings.
	mListView = new ListView();
	mListView->fillSpaceVertically();
//	mListView->setHeight(600);
	mMainLayout->addChild(mListView);

	mAddNoteBtn = new ImageButton();
	mAddNoteBtn->setImage(RES_ADD_BTN);
	//mAddNoteBtn->setWidth(200); //mScreenWidth- 50; on center.
	mAddNoteBtn->fillSpaceHorizontally();
	mMainLayout->addChild(mAddNoteBtn);

	// Create the options menu.
	mScreen->addScreenListener(this);
	mDeleteAllOptionsMenu = mScreen->addOptionsMenuItem(
			"Delete All", MAW_OPTIONS_MENU_ICON_CONSTANT_DELETE, false);

	this->push(mScreen);
}

/**
 * Create the header section of the screen.
 */
void MainScreen::addScreenHeader()
{
	//Create a small header section for app icon and buttons.
	HorizontalLayout* headerLayout = new HorizontalLayout();
	headerLayout->setHeight(HEADER_HEIGHT);
	headerLayout->setBackgroundColor(WHITE);

	HorizontalLayout* leftLayout = new HorizontalLayout();
	leftLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	Image* headerIcon = new Image();
	headerIcon->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
	headerIcon->setImage(RES_APP_LOGO);
	leftLayout->addChild(headerIcon);

	HorizontalLayout* rightLayout = new HorizontalLayout();
	rightLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	mSettingsBtn = new ImageButton();
	mSettingsBtn->setImage(RES_SETTINGS_BTN);
	rightLayout->addChild(mSettingsBtn);

	headerLayout->addChild(leftLayout);
	headerLayout->addChild(rightLayout);
	mMainLayout->addChild(headerLayout);

//	VerticalLayout* lineLayout = new VerticalLayout();
//	lineLayout->setBackgroundColor(QUARTZ);
//	lineLayout->setHeight(LINE_HEIGHT);
//	mMainLayout->addChild(lineLayout);
}

/**
 * Update the count label according to the list.
 */
void MainScreen::updateNotesCount()
{
	if (mNotes->size() == 0)
	{
		mCountLabel->setText("No notes");
	}
	else if(mNotes->size() == 1)
	{
		mCountLabel->setText(
				MAUtil::integerToString(mNotes->size()) + " note");
	}
	else
	{
		mCountLabel->setText(
				MAUtil::integerToString(mNotes->size()) + " notes");
	}
}

/**
 * Create a note entry based on note information.
 * @param title
 * @param dateModified
 * @param noItems
 * @return the item.
 */
NoteListViewItem* MainScreen::createNoteEntryUI(
		const MAUtil::String title,
		const MAUtil::String dateCreated,
		const MAUtil::String dateModified,
		const MAUtil::String noItems)
{
	NoteListViewItem* item = new NoteListViewItem(
			title, dateCreated, dateModified, 3);
	mNotesItems.add(item);
	//item->getRemoveBtn()->addButtonListener(this);

	return item;
}

/**
 * Create a note entry based on a note object.
 * @param note The NoteGroup object.
 * @return the item.
 */
NoteListViewItem* MainScreen::createNoteEntryUI(NoteGroup* note)
{
	NoteListViewItem* item = new NoteListViewItem(
			note->getTitle(),
			note->getDateCreated(),
			note->getDateModified(),
			mNotesItems.size());
	item->updateType(mDisplayType);
	mNotesItems.add(item);

	return item;
}

/**
 * Load all the notes.
 */
void MainScreen::populateNotesList(MAUtil::Vector<NoteGroup*>* notes)
{
//	clearList();

	mNotes = new MAUtil::Vector<NoteGroup*> ();
	// By default display the notes based on date created.
	for (int i=0 ; i < notes->size(); i++)
	{
		mNotes->add((*notes)[i]);
		mListView->addChild(
				createNoteEntryUI((*mNotes)[i]) );
//		(*mNotes)[i]->setDisplayIndex(i);
	}
}

/**
 * Clear the notes before the list is populated
 * and sorted.
 */
void MainScreen::clearList()
{
//	if (mNotesItems.size() > 0)
//	{
		for (int i=0 ; i < mNotesItems.size(); i++)
		{
			mListView->removeChild(mNotesItems[i]);
		}
		mNotesItems.clear();
//	}
}

/**
 * Load all the notes back to List mode.
 */
void MainScreen::loadNotes()
{
	if (mListView)
	{/*
		// Destroy all the sections, their number can vary.
		if (mTodaySection != NULL)
		{
//			mListView->removeChild(mTodaySection);
			delete mTodaySection;
			mTodaySection = NULL;
		}
		if (mLastWeekSection)
		{
//			mListView->removeChild(mLastWeekSection);
			delete mLastWeekSection;
			mLastWeekSection = NULL;
		}
		if (mLastMonthSection)
		{
//			mListView->removeChild()(mLastMonthSection);
			delete mLastMonthSection;
			mLastMonthSection = NULL;
		}
		if (mOlderSection)
		{
//			mListView->removeChild(mOlderSection);
			delete mOlderSection;
			mOlderSection = NULL;
		}*/
		delete mListView;
		mListView = NULL;
	}

	// Create list with default font settings.
	mListView = new ListView();
	mListView->fillSpaceVertically();
//	mListView->setHeight(600);
	mListView->addListViewListener(this);

	// By default display the notes based on date created.
	for (int i=0 ; i < mNotesItems.size(); i++)
	{
		mNotesItems[i]->updateType(NOTE_LIST_VIEW_ITEM_DEFAULT);
		mListView->addChild(mNotesItems[i]);
	}
	mMainLayout->insertChild(mListView, 3);
}

/**
 * Sort notes based on history.
 */
void MainScreen::sortNotes()
{
	struct tm * now = new tm;
	split_time(maTime(), now);

	MAUtil::Vector<tm*> mDates;

	if (mListView)
	{
		for (int i=0; i < mNotesItems.size(); i++)
			mListView->removeChild(mNotesItems[i]);
		delete mListView;
		mListView = NULL;
	}

	mListView = new ListView(LIST_VIEW_TYPE_SEGMENTED); //remember to recreate it when filter is removed!!
	mListView->addListViewListener(this);
	mListView->fillSpaceVertically();

	// Create section for today.
	mTodaySection = new ListViewSection(LIST_VIEW_SECTION_TYPE_SEGMENTED);
	// Title is only for WP7.
	mTodaySection->setTitle("Today");
	mTodaySection->setHeaderText("Today");
	mTodaySection->setHeaderFontColor(BLUE);
//	mTodaySection->setFooterText("");
	mTodaySection->setHeaderFontSize(
			GeneralSettings::getInstance()->getFontSize()-10);
	mTodaySection->setHeaderBackgroundColor(QUARTZ);

//	mListView->addChild(mTodaySection);
//	mListView->addChild(mLastWeekSection);
//	mListView->addChild(mLastMonthSection);
//	mListView->addChild(mOlderSection);

	for (int i=0; i < mNotes->size(); i++)
	{
		tm* modified =  (*mNotes)[i]->getDateModifiedStruct();
		mDates.add(modified);

//		ListViewItem* item = new ListViewItem();
		mNotesItems[i]->updateType(NOTE_LIST_VIEW_ITEM_HISTORY);
		//		mNotesItems[i]->updatePreview();
		int contentLength  = (*mNotes)[i]->getContent().size();
		if (contentLength > PREVIEW_LENGTH )
			mNotesItems[i]->
				updatePreview((*mNotes)[i]->getContent().substr(0, PREVIEW_LENGTH) + " ... ");
		else
			mNotesItems[i]->
				updatePreview((*mNotes)[i]->getContent() + " ... ");

//		NoteListViewItem* item = mNotesItems[i];
//		item->updateType(NOTE_LIST_VIEW_ITEM_HISTORY);
//		item->setText((*mNotes)[i]->getTitle());
//		item->setFontColor(GeneralSettings::getInstance()->getFontColor());
//		item->setFontSize(GeneralSettings::getInstance()->getFontSize());
		if ( i < 2 )
		{
			mTodaySection->addItem(mNotesItems[i]);
		}
		else
		{
			if (mLastWeekSection == NULL)
			{
				// Create section for last week.
				mLastWeekSection = new ListViewSection(
						LIST_VIEW_SECTION_TYPE_SEGMENTED);
				mLastWeekSection->setHeaderText("Older");
//				mLastWeekSection->setFooterText("old");
				mLastWeekSection->setHeaderFontColor(BLUE);
				mLastWeekSection->setHeaderFontSize(
						GeneralSettings::getInstance()->getFontSize()-10);
				mLastWeekSection->setHeaderBackgroundColor(QUARTZ);
			}
			mLastWeekSection->addItem(mNotesItems[i]);
			// Do not copy today's items to this week's items.
		}


//		else if( isSameWeek(*modified,*now) )
//		{
//			if (!mLastWeekSection)
//			{
//				// Create section for last week.
//				mLastWeekSection = new ListViewSection(
//						LIST_VIEW_SECTION_TYPE_SEGMENTED);
//			}
//			ListViewItem* item = new ListViewItem();
//			item->setText((*mNotes)[i]->getTitle());
//			mLastWeekSection->addItem(item);
//		}
//		else if( isSameMonth(*modified,*now) )
//		{
//			if(!mLastMonthSection)
//			{
//				// Create section for last month.
//				mLastMonthSection = new ListViewSection(
//						LIST_VIEW_SECTION_TYPE_SEGMENTED);
//			}
//			ListViewItem* item = new ListViewItem();
//			item->setText((*mNotes)[i]->getTitle());
//			mLastMonthSection->addItem(item);
//		}
//		else
//		{
//			if(!mOlderSection)
//			{
//				// Create older section.
//				mOlderSection = new ListViewSection(
//						LIST_VIEW_SECTION_TYPE_SEGMENTED);
//			}
//			// If not in filter, add to Older section.
//			ListViewItem* item = new ListViewItem();
//			item->setText((*mNotes)[i]->getTitle());
//			mOlderSection->addItem(item);
//		}
	}

	// Add all the sections to the list.
	mListView->addChild(mTodaySection);
	mTodaySection->setFooterText("");

	if (mLastWeekSection != NULL)
	{
		mListView->addChild(mLastWeekSection);
	}
	if(mLastMonthSection != NULL)
	{
		mListView->addChild(mLastMonthSection);
	}
	if(mOlderSection != NULL)
	{
		mListView->addChild(mOlderSection);
	}
	mMainLayout->insertChild(mListView, 3);
//	mListView->reloadData();
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void MainScreen::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param index The index on which the list view item is positioned.
 */
void MainScreen::listViewItemClicked(
	ListView* listView,
	int index)
{
	// TODO change background color on selected, and then reset it.
	if ( listView == mListView )
	{
		// Store the current note index.
		mNoteIndex = index;

		// Open the corresponding note.
		mNoteScreen->loadNoteGroup((*mNotes)[mNoteIndex]);
		this->push(mNoteScreen);
	}
}

/**
 * This method is called when a segmented/alphabetical list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewSection The ListViewSection object that contains the selected item.
 * @param listViewItem The ListViewItem object clicked.
 */
void MainScreen::segmentedListViewItemClicked(
	ListView* listView,
	ListViewSection* listViewSection,
	ListViewItem* listViewItem)
{
	if (listView == mListView)
	{
		NoteListViewItem* it = (NoteListViewItem*) listViewItem;
		if ( it->getDisplayIndex() >= 0 )
		{
			mNoteIndex = it->getDisplayIndex();
			// Open the corresponding note.
			mNoteScreen->loadNoteGroup((*mNotes)[mNoteIndex]);
			this->push(mNoteScreen);
		}
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mAddNoteBtn)
    {
		// Set the selected image on the button.
		mAddNoteBtn->setImage(RES_ADD_BTN_SELECTED);

		// Set empty fields.
		mNoteScreen->emptyUIFields();
		NoteGroup* note = new NoteGroup();
		mNotes->add(note);
		mNoteScreen->loadNoteGroup(note);
		this->push(mNoteScreen);
		mAddNoteBtn->setImage(RES_ADD_BTN);

		mNoteIndex = mNotes->size()-1;
		/**
		* Add a list item to the list, or to the respective
		* section if the history display type is set.
		*/
		NoteListViewItem* item = createNoteEntryUI(note);
		if (mDisplayType == NOTE_LIST_VIEW_ITEM_DEFAULT)
		{
			mListView->addChild(item);
		}
		else
		{
			createTodaySection();
			mTodaySection->addItem(item);// maybe here, because the item is added after the section is added.
		}
    }
    else if (button == mSettingsBtn)
    {
		mSettingsBtn->setImage(RES_SETTINGS_BTN_SELECTED);
		// Push the settings screen.
		this->push(mSettingsScreen);
		mSettingsScreen->requestListFocus();
		mSettingsBtn->setImage(RES_SETTINGS_BTN);
    }
    else if(button == mNoteScreen->getBackBtn())
    {
		if ( mNoteIndex >= 0 )
		{
			updateListViewItem(* (*mNotes)[mNoteIndex]);
		}

		// Reload notes list.
		updateNotesCount();
		this->pop();
    }
    else if(button == mNoteScreen->getPreviousBtn())
    {
		mNoteIndex --;
		mNoteScreen->loadNoteGroup((*mNotes)[mNoteIndex]);
    }
    else if(button == mNoteScreen->getNextBtn())
    {
		mNoteIndex++;
		mNoteScreen->loadNoteGroup((*mNotes)[mNoteIndex]);
    }
    else if(button == mSettingsScreen->getBackBtn())
    {
		this->pop();
    }
    else if(mYesBtn == button)
    {
		// Confirmation received, remove all notes.
		allNotesRemoved();
		mDeleteAllDialog->hide();
    }
    else if(mNoBtn == button)
    {
		// Cancel removal of all notes.
		mDeleteAllDialog->hide();
    }
    else if(button == mArrangeListButton)
    {
		mDisplayType = NOTE_LIST_VIEW_ITEM_DEFAULT;
		mArrangeListButton->setFontColor(RED);
		mArrangeHistoryButton->setFontColor(BLACK);
		loadNotes();
    }
    else if(button == mArrangeHistoryButton)
    {
		mDisplayType = NOTE_LIST_VIEW_ITEM_HISTORY;
		// Rearrange all notes based on history.
		mArrangeHistoryButton->setFontColor(RED);
		mArrangeListButton->setFontColor(BLACK);
		sortNotes();
    }
    else
    {
		// This part will be removed after RemoveButtons will be ditched.
		for(int i=0; i < mNotesItems.size(); i++)
		{
			if (mNotesItems[i]->getRemoveBtn() == button)
			{
				// Remove the item on position i.
				mListView->removeChild(mNotesItems[i]);
				mNotesItems.remove(i);
				// Update the repository.
				//Listener->noteRemoved(i);
				mNoteIndex = -1;
				break;
			}
		}
    }
}

/**
 * Update a note entry in the main list.
 * @param note The modified note object.
 */
void MainScreen::updateListViewItem(NoteGroup& note)
{
	mNotesItems[mNoteIndex]->updateTitle(note.getTitle());
//	mNotesItems[mNoteIndex]->updateDateCreated(note.getDateCreated());
	int contentLength  = note.getContent().size();
	if (contentLength > PREVIEW_LENGTH )
		mNotesItems[mNoteIndex]->
			updatePreview(note.getContent().substr(0, PREVIEW_LENGTH) + " ... ");
	else
		mNotesItems[mNoteIndex]->
			updatePreview(note.getContent() + " ... ");
}

/**
 * This method is called when a screen has been popped from a stack
 * screen.
 * @param stackScreen The stack screen object that generated the event.
 * @param fromScreen The screen that was popped from the stack screen.
 * @param toScreen The screen that will be shown.
 */
void MainScreen::stackScreenScreenPopped(
    StackScreen* stackScreen,
    Screen* fromScreen,
    Screen* toScreen)
{
	// When returning from settings, reload the items.
	if ( fromScreen == mSettingsScreen )
	{
		// Refresh the notes list with the new settings.
		applySettingsOnList();

		// Workaround for an ICS bug, so the list does not become unresponsive.
		mListView->setProperty("requestFocus","");

		// Apply the settings on the Note Screen.
		mNoteScreen->applyAppSettings();
	}
	else if( fromScreen == mNoteScreen )
	{
		// Workaround for an ICS bug, so the list does not become unresponsive.
		mListView->setProperty("requestFocus","");
		updateNotesCount();
	}
}

/**
 * Create confirmation dialog for removing all the notes.
 */
void MainScreen::createDeleteDialog()
{
	mDeleteAllDialog = new Dialog();
	mDeleteAllDialog->setTitle("Warning");
	VerticalLayout* dialogLayout = new VerticalLayout();
	Label* info = new Label();
	info->setText("   Are you sure you want to remove all your notes?   ");
	HorizontalLayout* buttons = new HorizontalLayout();
	mYesBtn = new Button();
	mYesBtn->setText(" Yes ");
	mYesBtn->fillSpaceHorizontally();
	mYesBtn->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mNoBtn = new Button();
	mNoBtn->fillSpaceHorizontally();
	mNoBtn->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mNoBtn->setText(" No ");
	buttons->addChild(mYesBtn);
	buttons->addChild(mNoBtn);

	dialogLayout->addChild(info);
	dialogLayout->addChild(buttons);
	mDeleteAllDialog->setMainWidget(dialogLayout);
}

/**
 * This method is called when an options menu item is selected.
 * @param index The index on which the item is placed into the
 * OptionsMenu.
 * @param screen The screen that generated the event.
 */
void MainScreen::optionsMenuItemSelected(Screen* screen, int index)
{
	if (index == mDeleteAllOptionsMenu&& mScreen == screen)
	{
		mDeleteAllDialog->show();
	}
}

/**
 * This method is called when the OptionsMenu is being closed
 * (either by the user canceling the menu with the back/menu
 * button, or when an item is selected.
 * @param screen The screen that generated the event.
 */
void MainScreen::optionsMenuClosed(Screen* screen)
{
}

/**
 * Apply the selected settings to the notes list.
 */
void MainScreen::applySettingsOnList()
{
	for (int i=0; i < mNotesItems.size(); i++)
	{
		mNotesItems[i]->applyFontSize(
				GeneralSettings::getInstance()->getFontSize());
		mNotesItems[i]->applyFontColor(
				GeneralSettings::getInstance()->getFontColor());
//		mNotesItems[i]->enableDisplayDateModified(
//				GeneralSettings::getInstance()->isDisplayDateModifiedEnabled());
		mNotesItems[i]->enableDisplayDateCreated(
				GeneralSettings::getInstance()->isDisplayDateCreatedEnabled());
	}
	mCountLabel->setVisible(
			GeneralSettings::getInstance()->isDisplayCountEnabled());
	updateNotesCount();
}

NoteGroup& MainScreen::getNote(int index)
{
	if (index < mNotes->size())
	{
		return (*(*mNotes)[index]);
	}
//	return NULL;
}

/**
 * Check if backwards navigation is possible.
 */
bool MainScreen::hasNextNote()
{
	if ( mNoteIndex < mNotes->size()-1 )
	{
		return true;
	}
	return false;
}

/**
 * Check if forward navigation is possible.
 */
bool MainScreen::hasPreviousNote()
{
	if ( mNoteIndex > 0 )
	{
		return true;
	}
	return false;
}
/*
int MainScreen::addNote()
{
	// Set empty fields.
	NoteGroup* newNote = new NoteGroup();
	mNotes->add(newNote);
	mNoteIndex = mNotes->size()-1;
	NoteListViewItem* item = createNoteEntryUI(newNote);

	// If items are arranged by history, add the new item to the today's section.
	if(mDisplayType == NOTE_LIST_VIEW_ITEM_HISTORY)
	{
		createTodaySection();
		mTodaySection->addItem(item);
	}
	else
	{
		mListView->addChild(item);
	}

	mNoteScreen->loadNoteGroup(newNote);
}*/

/**
 * Add Today's section if does not exist already.
 */
void MainScreen::createTodaySection()
{
	if (mTodaySection == NULL)
	{
		// Create section for today.
		mTodaySection = new ListViewSection(
				LIST_VIEW_SECTION_TYPE_SEGMENTED);
		mTodaySection->setHeaderText("Today");
//				mTodaySection->setFooterText("");
		mTodaySection->setHeaderFontColor(BLUE);
		mTodaySection->setHeaderFontSize(
				GeneralSettings::getInstance()->getFontSize()-10);
		mTodaySection->setHeaderBackgroundColor(QUARTZ);
		mListView->addChild(mTodaySection);
	}
}

/**
 * Add last week section if does not exist already.
 */
void MainScreen::createLastWeekSection()
{
	if (mLastWeekSection == NULL)
	{
		// Create section for last week.
		mLastWeekSection = new ListViewSection(
				LIST_VIEW_SECTION_TYPE_SEGMENTED);
		mLastWeekSection->setHeaderText("Last week");
		mLastWeekSection->setHeaderFontColor(BLUE);
		mLastWeekSection->setHeaderFontSize(
				GeneralSettings::getInstance()->getFontSize()-10);
		mLastWeekSection->setHeaderBackgroundColor(QUARTZ);
	}
}

/**
 * Add last month section if does not exist already.
 */
void MainScreen::createLastMonthSection()
{
	if (mLastMonthSection == NULL)
	{
		// Create section for last month.
		mLastMonthSection = new ListViewSection(
				LIST_VIEW_SECTION_TYPE_SEGMENTED);
		mLastMonthSection->setHeaderText("Last month");
		mLastMonthSection->setHeaderFontColor(BLUE);
		mLastMonthSection->setHeaderFontSize(
				GeneralSettings::getInstance()->getFontSize()-10);
		mLastMonthSection->setHeaderBackgroundColor(QUARTZ);
	}
}

/**
 * Add older section if does not exist already.
 */
void MainScreen::createOlderSection()
{
	if (mOlderSection == NULL)
	{
		// Create section for notes older than last month.
		mOlderSection = new ListViewSection(
				LIST_VIEW_SECTION_TYPE_SEGMENTED);
		mOlderSection->setHeaderText("Older");
		mOlderSection->setHeaderFontColor(BLUE);
		mOlderSection->setHeaderFontSize(
				GeneralSettings::getInstance()->getFontSize()-10);
		mOlderSection->setHeaderBackgroundColor(QUARTZ);
	}
}

void MainScreen::noteSaved(NoteGroup* note)
{
	(*mNotes)[mNoteIndex] = note;
	(*mNotes)[mNoteIndex] ->setDateModified(getTimeString());
	updateListViewItem(*note);
}

//TODO remove the index
void MainScreen::noteRemoved()
{
	// If note was removed from NotesScreen, display previous
	// note if available, or go back to MainScreen.
	mNotes->remove(mNoteIndex);
	mListView->removeChild(mNotesItems[mNoteIndex]);
	mNotesItems.remove(mNoteIndex);
	if ( mNotes->size() > 0 )
	{
		mNoteIndex = mNotes->size()-1;
	}
	updateNotesCount();

	this->pop();
}

MAUtil::Vector<NoteGroup*>* MainScreen::getNotes()
{
	return mNotes;
}

void MainScreen::allNotesRemoved()
{
	if (mDisplayType == NOTE_LIST_VIEW_ITEM_DEFAULT)
	{
		for (int i=0; i < mNotes->size(); i++)
		{
			mListView->removeChild(mNotesItems[i]);
		}
	}
	else
	{
		// For the History display type, remove all section items and headers.
		if (mTodaySection != NULL)
			mListView->removeChild(mTodaySection);
		if(mLastWeekSection != NULL)
			mListView->removeChild(mLastWeekSection);
		if(mLastMonthSection != NULL)
			mListView->removeChild(mLastMonthSection);
		if(mOlderSection != NULL)
			mListView->removeChild(mOlderSection);
	}

	mNotesItems.clear();
	mNotes->clear();
	updateNotesCount();
}
