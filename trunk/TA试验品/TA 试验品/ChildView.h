
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once

class TNTtoMiniMap;
// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	//CFile * myTNTFile_CFile;
	
private:
	CPalette TAPalette_CPal;
	HBITMAP myBitmap_H;
// ����
public:
	TNTtoMiniMap * myTNTMiniMap_PTNTMM;
// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	CBitmap * MyView_CBitmapPTR;
	LPVOID ColorsBit;
	CStringA TNTFileName_CstrA;
};

