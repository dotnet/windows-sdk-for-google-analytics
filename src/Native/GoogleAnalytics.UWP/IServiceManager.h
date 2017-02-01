#pragma once

using namespace Platform; 
namespace GoogleAnalytics
{
	/// <summary>
	/// Interface for a service manager used to send hits to Google Analytics.
	/// </summary>
	public interface class IServiceManager
	{ 
		void EnqueueHit(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ params);		 
	}; 

} 