using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GoogleAnalytics; 
namespace GoogleAnalytics.UnitTests
{
    internal class MockConfig
    {
        int randomScreenCount = 10;

        private Random random;
        private string defaultClientId = "me";
        private string defaultScreenName = "screen";
        private string machineName = string.Empty;

        public MockConfig()
        {
            random = new Random(DateTime.Now.Millisecond);
        }

        public RandomNess ClientIdRandomization { get; set; } = RandomNess.None;
        public bool UsesRandomScreenName { get; set; } = false;

        public bool IsDebug { get; set; } = true;
        public string PropertyId { get; set; }
        public string ClientId
        {
            get
            {
                // a  hack towards one-time ;) 
                if (ClientIdRandomization == RandomNess.OneTime)
                {
                    defaultClientId = GetRandomClientId();
                    ClientIdRandomization = RandomNess.None;
                }
                else if (ClientIdRandomization == RandomNess.EveryTime)
                {
                    return GetRandomClientId();
                }
                return defaultClientId;
            }
            set {
                defaultClientId = value;
            }
        }
        public string AppName { get; set; }

        public string ScreenName
        {
            get { return (UsesRandomScreenName ? GetRandomScreenName() : defaultScreenName); }
            set { defaultScreenName = value; }
        }
        public string GetRandomScreenName()
        {
            int index = random.Next(0, randomScreenCount);
            return $"{defaultScreenName}{index:f0}";
        }

        public string GetRandomClientId()
        {
            int index = random.Next(0, randomScreenCount);
            return $"{defaultClientId}{index:00}";
        }

        private static MockConfig GetConfig()
        {
            return oneTimeConfigNoDebug;
        }
        public Ecommerce.Product GetProduct  (int index) 
        {
            return MakeProduct(index); 
        }


        static MockConfig _current;
        public static MockConfig Current
        {
            get
            {
                if (_current == null)
                    _current = GetConfig();
                return _current;
            }
            set
            {
                _current = value;
            }
        }

        internal enum RandomNess
        {
            None,
            OneTime,
            EveryTime
        };


        public string MachineName
        {
            get
            {
#if NETFX_CORE
                if (string.IsNullOrEmpty(machineName))
                {
                    var hostNamesList = Windows.Networking.Connectivity.NetworkInformation
                            .GetHostNames();
                    foreach (var entry in hostNamesList)
                    {
                        if (entry.Type == Windows.Networking.HostNameType.DomainName)
                        {
                            return entry.CanonicalName;
                        }
                    }
                }
#endif 
                return machineName;
            }
            set
            {
                machineName = value;
            }
        }

        public IPlatformInfoProvider PlatformInfoProvider
        {
            get
            {
                //TODO... make this flexible.. 
                return new MockPlatformInfoProvider();
            }            
        }

        #region MockData 

        internal static MockConfig defaultTestConfig = new MockConfig()
        {
            PropertyId = "UA-39959863-1",
            ClientId = "me",
            AppName = "Integration Test",
            ScreenName = "home",
        };

        static internal MockConfig oneTimeConfigNoDebug = new MockConfig()
        {
            PropertyId = "UA-85149873-4",
            ClientId = "Jaime Test",
            AppName = "One-Time Randomization",
            ScreenName = "screen",
            ClientIdRandomization = RandomNess.OneTime,
            UsesRandomScreenName = true,
            IsDebug = false
        };

        static internal Ecommerce.Product Product1 = MakeProduct(1);
        static internal Ecommerce.Product Product2 = MakeProduct(2); 


        static internal Ecommerce.Product MakeProduct(int index = 0 )
        {
                if (index < 1)
                    index = new Random().Next(0, 5); 

            var product = new Ecommerce.Product()
            {
                Id = index.ToString("000"),
                Category = $"Category{index:000}",
                Name = $"Product{index:000}",
                Price = 0.99 + (index - 1),
                Quantity = index,            
            };
            return product;
        }

        static internal Ecommerce.ProductAction  MakePurchaseAction(int index = 0)
        {
            if ( index < 1 )
                index = new Random().Next(0, 5);
            var productAction = new Ecommerce.ProductAction(Ecommerce.ActionEnum.Purchase)
            {
                TransactionId = $"TI{index:000}",
                TransactionAffiliation = $"TA{index:000}",
                TransactionRevenue = index * .99,
                TransactionTax = index * .9 , 
                TransactionShipping = index * 2 * .99
            };
            return productAction;
        }
        #endregion
    }
}
