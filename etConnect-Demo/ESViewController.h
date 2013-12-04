//
//  ESViewController.h
//  etConnect-Demo
//
//  Created by Thomas Krautter on 30.11.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <ETConnect/ETConnect.h>
#import "ESRemoteObject.h"
#import "ESDetailViewController.h"

@interface ESViewController : UIViewController
<UITableViewDataSource, UITableViewDelegate>
{
}

- (IBAction)publish:(UISwitch *)sender;
- (IBAction)enableBluetooth:(UISwitch *)sender;

@property (retain, nonatomic) IBOutlet UISwitch *swPublish;
@property (retain, nonatomic) IBOutlet UISwitch *swBluetooth;
@property (retain, nonatomic) IBOutlet UITableView *tvHosts;
   
/**
 *  this property holds (and retains) the current list of host.
 */
@property (retain, nonatomic) NSArray *hosts;
    
/**
 * if we're publishing, these propertiers hold the root and the EIConnection
 */
@property (retain, nonatomic) EIConnection *publishedConnection;
@property (retain, nonatomic) ESRemoteObject *publishedRoot;
    
@end
