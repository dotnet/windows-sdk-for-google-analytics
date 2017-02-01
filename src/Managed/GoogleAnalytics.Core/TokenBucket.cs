using System;

namespace GoogleAnalytics
{
    internal class TokenBucket
    {
        readonly object locker = new object();

        double capacity = 0;
        double tokens = 0;
        double fillRate = 0;
        DateTime timeStamp;

        public TokenBucket(double tokens, double fillRate)
        {
            this.capacity = tokens;
            this.tokens = tokens;
            this.fillRate = fillRate;
            this.timeStamp = DateTime.UtcNow;
        }

        public bool Consume(double tokens = 1.0)
        {
            lock (locker) // make thread safe
            {
                if (GetTokens() - tokens > 0)
                {
                    this.tokens -= tokens;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        double GetTokens()
        {
            var now = DateTime.UtcNow;
            if (tokens < capacity)
            {
                var delta = fillRate * (now - timeStamp).TotalSeconds;
                tokens = Math.Min(capacity, tokens + delta);
                timeStamp = now;
            }
            return tokens;
        }
    }
}
