/*
 * =====================================================================================
 *
 *       Filename:  DemoH264MediaSubsession.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年09月07日 21时49分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  max_min_,
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __DEMOH264MEDIASUBSESSION_H__
#define __DEMOH264MEDIASUBSESSION_H__

//#include "FileServerMediaSubsession.hh"
#include "OnDemandServerMediaSubsession.hh"

class DemoH264MediaSubsession:public OnDemandServerMediaSubsession 
{
	public:
		static DemoH264MediaSubsession* createNew(UsageEnvironment& env, int streamType, int videoType, int channelNO, 
				bool reuseFirstSource, portNumbits initalNumPort = 6970);
	protected:
		DemoH264MediaSubsession(UsageEnvironment& env, int streamType, int videoType, int channelNO, 
				bool reuseFirstSource, portNumbits initalNumPort);
		~DemoH264MediaSubsession();
		
	protected:
		// virtual function  new source and rtplink
		// 继承父类的虚函数，必须自己实现
		FramedSource* createNewStreamSource(unsigned clientsessionId, unsigned& estBitrate);
		RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);
	
		char const* sdpLines();
	public:
		int fStreamType;
		int fVideoType;
		int fChannelNO;
		
};






#endif // DemoH264MediaSubsession.h
