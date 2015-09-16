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

#include "DemoH264Interface.h"

int main(int argc, char* argv[])
{

	// Init
	// 添加一些需要设置的rtsp服务信息，如用户名，密码 端口等，通过参数传递
	void* param = NULL;
	DemoH264Interface::createNew()->InitLive555(param);

	// start 
	if( -1 == DemoH264Interface::createNew()->startLive555())
	{
		DBG_LIVE555_PRINT(" start live555 moudle failed!\n");
		return 0;
	}

	//stop 
	DemoH264Interface::createNew()->stopLive555();
	return 0;
}
