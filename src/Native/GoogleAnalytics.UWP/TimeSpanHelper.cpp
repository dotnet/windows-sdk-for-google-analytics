//
// TimeSpanHelper.cpp
// Implementation of the TimeSpanHelper class.
//

#include "pch.h"
#include <string>
#include "TimeSpanHelper.h"

using namespace GoogleAnalytics;
using namespace Platform;
using namespace Windows::Foundation;

TimeSpan TimeSpanHelper::Min(TimeSpan timeSpan1, TimeSpan timeSpan2)
{
	return timeSpan1.Duration < timeSpan2.Duration ? timeSpan1 : timeSpan2;
}

TimeSpan TimeSpanHelper::Max(TimeSpan timeSpan1, TimeSpan timeSpan2)
{
	return timeSpan1.Duration > timeSpan2.Duration ? timeSpan1 : timeSpan2;
}

TimeSpan TimeSpanHelper::Parse(String^ string)
{
	double decimal = 0.0;
	long seconds = 0;
	long minutes = 0;
	long hours = 0;

	std::wstring s(string->Data());
	// decimal
	size_t di = s.find_last_of('.');
	if (di != std::string::npos)
	{
		std::wstring ds(s.substr(di + 1));
		long parsedDecimal = std::wcstol(ds.data(), nullptr, 10);
		decimal = parsedDecimal / std::pow(10, ds.length());
	}
	// seconds
	size_t si = s.find_last_of(':');
	{
		size_t starti = si != std::string::npos ? si : -1;
		size_t endi = di != std::string::npos ? di : s.length();
		std::wstring ss(s.substr(starti + 1, endi - starti - 1));
		seconds = std::wcstol(ss.data(), nullptr, 10);
	}
	// minutes
	if (si != std::string::npos)
	{
		size_t mi = s.find_last_of(':', si - 1);
		size_t starti = mi != std::string::npos ? mi : -1;
		std::wstring ms(s.substr(starti + 1, si - starti - 1));
		minutes = std::wcstol(ms.data(), nullptr, 10);
		// hours
		if (mi != std::string::npos)
		{
			std::wstring hs(s.substr(0, mi));
			hours = std::wcstol(hs.data(), nullptr, 10);
		}
	}

	double totalSeconds = ((hours * 60 + minutes) * 60 + seconds) + decimal;
	return TimeSpanHelper::FromSeconds(totalSeconds);
}

String^ TimeSpanHelper::ConvertToString(TimeSpan timeSpan)
{
	long long runningDuration = timeSpan.Duration;

	int hours = (int)(runningDuration / 36000000000);
	runningDuration -= hours * 36000000000;

	int minutes = (int)(runningDuration / 600000000);
	runningDuration -= minutes * 600000000;

	int seconds = (int)(runningDuration / 10000000);
	runningDuration -= seconds * 10000000;

	int milliseconds = (int)(runningDuration / 10000);
	runningDuration -= milliseconds * 10000;

	String^ result = "";
	// hours
	if (hours != 0)
	{
		result += hours.ToString() + ":";
	}
	// minutes
	if (hours == 0)
	{
		result += minutes.ToString() + ":";
	}
	else
	{
		if (minutes < 10)
		{
			result += "0" + minutes.ToString();
		}
		result += minutes.ToString() + ":";
	}
	// seconds
	if (seconds < 10)
	{
		result += "0" + seconds.ToString();
	}
	result += seconds.ToString();
	// milliseconds
	if (milliseconds > 0)
	{
		result += "." + milliseconds.ToString();
	}

	return result;
}

TimeSpan TimeSpanHelper::FromTicks(long long ticks)
{
	TimeSpan ts;
	ts.Duration = ticks;
	return ts;
}

TimeSpan TimeSpanHelper::FromHours(double hours)
{
	TimeSpan ts;
	ts.Duration = (long long)(hours * 36000000000.0);
	return ts;
}

TimeSpan TimeSpanHelper::FromMinutes(double minutes)
{
	TimeSpan ts;
	ts.Duration = (long long)(minutes * 600000000.0);
	return ts;
}

TimeSpan TimeSpanHelper::FromSeconds(double seconds)
{
	TimeSpan ts;
	ts.Duration = (long long)(seconds * 10000000.0);
	return ts;
}

TimeSpan TimeSpanHelper::FromMilliseconds(double milliseconds)
{
	TimeSpan ts;
	ts.Duration = (long long)(milliseconds * 10000.0);
	return ts;
}

double TimeSpanHelper::GetTotalMinutes(TimeSpan timeSpan)
{
	return (double)timeSpan.Duration / 600000000.0;
}

double TimeSpanHelper::GetTotalHours(TimeSpan timeSpan)
{
	return (double)timeSpan.Duration / 36000000000.0;
}

double TimeSpanHelper::GetTotalSeconds(TimeSpan timeSpan)
{
	return (double)timeSpan.Duration / 10000000.0;
}

double TimeSpanHelper::GetTotalMilliseconds(TimeSpan timeSpan)
{
	return (double)(timeSpan.Duration) / 10000.0;
}