#include <stdio.h>
#include "DemoH264FrameSource.h"

DemoH264FrameSource::DemoH264FrameSource(UsageEnvironment& env, const char* fileName,
		unsigned int preferredFrameSize, unsigned int playTimePerFrame):FramedSource(env)
{
	// ready for the source data; 
	fp = fopen(fileName, "rb");
}


DemoH264FrameSource::~DemoH264FrameSource()
{
	
}


DemoH264FrameSource* DemoH264FrameSource::createNew(UsageEnvironment& env, const char* fileName, 
		unsigned preferredFrameSize ,  unsigned playTimePerFrame )
{
	return new DemoH264FrameSource(env, fileName, preferredFrameSize, playTimePerFrame);
}


/* Get the leave data length */
long filesize(FILE *stream)
{
	long curpos, length;
	curpos = ftell(stream);
	fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	fseek(stream, curpos, SEEK_SET);
	return length;
}

void DemoH264FrameSource::doGetNextFrame()
{

	if( filesize(fp) > fMaxSize)
	{
		fFrameSize = fread(fTo, 1, fMaxSize, fp);	
	}
	else 
	{
		fFrameSize = fread(fTo,1, filesize(fp), fp);
		fseek(fp, 0, SEEK_SET);
	}
	

	nextTask() = envir().taskScheduler().scheduleDelayedTask(0,
			(TaskFunc*)FramedSource::afterGetting, this); 
	// 表示延迟0秒后再执行 afterGetting 函数
	
}

