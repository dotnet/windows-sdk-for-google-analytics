using System;

namespace GoogleAnalytics.Ecommerce
{
    /// <summary>
    /// Class to construct promotion related fields for Google Analytics hits. The fields from this class can be used to represent internal promotions that run within an app, such as banners, banner ads etc.
    /// </summary>
    public sealed class Promotion
    {
        /// <summary>
        /// Gets or sets the name of the creative associated with the promotion.
        /// </summary>
        public string Creative { get; set; }

        /// <summary>
        /// Gets or sets the id that is used to identify a promotion in GA reports.
        /// </summary>
        public string Id { get; set; }

        /// <summary>
        /// Gets or sets the name that is used to identify the promotion in GA reports.
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the position of the promotion.
        /// </summary>
        public string Position { get; set; }

        internal static string GetAction(PromotionAction action)
        {
            switch (action)
            {
                case PromotionAction.Click:
                    return "click";
                case PromotionAction.View:
                    return "view";
                default:
                    throw new NotImplementedException();
            }
        }
    }

    /// <summary>
    /// The product action for all the products included in the hit.
    /// </summary>
    public enum PromotionAction
    {
        /// <summary>
        /// Action to use when the user clicks/taps on a promotion.
        /// </summary>
        Click,
        /// <summary>
        /// Action to use when the user views a promotion.
        /// </summary>
        View
    }
}
