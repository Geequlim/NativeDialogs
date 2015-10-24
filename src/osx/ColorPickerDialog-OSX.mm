#include "../NativeDialog.h"
#ifdef ND_PLATFORM_OSX

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

using namespace Walnut;

@interface WalnutColorPanel : NSObject
-(void)colorUpdate:(NSColorPanel*)colorPanel;
-(void)show:(Walnut::Util::ColorPickerDialog*)dialog;
@property Walnut::Util::ColorPickerDialog* dialog;
@end

@implementation WalnutColorPanel
-(void)show:(Walnut::Util::ColorPickerDialog*)dialog{
    [self setDialog:dialog];
    NSColorPanel *colorPanel = [NSColorPanel sharedColorPanel];
    if(dialog)
    {
        const Color & color = dialog->color();
        NSColor* nscolor = [NSColor colorWithRed:color.redPercent()
                                            green:color.greenPercent()
                                            blue:color.bluePercent()
                                            alpha:color.alphaPercent()];
        
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
    Color color{
        [theColor redComponent] ,
        [theColor greenComponent],
        [theColor blueComponent],
        [theColor alphaComponent]
    };
    
    if(_dialog)
    {   _dialog->setColor(color);
        Event e(NormalEventType::CONFIRMED,false,false);
        _dialog->dispatchEvent(e);
    }
}
@end

namespace Walnut
{
    namespace Util
    {
        WalnutColorPanel * nativeCaller = nil;
        void ColorPickerDialog::show()
        {
            if(nativeCaller == nil) nativeCaller = [[WalnutColorPanel alloc]init];
            [nativeCaller show:this];
        }
        
        ColorPickerDialog::~ColorPickerDialog()
        {
            if(nativeCaller)
            {
                [nativeCaller setDialog:nullptr];
                [nativeCaller release];
            }
        }
    }
}
#endif