#include "../NativeDialog.h"
#ifdef ND_PLATFORM_WIN
#include "bubble/bubble.hpp"

namespace NativeDialog
{
	void MessageDialog::show()
	{
		string dialogConfig = "body.text=";
		(dialogConfig += m_message) += ";";
		((dialogConfig += "title.text=") += m_title) += ";";

		for (size_t i = 0; i != m_buttons.size(); ++i)
		{
			((dialogConfig += std::to_string(i) + ".text=") += m_buttons[i]) += ";";
		}

		int choice = bubble::show(bubble::string(dialogConfig));

		if (choice >= 0 && choice < m_buttons.size())
		{
			m_responseIndex = choice;
			m_decideHandler(*this);
		}
		else
		{
			m_cancelHandler(*this);
		}
	}
}
#endif //ND_PLATFORM_WIN