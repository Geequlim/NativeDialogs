#ifndef __ND_ColorPicker_Dialog_H__
#define __ND_ColorPicker_Dialog_H__

namespace NativeDialog
{
    /// Color Chooser Dialog
    class ColorPickerDialog : public Dialog
    {
        using super = Dialog;
  public:

        /// Color structure all compnents' value is in range [0,1]
        struct Color
        {
          double r,g,b,a;
        };

        /*!
         @brief  Construct a color picker dialog
         @param title The dialog title
         */
        ColorPickerDialog(const string& title)
        {
            m_title = title;
        }
        ~ColorPickerDialog();

        /// Show the color chooser dialog
        virtual void show() override;

        /// Set selected color of the dialog
        inline ColorPickerDialog& setColor( const Color& color )
        { 
          m_color = color;
          return *this;
        }

        /// Get selected color of the dialog
        inline const Color& color()const{ return m_color;}

    private:
        /// Selected color
        Color m_color;
    };
}

#endif //__ND_ColorPicker_Dialog_H__
