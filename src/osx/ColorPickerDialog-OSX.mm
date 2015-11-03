#include "../NativeDialog.h"
#ifdef ND_PLATFORM_OSX

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include <map>

using namespace NativeDialog;

@interface OSXColorPanel : NSObject
-(void)colorUpdate:(NSColorPanel*)colorPanel;
-(void)show:(ColorPickerDialog*)dialog;
@property ColorPickerDialog* dialog;
@end

@implementation OSXColorPanel
-(void)show:(NativeDialog::ColorPickerDialog*)dialog{
    [self setDialog:dialog];
    NSColorPanel *colorPanel = [NSColorPanel sharedColorPanel];
    if(dialog)
    {
        const auto & color = dialog->color();
        NSColor* nscolor = [NSColor colorWithRed:color.r
                                            green:color.g
                                            blue:color.b
                                            alpha:color.a];

        [colorPanel setColor:nscolor];
        [colorPanel setTitle: [NSString stringWithUTF8String:dialog->title().c_str()]];
    }
    [colorPanel setShowsAlpha:YES];
    [colorPanel setTarget:self];
    [colorPanel setAction: @selector(colorUpdate:) ];
    [colorPanel setIsVisible:YES];
}

-(void)colorUpdate:(NSColorPanel*)colorPanel{
    NSColor* theColor = colorPanel.color;
    ColorPickerDialog::Color color({
        [theColor redComponent] ,
        [theColor greenComponent],
        [theColor blueComponent],
        [theColor alphaComponent]
    });

    if( [self dialog] )
    {   [self dialog]->setColor(color);
        [self dialog]->decideHandler()(*[self dialog]);
    }
}
@end

namespace NativeDialog
{
    OSXColorPanel * nativeCaller = nil;

    std::map<ColorPickerDialog*,OSXColorPanel*> dialog_panelMap;


    void ColorPickerDialog::show()
    {
        auto foundIt = dialog_panelMap.find(this);
        if( foundIt == dialog_panelMap.end() || foundIt->second == nullptr )
        {
            OSXColorPanel* colorPanel = [[OSXColorPanel alloc]init];
            dialog_panelMap[this] = colorPanel;
            [colorPanel show:this];
        }
        else
        {
            OSXColorPanel* colorPanel = foundIt->second;
            [colorPanel show:this];
        }

    }

    ColorPickerDialog::~ColorPickerDialog()
    {
        auto foundIt = dialog_panelMap.find(this);
        if( foundIt!= dialog_panelMap.end())
        {
            OSXColorPanel* bindPanel = foundIt->second;
            if( bindPanel )
            {
                [bindPanel setDialog:nullptr];
                [bindPanel release];
            }
            foundIt->second = nullptr;
            dialog_panelMap[this] = nullptr;
        }
    }
}
#endif
