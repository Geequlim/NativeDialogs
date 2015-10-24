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
    }
}