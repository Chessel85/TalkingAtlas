// GoLocationDlg.cpp : implementation file
//

#include "pch.h"
#include "SFRoamer.h"
#include "afxdialogex.h"
#include "GoLocationDlg.h"
#include <string>

//Define ID constants 
constexpr  UINT ID_EDIT_LATITUDE = 1000;
constexpr UINT ID_EDIT_LONGITUDE = 1001;



// CGoLocationDlg dialog

IMPLEMENT_DYNAMIC(CGoLocationDlg, CDialogEx)

CGoLocationDlg::CGoLocationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GO_LOCATION, pParent)
{

}

CGoLocationDlg::~CGoLocationDlg()
{
}

void CGoLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGoLocationDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGoLocationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGoLocationDlg message handlers


BOOL CGoLocationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Create label and edit box for latitude
	m_lblLatitude.Create(_T("Latitude:"), WS_CHILD | WS_VISIBLE, CRect(10, 10, 100, 25), this);
	m_editLatitude.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(10, 30, 100, 45), this, ID_EDIT_LATITUDE);
	std::string stdLat = std::to_string( m_latitude);
	CString sLat(stdLat.c_str());
	m_editLatitude.SetWindowText(sLat );


	//And for longitude 
	m_lblLongitude.Create(_T("Longitude:"), WS_CHILD | WS_VISIBLE, CRect(10, 50, 100, 65), this);
	m_editLongitude.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(10, 70, 100, 85), this, ID_EDIT_LONGITUDE);
	std::string stdLon = std::to_string(m_longitude);
	CString sLon(stdLon.c_str());
	m_editLongitude.SetWindowText(sLon);


	// Set tab order
	::SetWindowPos(m_lblLatitude.GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(m_editLatitude.GetSafeHwnd(), m_lblLatitude.GetSafeHwnd(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(m_lblLongitude.GetSafeHwnd(), m_editLatitude.GetSafeHwnd(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(m_editLongitude.GetSafeHwnd(), m_lblLongitude.GetSafeHwnd(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

return TRUE;  
}


void CGoLocationDlg::OnBnClickedOk()
{
	//Get the longitude and latitude from the edit boxes and pass to the double member variables 
	CString text;
	m_editLatitude.GetWindowText(text);

	try
	{
		m_latitude = _tstof(text); // Convert the string to a double
	}
	catch (...)
	{
		//do nothing 
	}

	m_editLongitude.GetWindowText(text);

	try
	{
		m_longitude = _tstof(text); // Convert the string to a double
	}
	catch (...)
	{
		//do nothing 
	}


	CDialogEx::OnOK();
}



double CGoLocationDlg::GetLatitude()
{
	return m_latitude;
}



double CGoLocationDlg::GetLongitude()
{
	return m_longitude;
}

void CGoLocationDlg::SetLongitude( double lon)
{
	m_longitude = lon;
}


void CGoLocationDlg::SetLatitude(double lat)
{
	m_latitude = lat;
}