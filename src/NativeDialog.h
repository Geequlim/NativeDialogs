#ifndef __ND_DIALOGS_H__
#define __ND_DIALOGS_H__

#if _WIN64 || _WIN32
  #define ND_PLATFORM_WIN
#elif __APPLE__
    #include "TargetConditionals.h"
    //iOS Simulator
    #if !TARGET_OS_IPHONE
    #define ND_PLATFORM_OSX
    #endif
#else
  #define ND_PLATFORM_GTK
  namespace NativeDialog
  {
    /// Iterate GTK events
    void iterateGTKEvents();
  }
#endif


#include <string>
#include <vector>
#include <functional>

namespace NativeDialog
{
    using std::string;
    using std::vector;
    /// The empty string definition
    extern string nullstr;

    /**
     * @class Dialog
     * @brief The abstract dialog class for native dialogs
     */
    class Dialog
    {
      /// Event handler type definition
      using EventHandler = std::function<void(const Dialog&) >;
    public:
        /// The event handler do nothing
        static const EventHandler nullHandler;
        /// Show dialog
        virtual void show() = 0;

        /// Get dialog title
        const string& title()const{ return m_title; }

        /// Set dialog title
        inline Dialog& setTitle(const string& title)
        { 
          m_title = title;
          return *this;
        }
        
        /// Get decide handler function
        inline const EventHandler& decideHandler()const { return m_decideHandler;}
        
        /// Set decide handler function
        inline Dialog& setDecideHandler(const EventHandler& handler)
        {
          m_decideHandler = handler;
          return *this;
        }
        
        /// Get cancel handler function
        inline const EventHandler& cancelHandler()const{ return m_cancelHandler;}
        
        /// Set cancel handler function
        inline Dialog& setCancelHandler(const EventHandler& handler)
        {
          m_cancelHandler = handler;
          return *this;
        }
        
    protected:
        /// Decide function handler
        EventHandler m_decideHandler = nullHandler;
        /// Cancel function handler
        EventHandler m_cancelHandler = nullHandler;
        /// Title text
        string m_title = nullstr;
    };
    
    
}

#include "FileDialog.h"
#include "MessageDialog.h"
#include "ColorPickerDialog.h"

#endif //__ND_DIALOGS_H__