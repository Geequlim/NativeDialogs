#include "../NativeDialog.h"
#ifdef ND_PLATFORM_GTK 
#include <gtk/gtk.h>

namespace NativeDialog
{
    // Show gtk color chooser dialog
    void ColorPickerDialog::show()
    {
        const int COLOR_SELECTED = -5;
        auto dialog = gtk_color_chooser_dialog_new(m_title.c_str(),nullptr);
        GdkRGBA  color{ m_color.r,
                        m_color.g,
                        m_color.b,
                        m_color.a
                      };
                      
        gtk_color_chooser_set_rgba((GtkColorChooser *)dialog,&color);
        if( gtk_dialog_run(GTK_DIALOG (dialog) ) == COLOR_SELECTED )
        {
            gtk_color_chooser_get_rgba((GtkColorChooser *)dialog,&color);

            // Get selected color value
            m_color = Color{ color.red , color.green , color.blue , color.alpha};

            m_decideHandler(*this);
        }
        else
        {
          m_cancelHandler(*this);
        }
        gtk_widget_destroy(dialog);
        iterateGTKEvents();
    }

    ColorPickerDialog::~ColorPickerDialog(){}
}

#endif
