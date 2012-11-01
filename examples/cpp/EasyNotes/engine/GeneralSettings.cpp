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
 * @file GeneralSettings.cpp
 * @author Emma Tresanszki.
 */

#include "../Util.h"
#include "GeneralSettings.h"

//  Initialize the singleton variable to NULL.
GeneralSettings* GeneralSettings::sInstance = NULL;

GeneralSettings::GeneralSettings()
{
	mFontSize = getDefaultFontSize();
	mFontColor = BLUE;
	mDisplayDateModified = true;
	mDisplayDateCreated = false;
	mDisplayCount = false;
}

GeneralSettings::~GeneralSettings()
{
}

/**
 * Initialize the app with the default settings,
 * in case the Settings file is not found.
 * @param fontSize
 * @param fontColor
 * @param mDate
 * @param mCount
 */
void GeneralSettings::initialize(
		int fontSize, int fontColor, bool mDateCreated, bool mDateModified, bool mCount)
{
	mFontSize = fontSize;
	mFontColor = fontColor;
	mDisplayDateModified = mDateModified;
	mDisplayDateCreated = mDateCreated;
	mDisplayCount = mCount;
}

void GeneralSettings::setFontSize(int fontSize)
{
	mFontSize = fontSize;
}

void GeneralSettings::setFontColor(int fontColor)
{
	mFontColor = fontColor;
}

void GeneralSettings::setDisplayCount(bool displayCount)
{
	mDisplayCount = displayCount;
}

int GeneralSettings::getFontSize()
{
	return mFontSize;
}

GeneralSettings* GeneralSettings::getInstance()
{
    if (NULL == GeneralSettings::sInstance)
    {
		GeneralSettings::sInstance = new GeneralSettings();
    }

    return sInstance;
}
