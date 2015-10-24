#include "../NativeDialog.h"
#ifdef ND_PLATFORM_WIN

#include <Commdlg.h>
namespace Walnut
{
	namespace Util
	{
		void ColorPickerDialog::show()
		{
			CHOOSECOLOR cc;                 // common dialog box structure 
			static COLORREF acrCustClr[16]; // array of custom colors 
			HWND hwnd = nullptr;            // owner window

			
			static DWORD rgbCurrent = RGB( // initial color selection
						m_color.r, 
						m_color.g,
						m_color.b);        
			
			// Initialize CHOOSECOLOR 
			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hwnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = rgbCurrent;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR ;
			wstring title = String::string2wstring(m_title);
			cc.lpTemplateName = title.c_str();
			if (ChooseColor(&cc) == TRUE)
			{
				rgbCurrent = cc.rgbResult;
				m_color = { GetRValue(rgbCurrent)/255.0,
							GetGValue(rgbCurrent)/255.0,
							GetBValue(rgbCurrent)/255.0,
							m_color.alphaPercent() };
				Event e(NormalEventType::CONFIRMED, false, false);
				dispatchEvent(e);
			}
			else
			{
				Event e(NormalEventType::CANCELED, false, false);
				dispatchEvent(e);
			}
		}

		ColorPickerDialog::~ColorPickerDialog()
		{

		}
	}
}
#endif