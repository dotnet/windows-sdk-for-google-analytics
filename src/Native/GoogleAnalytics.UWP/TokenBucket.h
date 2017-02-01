//
// TokenBucket.h
// Declaration of the TokenBucket class.
//

#pragma once

#include <mutex>

namespace GoogleAnalytics
{

	ref class TokenBucket sealed
	{
	private:

		double capacity;

		double tokens;

		double fillRate;

		Windows::Foundation::DateTime timeStamp;
		
		std::mutex locker;

		double GetTokens();

	internal:

		TokenBucket(double tokens, double fillRate);

		bool Consume(double tokens = 1.0);

	};
}
