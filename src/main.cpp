/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年09月07日 23时03分31秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  max_min_, 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"
#include "DemoH264MediaSubsession.h"


int main(int argc, char* argv[])
{
	
	printf(" live555 stream start\n");
	
	// Begin by setting up the live555 usage environment 
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	UsageEnvironment* env    = BasicUsageEnvironment::createNew(*scheduler);
	

	UserAuthenticationDatabase* authDB = NULL;
#if ACCESS_CONTROL   // 认证
	authDB = new UserAuthenticationDatabase;
	authDB->addUserRecord(argv[1], argv[2]);
#endif 

	RTSPServer* rtspServer = NULL;
	portNumBits rtspServerPortNum = 554; // rtsp port 

	// 建立RTSP服务
	rtspServer = RTSPServer::createNew(*env, rtspServerPortNum, authDB);
	if( rtspServer == NULL)
	{
		*env << " create RTSPServer Failed:" << env->getResultMsg() << "\n";
		return 0;
	}

	
	const char* decription = " Session Test By live555 Stream";
	

	 //H264 Subsession 
	const char* streamName = "h264_streaming";
	const char* inputName = "tc10.264";

	
	ServerMediaSession *sms = ServerMediaSession::createNew(*env, streamName, streamName, decription);

	// 添加自己派生的子类MediaSubsession类，并添加到ServerMediaSession
	// 当有client链接上过来的时候，会调用server的lookup寻找次streamName的subsession
	sms->addSubsession(DemoH264MediaSubsession::createNew(*env, inputName, false));
	

	rtspServer->addServerMediaSession(sms);
	
	char* url = rtspServer->rtspURL(sms);	
	*env <<  "URL:" << url << "\n";
	

	// loop and not come back~
	env->taskScheduler().doEventLoop();
	
	return 0;
}
