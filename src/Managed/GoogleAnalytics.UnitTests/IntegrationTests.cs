// #define RUN_SYNCHRONOUS  

using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;
using System;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Threading.Tasks;

namespace GoogleAnalytics.UnitTests
{
    [TestClass]
    public class IntegrationTests
    {
        [TestMethod]
        public async Task SendScreenView()
        {
            var hit = HitBuilder.CreateScreenView();            
            await SendHitAsync(hit);             
        }

        [TestMethod]
        public async Task SendEvent()
        {
            var hit = HitBuilder.CreateCustomEvent ("category", "action", "label", 2);
            await SendHitAsync(hit);
        }

        [TestMethod]
        public async Task SendTiming()
        {
            var hit = HitBuilder.CreateTiming("category", "variable", TimeSpan.FromSeconds(2), "label");
            await SendHitAsync(hit);
        }

        [TestMethod]
        public async Task SendFatalException()
        {
            await SendException(true);
        }

        private async Task SendException(bool isFatal)
        {
            var hit = HitBuilder.CreateException("Exception description", isFatal);
            await SendHitAsync(hit);
        }

        [TestMethod]
        public async Task SendNonFatalException()
        {
            await SendException(false);
        }

        [TestMethod]
        public async Task SendCommerceEvent()
        {
            var product = MockConfig.MakeProduct();
            var hit = HitBuilder.CreateCustomEvent("commerce", "purchase", product.Name, 0);
            hit.AddProduct(product);
            hit.SetProductAction(MockConfig.MakePurchaseAction());
            await SendHitAsync(hit);
        }

        [TestMethod]
        public async Task SendCommercePageView()
        {
            var product = MockConfig.MakeProduct();
            var hit = HitBuilder.CreateScreenView();
            hit.AddProduct(product);
            await SendHitAsync(hit);
        }


        [TestMethod]
        public async Task SendSocialInteraction()
        {
            var hit = HitBuilder.CreateSocialInteraction("socialnetwork", "action", "target");
            await SendHitAsync(hit);
        }

        static object syncObject = new object();

        async Task SendHitAsync(HitBuilder hit)
        {

#if RUN_SYNCHRONOUS
            lock (syncObject)
            {
#endif 
            HitSentEventArgs args = null;

            var serviceManager = new AnalyticsManager(MockConfig.Current.PlatformInfoProvider);
            serviceManager.IsDebug = true;
            serviceManager.HitSent +=       (s, e) => { args = e;    Assert.IsFalse(args == null);  };
            serviceManager.HitMalformed +=  (s, e) => { Assert.Fail( "Malformed: " + e.Hit.Parse()); };
            serviceManager.HitFailed +=     (s, e) => { Assert.Fail("Failed:" + e.Error); }; 

#if !NATIVESDK_TEST
                var tracker = new SimpleTracker( MockConfig.Current.PropertyId , serviceManager);
#else
            var tracker = new Tracker(MockConfig.Current.PropertyId,
                        MockConfig.Current.PlatformInfoProvider, serviceManager);
                serviceManager.IsDebug = true;
#endif
                tracker.AppName = MockConfig.Current.AppName;
                tracker.ClientId = MockConfig.Current.ClientId;
                tracker.ScreenName = MockConfig.Current.ScreenName;
                tracker.Send(hit.Build());
                
                
#if RUN_SYNCHRONOUS
                    serviceManager.DispatchAsync().AsTask().Wait();
                    
#else
                    await serviceManager.DispatchAsync();
                    await Task.Delay(200); //HitSent processing is async.. since we need args, let it replicate                
#endif
                 

                Assert.IsTrue(args != null,  "args != null (" +  hit.BuildToString() + ")" ) ;
                using (var stream = new MemoryStream(System.Text.Encoding.UTF8.GetBytes(args.Response)))
                {
                    if (serviceManager.IsDebug)
                    {
                        var serializer = new DataContractJsonSerializer(typeof(DebugResponse));
                        var debugResponse = (DebugResponse)serializer.ReadObject(stream);
                        Assert.IsTrue(debugResponse.HitParsingResult.All(r => r.Valid), "Invalid Request: (" + args.Response  + ")"  );
                    }
                }
#if RUN_SYNCHRONOUS
             }  // lock 
#endif 
        }

        private void ServiceManager_HitMalformed(object sender, HitMalformedEventArgs e)
        {
            throw new NotImplementedException();
        }

        [TestMethod]
        public async Task SendEventWithDispatcherPeriod()
        {
            int delay = 4; 
            TimeSpan ts = TimeSpan.FromSeconds(delay);
            HitSentEventArgs args = null;          
            var hit = HitBuilder.CreateCustomEvent("category", "action", "label", 2);
            var serviceManager = new AnalyticsManager(MockConfig.Current.PlatformInfoProvider);
            serviceManager.HitSent += (s, e) => { args = e; };
            serviceManager.DispatchPeriod = ts;                        
            var tracker = new Tracker(MockConfig.Current.PropertyId,
            MockConfig.Current.PlatformInfoProvider, serviceManager);

            tracker.AppName = MockConfig.Current.AppName;
            tracker.ClientId = MockConfig.Current.ClientId;
            tracker.ScreenName = MockConfig.Current.ScreenName;

            tracker.Send(hit.Build()); 
            serviceManager.IsDebug = true;

            //Wait half our dispatch time, so should have no response yet
            await Task.Delay(TimeSpan.FromSeconds(delay / 2));
            Assert.IsTrue(args == null);

            //Wait for Dispatch 
            await Task.Delay(TimeSpan.FromSeconds(delay ));
            Assert.IsTrue(args != null);

            System.Diagnostics.Debug.WriteLine( args.Hit.Parse()); 
              
            using (var stream = new MemoryStream(System.Text.Encoding.UTF8.GetBytes(args.Response)))
            {
                if (serviceManager.IsDebug)
                {
                    var serializer = new DataContractJsonSerializer(typeof(DebugResponse));
                    var debugResponse = (DebugResponse)serializer.ReadObject(stream);
                    Assert.IsTrue(debugResponse.HitParsingResult.All(r => r.Valid));
                    var queueTimeParam = debugResponse.HitParsingResult[0].Hit.FindParamInQueryString(ParameterNames.QueueTime);                          
                    Assert.IsTrue(queueTimeParam != null);                                         
                }
            }             
        }

        [TestMethod]
        public async Task SentMalformedEventWithUIThreadCallback ()
        {        
#if NATIVESDK_TEST
            bool fireInUIThread = false;

            /* at most one of these three should be true*/ 
            bool fireHitSent = true ;
            bool fireHitMalformed = false ;
            bool fireHitFailed = false ; 

            int delay = 3;
            TimeSpan ts = TimeSpan.FromSeconds(delay);  
            
                                 
            var serviceManager = new AnalyticsManager(MockConfig.Current.PlatformInfoProvider);
            var initializeWindowTask = Windows.ApplicationModel.Core.CoreApplication.MainView.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                 () =>
                 {
                     serviceManager.FireEventsOnUIThread = fireInUIThread;
                 }).AsTask();

            initializeWindowTask.Wait();
                   
            serviceManager.DispatchPeriod = ts;
            serviceManager.IsDebug = true;

            bool isHitSent = false , isMalformed = false , isFailed = false;
            bool isUIThread = false; 


            serviceManager.HitSent += (s , e ) => {
                isHitSent = true;                
                isUIThread = IsCallingInUIThread();                
            };

            serviceManager.HitMalformed += (s, e) =>
            {
                isMalformed = true;
                isUIThread = IsCallingInUIThread();  
            };

            serviceManager.HitFailed += (s, e) =>
            {
                isFailed = true;
                isUIThread = IsCallingInUIThread(); 
            };

            var tracker = new Tracker(MockConfig.Current.PropertyId, MockConfig.Current.PlatformInfoProvider, serviceManager);
            tracker.AppName = MockConfig.Current.AppName;
            tracker.ClientId = MockConfig.Current.ClientId;
            tracker.ScreenName = MockConfig.Current.ScreenName;

            
            HitBuilder hit = null ; 
            
            // fireHitSent 
            hit  = HitBuilder.CreateCustomEvent("category", "action", "label", 2);
            var data = hit.Build();

            if (fireHitMalformed)
            {
               //malform               
            }
            else if ( fireHitFailed )
            {
                 
            }
                          
            tracker.Send( data );
                           
            await Task.Delay(TimeSpan.FromSeconds(delay*3));

            //Ensure event completed 
            Assert.IsTrue(fireHitMalformed == isMalformed );
            Assert.IsTrue(fireHitSent == isHitSent );
            Assert.IsTrue(fireHitFailed == isFailed );

            //Ensure it was in proper thread 
            Assert.IsTrue(isUIThread == fireInUIThread );
#endif          
        }


        private bool IsCallingInUIThread()
        {
            var window = Windows.UI.Core.CoreWindow.GetForCurrentThread();
            if (window != null)
            {
                return window.Dispatcher.HasThreadAccess;
            }
            return false;
        }
    }
}
