using System;
using System.Collections.Generic;
using System.Globalization;

namespace GoogleAnalytics
{
    /// <summary>
    /// Class to build hits. You can add any of the other fields to the builder using common set and get methods.
    /// </summary>
    public sealed class HitBuilder
    {
        const string HitType_Screenview = "screenview";
        const string HitType_Event = "event";
        const string HitType_Exception = "exception";
        const string HitType_SocialNetworkInteraction = "social";
        const string HitType_UserTiming = "timing";

        readonly IList<HitBuilder> lineage;
        readonly IDictionary<string, string> data;

        private IDictionary<string, string> Data { get { return data; } }
        private int ProductCount { get; set; }
        private int PromotionCount { get; set; }

        private HitBuilder()
        {
            lineage = new List<HitBuilder>();
            lineage.Add(this);

            data = new Dictionary<string, string>();
        }

        private HitBuilder(IDictionary<string, string> data)
        {
            this.lineage = new List<HitBuilder>();
            this.lineage.Add(this);

            this.data = new Dictionary<string, string>(data);
        }

        private HitBuilder(IList<HitBuilder> lineage, IDictionary<string, string> data)
        {
            this.lineage = new List<HitBuilder>(lineage);
            this.lineage.Add(this);

            this.data = new Dictionary<string, string>(data);
        }
        
        /// <summary>
        /// Creates an event hit to track events.
        /// </summary>
        /// <param name="category">Specifies the event category. Must not be empty.</param>
        /// <param name="action">Specifies the event action. Must not be empty.</param>
        /// <param name="label">Specifies the event label. Optional if null.</param>
        /// <param name="value">Specifies the event value. Values must be non-negative. Optional if zero.</param>
        public static HitBuilder CreateCustomEvent(string category, string action, string label = null, long value = 0L)
        {
            if (category == null) throw new ArgumentNullException(nameof(category));
            if (action == null) throw new ArgumentNullException(nameof(action));

            var data = new Dictionary<string, string>();
            data.Add("t", HitType_Event);
            data.Add("ec", category);
            data.Add("ea", action);
            if (label != null) data.Add("el", label);
            if (value != 0L) data.Add("ev", value.ToString(CultureInfo.InvariantCulture));
            return new HitBuilder(data);
        }

        /// <summary>
        /// Creates an exception hit to track errors.
        /// </summary>
        /// <param name="description">Specifies the description of an exception.</param>
        /// <param name="isFatal">Specifies whether the exception was fatal.</param>
        public static HitBuilder CreateException(string description, bool isFatal)
        {
            if (description == null) throw new ArgumentNullException(nameof(description));

            var data = new Dictionary<string, string>();
            data.Add("t", HitType_Exception);
            if (description != null) data.Add("exd", description);
            if (!isFatal) data.Add("exf", "0");
            return new HitBuilder(data);
        }

        /// <summary>
        /// Creates a screen view hit.
        /// </summary>
        /// <param name="screenName">Specifies the 'Screen Name' of the screenview hit. Note: this will not affect subsequent hits. To do this, set the ScreenName property on the <see cref="Tracker"/> instead.</param>
        public static HitBuilder CreateScreenView(string screenName = null)
        {
            var data = new Dictionary<string, string>();
            data.Add("t", HitType_Screenview);
            if (screenName != null) data.Add("cd", screenName);
            return new HitBuilder(data);
        }

        /// <summary>
        /// Creates a social networking interaction hit.
        /// </summary>
        /// <param name="network">Specifies the social network, for example Facebook or Google Plus.</param>
        /// <param name="action">Specifies the social interaction action. For example on Google Plus when a user clicks the +1 button, the social action is 'plus'.</param>
        /// <param name="target">Specifies the target of a social interaction. This value is typically a URL but can be any text.</param>
        public static HitBuilder CreateSocialInteraction(string network, string action, string target)
        {
            if (network == null) throw new ArgumentNullException(nameof(network));
            if (action == null) throw new ArgumentNullException(nameof(action));
            if (target == null) throw new ArgumentNullException(nameof(target));

            var data = new Dictionary<string, string>();
            data.Add("t", HitType_SocialNetworkInteraction);
            data.Add("sn", network);
            data.Add("sa", action);
            data.Add("st", target);
            return new HitBuilder(data);
        }

        /// <summary>
        /// Creates a user timing hit to measure app timing and performance.
        /// </summary>
        /// <param name="category">Specifies the user timing category.</param>
        /// <param name="variable">Specifies the user timing variable.</param>
        /// <param name="value">Specifies the user timing value.</param>
        /// <param name="label">Specifies the user timing label.</param>
        public static HitBuilder CreateTiming(string category, string variable, TimeSpan? value, string label = null)
        {
            var data = new Dictionary<string, string>();
            data.Add("t", HitType_UserTiming);
            if (category != null) data.Add("utc", category);
            if (variable != null) data.Add("utv", variable);
            if (value.HasValue) data.Add("utt", Math.Round(value.Value.TotalMilliseconds).ToString(CultureInfo.InvariantCulture));
            if (label != null) data.Add("utl", label);
            return new HitBuilder(data);
        }


        // TODO: setCampaignParamsFromUrl(String utmParams) 
        // TODO: AddImpression(Ecommerce.Product product, string impressions)

        /// <summary>
        /// Looks up a value by name from the current instance.
        /// </summary>
        /// <param name="paramName">The parameter name to get the value for.</param>
        /// <returns>The value associated with the supplied parameter name.</returns>
        public string Get(string paramName)
        {
            return data[paramName];
        }

        /// <summary>
        /// Sets the value for the given parameter name. These values will be added to the hit when it is built. This function should only be used for advanced cases where none of the explicit setters do not work. This function should usually be called after all the explicit setter have been called.
        /// </summary>
        /// <param name="paramName">The parameter name to set the value for.</param>
        /// <param name="paramValue">The value associated with the parameter name.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder Set(string paramName, string paramValue)
        {
            return new HitBuilder(lineage, new Dictionary<string, string>() { { paramName, paramValue } });
        }

        /// <summary>
        /// Adds a set of key, value pairs to the hit builder. These values will be added to the hit when it is built. This function should only be used for advanced cases where none of the explicit setters work. This function should usually be called after all the explicit setter have been called.
        /// </summary>
        /// <param name="params">A dictionary of all the values to be added to the builder.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder SetAll(IDictionary<string, string> @params)
        {
            return new HitBuilder(lineage, @params);
        }

        /// <summary>
        /// Adds a custom dimension to the current hit builder. Calling this method with the same index will overwrite the previous dimension with the new one. Refer <see href="https://goo.gl/igziD2"/> for details on how to set the custom dimensions up.
        /// </summary>
        /// <param name="index">The index/slot in which the dimension will be set.</param>
        /// <param name="dimension">The value of the dimension for the given index.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder SetCustomDimension(int index, string dimension)
        {
            return new HitBuilder(lineage, new Dictionary<string, string>() { { $"cd{index}", dimension } });
        }

        /// <summary>
        /// Adds a custom metric to the current hit builder. Calling this method with the same index will overwrite the previous metric with the new one. Refer <see href="https://goo.gl/igziD2"/> for details on how to set the custom metrics up.
        /// </summary>
        /// <param name="index">The index/slot in which the metric will be set.</param>
        /// <param name="metric">The value of the metric for the given index.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder SetCustomMetric(int index, long metric)
        {
            return new HitBuilder(lineage, new Dictionary<string, string>() { { $"cm{index}", metric.ToString(CultureInfo.InvariantCulture) } });
        }

        /// <summary>
        /// Starts a new session for the hit.
        /// </summary>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder SetNewSession()
        {
            return new HitBuilder(lineage, new Dictionary<string, string>() { { "sc", "start" } });
        }

        /// <summary>
        /// Indicates that the hit did not involve a user interaction.
        /// </summary>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder SetNonInteraction()
        {
            return new HitBuilder(lineage, new Dictionary<string, string>() { { "ni", "1" } });
        }

        /// <summary>
        /// Adds product information to be sent with a given hit. The action provided affects how the products passed in through this method get processed.
        /// </summary>
        /// <param name="product">The product you wish to add.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder AddProduct(Ecommerce.Product product)
        {
            var index = ProductCount + 1;

            var data = new Dictionary<string, string>();
            if (product.Id != null) data.Add($"pr{index}id", product.Id);
            if (product.Name != null) data.Add($"pr{index}nm", product.Name);
            if (product.Brand != null) data.Add($"pr{index}br", product.Brand);
            if (product.Category != null) data.Add($"pr{index}ca", product.Category);
            if (product.Variant != null) data.Add($"pr{index}va", product.Variant);
            if (product.Price.HasValue) data.Add($"pr{index}pr", product.Price.Value.ToString(CultureInfo.InvariantCulture));
            if (product.Quantity.HasValue) data.Add($"pr{index}at", product.Quantity.Value.ToString(CultureInfo.InvariantCulture));
            if (product.CouponCode != null) data.Add($"pr{index}cc", product.CouponCode);
            if (product.Position.HasValue) data.Add($"pr{index}ps", product.Position.Value.ToString(CultureInfo.InvariantCulture));
            foreach (var item in product.CustomDimensions)
            {
                data.Add($"pr{index}cd{item.Key}", item.Value);
            }
            foreach (var item in product.CustomMetrics)
            {
                data.Add($"pr{index}cm{item.Key}", item.Value.ToString(CultureInfo.InvariantCulture));
            }
            return new HitBuilder(lineage, data) { ProductCount = index };
        }

        /// <summary>
        /// Adds promotion related information to the hit.
        /// </summary>
        /// <param name="promotion">The promotion related to the hit.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder AddPromotion(Ecommerce.Promotion promotion)
        {
            var index = PromotionCount + 1;

            var data = new Dictionary<string, string>();
            if (promotion.Id != null) data.Add($"promo{index}id", promotion.Id);
            if (promotion.Name != null) data.Add($"promo{index}nm", promotion.Name);
            if (promotion.Creative != null) data.Add($"promo{index}cr", promotion.Creative);
            if (promotion.Position != null) data.Add($"promo{index}ps", promotion.Position);
            return new HitBuilder(lineage, data) { PromotionCount = index };
        }

        /// <summary>
        /// Sets a product action for all the products included in this hit. The action and its associated properties affect how the products added through <see cref="AddProduct(Ecommerce.Product)"/> are processed. 
        /// </summary>
        /// <param name="action">The product action associated with the hit.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder SetProductAction(Ecommerce.ProductAction action)
        {
            var data = new Dictionary<string, string>();
            if (action.Action != null) data.Add("pa", action.Action);
            if (action.TransactionId != null) data.Add("ti", action.TransactionId);
            if (action.TransactionAffiliation != null) data.Add("ta", action.TransactionAffiliation);
            if (action.TransactionRevenue.HasValue) data.Add("tr", action.TransactionRevenue.Value.ToString(CultureInfo.InvariantCulture));
            if (action.TransactionTax.HasValue) data.Add("tt", action.TransactionTax.Value.ToString(CultureInfo.InvariantCulture));
            if (action.TransactionShipping.HasValue) data.Add("ts", action.TransactionShipping.Value.ToString(CultureInfo.InvariantCulture));
            if (action.TransactionCouponCode != null) data.Add("tcc", action.TransactionCouponCode);
            if (action.ProductActionList != null) data.Add("pal", action.ProductActionList);
            if (action.CheckoutStep.HasValue) data.Add("cos", action.CheckoutStep.Value.ToString(CultureInfo.InvariantCulture));
            if (action.CheckoutOptions != null) data.Add("col", action.CheckoutOptions);
            return new HitBuilder(lineage, data);
        }

        /// <summary>
        /// Adds an action associated with the promotions in a given hit.
        /// </summary>
        /// <param name="action">The action associated with the hit.</param>
        /// <returns>The builder object that you can use to chain calls.</returns>
        public HitBuilder SetPromotionAction(Ecommerce.PromotionAction action)
        {
            return new HitBuilder(lineage, new Dictionary<string, string>() { { "pa", Ecommerce.Promotion.GetAction(action) } });
        }

        /// <summary>
        /// Builds a dictionary of parameters and values that can be set on the <see cref="Tracker"/> object.
        /// </summary>
        /// <returns>The dictionary to send to <see cref="SimpleTracker.Send(IDictionary{string, string})"/>.</returns>
        public IDictionary<string, string> Build()
        {
            var result = new Dictionary<string, string>();
            foreach (var hitBuilder in lineage)
            {
                foreach (var item in hitBuilder.Data)
                {
                    result[item.Key] = item.Value;
                }
            }
            return result;
        }
    }
}
