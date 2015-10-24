#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include <GLFW/glfw3native.h>

#include "NativeDialog.h"
using namespace NativeDialog;

void showDialogs(GLFWwindow* wnd);

ColorPickerDialog cdlg("Pick a color you like");

FileDialog fdlg("Open some text files",
                FileDialog::SELECT_FILE|FileDialog::MULTI_SELECT);

MessageDialog msgDialog("Message Dialog","\nSelect the next dialog to show.\n",
                        {"File Dialog","Color Picker","Nothing"}
                        );

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwSetErrorCallback([](int,const char* e){
        MessageDialog("Error",e,{"OK"}).show();
    });
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 640, "Native Dialogs", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glClearColor(0,0,0, 1);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();
        if( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS )
        {
            showDialogs(window);
        }
        
        /* Render Color here */
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

void fileSelHandler(const Dialog& dlg);

void showDialogs(GLFWwindow* wnd)
{
    msgDialog.setDecideHandler([wnd](const Dialog& dlg)
    {
       auto mdlg = dynamic_cast<const MessageDialog&>(dlg);
       if( mdlg.responseButtonIndex() == 1 )
       {
           cdlg.setColor({1,0,1,1})
           .setDecideHandler( [](const Dialog& dlg)
             {
                 auto colorDlg = dynamic_cast<const ColorPickerDialog&>(dlg);
                 auto color = colorDlg.color();
                 glClearColor(color.r, color.g, color.b, color.a);
                 glClear(GL_COLOR_BUFFER_BIT);
             })
           .show();
       }
       else if( mdlg.responseButtonTitle() == "File Dialog" )
       {
           
           fdlg.setDefaultPath("..")
           .addFilter("Text Files","txt")
           .addFilter("Code files","c;cpp;h;hpp")
           .addFilter("All files","*")
           //.setSaveMode(true) // Set save mode
           .setHostWindow(glfwGetCocoaWindow(wnd) )
           .setDecideHandler(fileSelHandler)
           .setCancelHandler( [](const Dialog& dlg)
             {
                 MessageDialog("","You canceled select files",{"Close"}).show();
             })
           .show();
       }
    })
    .show();
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