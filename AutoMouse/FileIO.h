#pragma once
#include <vector>

struct MouseInfo
{
	CPoint m_point;
	int m_event;
	ULONGLONG m_time;
	CString m_title;
};

class FileIO
{
public:
	FileIO();
	~FileIO();
	virtual bool SaveFile(CString filename, std::vector<MouseInfo> points) = 0;
	virtual bool LoadFile(CString filename, std::vector<MouseInfo>& points) = 0;
};

