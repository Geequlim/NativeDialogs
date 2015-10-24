#include "../NativeDialog.h"
#ifdef ND_PLATFORM_OSX

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>


namespace NativeDialog
{
    void MessageDialog::show()
    {
        NSAlert *alert = [[NSAlert alloc] init];
        // Set text content
        [alert setMessageText:[NSString stringWithUTF8String:m_title.c_str()]];
        [alert setInformativeText:[NSString stringWithUTF8String:m_message.c_str()]];
        // Add buttons
        for(auto it=m_buttons.rbegin();it!=m_buttons.rend();++it)
            [alert addButtonWithTitle:[NSString stringWithUTF8String:it->c_str()]];
        
        int result = (int)[alert runModal];
        int btnIndex = (int)m_buttons.size()- 1 - ( result - NSAlertFirstButtonReturn );
        // Get responser
        if(btnIndex>=0 && btnIndex < m_buttons.size())
        {
            m_responseIndex = btnIndex;
            m_decideHandler(*this);
        }
        else
        {
            m_cancelHandler(*this);
        }
        [alert release];
    }
}
#endif