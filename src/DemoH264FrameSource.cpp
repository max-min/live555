#include <stdio.h>
#include "DemoH264FrameSource.h"

DemoH264FrameSource::DemoH264FrameSource(UsageEnvironment& env, const char* fileName,
		unsigned int preferredFrameSize, unsigned int playTimePerFrame):FramedSource(env)
{
	// ready for the source data; 
	// 打开流媒体文件，在实时流时，这里就是开始传送流之前的一些准备工作
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


/* 获取需要读取文件的总长度，live555对每次数据的发送有长度限制 */
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
	// 判断是否超过最长，分开处理
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

