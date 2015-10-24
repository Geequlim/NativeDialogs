#include "NativeDialog.h"
#ifdef ND_PLATFORM_GTK 
  #include <gtk/gtk.h>
#endif
namespace NativeDialog
{
   // define null string
   string nullstr;
   
   const Dialog::EventHandler Dialog::nullHandler = [](const Dialog&){};
   
   #ifdef ND_PLATFORM_GTK 
    // Iterate GTK events
    void iterateGTKEvents()
    {
      while (gtk_events_pending())
        gtk_main_iteration();
    }
   #endif
}