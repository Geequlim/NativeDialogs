#ifndef __ND_NativeMessageDialog_H__
#define __ND_NativeMessageDialog_H__

namespace NativeDialog
{
  /// Meesage dialog with native user interfaces
  class MessageDialog : public Dialog
  {
    using super = Dialog;
  public:

   /*!
    @brief Construct a message dialog
    @param title   The dialog title
    @param message The message content
    @param buttons Button titles
   */
    MessageDialog( const string& title,
                   const string& message,
                   const vector<string>& buttons)
    {
      this->m_title = title;
      m_message = message;
      m_buttons = buttons;
    }

    ~MessageDialog(){}

    /// Get button titles
    inline const vector<string>& buttons()const{ return m_buttons; }

    /// Get writable button titles
    inline vector<string>& buttons(){ return m_buttons; }

    /// Set button titles
    inline MessageDialog& setButtons(const vector<string>& buttons)
    {
      m_buttons = buttons;
      return *this;
    }

    /// Get message content
    inline const string& message()const { return m_message; }

    /// Set message content
    inline MessageDialog& setMessage(const string& msg)
    {
      m_message = msg;
      return *this;
    }

    /// Show the dialog
    virtual void show() override;

    /// Get the response button title
    inline const string& responseButtonTitle()const
    {
      const string* buttonTitle = &nullstr;
      if( m_responseIndex>=0 && m_responseIndex< int(m_buttons.size()) )
        buttonTitle = &m_buttons.at(m_responseIndex);
      return *buttonTitle;
    }

    /// Get the response button index starts with 0
    inline int responseButtonIndex()const { return m_responseIndex; }

  private:
    /// The resonse button index
    int m_responseIndex;

    /// Message content
    string m_message;

    /// Buttons' title
    vector<string> m_buttons;
  };

}
#endif //__ND_NativeMessageDialog_H__
