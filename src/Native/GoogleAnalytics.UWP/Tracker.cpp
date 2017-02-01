//
// Tracker.cpp
// Implementation of the Tracker class.
//

#include "pch.h"
#include "Tracker.h"
#include "TokenBucket.h"

using namespace GoogleAnalytics;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation;

Tracker::Tracker(String^ propertyId, IPlatformInfoProvider^ platformInfoProvider, IServiceManager^ analyticsManager)
{
	this->propertyId = propertyId;
	this->platformInfoProvider = platformInfoProvider;
	this->analyticsManager = analyticsManager;
	SampleRate = 100.0F;
	data = ref new Map<String^, String^>();

	if (platformInfoProvider != nullptr)
	{
		ClientId = platformInfoProvider->AnonymousClientId;
		ScreenColors = platformInfoProvider->ScreenColors;
		ScreenResolution = platformInfoProvider->ScreenResolution;
		Language = platformInfoProvider->UserLanguage;
		ViewportSize = platformInfoProvider->ViewPortResolution;
		viewPortResolutionChangedEventToken = platformInfoProvider->ViewPortResolutionChanged += ref new EventHandler<Object^>(this, &Tracker::platformTrackingInfo_ViewPortResolutionChanged);
		screenResolutionChangedEventToken = platformInfoProvider->ScreenResolutionChanged += ref new EventHandler<Object^>(this, &Tracker::platformTrackingInfo_ScreenResolutionChanged);
	} 
}

void Tracker::platformTrackingInfo_ViewPortResolutionChanged(Object^ sender, Object^ args)
{
	ViewportSize = platformInfoProvider->ViewPortResolution;
}

void Tracker::platformTrackingInfo_ScreenResolutionChanged(Object^ sender, Object^ args)
{
	ScreenResolution = platformInfoProvider->ScreenResolution;
}

void Tracker::Send(IMap<String^, String^>^ params)
{
	if (propertyId)
	{
		if (!IsSampledOut())
		{
			analyticsManager->EnqueueHit(AddRequiredHitData(params));
		}
	}
}

IMap<String^, String^>^ Tracker::AddRequiredHitData(IMap<String^, String^>^ params)
{
	auto result = ref new Map<String^, String^>();
	result->Insert("v", "1");
	result->Insert("tid", PropertyId);
	result->Insert("cid", ClientId);
	result->Insert("an", AppName);
	result->Insert("av", AppVersion);

	if (AppId) result->Insert("aid", AppId);
	if (AppInstallerId) result->Insert("aiid", AppInstallerId);
	if (ScreenName) result->Insert("cd", ScreenName);
	if (AnonymizeIP) result->Insert("aip", "1");
	if (ScreenResolution) result->Insert("sr", ScreenResolution->Value.Width.ToString() + "x" + ScreenResolution->Value.Height.ToString());
	if (ViewportSize) result->Insert("vp", ViewportSize->Value.Width.ToString() + "x" + ViewportSize->Value.Height.ToString());
	if (Language) result->Insert("ul", Language);
	if (ScreenColors) result->Insert("sd", ScreenColors->Value.ToString() + "-bits");

	if (Referrer) result->Insert("dr", Referrer);
	if (Encoding) result->Insert("de", Encoding);
	if (IpOverride) result->Insert("uip", IpOverride);
	if (UserAgentOverride) result->Insert("ua", UserAgentOverride);
	if (HostName) result->Insert("dh", HostName);
	if (Page) result->Insert("dp", Page);
	if (Title) result->Insert("dt", Title);
	if (ExperimentId) result->Insert("xid", ExperimentId);
	if (ExperimentVariant) result->Insert("xvar", ExperimentVariant);
	if (LocationOverride) result->Insert("geoid", LocationOverride);

	for each (auto item in data)
	{
		result->Insert(item->Key, item->Value);
	}
	for each (auto item in params)
	{
		result->Insert(item->Key, item->Value);
	}

	return result;
}

String^ Tracker::Get(String^ key)
{
	return data->Lookup(key);
}

void Tracker::Set(String^ key, String^ value)
{
	if (data->HasKey(key))
	{
		data->Remove(key);
	}
	data->Insert(key, value);
}

bool Tracker::IsSampledOut()
{
	if (SampleRate <= 0.0F)
	{
		return true;
	}
	else if (SampleRate < 100.0F)
	{
		return ((ClientId != nullptr) && (std::abs(ClientId->GetHashCode()) % 10000 >= SampleRate * 100.0F));
	}
	else return false;
}


