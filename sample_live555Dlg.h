/******************************************************
 * ��һ�Ƽ����ṩ��¼�����Ƽ�ص�ȫƽ̨�޲�����������
 * ֧�ְ���ֱ������㲼�أ��ֲ�ʽ�����洢����̬���ݣ�
 * ֧��΢��ɨ���¼��ȫƽ̨�ʺ�ͳһ������΢��С����
 * ֧�ֶ����������룺����ͷIPC��rtmp��rtsp��MP4�ļ���
 * ֧��ȫʵʱ��ȫ��̬��ȫ��ҳ������ҳǰ��̨����IE8��
 * ֧�ֶ��ն��޲������Ӧ���ţ�flvjs/hls/rtmp�Զ����䣻
 ******************************************************
 * �ٷ���վ => https://myhaoyi.com
 * �������� => http://blog.csdn.net/haoyitech
 * ��Դ���� => https://github.com/HaoYiTech/
 * �Ƽ��Ⱥ => 483663026��QQȺ��
 * ��¼��Ⱥ => 630379661��QQȺ��
 ******************************************************
 * VS2010�Զ����ɵĶԻ�����ͷ�ļ�
 ******************************************************/

#pragma once

#include "HyperLink.h"

class CRtspThread;
class Csample_live555Dlg : public CDialogEx
{
public:
	Csample_live555Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	enum { IDD = IDD_SAMPLE_LIVE555_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonRtsp();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	HICON			m_hIcon;
	CHyperLink		m_ctrlHome;
	CRtspThread	 *	m_lpRtspThread;
public:
	afx_msg void OnDestroy();
};
