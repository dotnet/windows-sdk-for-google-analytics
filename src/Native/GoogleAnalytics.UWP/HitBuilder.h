//
// HitBuilder.h
// Declaration of the HitBuilder class.
//
#pragma once

#include "Ecommerce/Product.h"
#include "Ecommerce/ProductAction.h"
#include "Ecommerce/Promotion.h"

#include "Tracker.h" 
 

namespace GoogleAnalytics
{

	ref class GoogleAnalytics::Ecommerce::Product;
	ref class GoogleAnalytics::Ecommerce::ProductAction;

	/// <summary>
	/// Class to build hits. You can add any of the other fields to the builder using common set and get methods.
	/// </summary>
	public ref class HitBuilder sealed
	{
	private:

		static Platform::String^ HitType_Screenview;
		
		static Platform::String^ HitType_Event;
		
		static Platform::String^ HitType_Exception;
		
		static Platform::String^ HitType_SocialNetworkInteraction;
		
		static Platform::String^ HitType_UserTiming;
		
		Windows::Foundation::Collections::IVector<HitBuilder^>^ lineage;

		Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ data;

		HitBuilder();

		HitBuilder(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ data);

		HitBuilder(Windows::Foundation::Collections::IVector<HitBuilder^>^ lineage, Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ data);
		
	public:

		property Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ Data
		{
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ get() { return data; }
		}

		property int ProductCount;

		property int PromotionCount;
		
		/// <summary>
		/// Creates a screen view hit.
		/// </summary>
		static HitBuilder^ CreateScreenView();

		/// <summary>
		/// Creates a screen view hit.
		/// </summary>
        /// <param name="screenName">Specifies the 'Screen Name' of the screenview hit. Note: this will not affect subsequent hits. To do this, set the ScreenName property on the Tracker instead.</param>
		static HitBuilder^ CreateScreenView(Platform::String^ screenName);
		
		/// <summary>
		/// Creates an event hit to track events.
		/// </summary>
		/// <param name="category">Specifies the event category. Must not be empty.</param>
		/// <param name="action">Specifies the event action. Must not be empty.</param>
		/// <param name="label">Specifies the event label. Optional if null.</param>
		/// <param name="value">Specifies the event value. Values must be non-negative. Optional if zero.</param>
		static HitBuilder^ CreateCustomEvent(Platform::String^ category, Platform::String^ action, Platform::String^ label, long long value);
		
		/// <summary>
		/// Creates an exception hit to track errors.
		/// </summary>
		/// <param name="description">Specifies the description of an exception.</param>
		/// <param name="isFatal">Specifies whether the exception was fatal.</param>
		static HitBuilder^ CreateException(Platform::String^ description, bool isFatal);
		
		/// <summary>
		/// Creates a social networking interaction hit.
		/// </summary>
		/// <param name="network">Specifies the social network, for example Facebook or Google Plus.</param>
		/// <param name="action">Specifies the social interaction action. For example on Google Plus when a user clicks the +1 button, the social action is 'plus'.</param>
		/// <param name="target">Specifies the target of a social interaction. This value is typically a URL but can be any text.</param>
		static HitBuilder^ CreateSocialInteraction(Platform::String^ network, Platform::String^ action, Platform::String^ target);
		
		/// <summary>
		/// Creates a user timing hit to measure app timing and performance.
		/// </summary>
		/// <param name="category">Specifies the user timing category.</param>
		/// <param name="variable">Specifies the user timing variable.</param>
		/// <param name="value">Specifies the user timing value.</param>
		/// <param name="label">Specifies the user timing label.</param>
		static HitBuilder^ CreateTiming(Platform::String^ category, Platform::String^ variable, Platform::IBox<Windows::Foundation::TimeSpan>^ time, Platform::String^ label);
		
		/// <summary>
		/// Looks up a value by name from the current instance.
		/// </summary>
		/// <param name="paramName">The parameter name to get the value for.</param>
		/// <returns>The value associated with the supplied parameter name.</returns>
		Platform::String^ Get(Platform::String^ paramName);

		/// <summary>
		/// Sets the value for the given parameter name. These values will be added to the hit when it is built. This function should only be used for advanced cases where none of the explicit setters do not work. This function should usually be called after all the explicit setter have been called.
		/// </summary>
		/// <param name="paramName">The parameter name to set the value for.</param>
		/// <param name="paramValue">The value associated with the parameter name.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ Set(Platform::String^ paramName, Platform::String^ paramValue);

		/// <summary>
		/// Adds a set of key, value pairs to the hit builder. These values will be added to the hit when it is built. This function should only be used for advanced cases where none of the explicit setters work. This function should usually be called after all the explicit setter have been called.
		/// </summary>
		/// <param name="params">A dictionary of all the values to be added to the builder.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ SetAll(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ params);

		/// <summary>
		/// Adds a custom dimension to the current hit builder. Calling this method with the same index will overwrite the previous dimension with the new one. Refer <see href="https://goo.gl/igziD2"/> for details on how to set the custom dimensions up.
		/// </summary>
		/// <param name="index">The index/slot in which the dimension will be set.</param>
		/// <param name="dimension">The value of the dimension for the given index.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ SetCustomDimension(int index, Platform::String^ dimension);

		/// <summary>
		/// Adds a custom metric to the current hit builder. Calling this method with the same index will overwrite the previous metric with the new one. Refer <see href="https://goo.gl/igziD2"/> for details on how to set the custom metrics up.
		/// </summary>
		/// <param name="index">The index/slot in which the metric will be set.</param>
		/// <param name="metric">The value of the metric for the given index.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ SetCustomMetric(int index, long long metric);
		
		/// <summary>
		/// Starts a new session for the hit.
		/// </summary>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ SetNewSession();

		/// <summary>
		/// Indicates that the hit did not involve a user interaction.
		/// </summary>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ SetNonInteraction();

		/// <summary>
		/// Adds product information to be sent with a given hit. The action provided affects how the products passed in through this method get processed.
		/// </summary>
		/// <param name="product">The product you wish to add.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ AddProduct(Ecommerce::Product^ product);

		/// <summary>
		/// Adds promotion related information to the hit.
		/// </summary>
		/// <param name="promotion">The promotion related to the hit.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ AddPromotion(Ecommerce::Promotion^ promotion);

		/// <summary>
		/// Sets a product action for all the products included in this hit. The action and its associated properties affect how the products added through <see cref="AddProduct(Ecommerce.Product)"/> are processed. 
		/// </summary>
		/// <param name="action">The product action associated with the hit.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ SetProductAction(Ecommerce::ProductAction^ action);

		/// <summary>
		/// Adds an action associated with the promotions in a given hit.
		/// </summary>
		/// <param name="action">The action associated with the hit.</param>
		/// <returns>The builder object that you can use to chain calls.</returns>
		HitBuilder^ SetPromotionAction(Ecommerce::PromotionAction action);

		/// <summary>
		/// Builds a dictionary of parameters and values that can be set on the <see cref="Tracker"/> object.
		/// </summary>
		/// <returns>The dictionary to send to <see cref="Tracker::Send"/>.</returns>
		Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ Build();
	};
}
