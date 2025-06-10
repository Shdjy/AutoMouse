#pragma once
#include "FileIO.h"
class XMLFileIO :
    public FileIO
{
	bool SaveFile(CString filename, std::vector<MouseInfo> points) override;
	bool LoadFile(CString filename, std::vector<MouseInfo>& points)override;
};

