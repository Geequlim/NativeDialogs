#include "NativeDialog.h"
#ifdef ND_PLATFORM_GTK
	#include <gtk/gtk.h>
#endif
#ifdef ND_PLATFORM_WIN
	#include <Windows.h>
	#include <Winnls.h>
	#if defined(_M_IX86)
		#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined(_M_IA64)
		#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined(_M_X64)
		#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#else
		#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#endif
#endif

namespace NativeDialog
{
    /// The event handler do nothing
    const Dialog::EventHandler Dialog::nullHandler = [](const Dialog&){};

    #ifdef ND_PLATFORM_GTK
        // Iterate GTK events
        void iterateGTKEvents()
        {
            while (gtk_events_pending())
            gtk_main_iteration();
        }
   #endif

    namespace String
    {
        // split string to vector
        vector<string> split(const string & soueceStr,const string& pattern)
        {
            string::size_type pos;
            std::vector<string> result;
            //expend for convenience
            string str( soueceStr + pattern);
            string::size_type  size =str.size();
            for( string::size_type i=0; i<size; i++)
            {
                pos=str.find(pattern,i);
                if(pos<size)
                {
                    string s=str.substr(i,pos-i);
                    result.push_back(s);
                    i=pos+pattern.size()-1;
                }
            }
            return result;
        }
#ifdef ND_PLATFORM_WIN

		wstring nullwstr;

		wstring string2wstring(const string& utf8Str)
		{
			int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
			if (len == 0)
				return nullwstr;
			vector<wchar_t> unicode(len);
			MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &unicode[0], len);
			return wstring(&unicode[0]);
		}

		string wstring2string(const wstring& wideStr)
		{
			int len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
			if (len == 0)
				return nullstr;
			vector<char> utf8(len);
			WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8[0], len, NULL, NULL);
			return string(&utf8[0]);
		}

		wstring multibyteString2wstring(const string& multibyteStr)
		{
			int len = MultiByteToWideChar(CP_ACP, 0, multibyteStr.c_str(), -1, NULL, 0);
			if (len == 0)
				return nullwstr;
			vector<wchar_t> unicode(len);
			MultiByteToWideChar(CP_ACP, 0, multibyteStr.c_str(), -1, &unicode[0], len);
			return wstring(&unicode[0]);
		}

		string wstring2multibyteString(const wstring& wideStr)
		{
			int len = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
			if (len == 0)
				return nullstr;
			vector<char> utf8(len);
			WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &utf8[0], len, NULL, NULL);
			return string(&utf8[0]);
		}
#endif
    }
}
