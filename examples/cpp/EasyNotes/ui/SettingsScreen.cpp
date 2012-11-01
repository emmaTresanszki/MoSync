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
 * @file SettingsScreen.cpp
 * @author Emma Tresanszki.
 */

#include "SettingsScreen.h"
#include "../engine/GeneralSettings.h"
#include "../Util.h"
#include <MAFS/File.h>

#include "MAHeaders.h"

/**
 * Constructor.
 */
SettingsScreen::SettingsScreen() :
	Screen(),
	mMainLayout(NULL)
{
	createMainLayout();

	createColorSettingDialog();

	createSizeSelectionDialog();

	mSettingsList->addListViewListener(this);
	//mSizeDialogList->addListViewListener(this);

	mDateCreatedCheckBox->addCheckBoxListener(this);
	mDateModifiedCheckBox->addCheckBoxListener(this);
	mCountCheckBox->addCheckBoxListener(this);

	mSmallSizeCheckBox->addCheckBoxListener(this);
	mMediumSizeCheckBox->addCheckBoxListener(this);
	mLargeSizeCheckBox->addCheckBoxListener(this);
	mCancelDialog->addButtonListener(this);

	// Load previous settings, if available.
	mDateModifiedCheckBox->setState(
			GeneralSettings::getInstance()->isDisplayDateModifiedEnabled());
	mDateCreatedCheckBox->setState(
			GeneralSettings::getInstance()->isDisplayDateCreatedEnabled());
	mCountCheckBox->setState(
			GeneralSettings::getInstance()->isDisplayCountEnabled());

//	addScreenListener(this);
//	mRestoreDefaultsOptionsMenu =
//			addOptionsMenuItem("Restore defaults", MAW_OPTIONS_MENU_ICON_CONSTANT_REVERT, false);
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	mSettingsList->removeListViewListener(this);
	//mSizeDialogList->removeListViewListener(this);
	mCountCheckBox->removeCheckBoxListener(this);
	mDateModifiedCheckBox->removeCheckBoxListener(this);
	mDateCreatedCheckBox->removeCheckBoxListener(this);

	mSmallSizeCheckBox->removeCheckBoxListener(this);
	mMediumSizeCheckBox->removeCheckBoxListener(this);
	mLargeSizeCheckBox->removeCheckBoxListener(this);
	mCancelDialog->removeButtonListener(this);

    removeScreenListener(this);

    for (int i=0; i < mColorButtons.size(); i++)
    {
		mColorButtons[i]->removeButtonListener(this);
    }
}

/**
 * Create and add the main layout to the screen.
 */
void SettingsScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	setMainWidget(mMainLayout);

	createHeaderLayout();

	mSettingsList = new ListView();
	mSettingsList->fillSpaceHorizontally();
	addFontSizeSetting();
	addFontColorSetting();
	addDateModifiedSetting();
	addDateCreatedSetting();
	addItemsNumberSetting();
	addAboutSection();
	mMainLayout->addChild(mSettingsList);

//	mBackBtn = new Button();
//	mBackBtn->setText("< Back");
//	mBackBtn->fillSpaceHorizontally();
//	mBackBtn->setFontColor(SLATE_GRAY);
//	mMainLayout->addChild(mBackBtn);
}

/**
 * Create and add a header layout to the screen.
 */
void SettingsScreen::createHeaderLayout()
{
	//Create a small header section for app icon and buttons.
	HorizontalLayout* headerLayout = new HorizontalLayout();
	headerLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	headerLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	headerLayout->setHeight(HEADER_HEIGHT);// screen height /5
	headerLayout->setBackgroundColor(WHITE);

	Image* headerIcon = new Image();
	headerIcon->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
	headerIcon->setImage(RES_APP_LOGO);

	Label* infoLabel = new Label();
	infoLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	infoLabel->setText(" Settings ");
	infoLabel->setFontColor(BLACK);

	headerLayout->addChild(headerIcon);
	headerLayout->addChild(infoLabel);
	mMainLayout->addChild(headerLayout);

	VerticalLayout* lineLayout = new VerticalLayout();
	lineLayout->setBackgroundColor(QUARTZ);
	lineLayout->setHeight(5);
	mMainLayout->addChild(lineLayout);
}

/**
 * Create the font color dialog.
 * Grid layout with 9 possible colors.
 * The selected font color is only applicable for the notes.
 */
void SettingsScreen::createColorSettingDialog()
{
	mColorDialog = new Dialog();
	VerticalLayout* dialogLayout = new VerticalLayout();
	dialogLayout->setBackgroundColor(WHITE);
	dialogLayout->addChild(addColorLineLayout(RED, GREEN, BLUE));
	dialogLayout->addChild(addColorLineLayout(PINK, ORANGE,YELLOW));
	dialogLayout->addChild(addColorLineLayout(PURPLE, GRAY, BLACK));
	mColorDialog->setMainWidget(dialogLayout);
}

/**
 * Create the font size dialog.
 */
void SettingsScreen::createSizeSelectionDialog()
{
	mSizeDialog = new Dialog();
	mSizeDialog->setTitle("Default font size");

	VerticalLayout* dialogMainLayout = new VerticalLayout();

	RelativeLayout* smallLayout = new RelativeLayout();
	smallLayout->wrapContentVertically();
	Label* smallItemLabel = new Label();
	smallItemLabel->setText(" Small ");
	mSmallSizeCheckBox = new CheckBox();
	smallLayout->addChild(smallItemLabel);
	smallItemLabel->setPosition(PADDING, PADDING*2);
	smallLayout->addChild(mSmallSizeCheckBox);
	mSmallSizeCheckBox->setPosition(getScreenWidth()/2, 0);//PADDING);
// TODO add break lines.
	RelativeLayout* mediumLayout = new RelativeLayout();
//	mediumLayout->setHeight()
	mediumLayout->wrapContentVertically();
	Label* mediumItemLabel = new Label();
	mediumItemLabel->setText(" Medium ");
	mMediumSizeCheckBox = new CheckBox();
	mediumLayout->addChild(mediumItemLabel);
	mediumItemLabel->setPosition(PADDING, PADDING*2);
	mediumLayout->addChild(mMediumSizeCheckBox);
	mMediumSizeCheckBox->setPosition(getScreenWidth()/2, 0);

	RelativeLayout* largeLayout = new RelativeLayout();
	largeLayout->wrapContentVertically();
	Label* largeItemLabel = new Label();
	largeItemLabel->setText(" Large ");
	mLargeSizeCheckBox = new CheckBox();
	largeLayout->addChild(largeItemLabel);
	largeItemLabel->setPosition(PADDING, PADDING*2);
	largeLayout->addChild(mLargeSizeCheckBox);
	mLargeSizeCheckBox->setPosition(getScreenWidth()/2, 0);

	mCancelDialog = new Button();
	mCancelDialog->setText("Cancel");
	mCancelDialog->fillSpaceHorizontally();

	dialogMainLayout->addChild(smallLayout);
	dialogMainLayout->addChild(mediumLayout);
	dialogMainLayout->addChild(largeLayout);
	dialogMainLayout->addChild(mCancelDialog);

	// Check the default/previously selected size value.
	int fontSize = GeneralSettings::getInstance()->getFontSize();
	switch(fontSize)
	{
	case FONT_SMALL_RES_HIGH:
	case FONT_SMALL_RES_LOW:
		mSmallSizeCheckBox->setState(true);
	case FONT_MEDIUM_RES_HIGH:
	case FONT_MEDIUM_RES_LOW:
		mMediumSizeCheckBox->setState(true);
	case FONT_LARGE_RES_HIGH:
	case FONT_LARGE_RES_LOW:
		mLargeSizeCheckBox->setState(true);
	}

	mSizeDialog->setMainWidget(dialogMainLayout);
}

/**
 * Create a layout with 3 equal image buttons with the specified colors.
 * @param
 */
HorizontalLayout* SettingsScreen::addColorLineLayout(int leftColor, int centerColor, int rightColor)
{
	HorizontalLayout* lineLayout = new HorizontalLayout();
	lineLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	HorizontalLayout* leftLayout = new HorizontalLayout();
	ImageButton* leftButton = new ImageButton();
	leftButton->setImage(createColorImage(
			leftColor, getIconSize(), getIconSize()));
	HorizontalLayout* centerLayout = new HorizontalLayout();
	ImageButton* centerButton = new ImageButton();
	centerButton->setImage(createColorImage(
			centerColor, getIconSize(), getIconSize()));
	HorizontalLayout* rightLayout = new HorizontalLayout();
	ImageButton* rightButton = new ImageButton();
	rightButton->setImage(createColorImage(
			rightColor, getIconSize(), getIconSize()));

	mColorButtonLayouts.add(leftLayout);
	mColorButtonLayouts.add(centerLayout);
	mColorButtonLayouts.add(rightLayout);
	mColorButtons.add(leftButton);
	mColorButtons.add(centerButton);
	mColorButtons.add(rightButton);

	leftLayout->addChild(leftButton);
	centerLayout->addChild(centerButton);
	rightLayout->addChild(rightButton);

	lineLayout->addChild(leftLayout);
	lineLayout->addChild(centerLayout);
	lineLayout->addChild(rightLayout);

	leftButton->addButtonListener(this);
	centerButton->addButtonListener(this);
	rightButton->addButtonListener(this);

	return lineLayout;
}


ListView* SettingsScreen::addColorLineList(int leftColor, int centerColor, int rightColor)
{
	ListView* colorsLayout = new ListView();
	//colorsLayout->wrapContentHorizontally();
	ListViewItem* leftButton = new ListViewItem();
	leftButton->setIcon(createColorImage(leftColor, 40, 40));
	ListViewItem* centerButton = new ListViewItem();
	centerButton->setIcon(createColorImage(centerColor, 40, 40));
	ListViewItem* rightButton = new ListViewItem();
	rightButton->setIcon(createColorImage(rightColor, 40, 40));

	colorsLayout->addChild(leftButton);
	colorsLayout->addChild(centerButton);
	colorsLayout->addChild(rightButton);
	return colorsLayout;
}

/**
 * Create and add the font size list view item.
 */
void SettingsScreen::addFontSizeSetting()
{
	mSizeSettingItem = new ListViewItem();
	VerticalLayout* fontSizeLayout = new VerticalLayout();
	fontSizeLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	Label* fontSizeLabel = new Label();
	fontSizeLabel->setFontColor(WHITE);
	fontSizeLabel->setText("Default font size");
	fontSizeLabel->setFontSize(getDefaultFontSize());

	mSizeLabel = new Label();
//	mSizeLabel->setFontColor(
//			GeneralSettings::getInstance()->getFontColor());
	mSizeLabel->setFontColor(DARK_WHITE);
	mSizeLabel->setFontSize(getDefaultFontSize()-5);
	mSizeLabel->setText(getFontSize(
			GeneralSettings::getInstance()->getFontSize()));

	fontSizeLayout->addChild(fontSizeLabel);
	fontSizeLayout->addChild(mSizeLabel);
	fontSizeLayout->wrapContentVertically();
	mSizeSettingItem->addChild(fontSizeLayout);
	mSettingsList->addChild(mSizeSettingItem);
}

/**
 * Create and add the font color list view item.
 */
void SettingsScreen::addFontColorSetting()
{
	mColorSettingItem = new ListViewItem();
	HorizontalLayout* fontColorLayout = new HorizontalLayout();
	fontColorLayout->setHeight(getIconSize() + 10);
	//fontColorLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
//	fontColorLayout->wrapContentVertically();
	VerticalLayout* leftLayout = new VerticalLayout();
	leftLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	leftLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	Label* fontColorLabel = new Label();
	fontColorLabel->setFontColor(WHITE);
	fontColorLabel->setText("Default font color");
	VerticalLayout* rightLayout = new VerticalLayout();
	rightLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	rightLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);

	mColorSettingImage = new Image();
	mColorSettingImage->setImage(createColorImage(
			GeneralSettings::getInstance()->getFontColor(), getIconSize(), getIconSize()));

	leftLayout->addChild(fontColorLabel);
	rightLayout->addChild(mColorSettingImage);
	fontColorLayout->addChild(leftLayout);
	fontColorLayout->addChild(rightLayout);
	mColorSettingItem->addChild(fontColorLayout);
	mSettingsList->addChild(mColorSettingItem);
}

/**
 * Create a colored image.
 * @param color The color of the image (a hex value).
 * @param width The width of the image.
 * @param height The height of the image.
 * @return Handle to the image. The image needs to be
 * deallocated with maDestoryObject.
 */
MAHandle SettingsScreen::createColorImage(int color, int width, int height)
{
	MAHandle image = maCreatePlaceholder();
	maCreateDrawableImage(image, width, height);
	MAHandle previousDrawTarget = maSetDrawTarget(image);
	maSetColor(color);
	maFillRect(0, 0, width, height);
	maSetDrawTarget(previousDrawTarget);
	return image;
}

/*
void SettingsScreen::addDateModifiedSetting()
{
	ListViewItem* dateItem = new ListViewItem();
	dateItem->fillSpaceVertically();
	dateItem->setBackgroundColor(YELLOW);
	RelativeLayout* dateLayout = new RelativeLayout();
	dateLayout->fillSpaceVertically();
	dateLayout->fillSpaceHorizontally();
	Label* dateLabel = new Label();
	dateLabel->setFontColor(WHITE);
	dateLabel->setText("Display last modified date");

	mDateModifiedCheckBox = new CheckBox();

	dateLayout->addChild(dateLabel);
	dateLayout->addChild(mDateModifiedCheckBox);
	dateLabel->setPosition(0,PADDING*2);
	mDateModifiedCheckBox->setPosition((3*getScreenWidth())/4, PADDING);

	dateItem->addChild(dateLayout);
	mSettingsList->addChild(dateItem);
}*/

/**
 * Create and add the date modified setting item.
 */
void SettingsScreen::addDateModifiedSetting()
{
	ListViewItem* dateSettingItem = new ListViewItem();
//	dateSettingItem->fillSpaceHorizontally();
	dateSettingItem->setWidth(getScreenWidth());
	HorizontalLayout* dateSettingLayout = new HorizontalLayout();
//	dateSettingLayout->wrapContentVertically();
	VerticalLayout* leftLayout = new VerticalLayout();
	leftLayout->setBackgroundColor(RED);
//	leftLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	leftLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	Label* dateLabel = new Label();
	dateLabel->setFontColor(WHITE);
	dateLabel->setBackgroundColor(YELLOW);
	dateLabel->setText("Display last modified date");
	VerticalLayout* rightLayout = new VerticalLayout();
	rightLayout->setWidth(getScreenWidth()/3);
//	rightLayout->fillSpaceHorizontally();
	rightLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	rightLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mDateModifiedCheckBox = new CheckBox();

	leftLayout->addChild(dateLabel);
	leftLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	rightLayout->addChild(mDateModifiedCheckBox);
	dateSettingLayout->addChild(leftLayout);
	dateSettingLayout->addChild(rightLayout);
	dateSettingItem->addChild(dateSettingLayout);
	mSettingsList->addChild(dateSettingItem);
}

/**
 * Create and add the date created setting item.
 */
void SettingsScreen::addDateCreatedSetting()
{
	ListViewItem* dateSettingItem = new ListViewItem();
	dateSettingItem->setWidth(getScreenWidth());
	HorizontalLayout* dateSettingLayout = new HorizontalLayout();
	dateSettingLayout->wrapContentVertically();
//	dateSettingLayout->fillSpaceHorizontally();
//	dateSettingLayout->setBackgroundColor(BLUE);
	VerticalLayout* leftLayout = new VerticalLayout();
	leftLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	leftLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	Label* dateLabel = new Label();
	dateLabel->setFontColor(WHITE);
	dateLabel->setText("Display date created");
	VerticalLayout* rightLayout = new VerticalLayout();
//	rightLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
//	rightLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mDateCreatedCheckBox = new CheckBox();

	leftLayout->addChild(dateLabel);
	rightLayout->addChild(mDateCreatedCheckBox);
	rightLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	rightLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);

	dateSettingLayout->addChild(leftLayout);
	dateSettingLayout->addChild(rightLayout);
	dateSettingItem->addChild(dateSettingLayout);
	mSettingsList->addChild(dateSettingItem);
}

/**
 * Create and add the notes count setting item.
 */
void SettingsScreen::addItemsNumberSetting()
{
	ListViewItem* countSettingItem = new ListViewItem();
	countSettingItem->setWidth(getScreenWidth());
	HorizontalLayout* countSettingLayout = new HorizontalLayout();
	countSettingLayout->wrapContentVertically();
	VerticalLayout* leftLayout = new VerticalLayout();
	leftLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	leftLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	Label* countLabel = new Label();
	countLabel->setFontColor(WHITE);
	countLabel->setText("Display notes count");
	VerticalLayout* rightLayout = new VerticalLayout();
	rightLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	rightLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
	mCountCheckBox = new CheckBox();

	leftLayout->addChild(countLabel);
	rightLayout->addChild(mCountCheckBox);
	countSettingLayout->addChild(leftLayout);
	countSettingLayout->addChild(rightLayout);
	countSettingItem->addChild(countSettingLayout);
	mSettingsList->addChild(countSettingItem);
}

/**
 * Create and add an about item.
 */
void SettingsScreen::addAboutSection()
{
	mAboutItem = new ListViewItem();
	VerticalLayout* aboutLayout = new VerticalLayout();
	aboutLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
	Label* aboutLabel = new Label();
	aboutLabel->setFontColor(WHITE);
	aboutLabel->setText("About");
	aboutLabel->setFontSize(getDefaultFontSize());
	Label* versionLabel = new Label();
	versionLabel->setFontColor(DARK_WHITE);
	versionLabel->setFontSize(getDefaultFontSize()-5);
	versionLabel->setText("EasyNotes 0.0.1");

	aboutLayout->addChild(aboutLabel);
	aboutLayout->addChild(versionLabel);
//	aboutLayout->wrapContentVertically();
	mAboutItem->addChild(aboutLayout);
	mSettingsList->addChild(mAboutItem);
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void SettingsScreen::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
	if (listView == mSettingsList
			&& listViewItem == mColorSettingItem)
	{
		mColorDialog->show();
	}
	else if (listView == mSettingsList
			&& listViewItem == mSizeSettingItem)
	{
		//mSizeDialogList->setProperty("requestFocus","");
		// Launch font size selection dialog.
		mSizeDialog->show();
	}
	else if(listView == mSettingsList
			&& listViewItem == mAboutItem)
	{
		maAlert("About", "MoSync AB 2012 etc........", "OK", NULL, NULL);
	}
	else if(listView == mSizeDialogList )
	{
		mSizeDialog->hide();
		/*
		// Unmark the other check boxes and store the new value.
		if (listViewItem == mSmallItem)
		{
			mSmallSizeCheckBox->setState(true);
			mMediumSizeCheckBox->setState(false);
			mLargeSizeCheckBox->setState(false);
			GeneralSettings::getInstance()->setFontSize(
					getExactFontSize(SMALL));
		}
		else if(listViewItem == mMediumItem)
		{
			mSmallSizeCheckBox->setState(false);
			mMediumSizeCheckBox->setState(true);
			mLargeSizeCheckBox->setState(false);
			GeneralSettings::getInstance()->setFontSize(
					getExactFontSize(MEDIUM));
		}
		else if(listViewItem == mLargeItem)
		{
			mSmallSizeCheckBox->setState(false);
			mMediumSizeCheckBox->setState(false);
			mLargeSizeCheckBox->setState(true);
			GeneralSettings::getInstance()->setFontSize(
					getExactFontSize(LARGE));
		}

		mSizeDialog->hide();
		// Reload the font size item info.
		mSizeLabel->setText(getFontSize(
				GeneralSettings::getInstance()->getFontSize()));

		// Store the new settings.
		mListener->settingsUpdated();
		*/
	}
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void SettingsScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	if (checkBox == mDateModifiedCheckBox)
	{
		GeneralSettings::getInstance()->
				setDisplayDateModified(mDateModifiedCheckBox->isChecked());

	}
	else if (checkBox == mDateCreatedCheckBox)
	{
		GeneralSettings::getInstance()->
				setDisplayDateCreated(mDateCreatedCheckBox->isChecked());

	}
	else if (checkBox == mCountCheckBox)
	{
		GeneralSettings::getInstance()->
				setDisplayCount(mCountCheckBox->isChecked());
	}
	else if (state)
	{
		if (checkBox == mSmallSizeCheckBox)
		{
			mMediumSizeCheckBox->setState(false);
			mLargeSizeCheckBox->setState(false);
			GeneralSettings::getInstance()->setFontSize(FONT_SMALL_RES_HIGH);
			mSizeLabel->setText(getFontSize(
					GeneralSettings::getInstance()->getFontSize()));
			GeneralSettings::getInstance()->setFontSize(FONT_SMALL_RES_HIGH);
			mSizeDialog->hide();
		}
		else if(checkBox == mMediumSizeCheckBox)
		{
			mSmallSizeCheckBox->setState(false);
			mLargeSizeCheckBox->setState(false);
			GeneralSettings::getInstance()->setFontSize(FONT_MEDIUM_RES_HIGH);
			mSizeLabel->setText(getFontSize(
					GeneralSettings::getInstance()->getFontSize()));
			mSizeDialog->hide();
		}
		else if(checkBox == mLargeSizeCheckBox)
		{
			mMediumSizeCheckBox->setState(false);
			mSmallSizeCheckBox->setState(false);
			GeneralSettings::getInstance()->setFontSize(FONT_LARGE_RES_HIGH);
			mSizeLabel->setText(getFontSize(
					GeneralSettings::getInstance()->getFontSize()));
			mSizeDialog->hide();
		}
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS, Android, Windows Phone.
 * @param button The button object that generated the event.
 */
void SettingsScreen::buttonClicked(Widget* button)
{
	if(button == mCancelDialog)
	{
		mSizeDialog->hide();
		return;
	}

	int buttonIndex = -1;
	for (int i=0; i<mColorButtons.size(); i++)
	{
		if (mColorButtons[i] == button)
		{
			buttonIndex = i;
			break;
		}
	}

	if( buttonIndex != -1 )
	{
		mColorButtonLayouts[buttonIndex]->setBackgroundColor(DARK_GREY);

		// Get color based on button index.
		GeneralSettings::getInstance()->setFontColor(
				getColorForButtonIndex(buttonIndex));

		//TODO save font color, based on buttonIndex get the color
	    // check out BLACK on the black background. maybe add a shadow.
		mColorSettingImage->setImage(mColorButtons[buttonIndex]->getImage());
		mColorButtonLayouts[buttonIndex]->setBackgroundColor(WHITE);

		mColorDialog->hide();
	}
}

/**
 * This method is called when there is an touch-down event for
 * a button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void SettingsScreen::buttonPressed(Widget* button)
{
//	int buttonIndex = -1;
//	for (int i=0; i<mColorButtons.size(); i++)
//	{
//		if (mColorButtons[i] == button)
//		{
//			buttonIndex = i;
//			break;
//		}
//	}
//
//	if( buttonIndex != -1 )
//	{
//		mColorButtonLayouts[buttonIndex]->setBackgroundColor(DARK_GREY);
//		mColorDialog->hide();
//
//		//TODO save font color, based on buttonIndex get the color
//	    // check out BLACK on the black background. maybe add a shadow.
//		mColorSettingImage->setImage(mColorButtons[buttonIndex]->getImage());
//		mColorButtonLayouts[buttonIndex]->setBackgroundColor(WHITE);
//		// Get color based on button index.
//		GeneralSettings::getInstance()->setFontColor(
//				getColorForButtonIndex(buttonIndex));
//}
		// Store the new settings.
//		mListener->settingsUpdated();
}

/**
 * This method is called when an options menu item is selected.
 * @param index The index on which the item is placed into the
 * OptionsMenu.
 * @param screen The screen that generated the event.
 */
void SettingsScreen::optionsMenuItemSelected(Screen* screen, int index)
{
	if (index == mRestoreDefaultsOptionsMenu)
	{
		GeneralSettings::getInstance()->initialize(
				FONT_MEDIUM_RES_LOW, BLUE, false, true, false);
		// Reload controls from this screen.
		loadDefaultValues();
	}
}

/**
 * This method is called when the OptionsMenu is being closed
 * (either by the user canceling the menu with the back/menu
 * button, or when an item is selected.
 * @param screen The screen that generated the event.
 */
void SettingsScreen::optionsMenuClosed(Screen* screen)
{

}

/**
 * ICS workaround for the list focus bug.
 */
void SettingsScreen::requestListFocus()
{
	mSettingsList->setProperty("requestFocus","");
}

/**
 * Load the default settings, when the app is first launched,
 * or when Restore defaults is selected.
 */
void SettingsScreen::loadDefaultValues()
{
//	mSizeLabel->setText(getFontSize(
//			GeneralSettings::getInstance()->getFontSize()));
	mColorSettingImage->setImage(createColorImage(
			GeneralSettings::getInstance()->getFontColor(),
			getIconSize(), getIconSize()));
	mDateModifiedCheckBox->setState(true);
	mDateCreatedCheckBox->setState(false);
	mCountCheckBox->setState(false);
	int fontSize = GeneralSettings::getInstance()->getFontSize();
	switch(fontSize)
	{
	case FONT_SMALL_RES_HIGH:
	case FONT_SMALL_RES_LOW:
		setCheckBoxState(*mSmallSizeCheckBox, true);
		setCheckBoxState(*mMediumSizeCheckBox, false);
		setCheckBoxState(*mLargeSizeCheckBox, false);
	case FONT_MEDIUM_RES_HIGH:
	case FONT_MEDIUM_RES_LOW:
		setCheckBoxState(*mMediumSizeCheckBox, true);
		setCheckBoxState(*mSmallSizeCheckBox, false);
		setCheckBoxState(*mLargeSizeCheckBox, false);
	case FONT_LARGE_RES_HIGH:
	case FONT_LARGE_RES_LOW:
		setCheckBoxState(*mLargeSizeCheckBox, true);
		setCheckBoxState(*mSmallSizeCheckBox, false);
		setCheckBoxState(*mMediumSizeCheckBox, false);
	}
}

/**
 * Set the checkbox state only if the previous state is different.
 * use this method so that checkBoxStateChanged events do not occur
 * when it's not needed.
 * @param check The checkbox.
 * @param state The new state.
 */
void SettingsScreen::setCheckBoxState(CheckBox& check, bool state)
{
	if ( check.isChecked() != state)
	{
		check.setState(state);
	}
}
