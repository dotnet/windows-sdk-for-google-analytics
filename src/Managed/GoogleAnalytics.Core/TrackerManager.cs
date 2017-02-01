using System.Collections.Generic;
using System.Threading.Tasks;

namespace GoogleAnalytics
{
    /// <summary>
    /// Provides a way to manage multiple <see cref="Tracker"/> instances.
    /// </summary>
    public class TrackerManager : ServiceManager
    {
        readonly IPlatformInfoProvider platformInfoProvider;
        readonly Dictionary<string, Tracker> trackers;

        /// <summary>
        /// Instantiates a new instance of <see cref="TrackerManager"/>.
        /// </summary>
        /// <param name="platformInfoProvider">An object capable of providing platform and environment specific information.</param>
        public TrackerManager(IPlatformInfoProvider platformInfoProvider)
        {
            trackers = new Dictionary<string, Tracker>();
            this.platformInfoProvider = platformInfoProvider;
            UserAgent = platformInfoProvider.UserAgent;
        }

        /// <summary>
        /// Gets the collection of <see cref="Tracker"/> instances.
        /// </summary>
        protected ICollection<Tracker> Trackers
        {
            get { return trackers.Values; }
        }

        /// <summary>
        /// Gets or sets the default tracker instance for easy access.
        /// </summary>
        /// <remarks>This always returns the last tracker instance created.</remarks>
        public Tracker DefaultTracker { get; set; }

        /// <summary>
        /// Gets or sets whether the app should log information to Google Analtyics.
        /// </summary>
        /// <remarks>See Google Analytics usage guidelines for more information.</remarks>
        public virtual bool AppOptOut { get; set; }

        /// <summary>
        /// Gets a <see cref="Tracker"/> using a given property ID. Will creates a new instance if one does not exist yet.
        /// </summary>
        /// <param name="propertyId">The property ID that the <see cref="Tracker"/> should log to.</param>
        /// <returns>The new or existing instance keyed on the property ID.</returns>
        public virtual Tracker CreateTracker(string propertyId)
        {
            propertyId = propertyId ?? string.Empty;
            if (!trackers.ContainsKey(propertyId))
            {
                var tracker = new Tracker(propertyId, platformInfoProvider, this);
                trackers.Add(propertyId, tracker);
                if (DefaultTracker == null)
                {
                    DefaultTracker = tracker;
                }
                return tracker;
            }
            else
            {
                return trackers[propertyId];
            }
        }

        /// <summary>
        /// Removes and cleans up a given <see cref="Tracker"/>.
        /// </summary>
        /// <param name="tracker">The instance to remove and clean up.</param>
        public void CloseTracker(Tracker tracker)
        {
            trackers.Remove(tracker.PropertyId);
            if (DefaultTracker == tracker)
            {
                DefaultTracker = null;
            }
        }

        /// <summary>
        /// Gets the instance of <see cref="IPlatformInfoProvider"/> used by all <see cref="Tracker"/> instances.
        /// </summary>
        public IPlatformInfoProvider PlatformTrackingInfo
        {
            get { return platformInfoProvider; }
        }

        /// <inheritdoc/>
        public override void EnqueueHit(IDictionary<string, string> @params)
        {
            if (!AppOptOut)
            {
                base.EnqueueHit(@params);
            }
        }
    }
}
