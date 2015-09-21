#include "DemoH264MediaSubsession.h"
#include "DemoH264FrameSource.h"
#include "DemoH264Interface.h"

#include "H264VideoStreamFramer.hh"
#include "H264VideoRTPSink.hh"


DemoH264MediaSubsession::DemoH264MediaSubsession(UsageEnvironment& env, int streamType, int videoType, int channelNO, bool reuseFirstSource, portNumBits initalNumPort)
	:OnDemandServerMediaSubsession(env, reuseFirstSource), fStreamType(streamType), fVideoType(videoType), fChannelNO(channelNO)
{
}

DemoH264MediaSubsession::~DemoH264MediaSubsession()
{
}


DemoH264MediaSubsession* DemoH264MediaSubsession::createNew(UsageEnvironment& env, int streamType, int videoType, int channelNO, 
		bool reuseFirstSource, portNumBits initalNumPort)
{
	DemoH264MediaSubsession* sms = new DemoH264MediaSubsession(env, streamType, videoType, channelNO, reuseFirstSource, initalNumPort);
	return sms;
}


FramedSource* DemoH264MediaSubsession::createNewStreamSource(unsigned clientsessionId, unsigned& estBitrate)
{
	DBG_LIVE555_PRINT("create new stream source !\n");

	//这里根据实际请求的类型创建不同的source对象
	if(fVideoType == 0x01)
	{ // H264 video 
		estBitrate = 2000; // kbps 
		DemoH264FrameSource * source = DemoH264FrameSource::createNew(envir(), fStreamType, fChannelNO, 0);
		if ( source == NULL )
		{
			DBG_LIVE555_PRINT("create source failed videoType:%d!\n", fVideoType );
			return NULL;
		}
		return H264VideoStreamFramer::createNew(envir(), source);
	}
	else if ( fVideoType == 0x2) 
	{// Mpeg-4 video  
		
	}
	else if( fVideoType == 0x04)
	{ // G711 audio 
		estBitrate = 128; // kbps 
		DemoH264FrameSource * source = DemoH264FrameSource::createNew(envir(), fStreamType, fChannelNO, 1);
		if ( source == NULL )
		{
			DBG_LIVE555_PRINT("create source failed videoType:%d!\n", fVideoType );
			return NULL;
		}
		return source;
	}
	else 
	{ // unknow type
		
	}
	return NULL;	
}


RTPSink* DemoH264MediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{
	// 这里可以根据类型的不同创建不同sink 
	// 根据实际开发需要，继承不同的子类
	DBG_LIVE555_PRINT("createNewRTPnk videoType:%d!\n", fVideoType );
	if( fVideoType == 0x01)
	{
		return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
	}
	else if( fVideoType == 0x02)
	{ //  Mpeg-4
		
	}
	else if(fVideoType == 0x04)
	{// G711 audio
		
	}
	else 
	{ // unknow type ;
		return NULL;
	}
}


/* 根据开发实际情况填写SDP信息 */
/*
char const* DemoH264MediaSubsession::sdpLines()
{
	// create sdp info 
	

	return  fSDPLines;
}
*/

