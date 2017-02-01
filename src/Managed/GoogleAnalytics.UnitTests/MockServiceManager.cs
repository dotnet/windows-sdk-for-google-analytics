using System.Collections.Generic;

namespace GoogleAnalytics.UnitTests
{
    public class MockServiceManager : IServiceManager
    {
        public IDictionary<string, string> LastDataEnqueued { get; private set; }

        public void EnqueueHit(IDictionary<string, string> @params)
        {
            LastDataEnqueued = @params;
        }

        public void EnumerateDataEnqueed ( )
        {
            foreach ( string key in LastDataEnqueued.Keys  )
            {
                System.Diagnostics.Debug.WriteLine( $"{key}='{LastDataEnqueued[key]}'"); 
            }
        }
    }
}
