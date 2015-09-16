/*
 * =====================================================================================
 *
 *       Filename:  DemoH264FrameSource.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年09月07日 21时48分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  max_min_, 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __DEMOH264FRAMESOURCE_H__
#define __DEMOH264FRAMESOURCE_H__


#include "FramedSource.hh"

class DemoH264FrameSource:public FramedSource 
{
	public:
		static DemoH264FrameSource* createNew(UsageEnvironment& env, int streamtype, int channelno, int sourceType); 
	protected:
		DemoH264FrameSource(UsageEnvironment& env, long sourceHandle, int sourceType);
		~DemoH264FrameSource();
	private:
		virtual void doGetNextFrame();
	public:
		void doStopGetFrame();
	public:
		char *fDataBuf;
		long fSourceHandle; 
		unsigned fLastBufSize;
		unsigned fLeftDataSize;
		int fSourceType;
		int fFirstFrame;
		
};


#endif // DemoH264FrameSource.h
