//
// DateTimeHelper.cpp
// Implementation of the DateTimeHelper class.
//

#include "pch.h"
#include "DateTimeHelper.h"

using namespace GoogleAnalytics;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization;

DateTime DateTimeHelper::Now()
{
	auto calendar = ref new Calendar();
	return calendar->GetDateTime();
}

DateTime DateTimeHelper::FromUniversalTime(long long universalTime)
{
	DateTime result = DateTime();
	result.UniversalTime = universalTime;
	return result;
}