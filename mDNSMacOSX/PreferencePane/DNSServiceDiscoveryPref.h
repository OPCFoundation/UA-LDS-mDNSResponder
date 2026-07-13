/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
 *
 * OPC Foundation MIT License 1.00
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * The complete license agreement can be found here:
 * http://opcfoundation.org/License/MIT/1.00/
 * ======================================================================*/

#import <Cocoa/Cocoa.h>
#import <PreferencePanes/PreferencePanes.h>
#import <CoreFoundation/CoreFoundation.h>
#import <SecurityInterface/SFAuthorizationView.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <dns_sd.h>

typedef struct MyDNSServiceState {
    DNSServiceRef service;
    CFRunLoopSourceRef source;
    CFSocketRef socket;
} MyDNSServiceState;


@interface DNSServiceDiscoveryPref : NSPreferencePane
{
    IBOutlet NSTextField         *hostName;
    IBOutlet NSTextField         *sharedSecretName;
    IBOutlet NSSecureTextField   *sharedSecretValue;
    IBOutlet NSComboBox          *browseDomainsComboBox;
    IBOutlet NSComboBox          *regDomainsComboBox;
    IBOutlet NSButton            *wideAreaCheckBox;
    IBOutlet NSButton            *hostNameSharedSecretButton;
    IBOutlet NSButton            *registrationSharedSecretButton;
    IBOutlet NSButton            *applyButton;
    IBOutlet NSButton            *revertButton;
    IBOutlet NSWindow            *sharedSecretWindow;
    IBOutlet NSWindow            *addBrowseDomainWindow;
    IBOutlet NSButton            *addBrowseDomainButton;
    IBOutlet NSButton            *removeBrowseDomainButton;
    IBOutlet NSButton            *browseOKButton;
    IBOutlet NSButton            *browseCancelButton;
    IBOutlet NSButton            *secretOKButton;
    IBOutlet NSButton            *secretCancelButton;
    IBOutlet NSImageView         *statusImageView;
    IBOutlet NSTabView           *tabView;
    IBOutlet NSTableView         *browseDomainList;
    IBOutlet SFAuthorizationView *comboAuthButton;

    NSWindow       *mainWindow;
    NSString       *currentHostName;
    NSString       *currentRegDomain;
    NSArray        *currentBrowseDomainsArray;
    NSMutableArray *browseDomainsArray;
    NSMutableArray *defaultBrowseDomainsArray;
    NSString       *defaultRegDomain;

    NSString       *hostNameSharedSecretName;
    NSString       *hostNameSharedSecretValue;
    NSString       *regSharedSecretName;
    NSString       *regSharedSecretValue;
    BOOL currentWideAreaState;
    BOOL prefsNeedUpdating;
    BOOL toolInstalled;
    BOOL browseDomainListEnabled;
    BOOL justStartedEditing;
    NSImage        *successImage;
    NSImage        *inprogressImage;
    NSImage        *failureImage;

    MyDNSServiceState regQuery;
    MyDNSServiceState browseQuery;
    NSMutableArray    *browseDataSource;
    NSMutableArray    *registrationDataSource;
}

-(IBAction)applyClicked : (id)sender;
-(IBAction)enableBrowseDomainClicked : (id)sender;
-(IBAction)addBrowseDomainClicked : (id)sender;
-(IBAction)removeBrowseDomainClicked : (id)sender;
-(IBAction)revertClicked : (id)sender;
-(IBAction)changeButtonPressed : (id)sender;
-(IBAction)closeMyCustomSheet : (id)sender;
-(IBAction)comboAction : (id)sender;
-(IBAction)wideAreaCheckBoxChanged : (id)sender;


-(NSMutableArray *)browseDataSource;
-(NSMutableArray *)registrationDataSource;
-(NSComboBox *)browseDomainsComboBox;
-(NSComboBox *)regDomainsComboBox;
-(NSString *)currentRegDomain;
-(NSMutableArray *)defaultBrowseDomainsArray;
-(NSArray *)currentBrowseDomainsArray;
-(NSString *)currentHostName;
-(NSString *)defaultRegDomain;
-(void)setDefaultRegDomain : (NSString *)domain;



-(void)enableApplyButton;
-(void)disableApplyButton;
-(void)applyCurrentState;
-(void)setBrowseDomainsComboBox;
-(void)setupInitialValues;
-(void)startDomainBrowsing;
-(void)toggleWideAreaBonjour : (BOOL)state;
-(void)updateApplyButtonState;
-(void)enableControls;
-(void)disableControls;
-(void)validateTextFields;
-(void)readPreferences;
-(void)savePreferences;
-(void)restorePreferences;
-(void)watchForPreferenceChanges;
-(void)updateStatusImageView;


-(NSString *)sharedSecretKeyName : (NSString * )domain;
-(NSString *)domainForHostName : (NSString *)hostNameString;
-(int)statusForHostName : (NSString * )domain;
-(NSData *)dataForDomainArray : (NSArray *)domainArray;
-(NSData *)dataForDomain : (NSString *)domainName isEnabled : (BOOL)enabled;
-(NSData *)dataForSharedSecret : (NSString *)secret domain : (NSString *)domainName key : (NSString *)keyName;
-(BOOL)domainAlreadyInList : (NSString *)domainString;
-(NSString *)trimCharactersFromDomain : (NSString *)domain;


// Delegate methods
-(void)authorizationViewDidAuthorize : (SFAuthorizationView *)view;
-(void)authorizationViewDidDeauthorize : (SFAuthorizationView *)view;
-(void)mainViewDidLoad;
-(int)numberOfItemsInComboBox : (NSComboBox *)aComboBox;
-(id)comboBox : (NSComboBox *)aComboBox objectValueForItemAtIndex : (int)index;
-(void)controlTextDidChange : (NSNotification *) notification;

@end
