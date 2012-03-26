
// TA ����Ʒ.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TA ����Ʒ.h"
#include "ChildFrm.h"
#include "MainFrm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTA����ƷApp

BEGIN_MESSAGE_MAP(CTA����ƷApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTA����ƷApp::OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN,&CTA����ƷApp::OnFILEOPEN)
END_MESSAGE_MAP()
#define ON_COMMAND(id, memberFxn) { WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, AfxSigCmd_v, static_cast<AFX_PMSG> (memberFxn) },

// CTA����ƷApp ����

CTA����ƷApp::CTA����ƷApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TA ����Ʒ.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CTA����ƷApp ����

CTA����ƷApp theApp;


// CTA����ƷApp ��ʼ��

BOOL CTA����ƷApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ������ MDI ��ܴ���
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// ��ͼ���ع��� MDI �˵��Ϳ�ݼ���
	//TODO: ��Ӹ��ӳ�Ա�����������ض�
	//	Ӧ�ó��������Ҫ�ĸ��Ӳ˵����͵ĵ���
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_TATYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_TATYPE));


	// �������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}

int CTA����ƷApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinAppEx::ExitInstance();
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������//
void CTA����ƷApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTA����ƷApp �Զ������/���淽�� //

void CTA����ƷApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CTA����ƷApp::LoadCustomState()
{
}

void CTA����ƷApp::SaveCustomState()
{
}

// CTA����ƷApp ��Ϣ�������//



void CTA����ƷApp::OnFILEOPEN()
{
	// TODO: �ڴ���������������//

	//, _T(""), _T("[S] BOW v2.tnt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("*.tnt;")
	CFileDialog GetTNTFile_OpenDialog(TRUE, _T("tnt"), _T("*.tnt"),
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, _T("TNT Map(*.tnt)|*.tnt||"));
	CString FileName_Cstr;
	GetTNTFile_OpenDialog.GetOFN().lpstrFile = FileName_Cstr.GetBuffer ( MAX_PATH);
	GetTNTFile_OpenDialog.GetOFN().nMaxFile = MAX_PATH;
	GetTNTFile_OpenDialog.DoModal ( );
	FileName_Cstr.ReleaseBuffer();
	if (! FileName_Cstr.IsEmpty())
	{
		CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
		// �����µ� MDI �Ӵ���//
		CChildFrame * myChildFrame_PCCF= pFrame->CreateNewChildWithTNTFilePtr(
			RUNTIME_CLASS(CChildFrame), IDR_TATYPE, FileName_Cstr, m_hMDIMenu, m_hMDIAccel);
		//FileName_Cstr.ReleaseBuffer();
		//myChildFrame_PCCF->
	}

 }

// CTA����ƷApp ��Ϣ�������
/*

void CTA����ƷApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	// �����µ� MDI �Ӵ���
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_TATYPE, m_hMDIMenu, m_hMDIAccel);
}*/
