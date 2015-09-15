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


/* 获取码流接口函数，可以设置为回调可能会更方便*/

long openStreamHandle(int channelNO, int streamType);
int getStreamData(long lHandle, char* buf, unsigned* bufsize, unsigned* leftbufsize, int sourcetype);
void closeStreamHandle(long lHandle);

class DemoH264Interface 
{
	public:
		static DemoH264Interface* createNew();
		static int CloseInterface();
	
		void InitLive555();
		void startLive555();
		void stopLive555();
		
	private:
		DemoH264Interface();
		~DemoH264Interface();
	public:
		static DemoH264Interface* m_Instance;
		
};


#endif // DemoH264Interface.h 
