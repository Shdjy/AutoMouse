
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
	m_isRecording(false),
	m_stopClicking(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pThis = this;
}

CAutoMouseDlg::~CAutoMouseDlg()
{
	UnregisterHotKey(m_hWnd, HOTKEY_ID);
}

void CAutoMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_startOrStop);
	DDX_Control(pDX, IDC_STATIC_TIT, m_showChoose);
	DDX_Control(pDX, IDCANCEL3, m_execute);
}

BEGIN_MESSAGE_MAP(CAutoMouseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAutoMouseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL3, &CAutoMouseDlg::OnBnClickedCancel3)
	ON_BN_CLICKED(IDCANCEL2, &CAutoMouseDlg::OnBnClickedCancel2)
	ON_BN_CLICKED(IDCANCEL4, &CAutoMouseDlg::OnBnClickedCancel4)
	ON_WM_HOTKEY()
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
	if (!RegisterHotKey(m_hWnd, HOTKEY_ID, MOD_CONTROL, 'Q'))
	{
		AfxMessageBox(_T("热键注册失败！"));
	}
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
	if (nCode >= 0 && (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP))
	{
		MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
		if (pMouseStruct != nullptr && m_pThis != nullptr)
		{
			POINT pt = pMouseStruct->pt;
			if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
			{
				m_pThis->m_downPoint = pt;
				return CallNextHookEx(nullptr, nCode, wParam, lParam);
			}

			// 忽略点击MFC窗口本身
			HWND hWndClicked = ::WindowFromPoint(pt);
			if (hWndClicked == m_pThis->GetSafeHwnd() || ::IsChild(m_pThis->GetSafeHwnd(), hWndClicked))
			{
				return CallNextHookEx(nullptr, nCode, wParam, lParam);
			}
			int event = wParam;

			ULONGLONG currentTime = GetTickCount();  // 获取当前时间
			CString title;
			TCHAR windowTitle[256];
			::GetWindowText(hWndClicked, windowTitle, 256);
			title = windowTitle;
			// 计算时间间隔
			ULONGLONG interval = (m_pThis->m_lastClickTime == 0) ? 0 : (currentTime - m_pThis->m_lastClickTime);
			if (m_pThis->m_clickPoints.size() == 0)
			{
				interval = 0;
			}
			else
			{
				if (event == WM_LBUTTONUP)
				{
					if (interval < GetDoubleClickTime() && abs(pt.x - m_pThis->m_clickPoints.back().m_point.x) < GetSystemMetrics(SM_CXDOUBLECLK)
						&& abs(pt.y - m_pThis->m_clickPoints.back().m_point.y) < GetSystemMetrics(SM_CYDOUBLECLK))
					{
						interval += m_pThis->m_clickPoints.back().m_time;
						m_pThis->m_clickPoints.pop_back();
						event = WM_LBUTTONDBLCLK;
					}
				}
			}
			if (event == WM_LBUTTONUP)
			{
				if (abs(pt.x - m_pThis->m_downPoint.x) > GetSystemMetrics(SM_CXDOUBLECLK)
					&& abs(pt.y - m_pThis->m_downPoint.y) > GetSystemMetrics(SM_CYDOUBLECLK))
				{
					event = MOVE;
				}
			}
			// 更新上次点击时间
			m_pThis->m_lastClickTime = currentTime;

			// 记录点击数据
			MouseInfo info = { m_pThis->m_downPoint, pt, event, interval, title };
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
			if (SaveMouseInfo())
			{
				m_showChoose.SetWindowText(m_showText);
				AfxMessageBox(_T("数据保存成功！"));
			}
			else
			{
				AfxMessageBox(_T("数据保存失败！"));
			}
			m_startOrStop.SetWindowText(_T("开始录制"));
		}
	}
	//CDialogEx::OnOK();
}


void CAutoMouseDlg::OnBnClickedCancel3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_stopClicking = false;  // 重置停止标志

	// 创建点击线程
	std::thread clickThread(&CAutoMouseDlg::AutoClickThred, this);
	clickThread.detach();
}

bool CAutoMouseDlg::click(MouseInfo info)
{
	SetCursorPos(info.m_downPoint.x, info.m_downPoint.y);           // 移动鼠标到指定位置
	switch (info.m_event)
	{
	case WM_LBUTTONUP:
		// 左键单击
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;

	case WM_LBUTTONDBLCLK:
		// 左键双击 = 两次快速单击
		for (int i = 0; i < 2; ++i)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 间隔需足够短
		}
		break;

	case WM_RBUTTONUP:
		// 右键单击
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
		break;

	case MOVE:
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		SetCursorPos(info.m_point.x, info.m_point.y);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;

	default:
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;
	}

	return true;
}

bool CAutoMouseDlg::SaveMouseInfo()
{
	if (m_clickPoints.size() > 0)
	{
		CFileDialog dlg(FALSE, _T("xml"), _T("MouseData.xml"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("支持文件 (*.xml;*.txt)|*.xml;*.txt|XML (*.xml)|*.xml|TXT (*.txt)|*.txt||"));
		if (dlg.DoModal() == IDOK)
		{
			CString filePath = dlg.GetPathName();
			CString fileName = dlg.GetFileName();
			std::unique_ptr<FileIO> fileIO = GetFileExtension(fileName);
			m_showText = _T("操作：") + fileName;
			return fileIO->SaveFile(filePath, m_clickPoints);
		}
	}
	return false;
}


void CAutoMouseDlg::OnBnClickedCancel2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_hHook != nullptr)
	{
		UnhookWindowsHookEx(m_hHook);
		m_hHook = nullptr;
		//AfxMessageBox(_T("停止录制鼠标点击！"));
		m_isRecording = false;
		m_clickPoints.clear();
		m_startOrStop.SetWindowText(_T("开始录制"));
	}
}

bool CAutoMouseDlg::LoadChooseFile()
{
	// 创建文件打开对话框
	CFileDialog dlg(TRUE, _T("xml"), nullptr,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("支持文件 (*.xml;*.txt)|*.xml;*.txt|XML (*.xml)|*.xml|TXT (*.txt)|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		CString fileName = dlg.GetFileName();
		m_clickPoints.clear();  // 清空原有数据

		std::unique_ptr<FileIO> fileIO = GetFileExtension(fileName);
		m_showText = _T("操作：") + fileName;
		return fileIO->LoadFile(filePath, m_clickPoints);
	}
	return false;
}


std::unique_ptr<FileIO> CAutoMouseDlg::GetFileExtension(const CString& fileName)
{
	int dotPos = fileName.ReverseFind('.');
	if (dotPos != -1 && dotPos < fileName.GetLength() - 1)
	{
		if (fileName.Mid(dotPos + 1).MakeLower() == _T("xml"))// 转换为小写
		{
			return std::unique_ptr<FileIO>(new XMLFileIO());
		}
		else if (fileName.Mid(dotPos + 1).MakeLower() == _T("txt"))
		{
			return std::unique_ptr<FileIO>(new TXTFileIO());
		}
	}
	return nullptr;
}


void CAutoMouseDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}

void CAutoMouseDlg::OnBnClickedCancel4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (LoadChooseFile())
	{
		m_showChoose.SetWindowText(m_showText);
	}
}

void CAutoMouseDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (nHotKeyId == HOTKEY_ID)  // 判断是否为自定义热键
	{
		m_stopClicking = true;  // 设置停止标志
		g_cv.notify_all();
	}
}

void CAutoMouseDlg::AutoClickThred()
{
	if (m_clickPoints.size() > 0)
	{
		m_execute.SetWindowText(_T("执行中"));
		for (size_t i = 0; i < m_clickPoints.size(); i++)//(const auto& point : m_clickPoints)
		{
			if (m_stopClicking)  // 检测停止标志
			{
				m_execute.SetWindowText(_T("执行操作"));
				AfxMessageBox(_T("点击操作已中断！"));
				return;
			}

			click(m_clickPoints[i]);
			std::unique_lock<std::mutex> lock(g_mtx);
			if (g_cv.wait_for(lock,
				std::chrono::milliseconds(m_clickPoints[(i + 1) % m_clickPoints.size()].m_time),
				[&]() { return m_stopClicking.load(); }))
			{
				m_execute.SetWindowText(_T("执行操作"));
				AfxMessageBox(_T("点击操作已中断！"));
				return;
			}
		}
		//AfxMessageBox(_T("点击操作完成！"));
		m_execute.SetWindowText(_T("执行操作"));
	}
	else
	{
		AfxMessageBox(_T("没有录制任何鼠标点击！"));
	}
}
