
#include "stdafx.h"
#include "RTSPThread.h"

CRtspThread::CRtspThread()
{
	m_env_ = NULL;
	m_bFinished = false;
	m_scheduler_ = NULL;
	m_rtspClient_ = NULL;
	m_rtspEventLoopWatchVariable = 0;
}

CRtspThread::~CRtspThread()
{
	// ����rtspѭ���˳���־...
	m_rtspEventLoopWatchVariable = 1;
	// ֹͣ�߳�...
	this->StopAndWaitForThread();
	// ��ӡ�˳���Ϣ...
	TRACE("[~CRtspThread Thread] - Exit\n");
}

BOOL CRtspThread::InitThread(BOOL bUsingTCP, LPCTSTR lpszRtspUrl)
{
	// �жϲ�����rtsp������ַ...
	if( lpszRtspUrl == NULL || strlen(lpszRtspUrl) <= 0 )
		return false;
	m_strRtspUrl = lpszRtspUrl;

	// ����rtsp���ӻ���...
	m_scheduler_ = BasicTaskScheduler::createNew();
	m_env_ = BasicUsageEnvironment::createNew(*m_scheduler_);
	m_rtspClient_ = ourRTSPClient::createNew(*m_env_, m_strRtspUrl.c_str(), 1, "rtspThread", bUsingTCP);

	// 2017.07.21 - by jackey => ��Щ�����������ȷ�OPTIONS...
	// �����һ��rtsp���� => �ȷ��� OPTIONS ����...
	m_rtspClient_->sendOptionsCommand(continueAfterOPTIONS); 

	// �����߳�...
	this->Start();

	return true;
}

void CRtspThread::Entry()
{
	// ��������ѭ����⣬�޸� m_rtspEventLoopWatchVariable �����������˳�...
	ASSERT( m_env_ != NULL && m_rtspClient_ != NULL );
	m_env_->taskScheduler().doEventLoop(&m_rtspEventLoopWatchVariable);

	// �������ݽ�����־...
	m_bFinished = true;

	// �����˳�֮�����ͷ�rtsp�����Դ...
	// ֻ����������� shutdownStream() �����ط���Ҫ����...
	if( m_rtspClient_ != NULL ) {
		m_rtspClient_->shutdownStream();
		m_rtspClient_ = NULL;
	}
	// �ͷ��������...
	if( m_scheduler_ != NULL ) {
		delete m_scheduler_;
		m_scheduler_ = NULL;
	}
	// �ͷŻ�������...
	if( m_env_ != NULL ) {
		m_env_->reclaim();
		m_env_ = NULL;
	}
}
