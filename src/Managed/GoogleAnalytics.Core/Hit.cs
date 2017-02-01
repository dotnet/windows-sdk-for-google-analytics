using System;
using System.Collections.Generic;

namespace GoogleAnalytics
{
    /// <summary>
    /// Represents a single event to track.
    /// </summary>
    public sealed class Hit
    {
        internal Hit(IDictionary<string, string> data)
        {
            Data = data;
            TimeStamp = DateTimeOffset.UtcNow;
        }

        /// <summary>
        /// Gets the key value pairs to send to Google Analytics.
        /// </summary>
        public IDictionary<string, string> Data { get; private set; }

        /// <summary>
        /// Gets the timestamp that the event was created.
        /// </summary>
        public DateTimeOffset TimeStamp { get; private set; }
    }
}
