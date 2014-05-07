#include "hypnos.h"

#ifdef __APPLE__
//#ifdef __OBJC__
#import "Core/NSRenderer.h"
#import "AppKit/AppKit.h"
#import "Foundation/Foundation.h"
//#import <Cocoa/Cocoa.h>
//#import <Foundation/NSArray.h>
//#import <Foundation/NSString.h>
//#import <AppKit/NSApplication.h>
//#import <objc/Object.h>
//#endif
#endif

//#ifdef __cplusplus
//extern "C" {
//#endif
int main(int argc, char** argv)
{
    Hypnos app;
//#ifdef __OBJC__
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    printf("test\n");
    mAppDelegate = [[AppDelegate alloc] init];
    printf("test2\n");
    [[NSApplication sharedApplication] setDelegate:mAppDelegate];
    printf("test3\n");
    int retVal = NSApplicationMain(argc, (const char **) argv);
    printf("test4\n");
    [pool release];

    return retVal;
//#endif
}
//#ifdef __cplusplus
//}
//#endif
