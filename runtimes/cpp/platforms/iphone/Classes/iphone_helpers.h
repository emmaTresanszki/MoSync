/*
 *  iphone_helpers.h
 *  MoSync
 *
 *  Created by Niklas Nummelin on 2/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _IPHONEHELPERS_H_
#define _IPHONEHELPERS_H_

#include "ThreadPoolImpl.h"

// do all the const char* become memory leaks or does the garbage collector take care of that?.
const char *getReadablePath(const char* path);
const char *getWriteablePath(const char* path);
void sleepMillis(int ms);
const char *unicodeToAscii(const wchar_t* str);

/*
class Timer {
public:
	typedef void (*TimerCallback)(void *arg);

	struct TimerInfo {
		int ms;
		TimerCallback cb;
		void *arg;
	};
	
	Timer(TimerCallback cb, int ms, void *arg);
	~Timer();
	void run();
	
private:
	TimerInfo mTimerInfo;
	MoSyncThread mThread;
};
*/
#endif