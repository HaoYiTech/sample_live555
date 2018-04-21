#pragma once

#include "OSThread.h"
#include "myRTSPClient.h"

class CRtspThread : public OSThread
{
public:
	CRtspThread();
	virtual ~CRtspThread();
public:
	BOOL	InitThread(BOOL bUsingTCP, LPCTSTR lpszRtspUrl);
	void	ResetEventLoop() { m_rtspEventLoopWatchVariable = 1; }
public:
	virtual	void	Entry();
private:
	BOOL		m_bFinished;				// 网络流是否结束了
	string		m_strRtspUrl;				// rtsp连接地址
	TaskScheduler * m_scheduler_;			// rtsp需要的任务对象
	UsageEnvironment * m_env_;				// rtsp需要的环境
	ourRTSPClient * m_rtspClient_;			// rtsp对象
	char m_rtspEventLoopWatchVariable;		// rtsp退出标志 => 事件循环标志，控制它就可以控制任务线程...
};