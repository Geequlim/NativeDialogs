#include "../NativeDialog.h"
#ifdef ND_PLATFORM_GTK 
#include <gtk/gtk.h>

namespace NativeDialog
{
    void MessageDialog::show()
    {
      // Create the widgets
      auto dialog = gtk_dialog_new();
      // Set dialog title
      gtk_window_set_title( &((GtkDialog*)dialog)->window  ,m_title.c_str());
       
      // Add content widgets to dialog
      auto content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
      gtk_container_set_border_width(GTK_CONTAINER (content_area),8);
      
      // Add the message label
      auto  label = gtk_label_new (m_message.c_str());

      auto box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
      gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 12);
      gtk_container_add (GTK_CONTAINER (content_area), box);

      gtk_widget_show_all (dialog);
      
      // Add buttons
      for(int responseID=0; responseID!= int(m_buttons.size()); ++responseID)
      {
        gtk_dialog_add_button( (GtkDialog*)dialog,
                    m_buttons[responseID].c_str(),
                    responseID
                   );
      }
      // Show the dialog and retrive the selection result
      m_responseIndex =  gtk_dialog_run (GTK_DIALOG (dialog));
      if( m_responseIndex!= GTK_RESPONSE_NONE && m_responseIndex<int(m_buttons.size()) )
      {
        m_decideHandler(*this);
      }
      else
      {
        m_responseIndex = GTK_RESPONSE_NONE;
        m_cancelHandler(*this);
      }
      
      gtk_widget_destroy (dialog);
      iterateGTKEvents();
    }
}

#endif
