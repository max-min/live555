
#include "DemoH264FrameSource.h"
#include "DemoH264Interface.h"

DemoH264FrameSource::DemoH264FrameSource(UsageEnvironment& env, long sourceHandle, int sourceType):
	FramedSource(env), fSourceHandle(sourceHandle), fLastBufSize(0), fLeftDataSize(0), fSourceType(sourceType), fFirstFrame(1)
{
	// 打开流媒体文件，在实时流时，这里就是开始传送流之前的一些准备工作
	fDatabuf = (char*)malloc(2*1024*1024);
	if(fDatabuf == NULL )
	{
		DBG_LIVE555_PRINT(" create source data buf failed!\n");
	}
}


DemoH264FrameSource::~DemoH264FrameSource()
{
	
}


DemoH264FrameSource* DemoH264FrameSource::createNew(UsageEnvironment& env, int streamType, int channelNO, int sourceType)
{
	//通过streamType和channelNO来创建source，向前端请求对应的码流//  
	long souceHandle = openStreamHandle(channelNO, streamType);	
	if(sourceHandle == 0)
	{
		DBG_LIVE555_PRINT("open the source stream failed!\n");
		return NULL;
	}
	return new DemoH264FrameSource(env, sourceHandle, sourceType);
	
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
	int ret = 0;
	struct timeval pts;

	//调用设备接口获取一帧数据
	if (fLeftDataSize == 0)
	{
		
		ret = getStreamData(fSourceHandle, fDataBuf,&fLastBufSize, &fLeftDataSize,fSourceType, &pts);
		if (ret <= 0)
		{
			printf("getStreamData failed!\n");
			return;
		}
	}

	int fNewFramesize = fLeftDataSize;
	if(fNewFramesize > fMaxSize)
	{
		fFrameSize = fMaxSize;
		fNumTruncatedBytes = newFrameSize - fMaxSize;
		fLeftDataSize = newFrameSize - fMaxSize;
		memmove(fTo, fDataBuf, fFrameSize);
		memmove(fDataBuf, fDataBuf+fMaxSize, fLeftDataSize);	
	} 
	else 
	{
		fFrameSize = newFrameSize;
		fLeftDataSize = 0;          //注意
		memmove(fTo, fDataBuf, fFrameSize);
	}

		gettimeofday(&fPresentationTime, NULL);
	
		if (fFirstFrame)
		{
			fDurationInMicroseconds = 40000;
			nextTask() = envir().taskScheduler().scheduleDelayedTask(100000, 
										(TaskFunc*)FramedSource::afterGetting, this);
			fFirstFrame = 0;
		}
		else
		{
			FramedSource::afterGetting(this);
		}

	
}

void DemoH264FrameSource::doStopGetFrame()
{
	closeStreamHandle(fSourceHandle);
}

