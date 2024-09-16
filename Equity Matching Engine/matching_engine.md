# Equity_Matcher
Equity Order Matcher is a platform for matching active equity orders in an orderbook according to the type of order. It also facilitates addition of new orders,cancellation of orders or updation of order details.

## Command Description
* **Actions** :  N(New),  A(Amend),  X(Cancel),  M(Match).
* **OrderID** : An integer value in the range [1,2<sup>63</sup>-1] inclusive.
* **Timestamp** : An integer value, typically milliseconds since epoch.
* **Symbol** : Varying length string containing only alphabets.
* **OrderType** : M(Market), L(Limit) or I(IOC).
* **Side** : B(Buy) or S(Sell)
* **Price** : A float value, 0.00 if OrderType is M(Market). The price is given exactly to two places of decimal.
* **Quantity** : An integer value in the range [1,2<sup>63</sup>-1] inclusive.

## Operations

* **NEW** : Adds a new order into the orderbook.<br />
	`N`,*`OrderId,Timestamp,Symbol,OrderType,Side,Price,Quantity`*
* **AMEND** : Updates an existing order as per details in this command.<br />
	`A`,*`OrderId,Timestamp,Symbol,OrderType,Side,Price,Quantity`*
* **CANCEL** : Requests an existing order to be canceled.<br />
	`X`,*`OrderId,Timestamp`*
* **MATCH** : Matches the existing orders in the order book at that timestamp. <br />
	`M`,*`Timestamp,[Symbol]`*

For further information:  Visit this [link]().
