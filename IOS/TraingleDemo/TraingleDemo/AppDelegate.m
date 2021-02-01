//
//  AppDelegate.m
//  TraingleDemo
//
//  Created by xuweicheng on 29.1.21.
//

#import "AppDelegate.h"
#import "OpenGLViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    _window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    _window.rootViewController = [[OpenGLViewController alloc] init];
    [_window makeKeyAndVisible];
    return YES;
}

@end
