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
		static DemoH264FrameSource* createNew(UsageEnvironment& env, const char* fileName, 
				unsigned preferredFrameSize = 0,  unsigned playTimePerFrame = 0);
	protected:
		DemoH264FrameSource(UsageEnvironment& env, const char* fileName,
				unsigned preferredFrameSize, unsigned playTimePerFrame);
		~DemoH264FrameSource();
	private:
		virtual void doGetNextFrame();
		//int TransportData(unsigned char* to, unsigned maxSize);
		
	protected:
		FILE *fp;
};


#endif // DemoH264FrameSource.h
