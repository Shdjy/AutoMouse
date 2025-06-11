#include "pch.h"
#include "TXTFileIO.h"

bool TXTFileIO::SaveFile(CString filename, std::vector<MouseInfo> points)
{
	CStdioFile file;
	if (file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		for (const auto& info : points)
		{
			CString line;
			line.Format(_T("X:%d Y:%d Event:%d Interval:%d Title:%s\r\n"),
				info.m_point.x,
				info.m_point.y,
				info.m_event,
				info.m_time,
				info.m_title);

			file.WriteString(line);
		}

		file.Close();
		return true;
	}
	else
	{
		//AfxMessageBox(_T("文件打开失败，无法保存数据！"));
		return false;
	}
}

bool TXTFileIO::LoadFile(CString filename, std::vector<MouseInfo>& points)
{
	CStdioFile file;
	if (!file.Open(filename, CFile::modeRead | CFile::typeText))
	{
		AfxMessageBox(_T("文件打开失败！"));
		return false;
	}

	CString line;
	while (file.ReadString(line))
	{
		MouseInfo info;
		int x = 0, y = 0, event = 0;
		ULONGLONG interval = 0;
		CString title;

		// 示例格式: X:100 Y:200 Event:1 Interval:1500 Title:记事本
		int posX = line.Find(_T("X:"));
		int posY = line.Find(_T("Y:"));
		int posE = line.Find(_T("Event:"));
		int posI = line.Find(_T("Interval:"));
		int posT = line.Find(_T("Title:"));

		if (posX == -1 || posY == -1 || posE == -1 || posI == -1 || posT == -1)
		{
			continue; // 跳过格式错误的行
		}

		CString strX = line.Mid(posX + 2, posY - posX - 2);
		CString strY = line.Mid(posY + 2, posE - posY - 2);
		CString strE = line.Mid(posE + 6, posI - posE - 6);
		CString strI = line.Mid(posI + 9, posT - posI - 9);
		CString strT = line.Mid(posT + 6);

		x = _ttoi(strX);
		y = _ttoi(strY);
		event = _ttoi(strE);
		interval = _ttoi64(strI); // 注意：使用64位转换函数
		title = strT;

		info.m_point.x = static_cast<LONG>(x);
		info.m_point.y = static_cast<LONG>(y);
		info.m_event = event;
		info.m_time = interval;
		info.m_title = title;

		points.push_back(info);
	}

	file.Close();
	//AfxMessageBox(_T("数据加载成功！"));
	return true;
}
