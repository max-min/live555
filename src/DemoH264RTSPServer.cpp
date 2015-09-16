
#include "DemoH264RTSPServer.h"


DemoH264RTSPServer* DemoH264RTSPServer::createNew(UsageEnvironment& env,  Port rtspPort, 
		UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
{
	int rtspSock = -1;

	rtspSock = setUpOurSocket(env, rtspPort);
	if(rtspSock == -1 )
		return NULL;
	return new DemoH264RTSPServer(env, rtspSock, rtspPort, authDatabase, reclamationTestSeconds);
	
}


DemoH264RTSPServer::DemoH264RTSPServer(UsageEnvironment& env, int ourSock, Port rtspPort, UserAuthenticationDatabase* authDatabase,
		unsigned reclamationTestSeconds):RTSPSever(env, ourSock, rtspPort, authDatabase, reclamationTestSeconds), fRTSPServerState(true)
{
	 DBG_LIVE555_PRINT("create DemoH264RTSPServer \n");
}

DemoH264RTSPServer::~DemoH264RTSPServer()
{

}


ServerMediaSession* DemoH264RTSPServer::lookupServerMediaSession(const char* streamName)
{
	// streamName, 为URL地址后面的字符串 如 
	// rtsp://10.0.2.15/streamNameCH00StreamType00, 则streamName = "streamNameCH00StreamType00";
	// 当客户端发来url请求时，可以解析streamName来判断请求那个通道的哪种码流

	// 1 解析url 我这里不处理，可以自己回调接口进来处理
	int channelNO   = 0;  // 通道号
	int streamType  = 0;  // 码流类型
	int videoType   = 0;  // 视频 or 音频
	int requestType = 0;  // 请求类型 实时预览 or 回放

	
	ServerMediaSession* sms = NULL;
	sms = RTSPServer::lookupServerMediaSession(streamName);
	if ( NULL == sms )
	{
		switch(requestType)
		{
		case 0:  // realtime 
			sms = ServerMediaSession::createNew(envir(), streamName, NULL, NULL);
			DemoH264MediaSubsession *session = createNew(envir(), streamType, videoType, channelNO,false);

			sms->addSubsession(session);

			this->addServerMediaSession(sms);
			break;
		case 1: // play back
			break;
				
			
		}
	}
}


DemoH264RTSPServer::DemoH264RTSPClientSession* DemoH264RTSPServer::DemoH264RTSPClientSession::createNew(unsigned clientSessionID)
{
	DemoH264RTSPServer::DemoH264RTSPClientSession* client = new DemoH264RTSPServer::DemoH264RTSPClientSession(*this, clientSessionID);
	fClientSessionList.push_back(client);
	DBG_LIVE555_PRINT("add client session success!\n");
	return client;
}

int DemoH264RTSPServer::stopDemoH264RTSPServer()
{
	// 善后处理
	if(NULL == param)
	{
		printf(" param error !\n");
		return -1;
	}
	// 删除所有的客户端的session
	std::list<DemoH264RTSPServer::DemoH264RTSPClientSession*> ::iterator pos =
		this->fClientSessionList.begin();
	for(pos; pos != this->fClientSessionList.end(); pos ++ )
	{
		DemoH264RTSPServer::DemoH264RTSPClientSession* tmp = *pos;
		delete tmp;
	}

	delete this; //

	return 0;
	
}

DemoH264RTSPServer::DemoH264RTSPClientSession::DemoH264RTSPClientSession(DemoH264RTSPServer& rtspServer, 
	unsigned clietnSessionID):RTSPServer::RTSPClientSession(rtspServer, clietnSessionID);
{
}

DemoH264RTSPServer::DemoH264RTSPClientSession::~DemoH264RTSPClientSession()
{
	std::list<DemoH264RTSPServer::DemoH264RTSPClientSession*> ::iterator pos =
		(DemoH264RTSPServer&)fOurServer.fClientSessionList.begin();
	for(pos; pos != (DemoH264RTSPServer&)fOurServer.fClientSessionList.end(); pos ++ )
	{
		if ((*pos)->fOurSessionId == this->fOurSessionId) 
		{
			(DemoH264RTSPServer&)fOurServer.fClientSessionList.erase(pos);
			DBG_LIVE555_PRINT("client session has been delete !\n");
			break ;
		}
	}
}

