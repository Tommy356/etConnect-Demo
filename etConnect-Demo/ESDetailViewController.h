//
//  ESDetailViewController.h
//  etConnect-Demo
//
//  Created by Thomas Krautter on 03.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <ETConnect/ETConnect.h>
#import "ESRemoteObject.h"

@interface ESDetailViewController : UIViewController

- (IBAction)btnDoneSelected:(UIButton *)sender;
- (IBAction)connect:(UIButton *)sender;
- (IBAction)pingPong:(UIButton *)sender;
  
/* these properties will be set by the presenter: */
@property (nonatomic, retain) NSString* hostName;

/* the title label - will be set to the host name, after the view has been loaded */
@property (retain, nonatomic) IBOutlet UILabel *lblTitle;
@property (retain, nonatomic) IBOutlet UILabel *lblInfo;
    
/* a title icon - set active, if the device displayed publishes the demo service */    
@property (retain, nonatomic) IBOutlet UIImageView *titleIcon;

/* upon tap, this will connect to the service ... */
@property (retain, nonatomic) IBOutlet UIButton *btnConnect;
@property (retain, nonatomic) IBOutlet UIButton *btnPingPong;
@end
