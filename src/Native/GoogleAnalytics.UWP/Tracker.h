//
// Tracker.h
// Declaration of the Tracker class.
//

#pragma once

#include "TokenBucket.h"
#include "AnalyticsManager.h"
#include "Hit.h"
#include "IPlatformInfoProvider.h"
#include "IServiceManager.h"

namespace GoogleAnalytics
{
	ref class AnalyticsManager;

	/// <summary>
	/// Represents an object capable of tracking events for a single Google Analytics property.
	/// </summary>
	public ref class Tracker sealed
	{
	private:

		GoogleAnalytics::IServiceManager^ analyticsManager;

		Platform::String^ propertyId;

		Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ data;

		Windows::Foundation::EventRegistrationToken viewPortResolutionChangedEventToken;

		Windows::Foundation::EventRegistrationToken screenResolutionChangedEventToken;

		GoogleAnalytics::IPlatformInfoProvider^ platformInfoProvider;

		void platformTrackingInfo_ViewPortResolutionChanged(Platform::Object^ sender, Platform::Object^ args);

		void platformTrackingInfo_ScreenResolutionChanged(Platform::Object^ sender, Platform::Object^ args);

		Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ AddRequiredHitData(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ hit);

		bool IsSampledOut();

	public:

		Tracker(Platform::String^ propertyId, GoogleAnalytics::IPlatformInfoProvider^ platformInfoProvider,
			GoogleAnalytics::IServiceManager^ analyticsManager);


#pragma region General 
		/// <summary>
		/// Gets or sets the tracking ID / web property ID. The format is UA-XXXX-Y. All collected data is associated by this ID.
		/// </summary>
		/// <remarks>Required for all hit types.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#tid"/>

		property Platform::String^ PropertyId
		{
			Platform::String^ get()
			{
				return propertyId;
			}
		}

		/// <summary>
		/// Gets or sets whether the IP address of the sender will be anonymized.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aiid"/>
		property bool AnonymizeIP;

#pragma endregion General 


#pragma region User 

		/// <summary>
		/// Gets or sets the value that anonymously identifies a particular user, device, or browser instance. For the web, this is generally stored as a first-party cookie with a two-year expiration. For mobile apps, this is randomly generated for each particular instance of an application install. The value of this field should be a random UUID (version 4) as described in <see href="http://www.ietf.org/rfc/rfc4122.txt"/>.
		/// </summary>
		/// <remarks>Required for all hit types.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cid"/>
		property Platform::String^ ClientId;

#pragma endregion 


#pragma region Session 

		/// <summary>
		/// Gets or sets the IP address of the user. This should be a valid IP address in IPv4 or IPv6 format. It will always be anonymized just as though anonymize IP had been used.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#uip"/>
		property Platform::String^ IpOverride;

		/// <summary>
		/// Gets or sets the User Agent of the browser. Note that Google has libraries to identify real user agents. Hand crafting your own agent could break at any time.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ua"/>
		property Platform::String^ UserAgentOverride;

		/// <summary>
		/// Gets or sets the geographical location of the user. The geographical ID should be a two letter country code or a criteria ID representing a city or region (see http://developers.google.com/analytics/devguides/collection/protocol/v1/geoid). This parameter takes precedent over any location derived from IP address, including the IP Override parameter. An invalid code will result in geographical dimensions to be set to '(not set)'.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#geoid"/>
		property Platform::String^ LocationOverride;

#pragma endregion Session 


#pragma region Traffic Sources 

		/// <summary>
		/// Specifies which referral source brought traffic to a website. This value is also used to compute the traffic source. The format of this value is a URL.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dr"/>
		property Platform::String^ Referrer;

#pragma endregion 

#pragma region SystemInfo 

		/// <summary>
		/// Gets or sets the screen resolution.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sr"/>
		property Platform::IBox<Dimensions>^ ScreenResolution;

		/// <summary>
		/// Gets or sets the viewable area of the browser / device.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#vp"/>
		property Platform::IBox<Dimensions>^ ViewportSize;

		/// <summary>
		/// Gets or sets the character set used to encode the page / document.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#de"/> 
		property Platform::String^ Encoding;

		/// <summary>
		/// Gets or sets the screen color depth.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sd"/>
		property Platform::IBox<int>^ ScreenColors;

		/// <summary>
		/// Gets or sets the language.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ul"/>
		property Platform::String^ Language;

#pragma endregion System Info 


#pragma region Content Info 

		/// <summary>
		/// Gets or sets the hostname from which content was hosted.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dh"/>
		property Platform::String^ HostName;

		/// <summary>
		/// Gets or sets the path portion of the page URL.
		/// </summary>
		/// <remarks>Optional. Should begin with '/'.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dp"/>
		property Platform::String^ Page;

		/// <summary>
		/// Gets or sets the title of the page / document.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dt"/>
		property Platform::String^ Title;

		/// <summary>
		/// Gets or sets the 'Screen Name' of the screenview hit. On web properties this will default to the unique URL of the page.
		/// </summary>
		/// <remarks>Required for screenview hit type. Note: This parameter is optional on web properties, and required on mobile properties for screenview hits.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cd"/>
		property Platform::String^ ScreenName;

#pragma endregion Content Info 

#pragma region App Tracking 

		/// <summary>
		/// Gets or sets the application name. This field is required for any hit that has app related data (i.e., app version, app ID, or app installer ID). For hits sent to web properties, this field is optional.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#an"/>
		property Platform::String^ AppName;

		/// <summary>
		/// Gets or sets the application identifier.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aid"/>
		property Platform::String^ AppId;

		/// <summary>
		/// Gets or sets the application version.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#av"/>
		property Platform::String^ AppVersion;

		/// <summary>
		/// Gets or sets the application installer identifier.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aiid"/>
		property Platform::String^ AppInstallerId;

#pragma endregion App Tracking 


#pragma region Content Experiments 

		/// <summary>
		/// Gets or sets the parameter that specifies that this user has been exposed to an experiment with the given ID. It should be sent in conjunction with the Experiment Variant parameter.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#xid"/>
		property Platform::String^ ExperimentId;

		/// <summary>
		/// Gets or sets the parameter that specifies that this user has been exposed to a particular variation of an experiment. It should be sent in conjunction with the Experiment ID parameter.
		/// </summary>
		/// <remarks>Optional.</remarks>
		/// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#xvar"/>
		property Platform::String^ ExperimentVariant;

#pragma endregion Content experiments 


		/// <summary>
		/// Gets or sets the rate at which <see cref="Hit"/>s should be excluded for sampling purposes. Default is 100.
		/// </summary>
		/// <remarks>100 means no items should be excluded, 50 means half should be excluded, and 0 means all items should be excluded.</remarks>
		property float SampleRate;

		/// <summary>
		/// Gets the model value for the given key added through <see cref="Set"/>.
		/// </summary>
		/// <param name="key">The key to retrieve the value for.</param>
		/// <returns>The value associated with the key.</returns>
		Platform::String^ Get(Platform::String^ key);

		/// <summary>
		/// Sets the model value for the given key.
		/// </summary>
		/// <param name="key">The key of the field that needs to be set. It starts with "&amp;" followed by the parameter name. The complete list of fields can be found at <see href="https://goo.gl/M6dK2U"/>.</param>
		/// <param name="value">A string value to be sent to Google servers. A null value denotes that the value should not be sent over wire.</param>
		void Set(Platform::String^ key, Platform::String^ value);

		/// <summary>
		/// Merges the model values set on this Tracker with params and generates a hit to be sent.
		/// </summary>
		/// <param name="params">Dictionary of hit data to values which are merged with the existing values which are already set (using Set(String, String)). Values in this dictionary will override the values set earlier. The values in this dictionary will not be reused for the subsequent hits. If you need to send a value in multiple hits, you can use the Set(String, String) method.</param>
		/// <remarks>The hit may not be dispatched immediately.</remarks>
		void Send(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ params);
	};
}
