using System;

namespace GoogleAnalytics.UnitTests
{
    /// <summary>
    /// Provides a simple implementation of <see cref="IPlatformInfoProvider"/> that can be used without creating a custom implementation.
    /// </summary>
    public sealed class MockPlatformInfoProvider : IPlatformInfoProvider
    {
        Dimensions? viewPortResolution;
        Dimensions? screenResolution;


#if NATIVESDK_TEST        
        public MockPlatformInfoProvider ()
        {
            //TODO: Uninitialized C# strings are throwing in C++ land 
            AnonymousClientId = string.Empty;
            UserLanguage = "es-es";
            UserAgent = string.Empty; 
        }
#endif 
        /// <summary>
        /// Raised to indicate that something is about to be tracked.
        /// </summary>
        public event EventHandler Tracking;


#if NATIVESDK_TEST
        /// <inheritdoc/>
        public event EventHandler<object> ViewPortResolutionChanged;

        /// <inheritdoc/>
        public event EventHandler<object> ScreenResolutionChanged;
#else
        /// <inheritdoc/>
        public event EventHandler ViewPortResolutionChanged;

        /// <inheritdoc/>
        public event EventHandler ScreenResolutionChanged;
#endif 
        /// <inheritdoc/>
        public string AnonymousClientId { get; set; }

        /// <inheritdoc/>
        public void OnTracking()
        {
            Tracking?.Invoke(this, EventArgs.Empty);
        }

        /// <inheritdoc/>
        public int? ScreenColors { get; set; }

        /// <inheritdoc/>
        public string UserLanguage { get; set; }

        /// <inheritdoc/>
        public Dimensions? ScreenResolution
        {
            get { return screenResolution; }
            set
            {
                screenResolution = value;
                ScreenResolutionChanged?.Invoke(this, EventArgs.Empty);
            }
        }

        /// <inheritdoc/>
        public Dimensions? ViewPortResolution
        {
            get { return viewPortResolution; }
            set
            {
                viewPortResolution = value;
                ViewPortResolutionChanged?.Invoke(this, EventArgs.Empty);
            }
        }

        /// <inheritdoc/>
        public string UserAgent { get; set; }
    }
}
