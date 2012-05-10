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

/*! \addtogroup PurchaseLib
 *  @{
 */

/**
 *  @defgroup PurchaseLib Purchase Library
 *  @{
 */

/**
 * @file Purchase.h
 * @author Bogdan Iusco
 * @date 3 May 2012
 *
 * @brief
 */

#ifndef PURCHASE_H_
#define PURCHASE_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <maapi.h>

#include "PurchaseListener.h"

namespace Purchase
{

	// Forward declaration.
	class PurchaseManager;

	class Purchase
	{
	public:
		/**
		 * Constructor.
		 * @param productID String that identifies the product.
		 * This string must be used by the App Store / Google Play to identify
		 * the product.
		 */
		Purchase(const MAUtil::String& productID);

		/**
		 * Destructor.
		 */
		~Purchase();

		/**
		 * @return The handle of the purchase.
		 */
		MAHandle getHandle() const;

		/**
		 * Handles a purchase event.
		 * Observers will be notified regarding this event.
		 * @param purchaseData Purchase event data.
		 */
		void handlePurchaseEvent(const MAPurchaseEventData& purchaseData);

		/**
		 * Add an event listener for this purchase.
		 * @param listener The listener that will receive purchase events for
		 * this product.
		 */
		void addPurchaseListener(PurchaseListener* listener);

		/**
		 * Remove the event listener for this purchase.
		 * @param listener The listener that receives purchase events
		 * for this purchase.
		 */
		void removePurchaseListener(PurchaseListener* listener);

	private:
		/**
		 * Creates a purchase using a product handle.
		 * Used to create restored or refunded products.
		 * @param productHandle Handle to the product.
		 */
		Purchase(MAHandle productHandle);

		/**
		 * Handle MA_PURCHASE_EVENT_PRODUCT_CREATE.
		 * Notifies listeners about the event.
		 * @param purchaseData Event data.
		 */
		void handleProductCreateEvent(const MAPurchaseEventData& purchaseData);

		/**
		 * Handle MA_PURCHASE_EVENT_REQUEST.
		 * Notifies listeners about the event.
		 * @param purchaseData Event data.
		 */
		void handleRequestEvent(const MAPurchaseEventData& purchaseData);

		/**
		 * Handle MA_PURCHASE_EVENT_RECEIPT.
		 * Notifies listeners about the event.
		 * @param purchaseData Event data.
		 */
		void handleReceiptEvent(const MAPurchaseEventData& purchaseData);

	private:
		/**
		 * Handle that identifies the product.
		 */
		MAHandle mHandle;

		/**
		 * Transaction receipt.
		 */
		Receipt* mReceipt;

		/**
		 * Array with product listeners.
		 */
		MAUtil::Vector<PurchaseListener*> mPurchaseEventListeners;

		// Let the PurchaseManager create Purchase objects using a product handle.
		friend class PurchaseManager;
	};
}

#endif /* PURCHASE_H_ */

/*! @} */