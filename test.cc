#include "NativeDialog.h"
#ifdef ND_PLATFORM_GTK
  #include <gtk/gtk.h>
#endif

using namespace NativeDialog;

// File dialog decide function handler
// Called when file selected
void fileSelHandler(const Dialog& dlg);

int main(int argc , char *argv[] )
{
#ifdef ND_PLATFORM_GTK
  gtk_init(nullptr,nullptr);
#endif
  MessageDialog msgDialog("Message Dialog","\nSelect the next dialog to show.\n",
                          {"File Dialog","Color Picker","Nothing"}
  );
  msgDialog.setDecideHandler([](const Dialog& dlg)
    {
      auto mdlg = dynamic_cast<const MessageDialog&>(dlg);
      if( mdlg.responseButtonIndex() == 1 )
      {
        ColorPickerDialog cdlg("Pick a color you like");
        cdlg.setColor({1,0,1,1})
        .setDecideHandler( [](const Dialog& dlg)
          {
            auto colorDlg = dynamic_cast<const ColorPickerDialog&>(dlg);
            auto color = colorDlg.color();
            string msg = "You like the color: (";
            msg += std::to_string(color.r)+","+std::to_string(color.g)+","+
                   std::to_string(color.b)+","+std::to_string(color.a)+")";
            MessageDialog(nullstr,msg,{"Close"}).show(); 
          })
        .show();
      }
      else if( mdlg.responseButtonTitle() == "File Dialog" )
      {
        FileDialog fdlg("Open some text files",
                        FileDialog::SELECT_FILE|FileDialog::MULTI_SELECT);
        fdlg.setDefaultPath("..")
            .addFilter("Text Files","txt")
            .addFilter("Code files","c;cpp;h;hpp")
            .addFilter("All files","*")
//            .setSaveMode(true) // Set save mode
            .setDecideHandler(fileSelHandler)
            .setCancelHandler( [](const Dialog& dlg)
              {
                MessageDialog("","You canceled select files",{"Close"}).show();
              }
            )
            .show();
      }
    }
  )
  .show();
  
  ColorPickerDialog cdlg("Pick a color you like");
cdlg.setColor({1,0,1,1})
.setDecideHandler( [](const Dialog& dlg){
    auto colorDlg = dynamic_cast<const ColorPickerDialog&>(dlg);
    auto color = colorDlg.color();
    // Get color value with color.r,color.g,color.b and color.a
  })
.show();

  return 0;
}

void fileSelHandler(const Dialog& dlg)
{
  auto fdlg = dynamic_cast<const FileDialog&>(dlg);
  const auto& fileList = fdlg.selectedPathes();
  string msg = "You have selected files below:\n";
  for( const string& path : fileList )
  {
    msg += path;
    msg += "\n";
  }
  MessageDialog("Notice",msg,{"Close"}).show();
  
}