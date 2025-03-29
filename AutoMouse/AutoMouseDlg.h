
// AutoMouseDlg.h: 头文件
//

#pragma once
#include <vector>
#include <chrono>
#include <thread>
#include <string.h>
#include <atomic>
#include <conio.h>
#include "tinyxml2.h"

#define HOTKEY_ID 1001  // 热键ID
using namespace tinyxml2;

struct MouseInfo
{
	CPoint m_point;
	ULONGLONG m_time;
	CString m_title;
};

// CAutoMouseDlg 对话框
class CAutoMouseDlg : public CDialogEx
{
// 构造
public:
	CAutoMouseDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CAutoMouseDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTOMOUSE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//HICON m_hIcon;
	HHOOK m_hHook;
	bool m_isRecording;
	std::vector<MouseInfo> m_clickPoints;
	ULONGLONG m_lastClickTime;
	static CAutoMouseDlg* m_pThis;  // 静态指针，用于访问实例
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	CButton m_startOrStop;
	afx_msg void OnBnClickedCancel3();
	bool click(MouseInfo info);
	bool SaveMouseInfo();
	afx_msg void OnBnClickedCancel2();
	 bool LoadChooseFile();
	 virtual void OnCancel();
	 afx_msg void OnBnClickedCancel4();
	 void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

	 std::atomic<bool> m_stopClicking;
	 void AutoClickThred();
	 CStatic m_showChoose;
};

