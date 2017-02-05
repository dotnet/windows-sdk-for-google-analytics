using System;
using System.Linq;
using Windows.ApplicationModel;
using Windows.Devices.Input;
using Windows.Graphics.Display;
using Windows.Storage;
using Windows.System;
using Windows.System.Profile;
using Windows.UI.Core;
using Windows.UI.Xaml;

namespace GoogleAnalytics
{
    /// <summary>
	/// Windows 10, Universal Platform implementation of GoogleAnalytics.Core.IPlatformInfoProvider. 
	/// </summary>
    public sealed class PlatformInfoProvider : IPlatformInfoProvider
    {
        const string Key_AnonymousClientId = "GoogleAnaltyics.AnonymousClientId";

        static string userAgent;

        bool windowInitialized = false;
        string anonymousClientId;
        Dimensions? viewPortResolution;
        Dimensions? screenResolution;
         
        /// <inheritdoc /> 
        public event EventHandler ViewPortResolutionChanged;
        /// <inheritdoc /> 
        public event EventHandler ScreenResolutionChanged;

        public PlatformInfoProvider()
        {
            InitializeWindow();
        }

        /// <inheritdoc /> 
        public void OnTracking()
        {
            if (!windowInitialized)
            {
                InitializeWindow();
            }
        }

        /// <inheritdoc /> 
        public string AnonymousClientId
        {
            get
            {
                if (anonymousClientId == null)
                {
                    var appSettings = ApplicationData.Current.LocalSettings;
                    if (!appSettings.Values.ContainsKey(Key_AnonymousClientId))
                    {
                        anonymousClientId = Guid.NewGuid().ToString();
                        appSettings.Values[Key_AnonymousClientId] = anonymousClientId;
                    }
                    else
                    {
                        anonymousClientId = (string)appSettings.Values[Key_AnonymousClientId];
                    }
                }
                return anonymousClientId;
            }
            set { anonymousClientId = value; }
        }

        /// <inheritdoc /> 
        public Dimensions? ViewPortResolution
        {
            get { return viewPortResolution; }
            private set
            {
                viewPortResolution = value;
                ViewPortResolutionChanged?.Invoke(this, EventArgs.Empty);
            }
        }

        /// <inheritdoc /> 
        public Dimensions? ScreenResolution
        {
            get { return screenResolution; }
            private set
            {
                screenResolution = value;
                ScreenResolutionChanged?.Invoke(this, EventArgs.Empty);
            }
        }

        /// <inheritdoc /> 
        public string UserLanguage
        {
            get { return System.Globalization.CultureInfo.CurrentUICulture.Name; }
        }

        /// <inheritdoc /> 
        /// <remarks>This feature not implemented on this UWP implementation </remarks>
        public int? ScreenColors
        {
            get { return null; }
        }

        /// <inheritdoc /> 
        public string UserAgent
        {
            get
            {
                if (userAgent == null)
                {
                    userAgent = GetUserAgent();
                }
                return userAgent;
            }
        }

        void InitializeWindow()
        {
            try
            {
                if (Window.Current != null && Window.Current.Content != null)
                {
                    var bounds = Window.Current.Bounds;
                    double w, h;
                    var pointerDevice = PointerDevice.GetPointerDevices().FirstOrDefault();
                    if (pointerDevice != null)
                    {
                        w = pointerDevice.ScreenRect.Width;
                        h = pointerDevice.ScreenRect.Height;
                    }
                    else
                    {
                        w = bounds.Width;
                        h = bounds.Height;
                    }

                    var displayInfo = DisplayInformation.GetForCurrentView();
                    var scale = (double)(int)displayInfo.ResolutionScale / 100d;
                    w = Math.Round(w * scale);
                    h = Math.Round(h * scale);

                    if ((displayInfo.NativeOrientation & DisplayOrientations.Landscape) == DisplayOrientations.Landscape)
                    {
                        ScreenResolution = new Dimensions((int)w, (int)h);
                    }
                    else // portrait
                    {
                        ScreenResolution = new Dimensions((int)h, (int)w);
                    }
                    ViewPortResolution = new Dimensions((int)bounds.Width, (int)bounds.Height); // leave viewport at the scale unadjusted size
                    Window.Current.SizeChanged += Current_SizeChanged;
                    windowInitialized = true;
                }
            }
            catch { /* ignore, Bounds may not be ready yet */ }
        }

        void Current_SizeChanged(object sender, WindowSizeChangedEventArgs e)
        {
            ViewPortResolution = new Dimensions((int)e.Size.Width, (int)e.Size.Height);
        }

        static string GetUserAgent()
        {
            var sysInfo = new Windows.Security.ExchangeActiveSyncProvisioning.EasClientDeviceInformation();
            var ai = AnalyticsInfo.VersionInfo;
            string sv = ai.DeviceFamilyVersion;
            ulong v = ulong.Parse(sv);
            ulong v1 = (v & 0xFFFF000000000000L) >> 48;
            ulong v2 = (v & 0x0000FFFF00000000L) >> 32;

            string systemVersion = $"{v1}.{v2}";

            switch (ai.DeviceFamily)
            {
                case "Windows.Desktop":
                    // Edge: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36 Edge/15.15014
                    // This: Mozilla/5.0 (Windows NT 10.0; Win64; X64; Microsoft Corporation; Surface Pro 4)

                    String uaArchitecture;
                    switch (Package.Current.Id.Architecture)
                    {
                        case ProcessorArchitecture.X64:
                            uaArchitecture = "Win64; X64";
                            break;
                        case ProcessorArchitecture.X86:
                            uaArchitecture = "Win32; X86";
                            break;
                        case ProcessorArchitecture.Arm:
                            uaArchitecture = "ARM";
                            break;
                        default:
                            uaArchitecture = "Win64; X64";
                            break;
                    }

                    return $"Mozilla/5.0 (Windows NT {systemVersion}; {uaArchitecture}; {sysInfo.SystemManufacturer}; {sysInfo.SystemProductName})";
                case "Windows.Mobile":
                    // Edge: Mozilla/5.0 (Windows Phone 10.0; Android 6.0.1; Microsoft; Lumia 950) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Mobile Safari/537.36 Edge/15.15014
                    // This: Mozilla/5.0 (Windows Phone 10.0; MicrosoftMDG; RM-1105_12716)

                    return $"Mozilla/5.0 (Windows Phone {systemVersion}; {sysInfo.SystemManufacturer}; {sysInfo.SystemProductName})";
                case "Windows.Xbox":
                    // Edge: Mozilla/5.0 (Windows NT 10.0; Win64; x64; Xbox; Xbox One) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.79 Safari/537.36 Edge/14.14393
                    // This: Mozilla/5.0 (Windows NT 10.0; Win64; X64; Xbox; Xbox One)

                    return $"Mozilla/5.0 (Windows NT {systemVersion}; Win64; X64; Xbox; {sysInfo.SystemProductName})";
                default:
                    return $"Mozilla/5.0 (Windows {systemVersion}; {Package.Current.Id.Architecture.ToString()}; {sysInfo.SystemManufacturer}; {sysInfo.SystemProductName})";
            }
        }
    }
}
