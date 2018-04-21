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
	BOOL		m_bFinished;				// �������Ƿ������
	string		m_strRtspUrl;				// rtsp���ӵ�ַ
	TaskScheduler * m_scheduler_;			// rtsp��Ҫ���������
	UsageEnvironment * m_env_;				// rtsp��Ҫ�Ļ���
	ourRTSPClient * m_rtspClient_;			// rtsp����
	char m_rtspEventLoopWatchVariable;		// rtsp�˳���־ => �¼�ѭ����־���������Ϳ��Կ��������߳�...
};