#include "DemoH264Interface.h"
#include "DemoH264RTSPServer.h"

/*打开实时码流句柄*/
long openStreamHandle(int channelNO, int streamType)
{
	//开始实时流的一些准备工作:获取此类型实时码流的句柄，方便后面直接get码流

	// 我这里测试，所以还是用自己定义的文件码流来读，不过每次都是读一帧数据
	// 文件流格式为 FrameHeader_S + H264 + FrameHeader_S + H264 ...
	FILE* fp = fopen("stream264file.h264", "rb+");
	if (NULL == fp )
	{
		DBG_LIVE555_PRINT("open streamhandle failed!\n");
		return -1;
	}

	return (long)fp;
		
}
/*实时获取一帧数据*/
int getStreamData(long lHandle, char* buf, unsigned* bufsize, unsigned* leftbufsize, int sourcetype)
{
	if(lHandle <= 0)
	{
		DBG_LIVE555_PRINT(" lHandle error !\n");
		return -1;
	}
	FrameHead_S stFrameHead;
	memset(&stFrameHead, 0, sizeof(FrameHead_S));
	FILE* fp = (FILE*)lHandle;
	int readlen = 0;
	// 1、先读取一帧数据的头信息
	readlen = fread(&stFrameHead, 1, sizeof(FrameHead_S), fp);
	if( readlen != sizeof(FrameHead_S))
	{
		DBG_LIVE555_PRINT(" read Frame Header Failed !\n");
		return -1;
	}

	//2、获取一帧H264实时数据
	if(stFrameHead.FrameLen > 2*1024*1024) // 在source中databuf指分配了2M
	{
		DBG_LIVE555_PRINT("data is too long:framlen=%d\n", stFrameHead.FrameLen);
		//重新分配内存处理
		return 0;
	}
	readlen = fread(buf, 1, stFrameHead.FrameLen, fp);
	if(readlen != stFrameHead.FrameLen)
	{
		DBG_LIVE555_PRINT("read Frame rawdata Failed!\n");
		return -1;
	}

	return stFrameHead.FrameLen;
}
/*关闭码流句柄*/
void closeStreamHandle(long lHandle)
{
	//一些关闭码流的清理工作
	fclose((FILE*)lHandle);
}


DemoH264Interface* DemoH264Interface::m_Instance = NULL;

DemoH264Interface* DemoH264Interface::createNew()
{
	if(NULL == m_Instance)
	{
		m_Instance = new DemoH264Interface();
	}

	return m_Instance;
}


DemoH264Interface::DemoH264Interface()
{
	m_liveServerFlag = false;
}

DemoH264Interface::~DemoH264Interface()
{
}

void DemoH264Interface::InitLive555(void *param)
{
	//初始化
		
	DBG_LIVE555_PRINT(" ~~~~live555 stream start\n");
	
	// Begin by setting up the live555 usage environment 
	m_scheduler = BasicTaskScheduler::createNew();
	m_env = BasicUsageEnvironment::createNew(*m_scheduler);
		
#if ACCESS_CONTROL   // 认证
	m_authDB = new UserAuthenticationDatabase;
	m_authDB->addUserRecord("username", "password");
#endif 

	m_rtspServer = NULL;
	m_rtspServerPortNum = 554; // 可以修改
	m_liveServerFlag = true;
}


int DemoH264Interface::startLive555()
{
	if( !m_liveServerFlag)
	{
		DBG_LIVE555_PRINT("Not Init the live server !\n");
		return -1; 
	}

	// 建立RTSP服务
	m_rtspServer = DemoH264RTSPServer::createNew(*m_env, m_rtspServerPortNum, m_authDB);
	
	if( m_rtspServer == NULL)
	{
		// *m_env << " create RTSPServer Failed:" << m_env->getResultMsg() << "\n";
		DBG_LIVE555_PRINT("create RTSPServer Failed:%s\n", m_env->getResultMsg());
		return -1;
	}


	// loop and not come back~
	m_env->taskScheduler().doEventLoop();
	
	return 0;
}

int DemoH264Interface::stopLive555()
{
	if(m_liveServerFlag)
	{
		if(m_rtspServer)
			m_rtspServer->stopDemoH264RTSPServer();
		m_liveServerFlag = false;	
	}
}




