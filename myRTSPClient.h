
#pragma once

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

void continueAfterOPTIONS(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);

void subsessionAfterPlaying(void* clientData); // called when a stream's subsession (e.g., audio or video substream) ends
void subsessionByeHandler(void* clientData); // called when a RTCP "BYE" is received for a subsession
void streamTimerHandler(void* clientData);

class StreamClientState
{
public:
	StreamClientState();
	virtual ~StreamClientState();
public:
	MediaSubsessionIterator * m_iter;
	MediaSession * m_session;
	MediaSubsession * m_subsession;
	TaskToken m_streamTimerTask;
	double m_duration;
};

class CRtspThread;
class CRtspRecThread;
class ourRTSPClient : public RTSPClient
{
public:
	static ourRTSPClient * createNew(UsageEnvironment& env, 
					char const* rtspURL,
					int verbosityLevel,
					char const* applicationName,
					BOOL bStreamUsingTCP);
protected: // called only by createNew();
	ourRTSPClient(UsageEnvironment& env, char const* rtspURL, int verbosityLevel,
				  char const* applicationName, BOOL bStreamUsingTCP);
	virtual ~ourRTSPClient();
public:
	StreamClientState	m_scs;
public:
	void WriteSample(bool bIsVideo, string & inFrame, DWORD inTimeStamp, DWORD inRenderOffset, bool bIsKeyFrame);
	void myAfterOPTIONS(int resultCode, char* resultString);
	void myAfterDESCRIBE(int resultCode, char* resultString);
	void myAfterSETUP(int resultCode, char* resultString);
	void myAfterPLAY(int resultCode, char* resultString);
	void setupNextSubsession();
	void shutdownStream();
public:
	BOOL				m_bUsingTCP;			// 是否使用TCP获取数据
	BOOL				m_bHasVideo;			// 视频有效标志
	BOOL				m_bHasAudio;			// 音频有效标志
};

// Define a data sink (a subclass of "MediaSink") to receive the data for each subsession (i.e., each audio or video 'substream').
// In practice, this might be a class (or a chain of classes) that decodes and then renders the incoming audio or video.
// Or it might be a "FileSink", for outputting the received data into a file (as is done by the "openRTSP" application).
// In this example code, however, we define a simple 'dummy' sink that receives incoming data, but does nothing with it.
class DummySink: public MediaSink
{
public:
	static DummySink* createNew(UsageEnvironment& env,
							MediaSubsession& subsession, // identifies the kind of data that's being received
							char const* streamId,		 // identifies the stream itself (optional)
							ourRTSPClient * lpRtspClient);
private:
    // called only by "createNew()"
	DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId, ourRTSPClient * lpRtspClient);
	virtual ~DummySink();
	
	static void afterGettingFrame(void* clientData, unsigned frameSize,
								  unsigned numTruncatedBytes,
								  struct timeval presentationTime,
								  unsigned durationInMicroseconds);
	void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
					struct timeval presentationTime, unsigned durationInMicroseconds);
	
private: // redefined virtual functions:
	virtual Boolean continuePlaying();
private:
	char			*	fStreamId;				// 流地址
	u_int8_t		*	fReceiveBuffer;			// 帧缓冲区
	MediaSubsession &	fSubsession;			// 会话引用对象
	ourRTSPClient	*	fRtspClient;			// RTSP客户端对象
	ULARGE_INTEGER		m_llTimCountFirst;		// 音视频的0点时间 => 单位是0.1微妙...
	timeval				fSTimeVal;
};