#include "pch.h"
#include "XMLFileIO.h"

bool XMLFileIO::SaveFile(CString filename, std::vector<MouseInfo> points)
{
	// 创建XML文档
	tinyxml2::XMLDocument doc;
	XMLElement* root = doc.NewElement("MouseData");
	doc.InsertFirstChild(root);

	for (const auto& info : points)
	{
		XMLElement* clickElement = doc.NewElement("Click");

		XMLElement* DXElement = doc.NewElement("DX");
		DXElement->SetText(info.m_downPoint.x);
		clickElement->InsertEndChild(DXElement);

		XMLElement* DYElement = doc.NewElement("DY");
		DYElement->SetText(info.m_downPoint.y);
		clickElement->InsertEndChild(DYElement);

		XMLElement* xElement = doc.NewElement("X");
		xElement->SetText(info.m_point.x);
		clickElement->InsertEndChild(xElement);

		XMLElement* yElement = doc.NewElement("Y");
		yElement->SetText(info.m_point.y);
		clickElement->InsertEndChild(yElement);

		XMLElement* eventElement = doc.NewElement("Event");
		eventElement->SetText(info.m_event);
		clickElement->InsertEndChild(eventElement);

		XMLElement* intervalElement = doc.NewElement("Interval");
		intervalElement->SetText(info.m_time);
		clickElement->InsertEndChild(intervalElement);

		XMLElement* titleElement = doc.NewElement("Title");
		titleElement->SetText(CT2A(info.m_title));
		clickElement->InsertEndChild(titleElement);

		root->InsertEndChild(clickElement);
	}

	if (doc.SaveFile(CT2A(filename)) == XML_SUCCESS)
	{
		//AfxMessageBox(_T("数据保存成功！"));
		//m_showChoose.SetWindowText(_T("当前操作：") + fileName);
		return true;
	}
	else
	{
		//AfxMessageBox(_T("保存失败！"));
		return false;
	}
}

bool XMLFileIO::LoadFile(CString filename, std::vector<MouseInfo>& points)
{
	tinyxml2::XMLDocument doc;

	if (doc.LoadFile(CT2A(filename)) != XML_SUCCESS)
	{
		AfxMessageBox(_T("文件打开失败或格式错误！"));
		return false;
	}


	XMLElement* root = doc.FirstChildElement("MouseData");
	if (!root)
	{
		AfxMessageBox(_T("XML文件格式错误！"));
		return false;
	}

	for (XMLElement* clickElement = root->FirstChildElement("Click");
		clickElement != nullptr;
		clickElement = clickElement->NextSiblingElement("Click"))
	{
		MouseInfo info;
		XMLElement* DXElement = clickElement->FirstChildElement("DX");
		XMLElement* DYElement = clickElement->FirstChildElement("DY");
		XMLElement* xElement = clickElement->FirstChildElement("X");
		XMLElement* yElement = clickElement->FirstChildElement("Y");
		XMLElement* eventElement = clickElement->FirstChildElement("Event");
		XMLElement* intervalElement = clickElement->FirstChildElement("Interval");
		XMLElement* titleElement = clickElement->FirstChildElement("Title");

		if (DXElement && DYElement && xElement && yElement && eventElement && intervalElement && titleElement)
		{
			int dx, dy, x, y;
			DXElement->QueryIntText(&dx);
			DYElement->QueryIntText(&dy);
			info.m_downPoint.x = static_cast<LONG>(dx);
			info.m_downPoint.y = static_cast<LONG>(dy);
			xElement->QueryIntText(&x);
			yElement->QueryIntText(&y);
			eventElement->QueryIntText(&info.m_event);
			intervalElement->QueryUnsigned64Text(&info.m_time);
			info.m_title = titleElement->GetText();

			// 将int值转换为LONG类型
			info.m_point.x = static_cast<LONG>(x);
			info.m_point.y = static_cast<LONG>(y);
			points.push_back(info);
		}
	}

	//AfxMessageBox(_T("数据加载成功！"));
	return true;
}
