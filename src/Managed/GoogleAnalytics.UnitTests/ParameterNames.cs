using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoogleAnalytics.UnitTests
{
    public class ParameterNames
    {
        //General 
        public const string Version = "v";      // required 
        public const string PropertyId = "tid"; // required 
        public const string AnonymizeId = "aid";
        public const string DataSource = "ds";
        public const string CacheBuster = "z";
        public const string QueueTime = "qt";     

        //User
        public const string ClientId = "cid";   // required if userId is not included. 
        public const string UserId = "uid";     // required if ClientId is not included;  

        //Session 
        public const string SessionControl = "sc"; // 'start' , 'end' ; 
        public const string IPOverride = "uip";
        public const string UserAgentOverride = "ua";
        public const string GeographicOverride = "geoid";

        //Traffic Sources 
        public const string Referrer = "dr";
        public const string CampaignName = "cn";
        public const string CampaignSource = "cs";
        public const string CampaignMedium = "cm";
        public const string CampaignKeyword = "ck";
        public const string CampaignContent = "cc";
        public const string CampaignId = "ci";
        public const string GoogleAdwordsId = "gclid";      //unused 
        public const string GoogleDisplayAdsId = "dclid";   // unused 

        //System Info 
        public const string ScreenResolution = "sr";
        public const string ViewportSize = "vp";
        public const string Encoding = "de";
        public const string ScreenColors = "sd";
        public const string UserLanguage = "ul";
        public const string JavaEnabled = "je";  // unused 
        public const string FlashVersion = "fl";  //unused 

        //Hit  
        public const string HitType = "t";
        public const string NonInteractionHit = "ni";  //unused 

        //Content Information 
        public const string DocumentLocation = "dl";
        public const string DocumentHostName = "dh";
        public const string DocumentPath = "dp";
        public const string DocumentTitle = "dt";
        public const string ScreenName = "cd";   // required for screenview hit type 
        public const string ContentGroup = "cg{index}";
        public const string LinkId = "linkid";


        // App Tracking 
        public const string AppName = "an";
        public const string AppId = "aid";
        public const string AppVersion = "av";
        public const string AppInstallerId = "aiid";

        //Event tracking 
        public const string EventCategory = "ec"; // required for event hit type 
        public const string EventAction = "ea";  // required for event hit type 
        public const string EventLabel = "el";
        public const string EventValue = "ev";

        //ECommerce 
        public const string TransactionId = "ti"; //required for transaction type or item hit type 
        public const string TransactionAffiliation = "ta";
        public const string TransactionRevenue = "tr";
        public const string TransactionShipping = "ts";
        public const string TransactionTax = "tt";

        public const string ItemName = "in"; // required for item hit type 
        public const string ItemPrice = "ip";
        public const string ItemQuantity = "iq";
        public const string ItemCode = "ic";
        public const string ItemCategory = "iv";

        // Enhanced Commerce 
        public const string ProductSKU = "pr{productIndex}id";
        public const string ProductName = "pr{productIndex}nm";
        public const string ProductBrand = "pr{productIndex}br";
        public const string ProductCategory = "pr{productIndex}ca";
        public const string ProductVariant = "pr{productIndex}va";
        public const string ProductPrice = "pr{productIndex}pr";
        public const string ProductQuantity = "pr{productIndex}qt";
        public const string ProductCouponCode = "pr{productIndex}cc";
        public const string ProductPosition = "pr{productIndex}ps";
        public const string ProductDimension = "pr{productIndex}cd{dimensionIndex}";
        public const string ProductMetric = "pr{productIndex}cm{metricIndex}";

        public const string ProductAction = "pa";
        public const string ProductActionTransactionId = "ti";  // same as TransactionId 
        public const string ProductActionAffiliation = "ta";
        public const string ProductActionRevenue = "tr";
        public const string ProductActionTax = "tt";
        public const string ProductActionShipping = "ts";
        public const string ProductActionCouponCode = "tcc";
        public const string ProductActionList = "pal";
        public const string ProductActionCheckoutStep = "cos";
        public const string ProductActionCheckoutStepOption = "col";
        public const string ProductImpressionListName = "il{listIndex}nm";
        public const string ProductImpressionSKU = "il{listIndex}pi{productIndex}id";
        public const string ProductImpressionBrand = "il{listIndex}pi{productIndex}id";
        public const string ProductImpressionCategory = "il{listIndex}pi{productIndex}id";
        public const string ProductImpressionVariant = "il{listIndex}pi{productIndex}va";
        public const string ProductImpressionPosition = "il{listIndex}pi{productIndex}ps";
        public const string ProductImpressionPrice = "il{listIndex}pi{productIndex}pr";
        public const string ProductImpressionDimension = "il{listIndex}pi{productIndex}cd{dimensionIndex}";
        public const string ProductImpressionMetric = "il{listIndex}pi{productIndex}cm{dimensionIndex}";

        public const string PromotionId = "promo{promoIndex}id";
        public const string PromotionName = "promo{promoIndex}nm";
        public const string PromotionCreative = "promo{promoIndex}cr";
        public const string PromotionPosition = "promo{promoIndex}ps";
        public const string PromotionAction = "promoa";
        public const string CurrencyCode = "cu";


        //Social interactions 
        public const string SocialNetwork = "sn";      //required for social hit type 
        public const string SocialAction = "sa";       //required for social hit type 
        public const string SocialActionTarget = "st"; // required for social hit target 

        //Timing 
        public const string TimingCategory = "utc"; // required for timing hit type 
        public const string TimingVariable = "utv"; //required for timing hit type 
        public const string TimingLabel = "utl"; 
        public const string PageLoadTime = "plt";       // unused 
        public const string DnsLookupTime = "dns";      // unused 
        public const string PageDownloadTime = "pdt";   // unused 
        public const string RedirectTime = "rrt";        // unused 
        public const string TcpConectTime = "tcp";       //unused 
        public const string ServerResponseTime = "srt";  //unused 
        public const string DomInteractiveTime = "dit";  // unused 
        public const string ContentLoadTime = "clt";     // unused 

        //Exceptions 
        public const string ExceptionDescription = "exd";
        public const string ExceptionIsFatal = "exf";


        //Custom dimensions & metrics 
        public const string Dimension = "cd{dimensionIndex}";
        public const string Metric    = "cm{metricIndex}";

        //Content experiments 
        public const string ExperimentId = "xid";
        public const string ExperimentVariant = "xvar"; 

    }
}
