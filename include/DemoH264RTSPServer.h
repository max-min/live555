/*
 * =====================================================================================
 *
 *       Filename:  DemoH264RTSPServer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年09月14日 22时26分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __DEMOH264RTSPSERVER_H__
#define __DEMOH264RTSPSERVER_H__

#include <list>
#include "RTSPServer.hh"

class DemoH264RTSPServer:public RTSPServer
{
	public:
		static DemoH264RTSPServer* createNew(UsageEnvironment& env,  Port rtspPort = 554, 
			UserAuthenticationDatabase* authDatabase = NULL, unsigned reclamationTestSeconds = 65);
		
	protected:
		DemoH264RTSPServer(UsageEnvironment& env, int ourSock, Port rtspPort, UserAuthenticationDatabase* authDatabase,
			unsigned reclamationTestSeconds);
		virtual ~DemoH264RTSPServer();
		
		class DemoH264RTSPClientSession:public RTSPServer::RTSPClientSession
		{
			public:
				DemoH264RTSPClientSession(DemoH264RTSPServer& rtspServer, unsigned clientSessionID, int clientSocket, struct sockaddr_in clientAddr);
				virtual ~DemoH264RTSPClientSession();
		};
	public:
		// redefine
		virtual ServerMediaSession* lookupServerMediaSession(const char* streamName);
		virtual DemoH264RTSPServer::DemoH264RTSPClientSession* createNewClientSession(unsigned clietnSessionID, int clientSocket, struct sockaddr_in clientAddr);

	    int stopDemoH264RTSPServer();
	public:
		std::list<DemoH264RTSPServer::DemoH264RTSPClientSession*>  fClientSessionList;
		bool fRTSPServerState; //RTSPServer 运行状态	
};




#endif // DemoH264RTSPServer.h
