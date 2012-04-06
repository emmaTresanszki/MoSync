/*
 Copyright (C) 2012 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

/**
 * @file PurchaseManager.mm
 * @author Bogdan Iusco
 * @date 4 Apr 2012
 *
 * @brief Design pattern: singleton.
 * Provides a general mechanism for managing purchases.
 * It communicates with the Apple App Store through Store Kit.
 */

#import "PurchaseManager.h"
#import "PurchaseProduct.h"

@implementation PurchaseManager

static PurchaseManager *sharedInstance = nil;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(PurchaseManager*) getInstance
{
    if (nil == sharedInstance)
    {
        sharedInstance = [[PurchaseManager alloc] init];
    }

    return sharedInstance;
}

/**
 * Releases the class instance.
 */
+(void) deleteInstance
{
    [sharedInstance release];
}

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        _paymentQueue = [SKPaymentQueue defaultQueue];
        [_paymentQueue addTransactionObserver:self];
        _countProducts = 0;
        _productsDictionary = [[NSMutableDictionary alloc] init];
    }

    return self;
}

/**
 * Check if purchase is availble on the device.
 * @return One of the next constants:
 * - MA_PURCHASE_RES_OK if purchase is available.
 * - MA_PURCHASE_RES_DISABLED otherwise.
 */
-(int) isPurchaseSupported
{
    return [SKPaymentQueue canMakePayments] ? MA_PURCHASE_RES_OK : MA_PURCHASE_RES_DISABLED;
}

/**
 * Create a product object.
 * @param productID String that identifies the object.
 * @return MA_PURCHASE_RES_INVALID if the productID string is empty or a handle
 * to the product object.
 */
-(MAHandle) createProduct:(const char*) productID
{
    NSString* productIDString = [NSString stringWithUTF8String:productID];
    if ([productIDString length] == 0)
    {
        return MA_PURCHASE_RES_INVALID;
    }

    NSNumber* key = [NSNumber numberWithInt:_countProducts];
    PurchaseProduct* product = [[PurchaseProduct alloc] initWithHandle:[key intValue]
                                                             productID:productIDString];
    [_productsDictionary setObject:product forKey:key];
    _countProducts++;

    return [key intValue];
}

/**
 * Destroy a product object.
 * @param productHandle Handle to the product to destroy.
 * If the given handle is invalid the method does nothing.
 */
-(void) destroyProduct:(MAHandle) productHandle
{
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (product)
    {
        [_productsDictionary removeObjectForKey:key];
        [product release];
    }
}

/**
 * Add a given product to the payment queue.
 * @param productHandle Handle to the product to be added.
 */
-(void) requestProduct:(MAHandle) productHandle
{

}

#pragma mark SKPaymentTransactionObserver methods

/**
 * Called when one or more transactions have been updated.
 * @param queue The payment queue that updated the transactions.
 * @param transactions An array of the transactions that were updated.
 */
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    NSLog(@"updatedTransactions");
}

/**
 * Called when an error occurred while restoring transactions.
 * @param queue The payment queue that was restoring transactions.
 * @param error The error that occurred.
 */
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    NSLog(@"restoreCompletedTransactionsFailedWithError");
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [_productsDictionary release];

    [super dealloc];
}

@end