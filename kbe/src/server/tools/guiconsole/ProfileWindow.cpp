// ProfileWindow.cpp : implementation file
//

#include "stdafx.h"
#include "guiconsole.h"
#include "ProfileWindow.h"
#include "TimingLengthWindow.h"
#include "guiconsole.h"
#include "guiconsoleDlg.h"

#include "baseapp/baseapp_interface.hpp"
#include "cellapp/cellapp_interface.hpp"
// CProfileWindow dialog

IMPLEMENT_DYNAMIC(CProfileWindow, CDialog)

std::string profilename;

CProfileWindow::CProfileWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CProfileWindow::IDD, pParent)
{

}

CProfileWindow::~CProfileWindow()
{
}

void CProfileWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_pyprofile);
	DDX_Control(pDX, IDC_BUTTON2, m_cprofile);
	DDX_Control(pDX, IDC_BUTTON3, m_eventprofile);
	DDX_Control(pDX, IDC_LIST1, m_profileShowList);
	DDX_Control(pDX, IDC_RESULT, m_results);
}

BOOL CProfileWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = m_profileShowList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;					//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;						//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_ONECLICKACTIVATE;
	m_profileShowList.SetExtendedStyle(dwStyle);				//设置扩展风格

	std::stringstream ss;
	ss << KBEngine::genUUID64();
	ss << (DWORD)this;
	profilename = ss.str();

	int idx = 0;
	m_profileShowList.InsertColumn(idx++, _T("ncalls "),					LVCFMT_CENTER,	50);
	m_profileShowList.InsertColumn(idx++, _T("tottime"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("percall"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("cumtime"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("percall"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("filename:lineno(function)"),	LVCFMT_CENTER,	300);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProfileWindow::autoWndSize()
{
	CRect rect;
	GetClientRect(&rect);
	m_pyprofile.MoveWindow(2, int(rect.bottom * 0.95), rect.right / 3, int(rect.bottom * 0.05), TRUE);
	m_cprofile.MoveWindow(int(rect.right * 0.33) + 3, int(rect.bottom * 0.95), rect.right / 3, int(rect.bottom * 0.05), TRUE);
	m_eventprofile.MoveWindow(int(rect.right * 0.66) + 3, int(rect.bottom * 0.95), rect.right / 3, int(rect.bottom * 0.05), TRUE);
	
	m_profileShowList.MoveWindow(2, 3, rect.right, rect.bottom - int(rect.bottom * 0.05) - 5, TRUE);
	m_results.MoveWindow(2, 3, rect.right, rect.bottom - int(rect.bottom * 0.05) - 5, TRUE);
}


BEGIN_MESSAGE_MAP(CProfileWindow, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CProfileWindow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CProfileWindow::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CProfileWindow::OnBnClickedButton3)
END_MESSAGE_MAP()


// CProfileWindow message handlers

void CProfileWindow::OnBnClickedButton1()
{
	m_pyprofile.EnableWindow(FALSE);
	m_cprofile.EnableWindow(FALSE);
	m_eventprofile.EnableWindow(FALSE);

	CguiconsoleDlg* dlg = static_cast<CguiconsoleDlg*>(theApp.m_pMainWnd);

	CTimingLengthWindow wnd;
	wnd.m_profileName = profilename;
	if(wnd.DoModal() == IDOK)
	{
		if(dlg->startProfile(wnd.m_profileName, 0, wnd.m_timingLength))
		{
			m_results.SetWindowText(L"please waiting for the result.");
			return;
		}
	}

	m_pyprofile.EnableWindow(TRUE);
	m_cprofile.EnableWindow(TRUE);
	m_eventprofile.EnableWindow(TRUE);
	::AfxMessageBox(L"please select the baseapp|cellapp.");
}

void CProfileWindow::OnBnClickedButton2()
{
	m_pyprofile.EnableWindow(FALSE);
	m_cprofile.EnableWindow(FALSE);
	m_eventprofile.EnableWindow(FALSE);
	
	CguiconsoleDlg* dlg = static_cast<CguiconsoleDlg*>(theApp.m_pMainWnd);

	CTimingLengthWindow wnd;
	wnd.m_profileName = profilename;
	if(wnd.DoModal() == IDOK)
	{
		if(dlg->startProfile(wnd.m_profileName, 1, wnd.m_timingLength))
		{
			m_results.SetWindowText(L"please waiting for the result.");
			return;
		}
	}

	m_pyprofile.EnableWindow(TRUE);
	m_cprofile.EnableWindow(TRUE);
	m_eventprofile.EnableWindow(TRUE);
	::AfxMessageBox(L"please select the baseapp|cellapp.");
}

void CProfileWindow::OnBnClickedButton3()
{
	m_pyprofile.EnableWindow(FALSE);
	m_cprofile.EnableWindow(FALSE);
	m_eventprofile.EnableWindow(FALSE);
	
	CguiconsoleDlg* dlg = static_cast<CguiconsoleDlg*>(theApp.m_pMainWnd);

	CTimingLengthWindow wnd;
	wnd.m_profileName = profilename;
	if(wnd.DoModal() == IDOK)
	{
		if(dlg->startProfile(wnd.m_profileName, 2, wnd.m_timingLength))
		{
			m_results.SetWindowText(L"please waiting for the result.");
			return;
		}
	}

	m_pyprofile.EnableWindow(TRUE);
	m_cprofile.EnableWindow(TRUE);
	m_eventprofile.EnableWindow(TRUE);
	::AfxMessageBox(L"please select the baseapp|cellapp.");
}


void CProfileWindow::onReceiveData(KBEngine::int8 type, KBEngine::MemoryStream& s)
{
	m_pyprofile.EnableWindow(TRUE);
	m_cprofile.EnableWindow(TRUE);
	m_eventprofile.EnableWindow(TRUE);

	switch(type)
	{
	case 0:	// pyprofile
		{
			m_profileShowList.ShowWindow(FALSE);
			m_results.ShowWindow(TRUE);
			onReceivePyProfileData(s);
		}
		break;
	case 1:	// cprofile
		m_profileShowList.ShowWindow(TRUE);
		m_results.ShowWindow(FALSE);
		onReceiveCProfileData(s);
		break;
	case 2:	// eventprofile
		m_profileShowList.ShowWindow(FALSE);
		m_results.ShowWindow(TRUE);
		onReceiveEventProfileData(s);
		break;
	case 3:	// mercuryprofile
		m_profileShowList.ShowWindow(FALSE);
		m_results.ShowWindow(FALSE);
		onReceiveMercuryProfileData(s);
		break;
	default:
		ERROR_MSG(boost::format("CProfileWindow::onReceiveData: type(%1%) not support!\n") % 
			type);
		break;
	};
}

void CProfileWindow::onReceivePyProfileData(KBEngine::MemoryStream& s)
{
	std::string data;
	uint32 timinglen;
	s >> timinglen;
	s >> data;

	CString str;
	wchar_t* ws = KBEngine::strutil::char2wchar(data.c_str());
	str = ws;
	str.Replace(L"\n", L"\r\n");
	free(ws);

	m_results.SetWindowText(str);
}

void CProfileWindow::onReceiveCProfileData(KBEngine::MemoryStream& s)
{
	m_profileShowList.DeleteAllItems();

	/*
	if(m_profileShowList.GetHeaderCtrl())
	{
		int nColumnCount = m_profileShowList.GetHeaderCtrl()->GetItemCount();       
		for (int i=0;i < nColumnCount;i++)
		{
			m_profileShowList.DeleteColumn(0);
		}
	}
	
	int idx = 0;
	m_profileShowList.InsertColumn(idx++, _T("ncalls "),					LVCFMT_CENTER,	50);
	m_profileShowList.InsertColumn(idx++, _T("tottime"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("percall"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("cumtime"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("percall"),					LVCFMT_CENTER,	80);
	m_profileShowList.InsertColumn(idx++, _T("filename:lineno(function)"),	LVCFMT_CENTER,	300);
	*/

	uint32 timinglen;
	s >> timinglen;

	KBEngine::ArraySize size;
	s >> size;

	while(size-- > 0)
	{
		uint32 count;
		float lastTime;
		float sumTime;
		float lastIntTime;
		float sumIntTime;
		std::string name;

		s >> name >> count >> lastTime >> sumTime >> lastIntTime >> sumIntTime;

		CString str;
		str.Format(L"%u", count);
		m_profileShowList.InsertItem(0, str);

		str.Format(L"%.3f", sumTime);
		m_profileShowList.SetItemText(0, 1, str);

		str.Format(L"%.3f", lastTime);
		m_profileShowList.SetItemText(0, 2, str);

		str.Format(L"%.3f", sumIntTime);
		m_profileShowList.SetItemText(0, 3, str);

		str.Format(L"%.3f", lastIntTime);
		m_profileShowList.SetItemText(0, 4, str);

		wchar_t* ws = KBEngine::strutil::char2wchar(name.c_str());
		str = ws;
		free(ws);
		m_profileShowList.SetItemText(0, 5, str);
	};

	ListSortData *tmpp = new ListSortData;
	tmpp->listctrl = &m_profileShowList;
	tmpp->isub = 3;
	tmpp->seq = 1;
	m_profileShowList.SortItems(CompareFloatFunc,(LPARAM)tmpp);
	delete tmpp;
}

void CProfileWindow::onReceiveEventProfileData(KBEngine::MemoryStream& s)
{
	uint32 timinglen;
	s >> timinglen;

	KBEngine::ArraySize size;
	s >> size;
	
	CString outstr;
	outstr.Format(L"Waiting %.2f secs...\r\n\r\n", (float)timinglen);
	
	if(size == 0)
		outstr += L"\r\nresults is empty!";

	CString str;

	while(size-- > 0)
	{
		std::string type_name;
		s >> type_name;
		
		wchar_t* ws = KBEngine::strutil::char2wchar(type_name.c_str());

		str.Format(L"Event Type:%s\r\n\r\n(name|count|size)\r\n---------------------\r\n\r\n", ws);
		outstr += str;
		free(ws);

		KBEngine::ArraySize size1;
		s >> size1;

		while(size1-- > 0)
		{
			uint32 count;
			uint32 eventSize;
			std::string name;

			s >> name >> count >> eventSize;
			
			if(count == 0)
				continue;

			ws = KBEngine::strutil::char2wchar(name.c_str());
			str.Format(L"%s\t\t\t\t\t%u\t%u\r\n", ws, count, eventSize);
			outstr += str;
			free(ws);
		}

		outstr += L"\r\n\r\n";
	};

	m_results.SetWindowText(outstr);
}

void CProfileWindow::onReceiveMercuryProfileData(KBEngine::MemoryStream& s)
{

}

