
// logicDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// ClogicDlg �Ի���
class ClogicDlg : public CDialogEx
{
// ����
public:
	ClogicDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LOGIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit InputFrame;
	CEdit TypeFrame;
	CEdit PDNFFrame;
	CEdit PCNFFrame;
	CButton ResetButton;
	CButton CalcButton;
	CButton WatchButton;
	afx_msg void OnBnClickedOk();
//    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnEnUpdateEdit1();
    CEdit ConvertFrame;
    afx_msg void OnBnClickedButton2();
    CRichEditCtrl TruthTableFrame;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnStnClickedCopyType();
    CStatic CopyType;
//    CStatic COPY_PDNF;
    CStatic CopyPDNF;
    CStatic CopyPCNF;
    CStatic CopyConvert;
    afx_msg void OnStnClickedCopyPdnf();
    afx_msg void OnStnClickedCopyPcnf();
    afx_msg void OnStnClickedCopyConvert();
    afx_msg void OnStnClickedCopyTruthTable();
    CStatic CopyTruthTable;
};
