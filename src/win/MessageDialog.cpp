#include "../NativeDialog.h"
#ifdef ND_PLATFORM_WIN

namespace Walnut
{
  namespace Util
  {
    void MessageDialog::show()
    {
      string dialogConfig = "body.text=";
      (dialogConfig += m_message)+= ";";
      ((dialogConfig += "title.text=") += m_title) += ";";

      for (size_t i = 0; i != m_buttons.size(); ++i)
      {
        ((dialogConfig += std::to_string(i)+".text=") += m_buttons[i]) += ";";
      }

      int choice = bubble::show(bubble::string(dialogConfig));

      if (choice >= 0 && choice < m_buttons.size())
      {
        m_responseIndex = choice;
        Event e(NormalEventType::CONFIRMED, false, false);
        dispatchEvent(e);
      }
      else
      {
        Event e(NormalEventType::CANCELED, false, false);
        dispatchEvent(e);
      }
    }
  }
}
#endif