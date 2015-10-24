#include "../NativeDialog.h"
#ifdef ND_PLATFORM_OSX
#import <Cocoa/Cocoa.h>

namespace Walnut
{
    namespace Util
    {
        
        using namespace std;
        
        void initializePanel(NSSavePanel* panel , const string &wndTitle , const string &initDir ,vector<pair<string,string>> &filter)
        {
            //set initialize directory
            if(initDir.compare(nullstr))
            {
                
                NSString * _initDir = [[NSString alloc] initWithUTF8String:(initDir.c_str())];
                [panel setDirectoryURL:[NSURL URLWithString:_initDir] ];
            }
            //set window title text
            if(wndTitle.compare(nullstr))
            {
                NSString * _wndTitle = [[NSString alloc] initWithUTF8String:(wndTitle.c_str())];
                [panel setTitle:(_wndTitle)];
            }
            //Set filters
            NSMutableArray *allowTypeArray = [[NSMutableArray alloc] init];
            for( vector<pair<string, string>>::iterator it=filter.begin() ;it!=filter.end();++it )
            {
                string curFilterTypes = it->second;
                if (curFilterTypes.compare("*"))//OSX ignores '*' filter
                {
                    vector<string> curFilters = String::split(curFilterTypes, ";");
                    
                    for (const string & curType :curFilters)
                        [allowTypeArray addObject: [NSString stringWithUTF8String:curType.c_str()]];
                }
            }
            if([allowTypeArray count])
                [panel setAllowedFileTypes:allowTypeArray];
            [allowTypeArray release];
            
            //Can create directories
            [panel setCanCreateDirectories:YES];
        }
        
        void FileDialog::show()
        {
            bool saveModel = m_mode & SELECT_TO_SAVE;
            //File dialog panel object
            NSSavePanel * panel = nullptr;
            if(saveModel)
                panel = [NSSavePanel savePanel];
            else
                panel = [NSOpenPanel openPanel];
            
            initializePanel(panel,m_wndTitle,m_defaultPath,m_filter);
            m_selectedPathes.clear();
            
            if(!saveModel)
            {
                NSOpenPanel * openPanel = (NSOpenPanel *) panel;
                //Set options for opening
                [openPanel setCanChooseFiles:NO];
                [openPanel setCanChooseDirectories:NO];
                [openPanel setAllowsMultipleSelection:NO];
                if( m_mode & SELECT_DIR )
                    [openPanel setCanChooseDirectories:YES];
                if(  m_mode & SELECT_FILE )
                    [openPanel setCanChooseFiles:YES];
                if(  m_mode & MULTI_SELECT )
                    [openPanel setAllowsMultipleSelection:YES];
            }
            [panel setAllowsOtherFileTypes:NO];
            [panel setCanCreateDirectories:YES];
            
            //Set host window
            NSWindow * _hostWnd  =  nullptr;
            if(m_pHostWnd)
                _hostWnd  = glfwGetCocoaWindow( static_cast<GLFWwindow*>( m_pHostWnd->hostWindow()) );
            
            if ( [panel runModal] == NSModalResponseOK )
            {
                if( !saveMode() )
                {
                    NSArray *urls = [(NSOpenPanel*)panel URLs];
                    for ( int i=0; i!=[urls count];++i )
                    {
                        NSURL * url = [urls objectAtIndex:i];
                        m_selectedPathes.push_back( [[url path] UTF8String] );
                    }
                }
                else
                    m_selectedPathes.push_back( [[[panel URL] path] UTF8String] );
                
                Event e(NormalEventType::CONFIRMED,false,false);
                dispatchEvent(e);
            }
            else
            {
                Event e(NormalEventType::CANCELED,false,false);
                dispatchEvent(e);
            }
            [panel release];
        }
    }
}
#endif