//
// AnalyticsManager.h
// Declaration of the AnalyticsManager class.
//

#pragma once

#include <ppltasks.h>
#include <queue>
#include <collection.h>
#include <unordered_map>
#include <mutex>
#include "Hit.h"
#include "TokenBucket.h"
#include "Tracker.h"
#include "IPlatformInfoProvider.h"
#include "IServiceManager.h"

namespace GoogleAnalytics
{
	ref class Tracker;

	/// <summary>
	/// Supplies additional information when <see cref="Hit"/>s fail to send.
	/// </summary>
	public ref class HitFailedEventArgs sealed  
	{
	private:

		Platform::String^ error;
		GoogleAnalytics::Hit^ hit;

	internal:

		HitFailedEventArgs(GoogleAnalytics::Hit^ hit, Platform::String^ error)
			: error(error)
			, hit(hit)
		{ }

	public:

		/// <summary>
		/// Gets the <see cref="Exception"/> thrown when the failure occurred.
		/// </summary>
		property Platform::String^ Error
		{
			Platform::String^ get()
			{
				return error;
			}
		}

		/// <summary>
		/// Gets the <see cref="Hit"/> associated with the event.
		/// </summary>
		property GoogleAnalytics::Hit^ Hit
		{
			GoogleAnalytics::Hit^ get()
			{
				return hit;
			}
		}
	};

	/// <summary>
	/// Supplies additional information when <see cref="Hit"/>s are successfully sent.
	/// </summary>
	public ref class HitSentEventArgs sealed
	{
	private:

		Platform::String^ response;
		GoogleAnalytics::Hit^ hit;

	internal:

		HitSentEventArgs(GoogleAnalytics::Hit^ hit, Platform::String^ response)
			: response(response)
			, hit(hit)
		{ }

	public:
		/// <summary>
		/// Gets the response text.
		/// </summary>
		property Platform::String^ Response
		{
			Platform::String^ get()
			{
				return response;
			}
		}
		/// <summary>
		/// Gets the <see cref="Hit"/> associated with the event.
		/// </summary>
		property GoogleAnalytics::Hit^ Hit
		{
			GoogleAnalytics::Hit^ get()
			{
				return hit;
			}
		}
	};

	/// <summary>
	/// Supplies additional information when <see cref="Hit"/>s are malformed and cannot be sent.
	/// </summary>
	public ref class HitMalformedEventArgs sealed
	{
	private:

		int httpStatusCode;
		GoogleAnalytics::Hit^ hit;

	internal:

		HitMalformedEventArgs(GoogleAnalytics::Hit^ hit, int httpStatusCode)
			: httpStatusCode(httpStatusCode)
			, hit(hit)
		{ }

	public:
		/// <summary>
		/// Gets the HTTP status code that may provide more information about the problem.
		/// </summary>
		property int HttpStatusCode
		{
			int get()
			{
				return httpStatusCode;
			}
		}

		/// <summary>
		/// Gets the <see cref="Hit"/> associated with the event.
		/// </summary>
		property GoogleAnalytics::Hit^ Hit
		{
			GoogleAnalytics::Hit^ get()
			{
				return hit;
			}
		}
	};

	/// <summary>
	/// Provides shared infrastrcuture for <see cref="Tracker" /> in a Windows 10 Universal Windows app 
	/// </summary>
	public ref class AnalyticsManager sealed : IServiceManager 
	{
	private:

		static GoogleAnalytics::AnalyticsManager^ current;

		bool reportUncaughtExceptions;

		Windows::Foundation::EventRegistrationToken unhandledErrorDetectedEventToken;
		Windows::Foundation::EventRegistrationToken coreAppSuspendingEventToken; 
		Windows::Foundation::EventRegistrationToken coreAppResumingEventToken;

		void CoreApplication_UnhandledErrorDetected(Platform::Object^ sender, Windows::ApplicationModel::Core::UnhandledErrorDetectedEventArgs^ e);
		void AnalyticsManager::CoreApplication_Suspending(Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void AnalyticsManager::CoreApplication_Resuming(Object^ sender, Object^ unused); 
 
		bool autoTrackNetworkConnectivity;

		Windows::Foundation::EventRegistrationToken networkStatusChangedEventToken;

		void NetworkInformation_NetworkStatusChanged(Platform::Object^ sender);

		void UpdateConnectionStatus();

		void LoadAppOptOut();

		bool isAppOptOutSet;

		bool appOptOut;

		static Platform::String^ Key_AppOptOut;

		std::mutex dispatcherLock;

		std::mutex hitLock;

		static Windows::Foundation::Uri^ endPointUnsecureDebug;

		static Windows::Foundation::Uri^ endPointSecureDebug;

		static Windows::Foundation::Uri^ endPointUnsecure;

		static Windows::Foundation::Uri^ endPointSecure;

		std::queue<GoogleAnalytics::Hit^> hits;

		std::vector<concurrency::task<void>> dispatchingTasks;

		GoogleAnalytics::TokenBucket^ hitTokenBucket;

		Windows::System::Threading::ThreadPoolTimer^ timer;

		Windows::Foundation::TimeSpan dispatchPeriod;

		bool isEnabled; 

		bool autoAppLifetimeMonitoring; 

		void timer_Tick(Windows::System::Threading::ThreadPoolTimer^ sender);

		concurrency::task<void> _DispatchAsync();

		concurrency::task<void> _SuspendAsync();

		concurrency::task<void> RunDispatchingTask(concurrency::task<void> newDispatchingTask);

		concurrency::task<void> DispatchQueuedHits(std::vector<Hit^> hits);

		concurrency::task<void> DispatchImmediateHit(GoogleAnalytics::Hit^ hit);

		concurrency::task<void> DispatchHitData(GoogleAnalytics::Hit^ hit, Windows::Web::Http::HttpClient^ httpClient, std::unordered_map<Platform::String^, Platform::String^> hitData);

		concurrency::task<Windows::Web::Http::HttpResponseMessage^> SendHitAsync(GoogleAnalytics::Hit^ hit, Windows::Web::Http::HttpClient^ httpClient, std::unordered_map<Platform::String^, Platform::String^> hitData);

		void OnHitFailed(GoogleAnalytics::Hit^ hit, Platform::Exception^ exception);

		void OnHitSent(GoogleAnalytics::Hit^ hit, Windows::Web::Http::HttpResponseMessage^ response);

		void OnHitMalformed(GoogleAnalytics::Hit^ hit, Windows::Web::Http::HttpResponseMessage^ response);

		Windows::Web::Http::HttpClient^ GetHttpClient();

		static Platform::String^ GetCacheBuster();

		GoogleAnalytics::IPlatformInfoProvider^ platformTrackingInfo;

		std::unordered_map<Platform::String^, GoogleAnalytics::Tracker^> trackers;

		Windows::UI::Core::CoreDispatcher^ dispatcher; 
		bool fireEventsOnUIThread; 
		
		int   hitSentListenerCount, hitFailedListenerCount, hitMalformedListenerCount; 

		event Windows::Foundation::EventHandler<GoogleAnalytics::HitSentEventArgs^>^ internalHitSentEventHandler; 
		event Windows::Foundation::EventHandler<GoogleAnalytics::HitFailedEventArgs^>^ internalHitFailedEventHandler;
		event Windows::Foundation::EventHandler<GoogleAnalytics::HitMalformedEventArgs^>^ internalHitMalformedEventHandler;

	public:
		
		/// <summary>
		/// Instantiates a new instance of <see cref="AnalyticsManager"/> 
		/// </summary>
		/// <param name="platformInfoProvider"> The platform info provider to be used by this Analytics Manager. Can not be null.</param>
		AnalyticsManager(GoogleAnalytics::IPlatformInfoProvider^ platformInfoProvider);


		/// <summary>
		/// Shared, singleton instance of AnalyticsManager 
		/// </summary>
		static property GoogleAnalytics::AnalyticsManager^ Current
		{
			GoogleAnalytics::AnalyticsManager^ get();
		}

	
		/// <summary>
		/// Enables (when set to true) automatic catching and tracking of Unhandled Exceptions.
		/// </summary>
		property bool ReportUncaughtExceptions
		{
			bool get();
			void set(bool value);
		}

		/// <summary>
		/// Enables (when set to true) listening to network connectivity events to have trackers behave accordingly to their connectivity status.  
		/// </summary>
		property bool AutoTrackNetworkConnectivity
		{
			bool get();
			void set(bool value);
		}

		property GoogleAnalytics::Tracker^ DefaultTracker;

		/// <summary>
		/// True when the user has opted out of analytics, this disables all tracking activities. 
		/// </summary>				
		/// <remarks>See Google Analytics usage guidelines for more information.</remarks>
		property bool AppOptOut
		{
			bool get();
			void set(bool value);
		}

		/// <summary>
		/// Gets the instance of <see cref="IPlatformInfoProvider"/> used by the <see cref="Tracker"/> instantiated by this manager.
		/// </summary>
		property GoogleAnalytics::IPlatformInfoProvider^ PlatformInfoProvider
		{
			GoogleAnalytics::IPlatformInfoProvider^ get()
			{
				return platformTrackingInfo;
			}
		}

		/// <summary>
		/// Creates a new <see cref="Tracker"/> using a given property ID. 
		/// </summary>
		/// <param name="propertyId">The property ID that the <see cref="Tracker"/> should log to.</param>
		/// <returns>The new or existing instance keyed on the property ID.</returns>
		GoogleAnalytics::Tracker^ CreateTracker(Platform::String^ propertyId);

		/// <summary>
		/// Removes and cleans up a given <see cref="Tracker"/>.
		/// </summary>
		/// <param name="tracker">The instance to remove and clean up.</param>
		void CloseTracker(GoogleAnalytics::Tracker^ tracker);

		/// <summary>
		/// Provides notification that a <see cref="Hit"/> failed to send.
		/// </summary>
		/// <remarks>Failed <see cref="Hit"/>s will be added to the queue in order to reattempt at the next dispatch time.</remarks>		
		event Windows::Foundation::EventHandler<HitFailedEventArgs^>^ HitFailed {
			Windows::Foundation::EventRegistrationToken add(Windows::Foundation::EventHandler<HitFailedEventArgs^>^ handler);
			void remove(Windows::Foundation::EventRegistrationToken token); 
			void raise(Platform::Object^ sender, HitFailedEventArgs^ args);
		}

		/// <summary>
		/// Provides notification that a <see cref="Hit"/> has been been successfully sent.
		/// </summary>
		event Windows::Foundation::EventHandler<HitSentEventArgs^>^ HitSent
		{
			Windows::Foundation::EventRegistrationToken add(Windows::Foundation::EventHandler<HitSentEventArgs^>^ handler);  
			void remove(Windows::Foundation::EventRegistrationToken token); 			 
			void raise(Platform::Object^ sender, HitSentEventArgs^ args);
		}

		/// <summary>
		/// Provides notification that a <see cref="Hit"/> was malformed and rejected by Google Analytics.
		/// </summary>		
		event Windows::Foundation::EventHandler<HitMalformedEventArgs^>^ HitMalformed
		{
			Windows::Foundation::EventRegistrationToken add(Windows::Foundation::EventHandler<GoogleAnalytics::HitMalformedEventArgs^>^ handler);
			void remove(Windows::Foundation::EventRegistrationToken token);
			void raise(Platform::Object^ sender, HitMalformedEventArgs^ args);
		}
		/// <summary>
		/// Gets or sets whether <see cref="Hit"/>s should be sent via SSL. Default is true.
		/// </summary>
		property bool IsSecure;

		/// <summary>
		/// Gets or sets whether <see cref="Hit"/>s should be sent to the debug endpoint. Default is false.
		/// </summary>
		property bool IsDebug;

		/// <summary>
		/// Gets or sets whether throttling should be used. Default is false.
		/// </summary>
		property bool ThrottlingEnabled;

		/// <summary>
		/// Gets or sets whether data should be sent via POST or GET method. Default is POST.
		/// </summary>
		property bool PostData;

		/// <summary>
		/// Gets or sets whether a cache buster should be applied to all requests. Default is false.
		/// </summary>
		property bool BustCache;

		/// <summary>
		/// Gets or sets the user agent request header used by Google Analytics to determine the platform and device generating the hits.
		/// </summary>
		property Platform::String^ UserAgent;

		/// <summary>
		/// Gets or sets the frequency at which hits should be sent to the service. Default is immediate.
		/// </summary>
		/// <remarks>Setting to TimeSpan.Zero will cause the hit to get sent immediately.</remarks>
		property Windows::Foundation::TimeSpan DispatchPeriod
		{
			Windows::Foundation::TimeSpan get();
			void set(Windows::Foundation::TimeSpan value);
		}

		/// <summary>
		/// Gets or sets whether the dispatcher is enabled. If disabled, hits will be queued but not dispatched.
		/// </summary>
		/// <remarks>Typically this is used to indicate whether or not the network is available.</remarks>
		property bool IsEnabled
		{
			bool get();
			void set(bool value);
		}

		/// <summary>
		/// Enables (when set to true) automatic dispatching of queued <see cref="Hit"/> on app Suspend, and automatic restart of dispatch timer upon resume.         
		/// </summary>
		/// <remarks>
		/// Default value is false, since we default to immediate dispatching. See <see cref="DispatchPeriod" />
		/// </remarks>
		property bool AutoAppLifetimeMonitoring
		{
			bool get();
			void set(bool value);
		}


		/// <summary>		
		/// When set to true, <see cref="AnalyticsManager::HitSent" />, <see cref="AnalyticsManager::HitMalformed" />, and <see cref="AnalyticsManager::Failed"/> will fire back into UI thread.          
		/// </summary>
		/// <remarks>
		/// You must set this property to true to listen to these events from a Javascript app.
		/// </remarks>
		property bool FireEventsOnUIThread
		{
			bool get();
			void set(bool value);
		}


		/// <inheritdoc/>
		virtual void EnqueueHit(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ params);

		/// <summary>
		/// Empties the queue of <see cref="Hit"/>s waiting to be dispatched.
		/// </summary>
		/// <remarks>If a <see cref="Hit"/> is actively beeing sent, this will not abort the request.</remarks>
		void Clear();

		/// <summary>
		/// Dispatches all hits in the queue.
		/// </summary>
		/// <returns>Returns once all items that were in the queue at the time the method was called have finished being sent.</returns>
		Windows::Foundation::IAsyncAction^ DispatchAsync();

		/// <summary>
		/// Suspends operations and dispatches all pending hits in the queue.
		/// </summary>
		/// <remarks>Call <see cref="Resume"/> when returning from a suspended state to resume operations.</remarks>
		/// <returns>Operation returns when all <see cref="Hit"/>s have been flushed.</returns>
		Windows::Foundation::IAsyncAction^ SuspendAsync();


		/// <summary>
		/// Resumes operations after <see cref="SuspendAsync"/> is called.
		/// </summary>
		void Resume();
	};
}
