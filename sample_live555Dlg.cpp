/******************************************************
 * 浩一科技，提供云录播、云监控的全平台无插件解决方案。
 * 支持按需直播，多点布控，分布式海量存储，动态扩容；
 * 支持微信扫码登录，全平台帐号统一，关联微信小程序；
 * 支持多种数据输入：摄像头IPC、rtmp、rtsp、MP4文件；
 * 支持全实时、全动态、全网页管理，网页前后台兼容IE8；
 * 支持多终端无插件自适应播放，flvjs/hls/rtmp自动适配；
 ******************************************************
 * 官方网站 => https://myhaoyi.com
 * 技术博客 => http://blog.csdn.net/haoyitech
 * 开源代码 => https://github.com/HaoYiTech/
 * 云监控群 => 483663026（QQ群）
 * 云录播群 => 630379661（QQ群）
 ******************************************************
 * VS2010自动生成的App类实现代码
 ******************************************************/

#include "stdafx.h"
#include "sample_live555.h"
#include "sample_live555Dlg.h"
#include "afxdialogex.h"

#include "UtilTool.h"
#include "OSThread.h"
#include "SocketUtils.h"
#include "RTSPThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

Csample_live555Dlg::Csample_live555Dlg(CWnd* pParent /*=NULL*/)
  : CDialogEx(Csample_live555Dlg::IDD, pParent)
{
	m_lpRtspThread = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csample_live555Dlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_HOME_WEBSITE, m_ctrlHome);
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Csample_live555Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RTSP, &Csample_live555Dlg::OnBnClickedButtonRtsp)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL Csample_live555Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化网络、线程、套接字...
	WORD	wsVersion = MAKEWORD(2, 2);
	WSADATA	wsData	  = {0};
	(void)::WSAStartup(wsVersion, &wsData);
	OSThread::Initialize();
	SocketUtils::Initialize();

	// 设置访问连接地址...
	m_ctrlHome.SetUnderline(TRUE);
	m_ctrlHome.SetURL("https://myhaoyi.com");
	m_ctrlHome.SetWindowText("联系我们 => https://myhaoyi.com");
	m_ctrlHome.SetLinkCursor(CUtilTool::GetSysHandCursor());
	m_ctrlHome.SetAutoSize();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Csample_live555Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Csample_live555Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Csample_live555Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Csample_live555Dlg::OnBnClickedButtonRtsp()
{
	// 释放rtsp线程资源...
	if( m_lpRtspThread != NULL ) {
		delete m_lpRtspThread;
		m_lpRtspThread = NULL;
	}
	// 准备rtsp地址，创建新的线程资源...
	CString strUrl = "rtsp://admin:admin123@192.168.1.65/Streaming/Channels/101";
	m_lpRtspThread = new CRtspThread();
	m_lpRtspThread->InitThread(true, strUrl);
}

void Csample_live555Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// 释放rtsp线程资源...
	if( m_lpRtspThread != NULL ) {
		delete m_lpRtspThread;
		m_lpRtspThread = NULL;
	}
	// 释放一些公共资源...
	OSThread::UnInitialize();
	SocketUtils::UnInitialize();
}
