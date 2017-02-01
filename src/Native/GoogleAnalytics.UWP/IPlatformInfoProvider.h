#pragma once

#include "Dimensions.h"


namespace GoogleAnalytics
{
	/// <summary>
	/// Interface to offer a way to provide all environment and platform level information required by Google Analytics.
	/// </summary>
	public interface class IPlatformInfoProvider 
	{	
		/// <summary>
		/// Raised to indicate that the <see cref="ViewPortResolution"/> has changed.
		/// </summary>
		event Windows::Foundation::EventHandler<Platform::Object^>^ ViewPortResolutionChanged;
		
		/// <summary>
		/// Raised to indicate that the <see cref="ScreenResolution"/> has changed.
		/// </summary>
		event Windows::Foundation::EventHandler<Platform::Object^>^ ScreenResolutionChanged;

		/// <summary>
		/// Callback that indicates something is about to be logged.
		/// </summary>
		/// <remarks>This allows lazy loading of values that might not be available immediately.</remarks>
		void OnTracking();

		/// <summary>
		/// Gets the value that anonymously identifies a particular user, device, or browser instance. For the web, this is generally stored as a first-party cookie with a two-year expiration. For mobile apps, this is randomly generated for each particular instance of an application install. The value of this field should be a random UUID (version 4) as described in <see href="http://www.ietf.org/rfc/rfc4122.txt"/>.
		/// </summary>
		property Platform::String^ AnonymousClientId 
		{
			Platform::String^ get();
		}

		/// <summary>
		/// Gets the Viewport Resolution.
		/// </summary>
		property Platform::IBox<GoogleAnalytics::Dimensions>^ ViewPortResolution
		{
			Platform::IBox<GoogleAnalytics::Dimensions>^ get();
		}

		/// <summary>
		/// Gets the screen resolution.
		/// </summary>
		property Platform::IBox<GoogleAnalytics::Dimensions>^ ScreenResolution
		{
			Platform::IBox<GoogleAnalytics::Dimensions>^ get();
		}

		/// <summary>
		/// Gets the language (e.g. 'en-us').
		/// </summary>
		property Platform::String^ UserLanguage
		{
			Platform::String^ get();
		}

		/// <summary>
		/// Gets the screen color depth.
		/// </summary>
		property Platform::IBox<int>^ ScreenColors 
		{
			Platform::IBox<int>^ get();
		}

		property Platform::String^ UserAgent
		{
			Platform::String^ get();
		}
		 
	}; 
} 