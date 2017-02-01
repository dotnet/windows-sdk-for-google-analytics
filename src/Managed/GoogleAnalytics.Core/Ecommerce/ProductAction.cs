using System;

namespace GoogleAnalytics.Ecommerce
{
    /// <summary>
    /// Class to construct transaction/checkout or other product interaction related information for a Google Analytics hit. Use this class to report information about products sold, viewed or refunded. This class is intended to be used with <see cref="Product"/>. Instances of this class can be associated with <see cref="HitBuilder.SetProductAction(ProductAction)"/>.
    /// </summary>
    public sealed class ProductAction
    {
        /// <summary>
        /// Creates a new instance of <see cref="ProductAction"/> with the product action for all the products included in the hit. Valid values include "detail", "click", "add", "remove", "checkout", "checkout_option", "purchase" and "refund". All these values are also defined in this class for ease of use. You also also send additional values with the hit for some specific actions. See the action documentation for details.
        /// </summary>
        /// <param name="action">The action type to send.</param>
        public ProductAction(ActionEnum action)
        {
            Action = GetAction(action);
        }

        internal static string GetAction(ActionEnum action)
        {
            switch (action)
            {
                case ActionEnum.Add:
                    return "add";
                case ActionEnum.Checkout:
                    return "checkout";
                case ActionEnum.CheckoutOption:
                    return "checkout_option";
                case ActionEnum.Click:
                    return "click";
                case ActionEnum.Detail:
                    return "detail";
                case ActionEnum.Purchase:
                    return "purchase";
                case ActionEnum.Refund:
                    return "refund";
                case ActionEnum.Remove:
                    return "remove";
                default:
                    throw new NotImplementedException();
            }
        }

        /// <summary>
        /// Gets or sets the product action for all the products included in the hit.
        /// </summary>
        public string Action { get; private set; }

        /// <summary>
        /// Gets or sets the label associated with the checkout. This value is used for <see cref="ActionEnum.Checkout"/> and <see cref="ActionEnum.CheckoutOption"/> actions.
        /// </summary>
        public string CheckoutOptions { get; set; }

        /// <summary>
        /// Gets or sets the checkout processes's progress. This value is used for <see cref="ActionEnum.Checkout"/> and <see cref="ActionEnum.CheckoutOption"/> actions.
        /// </summary>
        public int? CheckoutStep { get; set; }

        /// <summary>
        /// Gets or sets the list name associated with the products in the analytics hit. This value is used for <see cref="ActionEnum.Detail"/> and <see cref="ActionEnum.Click"/> actions.
        /// </summary>
        public string ProductActionList { get; set; }

        /// <summary>
        /// Gets or sets the list source name associated with the products in the analytics hit. This value is used for <see cref="ActionEnum.Detail"/> and <see cref="ActionEnum.Click"/> actions.
        /// </summary>
        public string ProductListSource { get; set; }

        /// <summary>
        /// Gets or sets the transaction's affiliation value. This value is used for <see cref="ActionEnum.Purchase"/> and <see cref="ActionEnum.Refund"/> actions.
        /// </summary>
        public string TransactionAffiliation { get; set; }

        /// <summary>
        /// Gets or sets the coupon code used in a transaction. This value is used for <see cref="ActionEnum.Purchase"/> and <see cref="ActionEnum.Refund"/> actions.
        /// </summary>
        public string TransactionCouponCode { get; set; }

        /// <summary>
        /// The unique id associated with the transaction. This value is used for <see cref="ActionEnum.Purchase"/> and <see cref="ActionEnum.Refund"/> actions.
        /// </summary>
        public string TransactionId { get; set; }

        /// <summary>
        /// Gets or sets the transaction's total revenue. This value is used for <see cref="ActionEnum.Purchase"/> and <see cref="ActionEnum.Refund"/> actions.
        /// </summary>
        public double? TransactionRevenue { get; set; }

        /// <summary>
        /// Gets or sets the transaction's shipping costs. This value is used for <see cref="ActionEnum.Purchase"/> and <see cref="ActionEnum.Refund"/> actions.
        /// </summary>
        public double? TransactionShipping { get; set; }

        /// <summary>
        /// Gets or sets the transaction's total tax. This value is used for <see cref="ActionEnum.Purchase"/> and <see cref="ActionEnum.Refund"/> actions.
        /// </summary>
        public double? TransactionTax { get; set; }
    }

    /// <summary>
    /// The product action for all the products included in the hit.
    /// </summary>
    public enum ActionEnum
    {
        /// <summary>
        /// Action to use when a product is added to the cart.
        /// </summary>
        Add,
        /// <summary>
        /// Action to use for hits with checkout data.
        /// </summary>
        Checkout,
        /// <summary>
        /// Action to be used for supplemental checkout data that needs to be provided after a checkout hit.
        /// </summary>
        CheckoutOption,
        /// <summary>
        /// Action to use when the user clicks on a set of products.
        /// </summary>
        Click,
        /// <summary>
        /// Action to use when the user views detailed descriptions of products.
        /// </summary>
        Detail,
        /// <summary>
        /// Action that is used to report all the transaction data to GA.
        /// </summary>
        Purchase,
        /// <summary>
        /// Action to use while reporting refunded transactions to GA.
        /// </summary>
        Refund,
        /// <summary>
        /// Action to use when a product is removed from the cart.
        /// </summary>
        Remove
    }
}
