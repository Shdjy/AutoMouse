
// AutoMouseDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AutoMouse.h"
#include "AutoMouseDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAutoMouseDlg 对话框

CAutoMouseDlg* CAutoMouseDlg::m_pThis = nullptr;

CAutoMouseDlg::CAutoMouseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AUTOMOUSE_DIALOG, pParent),
	m_isRecording(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pThis = this;
}

void CAutoMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_startOrStop);
}

BEGIN_MESSAGE_MAP(CAutoMouseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAutoMouseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL3, &CAutoMouseDlg::OnBnClickedCancel3)
END_MESSAGE_MAP()


// CAutoMouseDlg 消息处理程序

BOOL CAutoMouseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAutoMouseDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoMouseDlg::OnPaint()
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
HCURSOR CAutoMouseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CALLBACK CAutoMouseDlg::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && wParam == WM_LBUTTONDOWN)
	{
		MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
		if (pMouseStruct != nullptr && m_pThis != nullptr)
		{
			// 获取点击位置
			POINT pt = pMouseStruct->pt;
			ULONGLONG currentTime = GetTickCount64();  // 获取当前时间

			// 忽略点击MFC窗口本身
			HWND hWndClicked = WindowFromPoint(pt)->GetSafeHwnd();
			if (hWndClicked == m_pThis->GetSafeHwnd())
			{
				return CallNextHookEx(nullptr, nCode, wParam, lParam);
			}

			// 计算时间间隔
			ULONGLONG interval = (m_pThis->m_lastClickTime == 0) ? 0 : (currentTime - m_pThis->m_lastClickTime);

			// 更新上次点击时间
			m_pThis->m_lastClickTime = currentTime;

			// 记录点击数据
			MouseInfo info = { pt, interval };
			m_pThis->m_clickPoints.push_back(info);
		}
	}
	return CallNextHookEx(nullptr, nCode, wParam, lParam);
}


void CAutoMouseDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_isRecording)
	{
		m_hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, AfxGetInstanceHandle(), 0);
		if (m_hHook)
		{
			m_clickPoints.clear();
			m_isRecording = true;
			//MessageBox(_T("开始录制鼠标点击位置！"), _T("提示"), MB_OK);
			m_startOrStop.SetWindowText(_T("停止录制"));
		}
	}
	else
	{
		if (m_hHook != nullptr)
		{
			UnhookWindowsHookEx(m_hHook);
			m_hHook = nullptr;
			//AfxMessageBox(_T("停止录制鼠标点击！"));
			m_isRecording = false;
			//保存数据

			m_startOrStop.SetWindowText(_T("开始录制"));
		}
	}
	//CDialogEx::OnOK();
}


void CAutoMouseDlg::OnBnClickedCancel3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_clickPoints.size() > 0)
	{
		for (const auto& point : m_clickPoints)
		{
			click(point);
		}
	}
	else
	{
		AfxMessageBox(_T("没有录制任何鼠标点击！"));
	}
}

void CAutoMouseDlg::click(MouseInfo info)
{
	SetCursorPos(info.m_point.x, info.m_point.y);           // 移动鼠标到指定位置
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);  // 鼠标左键按下
	std::this_thread::sleep_for(std::chrono::milliseconds(info.m_time)); // 延迟
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);    // 鼠标左键松开
}
