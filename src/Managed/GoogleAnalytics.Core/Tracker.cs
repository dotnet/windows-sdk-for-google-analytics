using System;

namespace GoogleAnalytics
{
    /// <summary>
    /// Represents an object capable of tracking events for a single Google Analytics property.
    /// </summary>
    public sealed class Tracker : SimpleTracker
    {
        readonly IPlatformInfoProvider platformInfoProvider;

        /// <summary>
        /// Instantiates a new instance of <see cref="Tracker"/>.
        /// </summary>
        /// <param name="propertyId">the property ID to track to.</param>
        /// <param name="platformInfoProvider">An object capable of providing platform and environment specific information.</param>
        /// <param name="serviceManager">The object used to send <see cref="Hit"/>s to the service.</param>
        public Tracker(string propertyId, IPlatformInfoProvider platformInfoProvider, IServiceManager serviceManager)
            :base(propertyId, serviceManager)
        {
            this.platformInfoProvider = platformInfoProvider;
            
            ClientId = platformInfoProvider.AnonymousClientId;
            ScreenColors = platformInfoProvider.ScreenColors;
            ScreenResolution = platformInfoProvider.ScreenResolution;
            Language = platformInfoProvider.UserLanguage;
            ViewportSize = platformInfoProvider.ViewPortResolution;

            platformInfoProvider.ViewPortResolutionChanged += platformTrackingInfo_ViewPortResolutionChanged;
            platformInfoProvider.ScreenResolutionChanged += platformTrackingInfo_ScreenResolutionChanged;
        }
        
        void platformTrackingInfo_ViewPortResolutionChanged(object sender, EventArgs args)
        {
            ViewportSize = platformInfoProvider.ViewPortResolution;
        }

        void platformTrackingInfo_ScreenResolutionChanged(object sender, EventArgs args)
        {
            ScreenResolution = platformInfoProvider.ScreenResolution;
        }
    }
}
