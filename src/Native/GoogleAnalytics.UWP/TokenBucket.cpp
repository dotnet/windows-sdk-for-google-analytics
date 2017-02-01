//
// TokenBucket.cpp
// Implementation of the TokenBucket class.
//

#include "pch.h"
#include "TokenBucket.h"
#include "DateTimeHelper.h"
#include "TimeSpanHelper.h"

using namespace GoogleAnalytics;
using namespace Platform;
using namespace Windows::Foundation;

TokenBucket::TokenBucket(double tokens, double fillRate) : 
	capacity(tokens), 
	tokens(tokens), 
	fillRate(fillRate)
{
	timeStamp = DateTimeHelper::Now();
}

bool TokenBucket::Consume(double tokens)
{
	std::lock_guard<std::mutex> lg(locker);  // make thread safe
	if (GetTokens() - tokens > 0)
	{
		this->tokens -= tokens;
		return true;
	}
	else
	{
		return false;
	}
}

double TokenBucket::GetTokens()
{
	auto now = DateTimeHelper::Now();
	if (tokens < capacity)
	{
		auto delta = fillRate * TimeSpanHelper::GetTotalSeconds(TimeSpanHelper::FromTicks(now.UniversalTime - timeStamp.UniversalTime));
		tokens = capacity < tokens + delta ? capacity : tokens + delta;
		timeStamp = now;
	}
	return tokens;
}

