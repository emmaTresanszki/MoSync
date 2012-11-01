/*
Copyright (C) 2011 MoSync AB

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
 * @file Util.h
 * @author Emma Tresanszki.
 *
 * Utility functions and constants for the application.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <mastring.h>		// C string functions
#include "mavsprintf.h"
#include <matime.h>

//#include <MAUtil/String.h>
#include <MAUtil/util.h>

#define BUF_MAX 256

#define SETTINGS_FILE "Settings.txt"
#define NOTES_FILE "Notes.txt"
#define FILE_FIELD_SEPARATOR ";"
#define FILE_NOTES_SEPARATOR "#"

/**
 * File fields.
 */
#define FIELD_FONT_SIZE "fontSize"
#define FIELD_FONT_COLOR "fontColor"
#define FIELD_MODIFIED_DATE "modifiedDate"
#define FIELD_CREATED_DATE "createdDate"

static int mFontSize;
static int mScreenWidth;
static int mScreenHeight;

/** Colors used for UI widgets. */
enum Colors{
	/** The background color for the top layout. */
	QUARTZ     = 0xD9D9F3 ,//!< QUARTZ

	ORANGE_RED = 0xff4500, //!< ORANGE_RED

	MOCCASIN = 0xffe4b5,   //!< MOCCASIN
	/** The background color for the main layout. */
	SLATE_GRAY = 0x9FB6CD ,//!< SLATE_GRAY
	DARK_SLATE_GRAY = 0x2f4f4f,
	/** The color used for buttons and spacers. */
	DARK_GREY  = 0x696969, //!< DARK_GREY
	/** The background color used for other layouts. */
	DARK_WHITE = 0xF7F7F7, //!< DARK_WHITE
	/** Font colors. */
	YELLOW = 0xffff00,     //!< YELLOW
	BLUE = 0x0000ff,       //!< BLUE
	GREEN = 0x00ff00,      //!< GREEN
	PINK = 0xffc0cb,       //!< PINK
	RED = 0xff0000,        //!< RED
	ORANGE = 0xffa500,     //!< ORANGE
	BLACK = 0x000000,      //!< BLACK
	GRAY = 0xbebebe,       //!< GRAY
	WHITE = 0xffffff,      //!< WHITE
	PURPLE = 0xa020f0      //!< PURPLE
};

/**
 * Different sizes for low and high resolutions,
 * with 3 possible choices.
 */
enum FontSize
{
	FONT_MEDIUM_RES_HIGH = 26,
	FONT_MEDIUM_RES_LOW = 24,
	FONT_SMALL_RES_HIGH = 22,
	FONT_SMALL_RES_LOW = 20,
	FONT_LARGE_RES_HIGH = 30,
	FONT_LARGE_RES_LOW = 28,
	PADDING = 10,
	HEADER_HEIGHT = 100,
	LINE_HEIGHT = 5,
	PREVIEW_LENGTH = 10
};

enum FontSizeOptions
{
	SMALL = 0,
	MEDIUM = 1,
	LARGE = 2
};

enum platform_code{
	ANDROID = 0,
	IOS = 1,
	WINDOWSPHONE7 = 2
};

/**
 * Icon size for colored image buttons.
 */
enum IconSize
{
	IMAGE_LARGE = 80,
	IMAGE_SMALL = 40,
	IMAGE_MEDIUM = 60
};

/**
 * Detects the current platform
 * @return platform_code specific for Android, iPhone OS or WindowsPhone
 */
static int getPlatform()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);

	if(strcmp(platform, "Android") == 0)
	{
		return ANDROID;
	}
	else
	{
		if(strcmp(platform, "iPhone OS") == 0)
			return IOS;
	}
	return WINDOWSPHONE7;
}

/**
 * Get the current time in DD/MM/YY HH:MM format.
 * @return The formated string.
 */
static MAUtil::String getTimeString()
{
	struct tm * tim_p = new tm;
	split_time(maTime(), tim_p);
	MAUtil::String date = MAUtil::integerToString(tim_p->tm_mday) + "/" +
			MAUtil::integerToString(tim_p->tm_mon+1) + "/" +
			MAUtil::integerToString(tim_p->tm_year + 1900) + " " +
			MAUtil::integerToString(tim_p->tm_hour) + ":" +
			MAUtil::integerToString(tim_p->tm_min);
	return date;
}

static tm* getTime()
{
	struct tm * tim_p = new tm;
	split_time(maTime(), tim_p);
	return tim_p;
}

/**
 * Methods for comparing dates.
 */
static bool isSameDay(tm& date1, tm& date2)
{
	if ( date1.tm_year == date2.tm_year
			&& date1.tm_mon == date2.tm_mon
			&& date1.tm_mday == date2.tm_mday)
		return true;
	return false;
}

static bool isSameMonth(tm& date1, tm& date2)
{
	if ( date1.tm_year == date2.tm_year
			&& date1.tm_mon == date2.tm_mon)
		return true;
	return false;
}

static bool isSameWeek(tm& date1, tm& date2)
{
	if ( date2.tm_wday == date2.tm_wday )
		return true;
	return false;
}

static MAUtil::String getColorString(int color)
{
	switch(color)
	{
	case RED:
		return "0xFF0000";
	case BLUE:
		return "0x0000FF";
	case GREEN:
		return "0x00FF00";
	case PINK:
		return "0xffc0cb";
	case ORANGE:
		return "0xffa500";
	case BLACK:
		return "0x000000";
	case GRAY:
		return "0xbebebe";
	case PURPLE:
		return "0xa020f0";
	case YELLOW:
		return "0xffff00";
	default:
		return "0x0000FF";
	}
}

static int getColorValue(MAUtil::String color)
{
	if (strcmp(color.c_str(),"0xFF0000") == 0)
		return RED;
	if (strcmp(color.c_str(),"0x0000FF") == 0)
		return BLUE;
	if (strcmp(color.c_str(),"0x00FF00") == 0)
		return GREEN;
	if (strcmp(color.c_str(),"0xffc0cb") == 0)
		return PINK;
	if (strcmp(color.c_str(),"0xffa500") == 0)
		return ORANGE;
	if (strcmp(color.c_str(),"0x000000") == 0)
		return BLACK;
	if (strcmp(color.c_str(),"0xbebebe") == 0)
		return GRAY;
	if (strcmp(color.c_str(),"0xa020f0") == 0)
		return PURPLE;
	if (strcmp(color.c_str(),"0xffff00") == 0)
		return YELLOW;
}

/**
 * Get buttons image size based on screen resolution.
 */
static int getIconSize()
{
	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	if ( mScreenHeight < 600 )
	{
		return IMAGE_SMALL;
	}
	else if( mScreenHeight < 800 )
	{
		return IMAGE_MEDIUM;
	}
	else
	{
		return IMAGE_LARGE;
	}
}

static int getScreenHeight()
{
	MAExtent screenSize = maGetScrSize();
	return EXTENT_Y(screenSize);
}

static int getScreenWidth()
{
	MAExtent screenSize = maGetScrSize();
	return EXTENT_X(screenSize);
}

/**
 * Based on user selection(Small/Medium/Large),
 * calculate the exact font size.
 */
static int getExactFontSize(FontSizeOptions size)
{
	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	switch(size)
	{
	case SMALL:
	{
		if ( mScreenHeight < 600 )
		{
			return FONT_SMALL_RES_LOW;
		}
		else
		{
			return FONT_SMALL_RES_HIGH;
		}
	}
	case MEDIUM:
		if ( mScreenHeight < 600 )
		{
			return FONT_MEDIUM_RES_LOW;
		}
		else
		{
			return FONT_MEDIUM_RES_HIGH;
		}
	case LARGE:
		if ( mScreenHeight < 600 )
		{
			return FONT_LARGE_RES_LOW;
		}
		else
		{
			return FONT_LARGE_RES_HIGH;
		}
	}
}

static int getDefaultFontSize()
{
	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	// For small screens, use small fonts.
	if ( mScreenHeight < 600 )
	{
		mFontSize = FONT_SMALL_RES_LOW;
	}
	else if( mScreenHeight < 800 )
	{
		mFontSize = FONT_MEDIUM_RES_LOW;
	}
	else
	{
		mFontSize = FONT_LARGE_RES_LOW;
	}

	// Set the font for iPhone devices.
	if (getPlatform() == IOS && mScreenHeight < 1000)
	{
		mFontSize = FONT_LARGE_RES_LOW;
	}

	return mFontSize;
}


static MAUtil::String getFontSize(int fontSize)
{
	switch(fontSize)
	{
	case FONT_SMALL_RES_HIGH:
	case FONT_SMALL_RES_LOW:
		return " Small ";
	case FONT_MEDIUM_RES_HIGH:
	case FONT_MEDIUM_RES_LOW:
		return " Medium ";
	case FONT_LARGE_RES_HIGH:
	case FONT_LARGE_RES_LOW:
		return "Large ";
	default:
		return " Medium ";
	}
}

/**
 * Get the color value, in hexa, based on the selected button into color dialog.
 * @param index The color button index.
 * @return The corresponding color.
 */
static int getColorForButtonIndex(int index)
{
	switch(index)
	{
	case 0:
		return RED;
	case 1:
		return GREEN;
	case 2:
		return BLUE;
	case 3:
		return PINK;
	case 4:
		return ORANGE;
	case 5:
		return YELLOW;
	case 6:
		return PURPLE;
	case 7:
		return GRAY;
	case 8:
		return BLACK;
	default:
		return RED;
	}
}

#endif /* UTIL_H_ */
