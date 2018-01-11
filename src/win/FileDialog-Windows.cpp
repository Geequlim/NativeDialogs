#include "../NativeDialog.h"
#ifdef ND_PLATFORM_WIN
#include <Windows.h>
#include <shlobj.h>
#include <Commdlg.h>
#pragma comment(lib, "Comdlg32.lib")
/// The file path string max length(in characters) definition on windows platform
#ifndef PAHT_MAX_LENGHT
#define PAHT_MAX_LENGHT 4096
#endif

namespace NativeDialog
{
		using String::wstring;
		using String::nullwstr;

		//The static variable for default diretory
		static wstring init_directory = nullwstr;

		// show directory selector dialog
		bool showDirectorySelector(wchar_t * buffer, wchar_t* wndTitle, bool withFile, const string& initDirectory);

		// convert string to TCHAR* string
		TCHAR* string2TString(const string& wstr)
		{
			TCHAR* tstring = nullptr;
			if (wstr.length())
			{
				wstring wideStr = String::string2wstring(wstr);
				tstring = new TCHAR[wideStr.length() + 1];
				for (size_t i = 0; i != wideStr.length(); ++i)
					tstring[i] = wideStr[i];
				tstring[wideStr.length()] = L'\0';
			}
			return tstring;
		}

		void FileDialog::show()
		{
			bool selected = false;

			// The buffer to save pathes selected
			TCHAR szBuffer[PAHT_MAX_LENGHT] = { 0 };

			// The dialog window title
			TCHAR *_wndTitle = string2TString(m_title);

			// The initilize directory
			TCHAR *_initDir = string2TString(m_defaultPath);

			//Select diectory or file
			if (allowsDirectorySelection())
				selected = showDirectorySelector(szBuffer, _wndTitle, allowsFileSelection(), m_defaultPath);
			//Select file
			else
			{
#pragma region Select files
				OPENFILENAME ofn = { 0 };
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = nullptr;
				//Dialog title
				ofn.lpstrTitle = _wndTitle;
				// default directory
				ofn.lpstrInitialDir = _initDir;

				//Set filters
				TCHAR * lpstrFilter = nullptr;

				if (m_filter.size())
				{
					string tempFilter;
					//Format filter string： Text Files%*.txt;*.TXT%XML Files%*.xml;*.XML%All Files%*.*%
					for (auto it = m_filter.begin(); it != m_filter.end(); ++it)
					{
						tempFilter += it->first;
						tempFilter += "%";
						//Add *. to extentions： txt;h;cpp -> *.txt;*.h;*.cpp
						{
							string curFilterResult = nullstr;
							string curFilter = it->second;
							size_t curTypeStartAt = 0;//Selected filter index
							for (size_t i = 0; i != curFilter.length(); ++i)
							{
								if (*(curFilter.c_str() + i) == ';')
								{
									if (curTypeStartAt != 0)
										curFilterResult += ";";

									curFilterResult += string("*.") + curFilter.substr(curTypeStartAt, i - curTypeStartAt);
									curTypeStartAt = i + 1;
								}

							}
							if (curTypeStartAt != 0)
								curFilterResult += ";";
							curFilterResult += string("*.") + curFilter.substr(curTypeStartAt, curFilter.length() - curTypeStartAt);
							tempFilter += curFilterResult;
							tempFilter += "%";
						}
					}
					//Format filter string： Text Files\0*.txt;*.TXT\0XML Files\0*.xml;*.XML\0All Files\0*.*\0
					lpstrFilter = string2TString(tempFilter);

					for (int i = 0; lpstrFilter[i] != '\0'; i++)
					{
						if (lpstrFilter[i] == L'%')
							lpstrFilter[i] = L'\0';
					}
					ofn.lpstrFilter = lpstrFilter;
				}
				else
					ofn.lpstrFilter = L"All Files(*.*)\0*.*\0";
				ofn.nFilterIndex = 0;

				// Set selected path buffer
				ofn.lpstrFile = szBuffer;
				ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);


				//Set dialog flags
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
				if ( allowsMultipleSelection() )
					ofn.Flags = ofn.Flags | OFN_ALLOWMULTISELECT;
				if(saveMode())
					ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
				//Save current working dirctory
				TCHAR curWorkDir[MAX_PATH] = { 0 };
				GetCurrentDirectory(MAX_PATH, curWorkDir);

				// Show the dialog
				if( saveMode() )
					GetSaveFileName(&ofn);
				else
					GetOpenFileName(&ofn);

				// Restore the saved working directory
				SetCurrentDirectory(curWorkDir);

				// free memery of fliters
				delete[] lpstrFilter;

#pragma endregion Select Files
			}
			//free memery
			delete[] _wndTitle;
			delete[] _initDir;

#pragma region Get selected file pathes

			m_selectedPathes.clear();
			string path = String::wstring2string(szBuffer);
			vector<size_t> speratorPos;
			size_t endPos = 0;
			// replace \0 to % in the buffer
			for (size_t i = 0; i != PAHT_MAX_LENGHT; ++i)
			{
				if (szBuffer[i] == L'\0' && szBuffer[i + 1] != '\0')
				{
					szBuffer[i] = '%';
					speratorPos.push_back(i);
				}
				else if (szBuffer[i] == '\0' && szBuffer[i + 1] == '\0')
				{
					endPos = i;
					break;
				}
			}
			if (speratorPos.size() >= 1)
			{
				//Get file pathes between %s
				for (int i = 0; i != speratorPos.size() - 1; ++i)
				{
					TCHAR * tempPath = new TCHAR[speratorPos.at(i + 1) - speratorPos.at(i)];
					size_t curTempPos = 0;
					for (size_t curCharPos = speratorPos.at(i) + 1; curCharPos != speratorPos.at(i + 1); ++curCharPos, ++curTempPos)
						tempPath[curTempPos] = szBuffer[curCharPos];
					tempPath[speratorPos.at(i + 1) - speratorPos.at(i) - 1] = '\0';

					string filePath = path + "\\" + String::wstring2string(tempPath);
					m_selectedPathes.push_back(filePath);
					delete[] tempPath;
				}
				// The last selected path
				if (endPos)
				{
					TCHAR * tempPath = new TCHAR[endPos - speratorPos.at(speratorPos.size() - 1)];
					size_t curTempPos = 0;
					for (size_t curCharPos = speratorPos.at(speratorPos.size() - 1) + 1; curCharPos != endPos; ++curCharPos, ++curTempPos)
						tempPath[curTempPos] = szBuffer[curCharPos];
					tempPath[endPos - speratorPos.at(speratorPos.size() - 1) - 1] = '\0';

					string filePath = path + "\\" + String::wstring2string(tempPath);
					m_selectedPathes.push_back(filePath);
					delete[] tempPath;
				}
			}
			else if (path.length())
				m_selectedPathes.push_back(path);

#pragma endregion Get selected file pathes

			// Dispatching event
			selected = m_selectedPathes.size() != 0;
			if(selected)
			{
				m_decideHandler(*this);
			}
			else
			{
				m_cancelHandler(*this);
			}
		}

		bool showDirectorySelector(wchar_t * buffer, wchar_t*  wndTitle, bool withFile, const string& initDirectory)
		{
			bool selected = false;

			if (initDirectory.length())
				init_directory = String::string2wstring(initDirectory);
			else
				init_directory = nullwstr;

			BROWSEINFO bi;
			ZeroMemory(&bi, sizeof(BROWSEINFO));
			bi.hwndOwner = nullptr;
			// The selected buffer
			bi.pszDisplayName = buffer;
			// Set window title
			bi.lpszTitle = wndTitle;
			// Set scan type
			bi.ulFlags = BIF_RETURNFSANCESTORS;
			if (withFile)
				bi.ulFlags = BIF_BROWSEINCLUDEFILES;//  allows select files
			bi.ulFlags = bi.ulFlags | BIF_NEWDIALOGSTYLE;//allow create new directory
			bi.ulFlags = bi.ulFlags | BIF_STATUSTEXT;
			//bi.ulFlags = bi.ulFlags | BIF_EDITBOX;// Show selected edit box

			//Set callbacks of the selector
			bi.lpfn = [](HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
			{
				switch (uMsg)
				{
				case BFFM_INITIALIZED:    //Set default selected path
					::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPTSTR)init_directory.c_str());
					break;
				default:
					break;
				}
				return 0;
			};
			bi.lParam = long(init_directory.c_str()); //Set default directory
			LPITEMIDLIST idl = SHBrowseForFolder(&bi);//Show the directory selector dialog
			if (idl)
			{
				SHGetPathFromIDList(idl, buffer); // save selected pathes
				selected = true;
			}
			return selected;
		}
}
#endif
