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
 * @file GeneralSettings.h
 * @author Emma Tresanszki.
 */

#ifndef GENERALSETTINGS_H_
#define GENERALSETTINGS_H_

/**
 * Class that holds the application settings:
 * The font color and size is used for notes group title,
 * and all notes.
 * The last modified date is displayed on the Main Screen.
 * The items count is also displayed on the Main Screen.
 * Default settings are:
 *  - mFontColor = BLUE
 *  - mFontSize it is based on screen resolution.
 *  - mDisplayDateModified = true
 *  - mDisplayDateCreated = false;
 *  - mDisplayCount = false.
 */
class GeneralSettings
{
public:
	GeneralSettings();
	virtual ~GeneralSettings();
	void initialize(
			int fontSize, int fontColor, bool mDateCreated, bool mDateModified, bool mCount);
	void setFontSize(int fontSize);
	void setFontColor(int fontColor);
	void setDisplayDateModified(bool displayDate) { mDisplayDateModified = displayDate; };
	void setDisplayDateCreated(bool displayDate) { mDisplayDateCreated = displayDate; };
	void setDisplayCount(bool displayCount);

	int getFontSize();
	int getFontColor() { return mFontColor;}
	bool isDisplayDateModifiedEnabled() {return mDisplayDateModified;}
	bool isDisplayDateCreatedEnabled() {return mDisplayDateCreated;}
	bool isDisplayCountEnabled() {return mDisplayCount;}
	static GeneralSettings* getInstance();
private:

	static GeneralSettings* sInstance;

	int mFontSize;
	int mFontColor;
	bool mDisplayDateModified;
	bool mDisplayDateCreated;
	bool mDisplayCount;
};

#endif /* GENERALSETTINGS_H_ */
