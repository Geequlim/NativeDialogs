#include "../NativeDialog.h"
#ifdef ND_PLATFORM_GTK
#include <gtk/gtk.h>

namespace NativeDialog
{
    // Add fileters to dialog
    void ApplyFiltersToDialog(const vector<std::pair<string,string> >& filters,GtkWidget* dialog)
    {
      for( auto & curFilterSrc : filters )
      {
        GtkFileFilter * filter =  gtk_file_filter_new ();
        gtk_file_filter_set_name (filter, curFilterSrc.first.c_str() );

        auto extNames =  String::split( curFilterSrc.second , ";" );
        for(auto curExt : extNames)
        {
          string pattern = "*";
          if(curExt != "*")
            pattern = "*.";pattern+=curExt;
          gtk_file_filter_add_pattern (filter, pattern.c_str());
        }
        gtk_file_chooser_add_filter( GTK_FILE_CHOOSER(dialog), filter );
      }
    }

    // Show GTK file chooser dialog
    void FileDialog::show()
    {
      m_selectedPathes.clear();
      // The dialgo mode
      GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN; // Open File mode
      if( allowsDirectorySelection() && saveMode() && !allowsFileSelection() )
        action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER; // Create folders mode
      else if ( saveMode() )
        action = GTK_FILE_CHOOSER_ACTION_SAVE; // Save file mode
      else if( allowsDirectorySelection()  )
        action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER; // Open folder mode

      // create file chooser dialog
      GtkWidget *dialog = gtk_file_chooser_dialog_new( m_title.c_str() ,
                          nullptr,
                          action,
                          "_Cancel",GTK_RESPONSE_CANCEL,
                          saveMode()?"_Save":"_Open", GTK_RESPONSE_ACCEPT,
                          nullptr );

      /// Build the filter list
      ApplyFiltersToDialog(m_filter,dialog);

      // Set the default path
      gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER(dialog), m_defaultPath.c_str() );

      // Set dialog flags
      if(saveMode())
        gtk_file_chooser_set_do_overwrite_confirmation( GTK_FILE_CHOOSER(dialog), true );
      else if(allowsMultipleSelection())
        gtk_file_chooser_set_select_multiple( GTK_FILE_CHOOSER(dialog), true );

      if( gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT )
      {
        GSList *fileList = gtk_file_chooser_get_filenames( GTK_FILE_CHOOSER(dialog) );
        while(fileList)
        {
          if( fileList->data )
          {
            const char* curCString = (char*)(fileList->data);
            if( curCString )
              m_selectedPathes.push_back( string(curCString) );
          }
          fileList = fileList->next;
        }

        m_decideHandler(*this);

      }
      else
      {
        m_cancelHandler(*this);
      }
      gtk_widget_destroy(dialog);
      iterateGTKEvents();
    }
}
#endif
