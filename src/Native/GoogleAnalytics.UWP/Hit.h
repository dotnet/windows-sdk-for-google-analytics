//
// Payload.h
// Declaration of the Payload class.
//

#pragma once

#include "DateTimeHelper.h"

namespace GoogleAnalytics
{

	/// <summary>
	/// Represents a single event to track.
	/// </summary>
	public ref class Hit sealed
	{
	private:
		Windows::Foundation::DateTime timeStamp;

		Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ data;

	internal:

		Hit(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ data)
			: data(data)
			, timeStamp(DateTimeHelper::Now())
		{ }


	public: 
		/// <summary>
		/// Gets the key value pairs to send to Google Analytics.
		/// </summary>
		property Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ Data
		{
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ get()
			{
				return data;
			}
		}

		/// <summary>
		/// Gets the timestamp that the event was created.
		/// </summary>
		property Windows::Foundation::DateTime TimeStamp
		{
			Windows::Foundation::DateTime get()
			{
				return timeStamp;
			}
		}

	};
}
