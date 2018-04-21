
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
	// 设置rtsp循环退出标志...
	m_rtspEventLoopWatchVariable = 1;
	// 停止线程...
	this->StopAndWaitForThread();
	// 打印退出信息...
	TRACE("[~CRtspThread Thread] - Exit\n");
}

BOOL CRtspThread::InitThread(BOOL bUsingTCP, LPCTSTR lpszRtspUrl)
{
	// 判断并保存rtsp拉流地址...
	if( lpszRtspUrl == NULL || strlen(lpszRtspUrl) <= 0 )
		return false;
	m_strRtspUrl = lpszRtspUrl;

	// 创建rtsp链接环境...
	m_scheduler_ = BasicTaskScheduler::createNew();
	m_env_ = BasicUsageEnvironment::createNew(*m_scheduler_);
	m_rtspClient_ = ourRTSPClient::createNew(*m_env_, m_strRtspUrl.c_str(), 1, "rtspThread", bUsingTCP);

	// 2017.07.21 - by jackey => 有些服务器必须先发OPTIONS...
	// 发起第一次rtsp握手 => 先发起 OPTIONS 命令...
	m_rtspClient_->sendOptionsCommand(continueAfterOPTIONS); 

	// 启动线程...
	this->Start();

	return true;
}

void CRtspThread::Entry()
{
	// 进行任务循环检测，修改 m_rtspEventLoopWatchVariable 可以让任务退出...
	ASSERT( m_env_ != NULL && m_rtspClient_ != NULL );
	m_env_->taskScheduler().doEventLoop(&m_rtspEventLoopWatchVariable);

	// 设置数据结束标志...
	m_bFinished = true;

	// 任务退出之后，再释放rtsp相关资源...
	// 只能在这里调用 shutdownStream() 其它地方不要调用...
	if( m_rtspClient_ != NULL ) {
		m_rtspClient_->shutdownStream();
		m_rtspClient_ = NULL;
	}
	// 释放任务对象...
	if( m_scheduler_ != NULL ) {
		delete m_scheduler_;
		m_scheduler_ = NULL;
	}
	// 释放环境变量...
	if( m_env_ != NULL ) {
		m_env_->reclaim();
		m_env_ = NULL;
	}
}
