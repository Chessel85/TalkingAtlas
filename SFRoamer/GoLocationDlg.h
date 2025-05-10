#pragma once
#include "afxdialogex.h"


// CGoLocationDlg dialog

class CGoLocationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGoLocationDlg)

public:
	CGoLocationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CGoLocationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GO_LOCATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


		//Method
public:
	double GetLatitude();
	double GetLongitude();
	void SetLatitude(double lat);
	void SetLongitude(double lon);


	//Attributes
private:
	CEdit m_editLatitude;
	CEdit m_editLongitude;
	CStatic m_lblLatitude;
	CStatic m_lblLongitude;
	double m_longitude;
	float m_latitude;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
