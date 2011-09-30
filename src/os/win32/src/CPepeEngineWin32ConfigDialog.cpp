#include "CPepeEngineConverter.h"
#include "CPepeEngineCore.h"
#include "CPepeEngineException.h"
#include "CPepeEngineWin32ConfigDialog.h"
#include "../resources/resource.h"

_PEPE_ENGINE_START

CPepeEngineWin32ConfigDialog* pDialog;

void refreshOptionList(HWND hDlg)
{
	// Reset list box
	HWND hDlgItem = GetDlgItem(hDlg, IDC_LIST_OPTIONS);	
	SendMessage(hDlgItem, LB_RESETCONTENT, 0, 0);

	CPepeEngineConfig*	pConfig = CPepeEngineCore::getSingleton().getConfig();
	ConfigOptions		opt		= pConfig->getOptions(RENDERER_OPTION);
	ConfigOptions::iterator optIt = opt.begin(), optEnd = opt.end();

	tstring strLine;
	while (optIt != optEnd)
	{
		strLine = optIt->first + _T(": ") + optIt->second->strValue;
		SendMessage(hDlgItem, LB_ADDSTRING, 0, (LPARAM)(TCHAR*)strLine.c_str());

		optIt++;
	}
}

// -----------------------------------------------------------------------------------------
#if PEPE_ENGINE_ARCH_TYPE == PEPE_ENGINE_ARCHITECTURE_64
	INT_PTR CALLBACK CPepeEngineWin32ConfigDialog::dlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
#else
	BOOL CALLBACK CPepeEngineWin32ConfigDialog::dlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
#endif
{
	HWND					hDlgItem;
	static RendererList*	pRendererList;
	RendererList::iterator	rendLstIt, rendLstEnd;

	int i = 0, sel = 0, savedSel = 0;
	tstring err;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		pRendererList = CPepeEngineCore::getSingleton().getAvailableRenderers();
		if (pRendererList->empty())
		{
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_INTERNAL_ERROR, 
				_T("Any available renderers - you should propably install suitable plugin."), 
				_T("CPepeEngineWin32ConfigDialog::dlgProc")
			);	
		} else
		{
			pDialog->m_pSelectedRenderer = *(pRendererList->begin());			
			CPepeEngineCore::getSingleton().setActiveRenderer(pDialog->m_pSelectedRenderer);
		}
		rendLstIt		= pRendererList->begin();
		rendLstEnd		= pRendererList->end();
		
		while (rendLstIt != rendLstEnd)
		{
			hDlgItem = GetDlgItem(hDlg, IDC_COMBO_RENDERER);
			
			SendMessage(hDlgItem, CB_ADDSTRING, 0, (LPARAM)(TCHAR*)(*rendLstIt)->getName().c_str());			
			SendMessage(hDlgItem, CB_SETCURSEL, (WPARAM)i, 0);
			
			if (*rendLstIt == pDialog->m_pSelectedRenderer)
			{
				// Reset list box
				refreshOptionList(hDlg);
			}

			i++;
			rendLstIt++;
		}

		// Center myself
		int x, y, screenWidth, screenHeight;
		RECT rcDlg;
		GetWindowRect(hDlg, &rcDlg);
		screenWidth		= GetSystemMetrics(SM_CXFULLSCREEN);
		screenHeight	= GetSystemMetrics(SM_CYFULLSCREEN);

		x = (screenWidth / 2) - ((rcDlg.right - rcDlg.left) / 2);
		y = (screenHeight / 2) - ((rcDlg.bottom - rcDlg.top) / 2);

		MoveWindow(hDlg, x, y, (rcDlg.right - rcDlg.left), (rcDlg.bottom - rcDlg.top), TRUE);

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST_OPTIONS:
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				// Selection in list box of options changed
				// Update combo and label in edit section
				hDlgItem	= GetDlgItem(hDlg, IDC_LIST_OPTIONS);
				sel			= SendMessage(hDlgItem, LB_GETCURSEL, 0, 0);
				if (sel != -1)
				{
					CPepeEngineConfig*	pConfig = CPepeEngineCore::getSingleton().getConfig();
					ConfigOptions		opt		= pConfig->getOptions(RENDERER_OPTION);
					ConfigOptions::iterator optIt = opt.begin(), optEnd = opt.end();
					
					for (int n = 0; n < sel; n++)
					{						
						optIt++;
					}
					// Set label text
					hDlgItem = GetDlgItem(hDlg, IDC_LABEL_OPTION);
					SetWindowText(hDlgItem, optIt->first.c_str());
					// Set combo options
					hDlgItem = GetDlgItem(hDlg, IDC_COMBO_OPTIONS);
					SendMessage(hDlgItem, CB_RESETCONTENT, 0, 0);
					
					std::vector<tstring>::iterator	posIt	=	optIt->second->possibleValues.begin(),
													posEnd	= 	optIt->second->possibleValues.end();

					int i = 0;
					while (posIt != posEnd)
					{
						SendMessage(hDlgItem, CB_ADDSTRING, 0, (LPARAM)(TCHAR*)(*posIt).c_str());
						if (*posIt == optIt->second->strValue)
						{
							// Select current value
							SendMessage(hDlgItem, CB_SETCURSEL, (WPARAM)i, 0);
						}
						++posIt;
						++i;
					}
					// Enable/disable combo depending on (not)immutable
					EnableWindow(hDlgItem, !(optIt->second->immutable));
				}
			}

			return true;
		case IDC_COMBO_RENDERER:
			hDlgItem	= GetDlgItem(hDlg, IDC_COMBO_RENDERER);
			sel			= SendMessage(hDlgItem, CB_GETCOUNT, 0, 0);

			if (HIWORD(wParam) == CBN_SELCHANGE)
			{				
				hDlgItem	= GetDlgItem(hDlg, IDC_COMBO_RENDERER);
				sel			= SendMessage(hDlgItem, CB_GETCURSEL, 0, 0);
				if (sel != -1)
				{					
					rendLstIt = pRendererList->begin();
					pDialog->m_pSelectedRenderer = rendLstIt[sel];
					
					// refresh config options 
				}                    
			}
			return true;
		case IDC_COMBO_OPTIONS:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				// Updated an option
				// Get option
				hDlgItem	= GetDlgItem(hDlg, IDC_LIST_OPTIONS);
				sel			= SendMessage(hDlgItem, LB_GETCURSEL, 0, 0);
				savedSel	= sel;

				CPepeEngineConfig*	pConfig		= CPepeEngineCore::getSingleton().getConfig();
				ConfigOptions		opt			= pConfig->getOptions(RENDERER_OPTION);
				ConfigOptions::iterator optIt	= opt.begin(), optEnd = opt.end();

				for (int n = 0; n < sel; n++)
				{						
					optIt++;
				}

				// Get selected value
				hDlgItem	= GetDlgItem(hDlg, IDC_COMBO_OPTIONS);
				sel			= SendMessage(hDlgItem, CB_GETCURSEL, 0, 0);

				if (sel != -1)
				{
					std::vector<tstring>::iterator	posIt	=	optIt->second->possibleValues.begin(),
													posEnd	= 	optIt->second->possibleValues.end();

					CPepeEngineConfig*	pConfig = CPepeEngineCore::getSingleton().getConfig();
					pConfig->setOptionValue(optIt->first, posIt[sel]);
															
					// Reset options list box					
					refreshOptionList(hDlg);

					// Select previously selected item
					hDlgItem = GetDlgItem(hDlg, IDC_LIST_OPTIONS);
					SendMessage(hDlgItem, LB_SETCURSEL, savedSel, 0);
				}

			}
			return true;
		case IDOK:			
			if (!pDialog->m_pSelectedRenderer)
			{
				MessageBox(NULL, _T("Please choose a renderer."), _T("Pepe Engine"), MB_OK | MB_ICONEXCLAMATION);
				return true;
			}

			err = pDialog->m_pSelectedRenderer->validateOptions();

			if (!err.empty())
			{
				refreshOptionList(hDlg);
				MessageBox(NULL, err.c_str(), _T("Pepe Engine"), MB_OK | MB_ICONEXCLAMATION);
				return true;
			}

			CPepeEngineCore::getSingleton().setActiveRenderer(pDialog->m_pSelectedRenderer);
			EndDialog(hDlg, true);		
			return true;
		case IDCANCEL:
			EndDialog(hDlg, false);
			return true;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------
CPepeEngineWin32ConfigDialog::CPepeEngineWin32ConfigDialog()
{
	#ifdef PEPE_ENGINE_STATIC_LIB
		m_hInstance = GetModuleHandle( NULL );
	#else		
		m_hInstance = GetModuleHandle(_T("OS_Win32.dll"));	
	#endif
}

// -----------------------------------------------------------------------------------------
CPepeEngineWin32ConfigDialog::~CPepeEngineWin32ConfigDialog()
{
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineWin32ConfigDialog::display()
{
	// Display dialog
	// Don't return to caller until dialog dismissed
	int i;
	pDialog = this;

	i = DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, CPepeEngineWin32ConfigDialog::dlgProc);

	if (i == -1)
	{
		int winError = GetLastError();
		TCHAR errDesc[255];
		int i;
		
		// Try windows errors first
		i = FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			winError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) errDesc,
			255,
			NULL
			);

		throw CPepeEngineException(
			winError, _TSTR(errDesc), _T("CPepeEngineWin32ConfigDialog::display"), 
			_T("Win32 operating system error"), __FILE__, __LINE__
		);		
	}
	if (i)
	{
		return true;
	}
	else
	{
		return false;
	}
}

_PEPE_ENGINE_END