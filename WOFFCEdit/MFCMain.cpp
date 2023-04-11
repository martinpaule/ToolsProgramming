#include "MFCMain.h"
#include "wtypes.h"

#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT,	&MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_CHANGEBRUSHSIZE, &MFCMain::MenuEditSetBrushSize)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButtonSave)
	ON_COMMAND(ID_BUTTON40012,	&MFCMain::ToolBarButtonRaise)
	ON_COMMAND(ID_BUTTON40010,	&MFCMain::ToolBarButtonLower)
	ON_COMMAND(ID_BUTTON40015,	&MFCMain::ToolBarButtonFlatten)
	ON_COMMAND(ID_BUTTON40019,	&MFCMain::ToolBarButtonCursor)
	ON_COMMAND(ID_BUTTON40020,	&MFCMain::ToolBarButtonGrid)
	ON_COMMAND(ID_BUTTON40023,	&MFCMain::ToolBarButtonWireframe)
	ON_COMMAND(ID_BUTTON40025,	&MFCMain::ToolBarButtonPaint)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;


	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)


	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(0, 0, desktop.right, desktop.bottom-50),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);



	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{	
			int ID = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString = L"Selected Object: " + std::to_wstring(ID);
			m_ToolSystem.Tick(&msg);

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

void MFCMain::MenuEditSetBrushSize()
{
	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolBSDialogue.Create(IDD_DIALOG2);	//Start up modeless
	m_ToolBSDialogue.ShowWindow(SW_SHOW);	//show modeless
	//m_ToolBSDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

void MFCMain::ToolBarButtonSave()
{
	
	m_ToolSystem.onActionSave();
}


MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}

void MFCMain::ToolBarButtonRaise() {

	m_ToolSystem.getGame()->setTerrainEditType(RAISE);


}
void MFCMain::ToolBarButtonLower() {

	m_ToolSystem.getGame()->setTerrainEditType(LOWER);



}


void MFCMain::ToolBarButtonFlatten() {

	m_ToolSystem.getGame()->setTerrainEditType(FLATTEN);



}

void MFCMain::ToolBarButtonPaint() {

	m_ToolSystem.getGame()->setTerrainEditType(PAINT);



}


void MFCMain::ToolBarButtonCursor() {
	m_ToolSystem.getGame()->setTerrainEditType(NOTHING);

}

void MFCMain::ToolBarButtonGrid() {

	m_ToolSystem.getGame()->ChangeGridState();

}

void MFCMain::ToolBarButtonWireframe() {

	m_ToolSystem.getGame()->renderInWireframe = !m_ToolSystem.getGame()->renderInWireframe;

}
