using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;
using System;
using System.Collections.Generic;

namespace GoogleAnalytics.UnitTests
{
    [TestClass]
    public class UnitTests
    {
        [TestMethod]
        public void GenerateEventHit()
        {
            var hit = HitBuilder.CreateCustomEvent("category", "action", "label", 2);
            var data = hit.Build();
            Assert.IsTrue(data[ParameterNames.HitType] == "event");
            Assert.IsTrue(data[ParameterNames.EventCategory] == "category");
            Assert.IsTrue(data[ParameterNames.EventAction] == "action");
            Assert.IsTrue(data[ParameterNames.EventLabel] == "label");
            Assert.IsTrue(data[ParameterNames.EventValue] == "2");
        }

        [TestMethod]
        public void GenerateEventHitWithoutOptionalInfo()
        {
            var hit = HitBuilder.CreateCustomEvent("category", "action", "", 0);
            var data = hit.Build();
            Assert.IsFalse(data.ContainsKey(ParameterNames.EventLabel));
            Assert.IsFalse(data.ContainsKey(ParameterNames.EventValue));
        }

        [TestMethod]
        public void GenerateHitWithOverrides()
        {
            var hit = HitBuilder.CreateCustomEvent("category", "action", "", 0).Set("ec", "new");
            var data = hit.Build();
            Assert.IsTrue(data[ParameterNames.EventCategory] == "new");
            hit = hit.Set(ParameterNames.EventCategory, "newer");
            data = hit.Build();
            Assert.IsTrue(data[ParameterNames.EventCategory] == "newer");
        }

        [TestMethod]
        public void SendWithTracker()
        {
            var mockServiceManager = new MockServiceManager();
            var tracker = new Tracker("fakePropertyId", null, mockServiceManager);
            tracker.Send(new Dictionary<string, string>() { { "key", "value" } });

            Assert.IsTrue(mockServiceManager.LastDataEnqueued["key"] == "value");
            Assert.IsTrue(mockServiceManager.LastDataEnqueued["tid"] == "fakePropertyId");
            // TODO: test additional fields
        }

        [TestMethod]
        public void GenerateHitWithTrackerProperties()
        {
            var mockServiceManager = new MockServiceManager();
            var tracker = new Tracker("fakePropertyId", null, mockServiceManager);
            const string screenName = "testscreen";
            const string referrer = "referrer";
            const string language = "de-de";
            const string ipOverride = "127.0.0.1";
            const string appInstallerId = "appInstallerid";
            const string appName = "appname";
            const string appVersion = "1.0.0.0";
            const string encoding = "utf-16";

            tracker.ScreenName = screenName;
            tracker.Referrer = referrer;
            tracker.Language = language;
            tracker.IpOverride = ipOverride;
            tracker.AppInstallerId = appInstallerId;
            tracker.AppName = appName;
            tracker.AppVersion = appVersion;
            tracker.Encoding = encoding;


            var hit = HitBuilder.CreateScreenView();
            tracker.Send(HitBuilder.CreateScreenView().Build());

            tracker.Send(new Dictionary<string, string>() { { "key", "value" } });

            Assert.IsTrue(mockServiceManager.LastDataEnqueued["key"] == "value");
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.PropertyId] == "fakePropertyId");
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.ScreenName] == screenName);
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.Referrer] == referrer);
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.UserLanguage] == language);
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.IPOverride] == ipOverride);
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.AppName] == appName);
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.AppVersion] == appVersion);
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.Encoding] == encoding);

            // TODO: test additional fields

        }

        [TestMethod]
        public void SendOverrideWithTracker()
        {
            var mockServiceManager = new MockServiceManager();
            var tracker = new Tracker("fakePropertyId", null, mockServiceManager);
            tracker.Set("key", "valueToOverride");
            tracker.Send(new Dictionary<string, string>() { { "key", "value" } });
            Assert.IsTrue(mockServiceManager.LastDataEnqueued["key"] == "value");
        }

        [TestMethod]
        public void MockPlatformInfoTest()
        {
            IPlatformInfoProvider platformInfo = new MockPlatformInfoProvider();
            MockServiceManager mockServiceManager = new MockServiceManager();
            RunPlatformInfoTest("fakePropertyId", ref platformInfo, ref mockServiceManager);

        }

        [TestMethod]
        public void NativePlatformInfoTest()
        {
            IPlatformInfoProvider platformInfo = null;

            var initializeWindowTask = Windows.ApplicationModel.Core.CoreApplication.MainView.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                 () =>
                 {
                     platformInfo = new PlatformInfoProvider();
                 }).AsTask();

            initializeWindowTask.Wait();
            if (platformInfo != null)
            {
                MockServiceManager mockServiceManager = new MockServiceManager();
                RunPlatformInfoTest("fakePropertyId", ref platformInfo, ref mockServiceManager);
            }
            else
                Assert.Fail("Failed to test UI thread required properties");
        }

        #region Helpers 
        static void RunPlatformInfoTest(string propertyId, ref IPlatformInfoProvider platformInfoProvider,
            ref MockServiceManager mockServiceManager)
        {
            var tracker = new Tracker(propertyId, platformInfoProvider, mockServiceManager);
            if (platformInfoProvider != null)
            {
                tracker.ClientId = platformInfoProvider.AnonymousClientId;
                tracker.ScreenColors = platformInfoProvider.ScreenColors;
                tracker.ScreenResolution = platformInfoProvider.ScreenResolution;
                tracker.ViewportSize = platformInfoProvider.ViewPortResolution;
            }

            tracker.Send(new Dictionary<string, string>() { { "platform", platformInfoProvider.AnonymousClientId ?? "me" } });

            mockServiceManager.EnumerateDataEnqueed();
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.PropertyId] == propertyId);
            Assert.IsNotNull(mockServiceManager.LastDataEnqueued[ParameterNames.UserLanguage]);
            Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.UserLanguage] == platformInfoProvider.UserLanguage);
            if (platformInfoProvider.ScreenColors != null)
                Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.ScreenColors] == platformInfoProvider.ScreenColors.Value.ToString());
            if (platformInfoProvider.ViewPortResolution != null)
                Assert.IsNotNull(mockServiceManager.LastDataEnqueued[ParameterNames.ViewportSize]);
            if (platformInfoProvider.AnonymousClientId != null)
            {
                Assert.IsNotNull(mockServiceManager.LastDataEnqueued[ParameterNames.ClientId]);
                Assert.IsTrue(mockServiceManager.LastDataEnqueued[ParameterNames.ClientId] == platformInfoProvider.AnonymousClientId);
            }
        }
        #endregion 

    }
}
