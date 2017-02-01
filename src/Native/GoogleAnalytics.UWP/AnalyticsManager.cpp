//
// AnalyticsManager.cpp
// Implementation of the AnalyticsManager class.
//

#include "pch.h"
#include "AnalyticsManager.h"
#include "TimeSpanHelper.h"
#include "DateTimeHelper.h"
#include "HitBuilder.h"

using namespace GoogleAnalytics;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System::Threading;
using namespace concurrency;
using namespace Windows::Web::Http;
using namespace Windows::Storage;
using namespace Windows::Networking::Connectivity;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;

String^ AnalyticsManager::Key_AppOptOut = "GoogleAnaltyics.AppOptOut";

AnalyticsManager^ AnalyticsManager::current = nullptr;

Uri^ AnalyticsManager::endPointUnsecureDebug = ref new Uri("http://www.google-analytics.com/debug/collect");

Uri^ AnalyticsManager::endPointSecureDebug = ref new Uri("https://ssl.google-analytics.com/debug/collect");

Uri^ AnalyticsManager::endPointUnsecure = ref new Uri("http://www.google-analytics.com/collect");

Uri^ AnalyticsManager::endPointSecure = ref new Uri("https://ssl.google-analytics.com/collect");

AnalyticsManager^ AnalyticsManager::Current::get()
{
	if (!current)
	{
		current = ref new AnalyticsManager(ref new GoogleAnalytics::PlatformInfoProvider());
	}
	return current;
}

AnalyticsManager::AnalyticsManager(GoogleAnalytics::IPlatformInfoProvider^ platformInfoProvider) :
	isEnabled(true),
	timer(nullptr),
	hitTokenBucket(ref new TokenBucket(60, .5)),
	reportUncaughtExceptions(false),
	autoTrackNetworkConnectivity(false), 
	autoAppLifetimeMonitoring(false) 
{
	this->platformTrackingInfo = platformInfoProvider;
	DefaultTracker = nullptr;
	AppOptOut = false;
	IsSecure = true;
	PostData = true;
	BustCache = false;
	dispatchPeriod = TimeSpanHelper::FromTicks(0);
}

Tracker^ AnalyticsManager::CreateTracker(String^ propertyId)
{
	if (trackers.find(propertyId) == end(trackers))
	{
		auto tracker = ref new Tracker(propertyId, platformTrackingInfo, this);
		tracker->AppName = Package::Current->Id->Name;
		tracker->AppVersion = Package::Current->Id->Version.Major.ToString() + "." + Package::Current->Id->Version.Minor.ToString() + "." + Package::Current->Id->Version.Build.ToString() + "." + Package::Current->Id->Version.Revision.ToString();
		
		trackers[propertyId] = tracker;
		if (!DefaultTracker)
		{
			DefaultTracker = tracker;
		}
		return tracker;
	}
	else
	{
		return trackers[propertyId];
	}
}

void AnalyticsManager::CloseTracker(Tracker^ tracker)
{
	trackers.erase(tracker->PropertyId);
	if (DefaultTracker == tracker)
	{
		DefaultTracker = nullptr;
	}
}

TimeSpan AnalyticsManager::DispatchPeriod::get()
{
	return dispatchPeriod;
}

void AnalyticsManager::DispatchPeriod::set(TimeSpan value)
{
	if (dispatchPeriod.Duration != value.Duration)
	{
		dispatchPeriod = value;	
		if (timer)
		{
			timer->Cancel();
			timer = nullptr;
		}
		if (dispatchPeriod.Duration > 0)
		{
			timer = ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler(this, &AnalyticsManager::timer_Tick), dispatchPeriod);
		}
	}
}

bool AnalyticsManager::IsEnabled::get()
{
	return isEnabled;
}
void AnalyticsManager::IsEnabled::set(bool value)
{
	if (isEnabled != value)
	{
		isEnabled = value;
		if (isEnabled)
		{
			if (dispatchPeriod.Duration >= 0)
			{
				DispatchAsync();
			}
		}
	}
}


bool AnalyticsManager::AutoAppLifetimeMonitoring::get()
{
	return autoAppLifetimeMonitoring;
}

void AnalyticsManager::AutoAppLifetimeMonitoring::set(bool value)
{
	if (autoAppLifetimeMonitoring != value)
	{
		autoAppLifetimeMonitoring = value;
		if (autoAppLifetimeMonitoring)
		{
			coreAppSuspendingEventToken = CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &AnalyticsManager::CoreApplication_Suspending);
			coreAppResumingEventToken = CoreApplication::Resuming += ref new EventHandler<Object^>(this, &AnalyticsManager::CoreApplication_Resuming);
		}
		else
		{
			CoreApplication::Suspending -= coreAppSuspendingEventToken; 
			CoreApplication::Resuming -= coreAppResumingEventToken; 	
			coreAppResumingEventToken.Value = 0; 
			coreAppSuspendingEventToken.Value = 0; 
		}
	}
}

void AnalyticsManager::Clear()
{
	std::lock_guard<std::mutex> lg(hitLock);
	while (!hits.empty()) hits.pop();
}

IAsyncAction^ AnalyticsManager::DispatchAsync()
{
	return create_async([this]() { return _DispatchAsync(); });
}

task<void> AnalyticsManager::_DispatchAsync()
{
	if (!isEnabled) return task<void>([]() {});

	task<void> allDispatchingTasks;
	{
		std::lock_guard<std::mutex> lg(dispatcherLock);
		allDispatchingTasks = when_all(begin(dispatchingTasks), end(dispatchingTasks));
	}

	return allDispatchingTasks.then([this]() {
		if (!isEnabled) return task<void>([]() {});

		std::vector<Hit^> hitsToSend;
		{
			std::lock_guard<std::mutex> lg(hitLock);
			while (!hits.empty())
			{
				hitsToSend.push_back(hits.front());
				hits.pop();
			}
		}
		if (!hitsToSend.empty())
		{
			return RunDispatchingTask(DispatchQueuedHits(hitsToSend));
		}
		else
		{
			return task<void>([]() {});
		}
	});
}

void AnalyticsManager::EnqueueHit(IMap<String^, String^>^ params)
{
	if (!AppOptOut)
	{
		auto hit = ref new Hit(params);
		if (DispatchPeriod.Duration == 0 && IsEnabled)
		{
			RunDispatchingTask(DispatchImmediateHit(hit));
		}
		else
		{
			std::lock_guard<std::mutex> lg(hitLock);
			hits.push(hit);
		}
	}
}

IAsyncAction^ AnalyticsManager::SuspendAsync()
{
	return create_async([this]() { return _SuspendAsync(); });
}

task<void> AnalyticsManager::_SuspendAsync()
{
	
	return _DispatchAsync().then([this] {	 
		if (timer)
		{	
			timer->Cancel();
			timer = nullptr;
		}
	});
}

void AnalyticsManager::Resume()
{  
	if (dispatchPeriod.Duration > 0)
	{
		//This would only happen if Suspend did not complete before due to a long Dispatch; very unlikely (I hope)
		if (timer != nullptr )
		{			 
			timer->Cancel();
			timer = nullptr;
		}		 
		timer = ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler(this, &AnalyticsManager::timer_Tick), dispatchPeriod);
	}
}

void AnalyticsManager::timer_Tick(ThreadPoolTimer^ sender)
{
	DispatchAsync();
}

task<void> AnalyticsManager::RunDispatchingTask(task<void> newDispatchingTask)
{
	{
		std::lock_guard<std::mutex> lg(dispatcherLock);
		dispatchingTasks.push_back(newDispatchingTask);
	}
	return newDispatchingTask.then([this, newDispatchingTask](task<void> t) {
		std::lock_guard<std::mutex> lg(dispatcherLock);
		dispatchingTasks.erase(begin(dispatchingTasks), std::find(begin(dispatchingTasks), end(dispatchingTasks), newDispatchingTask));
		t.get();
	});
}

task<void> AnalyticsManager::DispatchQueuedHits(std::vector<Hit^> hits)
{
	HttpClient^ httpClient = GetHttpClient();
	auto now = DateTimeHelper::Now();
	std::vector<task<void>> tasks;
	for (auto it = begin(hits); it != end(hits); ++it)
	{
		Hit^ hit = *it;

		if (isEnabled && (!ThrottlingEnabled || hitTokenBucket->Consume()))
		{
			// clone the data
			std::unordered_map<String^, String^> payloadData;
			for each (auto kvp in hit->Data)
			{
				payloadData[kvp->Key] = kvp->Value;
			}
			int milliSeconds = (int)(TimeSpanHelper::GetTotalMilliseconds(TimeSpanHelper::FromTicks(now.UniversalTime - hit->TimeStamp.UniversalTime))); 
			payloadData["qt"] = milliSeconds.ToString(); 

			tasks.push_back(DispatchHitData(hit, httpClient, payloadData));
		}
		else
		{
			std::lock_guard<std::mutex> lg(hitLock);
			this->hits.push(hit);
		}
	}
	return when_all(begin(tasks), end(tasks));
}

task<void> AnalyticsManager::DispatchImmediateHit(Hit^ payload)
{
	HttpClient^ httpClient = GetHttpClient();
	// clone the data
	std::unordered_map<String^, String^> hitData;
	for each (auto kvp in payload->Data)
	{
		hitData[kvp->Key] = kvp->Value;
	}
	return DispatchHitData(payload, httpClient, hitData);
}

task<void> AnalyticsManager::DispatchHitData(Hit^ hit, HttpClient^ httpClient, std::unordered_map<String^, String^> hitData)
{
	if (BustCache) hitData["z"] = GetCacheBuster();

	return SendHitAsync(hit, httpClient, hitData).then([this, hit](task<HttpResponseMessage^> t) {
		try
		{
			HttpResponseMessage^ response(t.get());
			try
			{
				response->EnsureSuccessStatusCode();
				OnHitSent(hit, response);
			}
			catch (Exception^ ex)
			{
				OnHitMalformed(hit, response);
			}
		}
		catch (Exception^ ex)
		{
			OnHitFailed(hit, ex);
		}
	}, task_continuation_context::use_current());
}

task<HttpResponseMessage^> AnalyticsManager::SendHitAsync(Hit^ payload, HttpClient^ httpClient, std::unordered_map<String^, String^> payloadData)
{
	auto endPoint = IsDebug ? (IsSecure ? endPointSecureDebug : endPointUnsecureDebug) : (IsSecure ? endPointSecure : endPointUnsecure);

	std::wstring content;
	auto it = begin(payloadData);
	while (true)
	{
		content += std::wstring(it->first->Data()) + L"=" + std::wstring(Uri::EscapeComponent(it->second)->Data());
		it++;
		if (it == end(payloadData)) break;
		content += '&';
	}

#ifdef _DEBUG 
// 	::OutputDebugString(content.c_str());
#endif 

	if (PostData)
	{
		auto httpContent = ref new Windows::Web::Http::HttpStringContent(ref new String(content.c_str()));
		return create_task([httpClient, endPoint, httpContent]() { return httpClient->PostAsync(endPoint, httpContent); });
	}
	else
	{
		auto url = endPoint->RawUri + L"?" + ref new String(content.c_str());
		return create_task([httpClient, url]() { return httpClient->GetAsync(ref new Uri(url)); });
	}
}

void AnalyticsManager::OnHitFailed(Hit^ payload, Exception^ exception)
{
	HitFailed(this, ref new HitFailedEventArgs(payload, exception->Message));
}

void AnalyticsManager::OnHitSent(Hit^ payload, HttpResponseMessage^ response)
{
	create_task([response]() { return response->Content->ReadAsStringAsync(); }).then([this, payload](task<Platform::String^> t) {
		HitSent(this, ref new HitSentEventArgs(payload, t.get()));
	});
}

void AnalyticsManager::OnHitMalformed(GoogleAnalytics::Hit^ payload, HttpResponseMessage^ response)
{
	HitMalformed(this, ref new HitMalformedEventArgs(payload, (int)(response->StatusCode)));
}

HttpClient^ AnalyticsManager::GetHttpClient()
{
	auto result = ref new HttpClient();
	if (UserAgent)
	{
		result->DefaultRequestHeaders->UserAgent->ParseAdd(UserAgent);
	}
	return result;
}

String^ AnalyticsManager::GetCacheBuster()
{
	return (rand() % 100000000).ToString();
}


bool AnalyticsManager::AppOptOut::get()
{
	if (!isAppOptOutSet) LoadAppOptOut();
	return appOptOut;
}

void AnalyticsManager::AppOptOut::set(bool value)
{
	appOptOut = value;
	isAppOptOutSet = true;
	ApplicationData::Current->LocalSettings->Values->Insert(Key_AppOptOut, value);
	if (value) Clear();
}

void AnalyticsManager::LoadAppOptOut()
{
	if (ApplicationData::Current->LocalSettings->Values->HasKey(Key_AppOptOut))
	{
		appOptOut = (bool)ApplicationData::Current->LocalSettings->Values->Lookup(Key_AppOptOut);
	}
	else
	{
		appOptOut = false;
	}
	isAppOptOutSet = true;
}

bool AnalyticsManager::ReportUncaughtExceptions::get()
{
	return reportUncaughtExceptions;
}

void AnalyticsManager::ReportUncaughtExceptions::set(bool value)
{
	if (reportUncaughtExceptions != value)
	{
		reportUncaughtExceptions = value;
		if (reportUncaughtExceptions)
		{
			unhandledErrorDetectedEventToken = CoreApplication::UnhandledErrorDetected += ref new EventHandler<UnhandledErrorDetectedEventArgs^>(this, &AnalyticsManager::CoreApplication_UnhandledErrorDetected);
		}
		else
		{
			CoreApplication::UnhandledErrorDetected -= unhandledErrorDetectedEventToken;
		}
	}
}

void AnalyticsManager::CoreApplication_Suspending (Object^ sender, SuspendingEventArgs^ e)
{
	auto deferral = e->SuspendingOperation->GetDeferral(); 
	_SuspendAsync().then( [deferral] ()
	{ 
		deferral->Complete(); 
	}); 
}

void AnalyticsManager::CoreApplication_Resuming(Object^ sender, Object^ unused)
{
	 Resume(); 	 
}

void AnalyticsManager::CoreApplication_UnhandledErrorDetected(Object^ sender, UnhandledErrorDetectedEventArgs^ e)
{
	try
	{
		e->UnhandledError->Propagate();
	}
	catch (Exception^ ex)
	{
		auto it = begin(trackers);
		while (true)
		{
			it->second->Send(HitBuilder::CreateException(ex->Message, true)->Build());
			it++;
			if (it == end(trackers)) break;
		}
		DispatchAsync();
		throw;
	}
}

bool AnalyticsManager::AutoTrackNetworkConnectivity::get()
{
	return autoTrackNetworkConnectivity;
}

void AnalyticsManager::AutoTrackNetworkConnectivity::set(bool value)
{
	if (autoTrackNetworkConnectivity != value)
	{
		autoTrackNetworkConnectivity = value;
		if (autoTrackNetworkConnectivity)
		{
			UpdateConnectionStatus();
			networkStatusChangedEventToken = NetworkInformation::NetworkStatusChanged += ref new NetworkStatusChangedEventHandler(this, &AnalyticsManager::NetworkInformation_NetworkStatusChanged);
		}
		else
		{
			NetworkInformation::NetworkStatusChanged -= networkStatusChangedEventToken;
			IsEnabled = true;
		}
	}
}

void AnalyticsManager::NetworkInformation_NetworkStatusChanged(Object^ sender)
{
	UpdateConnectionStatus();
}

void AnalyticsManager::UpdateConnectionStatus()
{
	auto profile = NetworkInformation::GetInternetConnectionProfile();
	if (profile)
	{
		switch (profile->GetNetworkConnectivityLevel())
		{
		case NetworkConnectivityLevel::InternetAccess:
		case NetworkConnectivityLevel::ConstrainedInternetAccess:
			IsEnabled = true;
			break;
		default:
			IsEnabled = false;
			break;
		}
	}
}