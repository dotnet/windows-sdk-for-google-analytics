using System.Collections.Generic;

namespace GoogleAnalytics
{
    /// <summary>
    /// Interface for a service manager used to send hits to Google Analytics.
    /// </summary>
    public interface IServiceManager
    {
        /// <summary>
        /// Enqueues the hit so it is sent at the next opportunity.
        /// </summary>
        /// <param name="params">Dictionary of hit data to send.</param>
        void EnqueueHit(IDictionary<string, string> @params);
    }
}