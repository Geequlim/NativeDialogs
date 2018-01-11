#include "../NativeDialog.h"
#ifdef ND_PLATFORM_WIN
#include <map>
#include <Windows.h>
#include <Shobjidl.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

namespace NativeDialog
{
	void MessageDialog::show()
	{
		int nButtonPressed          = 0;
		TASKDIALOGCONFIG config     = {0};
		TASKDIALOG_BUTTON * buttons = new TASKDIALOG_BUTTON[m_buttons.size()];
		vector<std::wstring> button_titles(m_buttons.size());

		std::map<int, int> choices;
		for( auto i = 0u; i < m_buttons.size(); ++i ) {
			button_titles[i]                = String::string2wstring(m_buttons[i]);
			buttons[i].pszButtonText        = button_titles[i].c_str();
			buttons[i].nButtonID            = i;
			choices[ buttons[i].nButtonID ] = i;
		}

		config.cbSize             = sizeof(config);
		config.hInstance          = NULL;
		std::wstring title        = String::string2wstring(m_title);
		config.pszMainInstruction = title.c_str();
		std::wstring content      = String::string2wstring(m_message);
		config.pszContent         = content.c_str();
		config.pButtons           = buttons;
		config.cButtons           = (UINT)m_buttons.size();

		int choice = -1;
		TaskDialogIndirect(&config, &nButtonPressed, NULL, NULL);
		if (choice >= 0 && choice < m_buttons.size())
		{
			m_responseIndex = choice;
			m_decideHandler(*this);
		}
		else
		{
			m_cancelHandler(*this);
		}

		delete [] buttons;
	}
}
#endif //ND_PLATFORM_WIN
