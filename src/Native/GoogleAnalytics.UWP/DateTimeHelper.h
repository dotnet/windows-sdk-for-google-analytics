//
// DateTimeHelper.h
// Declaration of the DateTimeHelper class.
//

#pragma once

namespace GoogleAnalytics
{
	ref class DateTimeHelper sealed
	{

	internal:
		static Windows::Foundation::DateTime Now();

		static Windows::Foundation::DateTime FromUniversalTime(long long universalTime);

	};
}