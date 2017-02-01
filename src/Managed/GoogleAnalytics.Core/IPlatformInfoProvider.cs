using System;

namespace GoogleAnalytics
{
    /// <summary>
    /// Interface to offer a way to provide all environment and platform level information required by Google Analytics.
    /// </summary>
    public interface IPlatformInfoProvider
    {
        /// <summary>
        /// Gets the value that anonymously identifies a particular user, device, or browser instance. For the web, this is generally stored as a first-party cookie with a two-year expiration. For mobile apps, this is randomly generated for each particular instance of an application install. The value of this field should be a random UUID (version 4) as described in <see href="http://www.ietf.org/rfc/rfc4122.txt"/>.
        /// </summary>
        string AnonymousClientId { get; }

        /// <summary>
        /// Callback that indicates something is about to be logged.
        /// </summary>
        /// <remarks>This allows lazy loading of values that might not be available immediately.</remarks>
        void OnTracking();

        /// <summary>
        /// Gets the screen color depth.
        /// </summary>
        int? ScreenColors { get; }

        /// <summary>
        /// Gets the screen resolution.
        /// </summary>
        Dimensions? ScreenResolution { get; }

        /// <summary>
        /// Gets the language (e.g. 'en-us').
        /// </summary>
        string UserLanguage { get; }

        /// <summary>
        /// Gets the Viewport Resolution.
        /// </summary>
        Dimensions? ViewPortResolution { get; }

        /// <summary>
        /// Gets the User Agent of the browser. This is what Google uses to identify the operating system and device used.
        /// </summary>
        string UserAgent { get; }

        /// <summary>
        /// Raised to indicate that the <see cref="ViewPortResolution"/> has changed.
        /// </summary>
        event EventHandler ViewPortResolutionChanged;

        /// <summary>
        /// Raised to indicate that the <see cref="ScreenResolution"/> has changed.
        /// </summary>
        event EventHandler ScreenResolutionChanged;
    }
}
