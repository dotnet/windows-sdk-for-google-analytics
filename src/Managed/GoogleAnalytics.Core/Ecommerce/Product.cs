using System.Collections.Generic;

namespace GoogleAnalytics.Ecommerce
{
    /// <summary>
    /// Class to construct product related information for a Google Analytics hit. Use this class to report information about products sold by merchants or impressions of products seen by users. Instances of this class can be associated with both <see cref="ProductAction"/>s via addProduct(Product) and Product Impressions via addImpression(Product, String).
    /// </summary>
    public sealed class Product
    {
        /// <summary>
        /// Creates a new instance of the <see cref="Product"/> class.
        /// </summary>
        public Product()
        {
            CustomDimensions = new Dictionary<int, string>();
            CustomMetrics = new Dictionary<int, int>();
        }

        /// <summary>
        /// Gets or sets the brand associated with the product in GA reports.
        /// </summary>
        public string Brand { get; set; }

        /// <summary>
        /// Gets or sets the category associated with the product in GA reports.
        /// </summary>
        public string Category { get; set; }

        /// <summary>
        /// Gets or sets the coupon code associated with the product.
        /// </summary>
        public string CouponCode { get; set; }

        /// <summary>
        /// Gets or sets the custom dimensions associated with the product.
        /// </summary>
        public IDictionary<int, string> CustomDimensions { get; private set; }

        /// <summary>
        /// Gets or sets the custom metrics associated with the product.
        /// </summary>
        public IDictionary<int, int> CustomMetrics { get; private set; }

        /// <summary>
        /// Gets or sets the id that is used to identify a product in GA reports.
        /// </summary>
        public string Id { get; set; }

        /// <summary>
        /// Gets or sets the name that is used to identify the product in GA reports.
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the position of the product on the page/product impression list etc.
        /// </summary>
        public int? Position { get; set; }

        /// <summary>
        /// Gets or sets the price of the product.
        /// </summary>
        public double? Price { get; set; }

        /// <summary>
        /// Gets or sets the quantity of the product.
        /// </summary>
        public int? Quantity { get; set; }

        /// <summary>
        /// Gets or sets the variant of the product.
        /// </summary>
        public string Variant { get; set; }
    }
}
