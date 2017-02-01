//
// Product.h
// Declaration of the Product class.
//

#pragma once

#include <collection.h>
#include "ProductAction.h"  // this is needed for Xmldocs :) 
namespace GoogleAnalytics
{
	namespace Ecommerce
	{
		/// <summary>
		/// Class to construct product related information for a Google Analytics hit. Use this class to report information about products sold by merchants or impressions of products seen by users. Instances of this class can be associated with both <see cref="ProductAction"/>s via addProduct(Product) and Product Impressions via addImpression(Product, String).
		/// </summary>
		public ref class Product sealed
		{

		private:

			Windows::Foundation::Collections::IMap<int, Platform::String^>^ customDimensions;

			Windows::Foundation::Collections::IMap<int, int>^ customMetrics;

		public:

			Product()
				: customDimensions(ref new Platform::Collections::Map<int, Platform::String^>())
				, customMetrics(ref new Platform::Collections::Map<int, int>())
			{ }

			/// <summary>
			/// Gets or sets the brand associated with the product in GA reports.
			/// </summary>
			property Platform::String^ Brand;

			/// <summary>
			/// Gets or sets the category associated with the product in GA reports.
			/// </summary>
			property Platform::String^ Category;

			/// <summary>
			/// Gets or sets the coupon code associated with the product.
			/// </summary>
			property Platform::String^ CouponCode;

			/// <summary>
			/// Gets or sets the custom dimensions associated with the product.
			/// </summary>
			property Windows::Foundation::Collections::IMap<int, Platform::String^>^ CustomDimensions
			{
				Windows::Foundation::Collections::IMap<int, Platform::String^>^ get() { return customDimensions; }
			}

			/// <summary>
			/// Gets or sets the custom metrics associated with the product.
			/// </summary>
			property Windows::Foundation::Collections::IMap<int, int>^ CustomMetrics
			{
				Windows::Foundation::Collections::IMap<int, int>^ get() { return customMetrics; }
			}

			/// <summary>
			/// Gets or sets the id that is used to identify a product in GA reports.
			/// </summary>
			property Platform::String^ Id;

			/// <summary>
			/// Gets or sets the name that is used to identify the product in GA reports.
			/// </summary>
			property Platform::String^ Name;

			/// <summary>
			/// Gets or sets the position of the product on the page/product impression list etc.
			/// </summary>
			property Platform::IBox<int>^ Position;

			/// <summary>
			/// Gets or sets the price of the product.
			/// </summary>
			property Platform::IBox<double>^ Price;

			/// <summary>
			/// Gets or sets the quantity of the product.
			/// </summary>
			property Platform::IBox<int>^ Quantity;

			/// <summary>
			/// Gets or sets the variant of the product.
			/// </summary>
			property Platform::String^ Variant;

		};
	}
}