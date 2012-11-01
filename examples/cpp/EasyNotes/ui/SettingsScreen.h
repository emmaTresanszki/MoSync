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
 * @file SettingsScreen.h
 * @author Emma Tresanszki.
 */

#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "../engine/NotesListener.h"

using namespace NativeUI;

/**
 * The settings Screen of the application.
 * OptionsMenu: restore default settings.
 * All the settings are automatically applied.
 * Default values:
 * 	 - fontSize = FONT_MEDIUM_RES_HIGH/FONT_MEDIUM_RES_LOW.
 * 	 - fontColor = RED
 * 	 - displayDateModified = false
 * 	 - displayDateCreated = false.
 * 	 - displayCount = false.
 */
class SettingsScreen:
	public Screen,
	public ListViewListener,
	public ScreenListener,
	public CheckBoxListener,
	public ButtonListener
{

public:
	/**
	 * Constructor.
	 */
	SettingsScreen();

	/**
	 * Destructor.
	 */
	virtual ~SettingsScreen();

	/**
	 * Create and add the main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Create and add a header layout to the screen.
	 */
	void createHeaderLayout();

	/**
	 * Create the font color dialog.
	 * Grid layout with 9 possible colors.
	 * The selected font color is only applicable for the notes.
	 */
	void createColorSettingDialog();

	/**
	 * Create the font size dialog.
	 */
	void createSizeSelectionDialog();

	/**
	 * Create and add the font size list view item.
	 */
	void addFontSizeSetting();

	/**
	 * Create and add the font color list view item.
	 */
	void addFontColorSetting();

	/**
	 * Create and add the date modified setting item.
	 */
	void addDateModifiedSetting();

	/**
	 * Create and add the date created setting item.
	 */
	void addDateCreatedSetting();

	/**
	 * Create and add the notes count setting item.
	 */
	void addItemsNumberSetting();

	/**
	 * Create and add an about item.
	 */
	void addAboutSection();

	/**
	 * Create a colored image.
	 * @param color The color of the image (a hex value).
	 * @param width The width of the image.
	 * @param height The height of the image.
	 * @return Handle to the image. The image needs to be
	 * deallocated with maDestoryObject.
	 */
	MAHandle SettingsScreen::createColorImage(int color, int width, int height);

	/**
	 * Create a layout with 3 equal image buttons with the specified colors.
	 * @param
	 */
	HorizontalLayout* addColorLineLayout(int leftColor, int centerColor, int rightColor);
	ListView* addColorLineList(int leftColor, int centerColor, int rightColor);

	/**
	 * ICS workaround for the list focus bug.
	 */
	void requestListFocus();

	/**
	 * Load the default settings, when the app is first launched,
	 * or when Restore defaults is selected.
	 */
	void loadDefaultValues();

	/**
	 * Set the checkbox state only if the previous state is different.
	 * use this method so that checkBoxStateChanged events do not occur
	 * when it's not needed.
	 * @param check The checkbox.
	 * @param state The new state.
	 */
	void setCheckBoxState(CheckBox& check, bool state);

	/**
	 * Add the notes listener.
	 * @param listener
	 */
	void addNotesListener(NotesListener* listener) { mListener = listener;};
	Button* getBackBtn(){ return mBackBtn; };
private:
    /**
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);

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
     * This method is called when the state of the check box was changed
     * by the user.
     * @param checkBox The check box object that generated the event.
     * @param state True if the check box is checked, false otherwise.
     */
    virtual void checkBoxStateChanged(
        CheckBox* checkBox,
        bool state);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS, Android, Windows Phone.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when there is an touch-down event for
     * a button.
     * Platform: iOS and Android.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * List with application's settings.
	 */
	ListView* mSettingsList;

	/**
	 * Font size setting.
	 * Opens a dialog with 3 options, that acts
	 * like a picker widget, so that when an option
	 * is selected, the other ones are deselected.
	 */
	ListViewItem* mSizeSettingItem;
	Label* mSizeLabel;
	Dialog* mSizeDialog;
	ListView* mSizeDialogList;
	ListViewItem* mSmallItem;
	ListViewItem* mMediumItem;
	ListViewItem* mLargeItem;
	CheckBox* mSmallSizeCheckBox;
	CheckBox* mMediumSizeCheckBox;
	CheckBox* mLargeSizeCheckBox;
	/**
	 * Font color setting.
	 */
	ListViewItem* mColorSettingItem;
	Image* mColorSettingImage;
	Dialog* mColorDialog;
	Button* mCancelDialog;

	ListViewItem* mAboutItem;

	/**
	 * Check box for enabling/disabling notes count display.
	 */
	CheckBox* mCountCheckBox;

	/**
	 * Check box for enabling/disabling display of last modified date.
	 */
	CheckBox* mDateModifiedCheckBox;
	CheckBox* mDateCreatedCheckBox;

	/**
	 * 9 values for font color.
	 */
	MAUtil::Vector<ImageButton*> mColorButtons;
	MAUtil::Vector<HorizontalLayout*> mColorButtonLayouts;

	/**
	 * Listener that notifies repository when changes are made.
	 */
	NotesListener* mListener;

	Button* mBackBtn;

	int mRestoreDefaultsOptionsMenu;
};


#endif /* SETTINGSSCREEN_H_ */
