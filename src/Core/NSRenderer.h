#ifndef NSRENDERER_H
#define NSRENDERER_H

#import <Cocoa/Cocoa.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSString.h>
#import <AppKit/NSApplication.h>
#import <Foundation/NSTimer.h>
#import <Foundation/NSDate.h>
//#import <objc/Object.h>
//#import <Cocoa/Cocoa.h>

static id mAppDelegate;

@interface AppDelegate : NSObject <NSApplicationDelegate>

{
    NSTimer *mTimer;
    NSDate *mDate;
    NSTimeInterval mLastFrameTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (retain) NSTimer *mTimer;
@property (nonatomic) NSTimeInterval mLastFrameTime;

@end
#endif // NSRENDERER_H
