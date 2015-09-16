/*
 * =====================================================================================
 *
 *       Filename:  DemoH264Interface.h
 *
 *    Description:  外部调用live555的接口管理类 
 *
 *        Version:  1.0
 *        Created:  2015年09月15日 23时32分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization: max_min_, 
 *
 * =====================================================================================
 */
#ifndef __DEMOH264INTERFACE_H__
#define __DEMOH264INTERFACE_H__
#include <stdio.h>

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"

#ifdef DEBUG 
#define DBG_LIVE555_PRINT(pFmt, ...) \
	do \
	{\
		fprintf(stderr, "[Live555_DBG]-[%s %d]:"pFmt, __FILE__, __LINE__, ##__VA_ARGS__);\
		fflush(stderr);\
	}while(0)
#else 
#define DBG_LIVE555_PRINT(pFmt,...)  
#endif 

// 实时码流文件中数据帧头信息 
typedef struct FrameHead_S_
{
	unsigned short FrameFlag;		//0x3448 magic data
	unsigned char  FrameType;		//I-0x7 p--0x8 b--0xb A--0xa
	unsigned char  Res[1];			//保留
	unsigned int   FrameNo;			//帧号
	unsigned int   SysTime;			//系统时间
	unsigned int   FrameLen;		//数据流长度
	unsigned long long TimeStamp;		//时间戳
	
	unsigned short ImageWidth;	//图像宽度
	unsigned short ImageHeight;	//图像高度
	unsigned char  EncodeType;	//视频编码格式0-h264 1-mjpeg 2-jpeg
	unsigned char  FrameRate;	//帧率(保留)
	unsigned char  Pal;			//制式		0-n制1-pal制
	unsigned char  Res2[1];
}FrameHead_S;


/* 获取码流接口函数，可以设置为回调可能会更方便*/
long openStreamHandle(int channelNO, int streamType);
int getStreamData(long lHandle, char* buf, unsigned* bufsize, unsigned* leftbufsize, int sourcetype);
void closeStreamHandle(long lHandle);

class DemoH264Interface 
{
	public:
		static DemoH264Interface* createNew();
		static int CloseInterface();
	
		void InitLive555(void *param);
		int  startLive555();
		int  stopLive555();
		
	private:
		DemoH264Interface();
		~DemoH264Interface();
	public:
		static DemoH264Interface* m_Instance;
	private:
		//live555
		bool m_liveServerFlag;
		TaskScheduler* m_scheduler;
		UsageEnvironment* m_env;
		UserAuthenticationDatabase* m_authDB;
		DemoH264RTSPServer* m_rtspServer;
		portNumBits m_rtspServerPortNum; // rtsp port 
		
};


#endif // DemoH264Interface.h 
