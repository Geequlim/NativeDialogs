#include "../NativeDialog.h"
#ifdef ND_PLATFORM_OSX

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>


namespace Walnut
{
    namespace Util
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
            
            int result = [alert runModal];
            int btnIndex = m_buttons.size()- 1 - ( result - NSAlertFirstButtonReturn );
            // Get responser
            if(btnIndex>=0 && result < m_buttons.size())
            {
                m_responseIndex = btnIndex;
                Event e(NormalEventType::CONFIRMED,false,false);
                dispatchEvent(e);
            }
            else
            {
                Event e(NormalEventType::CANCELED,false,false);
                dispatchEvent(e);
            }
            [alert release];
        }
    }
}
endif