using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;

namespace GoogleAnalytics.UnitTests
{
    static class Extensions
    {
        public static string Parse(this Hit hit)
        {
            StringBuilder builder = new StringBuilder();            
            foreach (string param in hit.Data.Keys)
            {
                builder.Append($"{param}:{hit.Data[param]}&");
            }             
            return builder.ToString();
        }

        public static string BuildToString (this HitBuilder hitBuilder )
        {
            StringBuilder builder = new StringBuilder();
            var all = hitBuilder.Build();
            foreach (string key in all.Keys)
            {
                builder.Append($"{key}:{all[key]}&");
            }             
            return builder.ToString();
        }

        public static string FindParamInQueryString ( this string hit , string param )
        {
            WwwFormUrlDecoder decoder = new WwwFormUrlDecoder(hit);
            return decoder.GetFirstValueByName(param);              
        }
    }
     
}
