//
// TimeSpanHelper.h
// Declaration of the TimeSpanHelper class.
//

#pragma once

namespace GoogleAnalytics
{
	ref class TimeSpanHelper sealed
	{

	internal:
		static Windows::Foundation::TimeSpan Min(Windows::Foundation::TimeSpan timeSpan1, Windows::Foundation::TimeSpan timeSpan2);

		static Windows::Foundation::TimeSpan Max(Windows::Foundation::TimeSpan timeSpan1, Windows::Foundation::TimeSpan timeSpan2);

		static Windows::Foundation::TimeSpan FromTicks(long long ticks);

		static Windows::Foundation::TimeSpan FromHours(double seconds);

		static Windows::Foundation::TimeSpan FromMinutes(double seconds);

		static Windows::Foundation::TimeSpan FromSeconds(double seconds);

		static Windows::Foundation::TimeSpan FromMilliseconds(double milliseconds);

		static double GetTotalMinutes(Windows::Foundation::TimeSpan timeSpan);

		static double GetTotalHours(Windows::Foundation::TimeSpan timeSpan);

		static double GetTotalSeconds(Windows::Foundation::TimeSpan timeSpan);

		static double GetTotalMilliseconds(Windows::Foundation::TimeSpan timeSpan);

		static Windows::Foundation::TimeSpan Parse(Platform::String^ string);

		static Platform::String^ ConvertToString(Windows::Foundation::TimeSpan timeSpan);
	};
}