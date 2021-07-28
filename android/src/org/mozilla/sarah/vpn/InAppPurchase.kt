/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.sarah.vpn

import androidx.lifecycle.MutableLiveData
import android.util.Log
import android.content.Context
import com.android.billingclient.api.*
import com.android.billingclient.api.BillingClient.SkuType


class InAppPurchase () {

    companion object {

        private const val TAG = "InAppPurchase"

        @JvmStatic
        fun startBillingClient(c: Context, p: String) {

            /**
             * SkuDetails for all known SKUs.
             */
            val skusWithSkuDetails = MutableLiveData<Map<String, SkuDetails>>()

            val purchasesUpdatedListener =
                PurchasesUpdatedListener { billingResult: BillingResult, purchases ->
                    // To be implemented in a later section.
                }

            val billingClient = BillingClient.newBuilder(c)
                .setListener(purchasesUpdatedListener)
                .enablePendingPurchases()
                .build()

            billingClient.startConnection(object : BillingClientStateListener, SkuDetailsResponseListener {

                override fun onSkuDetailsResponse(billingResult: BillingResult, skuDetailsList: MutableList<SkuDetails>?) {
                    val responseCode = billingResult.responseCode
                    val debugMessage = billingResult.debugMessage
                    when (responseCode) {
                        BillingClient.BillingResponseCode.OK -> {
                            Log.i(TAG, "onSkuDetailsResponse: $responseCode $debugMessage")
                            val expectedSkuDetailsCount = 1
                            if (skuDetailsList == null) {
                                skusWithSkuDetails.postValue(emptyMap())
                                Log.e(TAG, "onSkuDetailsResponse: " +
                                        "Expected ${expectedSkuDetailsCount}, " +
                                        "Found null SkuDetails. " +
                                        "Check to see if the SKUs you requested are correctly published " +
                                        "in the Google Play Console.")
                            } else
                                skusWithSkuDetails.postValue(HashMap<String, SkuDetails>().apply {
                                    for (details in skuDetailsList) {
                                        Log.i(TAG, "here we go")
                                        Log.v(TAG, details.description)
                                        Log.v(TAG, details.price)
                                        Log.v(TAG, details.priceCurrencyCode)
                                        put(details.sku, details)
                                    }
                                }.also { postedValue ->
                                    val skuDetailsCount = postedValue.size
                                    if (skuDetailsCount == expectedSkuDetailsCount) {
                                        Log.i(TAG, "onSkuDetailsResponse: Found ${skuDetailsCount} SkuDetails")
                                    } else {
                                        Log.e(TAG, "onSkuDetailsResponse: " +
                                                "Expected ${expectedSkuDetailsCount}, " +
                                                "Found ${skuDetailsCount} SkuDetails. " +
                                                "Check to see if the SKUs you requested are correctly published " +
                                                "in the Google Play Console.")
                                    }
                                })
                        }
                        BillingClient.BillingResponseCode.SERVICE_DISCONNECTED,
                        BillingClient.BillingResponseCode.SERVICE_UNAVAILABLE,
                        BillingClient.BillingResponseCode.BILLING_UNAVAILABLE,
                        BillingClient.BillingResponseCode.ITEM_UNAVAILABLE,
                        BillingClient.BillingResponseCode.DEVELOPER_ERROR,
                        BillingClient.BillingResponseCode.ERROR -> {
                            Log.e(TAG, "onSkuDetailsResponse: $responseCode $debugMessage")
                        }
                        BillingClient.BillingResponseCode.USER_CANCELED,
                        BillingClient.BillingResponseCode.FEATURE_NOT_SUPPORTED,
                        BillingClient.BillingResponseCode.ITEM_ALREADY_OWNED,
                        BillingClient.BillingResponseCode.ITEM_NOT_OWNED -> {
                            // These response codes are not expected.
                            Log.wtf(TAG, "onSkuDetailsResponse: $responseCode $debugMessage")
                        }
                    }
                }

                override fun onBillingSetupFinished(billingResult: BillingResult) {
                    if (billingResult.responseCode ==  BillingClient.BillingResponseCode.OK) {
                        val productList = listOf("org.mozilla.sarah.vpn.monthly")
                        /*
                        val params = SkuDetailsParams.newBuilder()
                            .setSkusList(productList)
                            .setType(SkuType.SUBS)
                            .build()
                        Log.v(TAG, params.skusList.toString())
                        val productDetailsResult = billingClient.querySkuDetailsAsync(params, skuDetailsResponseListener)
                        */
                        val params = SkuDetailsParams.newBuilder()
                            .setType(BillingClient.SkuType.SUBS)
                            .setSkusList(productList)
                            .build()
                        Log.v(TAG, "The params list is: ${params.skuType} ${params.skusList.toString()}")
                        params.let { skuDetailsParams ->
                            Log.i(TAG, "querySkuDetailsAsync")
                            billingClient.querySkuDetailsAsync(skuDetailsParams, this)
                        }
                    }
                }

                override fun onBillingServiceDisconnected() {
                    // Try to restart the connection on the next request to
                    // Google Play by calling the startConnection() method.
                    Log.v(TAG, "BILLING SERVICE DISCONNECTED")
                }
            })
        }
    }
}
