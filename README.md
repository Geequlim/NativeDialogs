Native Dialogs
-----------------
**Use native dialogs with same code on Windows,Mac OSX and Linux(GTK3 required).**

### Integrate to your project
* Add all files under src to your project
* inclue NativeDialog.h file to your code
* Remove code under osx if it's not a OSX application

### dependence

* r-lyeh's [bubble](https://github.com/r-lyeh/bubble) for message dialog on Window
* GTK3 is required on Linux
* Cocoa framwork on Mac OSX

# Usage:

### Features:

* C++11 support and required
* Easy to use
* Native user interface

### Message Dialog

* Show native notice dialog with one line

```c++
MessageDialog("Notice","Hello World!",{"Close"}).show();
```

Platform|Screenshot
---|---
GTK|![](screenshot/gtk_message_0.png)

* Deal events as you like

```c++
MessageDialog mdlg("Notice","Do you want to quit?",{"Yes","No"});
mdlg.setDecideHandler([](const Dialog& dlg){
    auto mdlg = dynamic_cast<const MessageDialog&>(dlg);
    if( mdlg.responseButtonTitle() == "Yes" ){
        // He want to quit
    }
    else if(mdlg.responseButtonIndex() == 1) {
        // He is not sure to quit
    }
})
.show();
```
Platform|Screenshot
---|---
GTK|![](screenshot/gtk_message_1.png)


### File Chooser Dialog

* File selection
* Directory selection
* Multiselection
* Save file mode
* Filters are easy to set

```c++
FileDialog fdlg("Open some text files",
                FileDialog::SELECT_FILE|FileDialog::MULTI_SELECT);
fdlg.setDefaultPath("..")
    .addFilter("Text Files","txt")
    .addFilter("Code files","c;cpp;h;hpp")
    .addFilter("All files","*")
    //.setSaveMode(true) // Set save mode
    .setDecideHandler([](const Dialog& dlg){
        auto fdlg = dynamic_cast<const FileDialog&>(dlg);
        const std::vector<std::string>& fileList = fdlg.selectedPathes();
        // All pathes are in absolute format
      })
    .setCancelHandler( [](const Dialog& dlg){
        // Nothing selected as it was canceled
      }
    )
    .show();
```
Platform|Screenshot
---|---
GTK|![](screenshot/gtk_file_open.png)

### Color Picker Dialog

No more thing but you want

```c++
ColorPickerDialog cdlg("Pick a color you like");
cdlg.setColor({1,0,1,1}) // Set default selected color
.setDecideHandler( [](const Dialog& dlg){
    auto colorDlg = dynamic_cast<const ColorPickerDialog&>(dlg);
    auto color = colorDlg.color();
    // Get color value with color.r,color.g,color.b and color.a
  })
.show();
```

Platform|Screenshot
---|---
GTK|![](screenshot/gtk_color_picker.png)

#### More about usage : [test.cc](test.cc)

#### TODO:

* Native font select dialog