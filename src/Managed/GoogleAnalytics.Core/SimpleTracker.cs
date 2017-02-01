using System;
using System.Collections.Generic;
using System.Linq;

namespace GoogleAnalytics
{
    /// <summary>
    /// Represents an object capable of tracking events for a single Google Analytics property.
    /// </summary>
    public class SimpleTracker
    {
        readonly IServiceManager serviceManager;
        readonly IDictionary<string, string> data;

        /// <summary>
        /// Instantiates a new instance of <see cref="Tracker"/>.
        /// </summary>
        /// <param name="propertyId">the property ID to track to.</param>
        /// <param name="serviceManager">the object responsible for receiving hits ready to be sent to the service.</param>
        public SimpleTracker(string propertyId, IServiceManager serviceManager)
        {
            if (propertyId == null) throw new ArgumentNullException(nameof(propertyId));
            if (serviceManager == null) throw new ArgumentNullException(nameof(serviceManager));

            data = new Dictionary<string, string>();
            this.serviceManager = serviceManager;

            PropertyId = propertyId;
            SampleRate = 100.0F;
        }

        // TODO: enableAdvertisingIdCollection(boolean enabled) 
        // TODO: setCampaignParamsOnNextHit(Uri uri) 
        // TODO: setSessionTimeout(long sessionTimeout) 

        #region General

        /// <summary>
        /// Gets or sets the tracking ID / web property ID. The format is UA-XXXX-Y. All collected data is associated by this ID.
        /// </summary>
        /// <remarks>Required for all hit types.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#tid"/>
        public string PropertyId { get; private set; }

        /// <summary>
        /// Gets or sets whether the IP address of the sender will be anonymized.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aiid"/>
        public bool AnonymizeIP { get; set; }

        ///// <summary>
        ///// Gets or sets the data source of the hit. Hits sent from analytics.js will have data source set to 'web'; hits sent from one of the mobile SDKs will have data source set to 'app'.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ds"/>
        //public string DataSource { get; set; }

        #endregion

        #region User

        /// <summary>
        /// Gets or sets the value that anonymously identifies a particular user, device, or browser instance. For the web, this is generally stored as a first-party cookie with a two-year expiration. For mobile apps, this is randomly generated for each particular instance of an application install. The value of this field should be a random UUID (version 4) as described in <see href="http://www.ietf.org/rfc/rfc4122.txt"/>.
        /// </summary>
        /// <remarks>Required for all hit types.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cid"/>
        public string ClientId { get; set; }

        ///// <summary>
        ///// Gets or sets the known identifier for a user provided by the site owner/tracking library user. It may not itself be PII (personally identifiable information). The value should never be persisted in GA cookies or other Analytics provided storage.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#uid"/>
        //public string UserId { get; set; }

        #endregion

        #region Session

        /// <summary>
        /// Gets or sets the IP address of the user. This should be a valid IP address in IPv4 or IPv6 format. It will always be anonymized just as though anonymize IP had been used.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#uip"/>
        public string IpOverride { get; set; }

        /// <summary>
        /// Gets or sets the User Agent of the browser. Note that Google has libraries to identify real user agents. Hand crafting your own agent could break at any time.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ua"/>
        public string UserAgentOverride { get; set; }

        /// <summary>
        /// Gets or sets the geographical location of the user. The geographical ID should be a two letter country code or a criteria ID representing a city or region (see http://developers.google.com/analytics/devguides/collection/protocol/v1/geoid). This parameter takes precedent over any location derived from IP address, including the IP Override parameter. An invalid code will result in geographical dimensions to be set to '(not set)'.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#geoid"/>
        public string LocationOverride { get; set; }

        #endregion

        #region Traffic Sources

        /// <summary>
        /// Specifies which referral source brought traffic to a website. This value is also used to compute the traffic source. The format of this value is a URL.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dr"/>
        public string Referrer { get; set; }

        ///// <summary>
        ///// Specifies the campaign name.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cn"/>
        //public string CampaignName { get; set; }

        ///// <summary>
        ///// Specifies the campaign source.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cs"/>
        //public string CampaignSource { get; set; }

        ///// <summary>
        ///// Specifies the campaign medium.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cm"/>
        //public string CampaignMedium { get; set; }

        ///// <summary>
        ///// Specifies the campaign keyword.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ck"/>
        //public string CampaignKeyword { get; set; }

        ///// <summary>
        ///// Specifies the campaign content.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cc"/>
        //public string CampaignContent { get; set; }

        ///// <summary>
        ///// Specifies the campaign ID.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ci"/>
        //public string CampaignId { get; set; }

        ///// <summary>
        ///// Specifies the Google AdWords Id.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#gclid"/>
        //public string GoogleAdWordsId { get; set; }

        ///// <summary>
        ///// Specifies the Google Display Ads Id.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dclid"/>
        //public string GoogleDisplayAdsId { get; set; }

        #endregion

        #region System Info

        /// <summary>
        /// Gets or sets the screen resolution.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sr"/>
        public Dimensions? ScreenResolution { get; set; }

        /// <summary>
        /// Gets or sets the viewable area of the browser / device.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#vp"/>
        public Dimensions? ViewportSize { get; set; }

        /// <summary>
        /// Gets or sets the character set used to encode the page / document.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#de"/>
        public string Encoding { get; set; }

        /// <summary>
        /// Gets or sets the screen color depth.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sd"/>
        public int? ScreenColors { get; set; }

        /// <summary>
        /// Gets or sets the language.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ul"/>
        public string Language { get; set; }

        #endregion

        #region Content Information

        ///// <summary>
        ///// Gets or sets the full URL (document location) of the page on which content resides.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dl"/>
        //public string DocumentLocationUrl { get; set; }

        /// <summary>
        /// Gets or sets the hostname from which content was hosted.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dh"/>
        public string HostName { get; set; }

        /// <summary>
        /// Gets or sets the path portion of the page URL.
        /// </summary>
        /// <remarks>Optional. Should begin with '/'.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dp"/>
        public string Page { get; set; }

        /// <summary>
        /// Gets or sets the title of the page / document.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#dt"/>
        public string Title { get; set; }

        /// <summary>
        /// Gets or sets the 'Screen Name' of the screenview hit. On web properties this will default to the unique URL of the page.
        /// </summary>
        /// <remarks>Required for screenview hit type. Note: This parameter is optional on web properties, and required on mobile properties for screenview hits.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cd"/>
        public string ScreenName { get; set; }

        ///// <summary>
        ///// Gets or sets the ID of a clicked DOM element, used to disambiguate multiple links to the same URL in In-Page Analytics reports when Enhanced Link Attribution is enabled for the property.
        ///// </summary>
        ///// <remarks>Optional.</remarks>
        ///// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#linkid"/>
        //public string LinkId { get; set; }

        #endregion

        #region App Tracking

        /// <summary>
        /// Gets or sets the application name. This field is required for any hit that has app related data (i.e., app version, app ID, or app installer ID). For hits sent to web properties, this field is optional.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#an"/>
        public string AppName { get; set; }

        /// <summary>
        /// Gets or sets the application identifier.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aid"/>
        public string AppId { get; set; }

        /// <summary>
        /// Gets or sets the application version.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#av"/>
        public string AppVersion { get; set; }

        /// <summary>
        /// Gets or sets the application installer identifier.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aiid"/>
        public string AppInstallerId { get; set; }

        #endregion

        #region Content Experiments

        /// <summary>
        /// Gets or sets the parameter that specifies that this user has been exposed to an experiment with the given ID. It should be sent in conjunction with the Experiment Variant parameter.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#xid"/>
        public string ExperimentId { get; set; }

        /// <summary>
        /// Gets or sets the parameter that specifies that this user has been exposed to a particular variation of an experiment. It should be sent in conjunction with the Experiment ID parameter.
        /// </summary>
        /// <remarks>Optional.</remarks>
        /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#xvar"/>
        public string ExperimentVariant { get; set; }

        #endregion

        /// <summary>
        /// Gets or sets the rate at which <see cref="Hit"/>s should be excluded for sampling purposes. Default is 100.
        /// </summary>
        /// <remarks>100 means no items should be excluded, 50 means half should be excluded, and 0 means all items should be excluded.</remarks>
        public float SampleRate { get; set; }

        /// <summary>
        /// Gets the model value for the given key added through <see cref="Set"/>.
        /// </summary>
        /// <param name="key">The key to retrieve the value for.</param>
        /// <returns>The value associated with the key.</returns>
        public string Get(string key)
        {
            return data[key];
        }

        /// <summary>
        /// Sets the model value for the given key.
        /// </summary>
        /// <param name="key">The key of the field that needs to be set. It starts with "&amp;" followed by the parameter name. The complete list of fields can be found at <see href="https://goo.gl/M6dK2U"/>.</param>
        /// <param name="value">A string value to be sent to Google servers. A null value denotes that the value should not be sent over wire.</param>
        public void Set(string key, string value)
        {
            data[key] = value;
        }
        
        IDictionary<string, string> AddRequiredHitData(IDictionary<string, string> @params)
        {
            var result = new Dictionary<string, string>();

            result.Add("v", "1");
            result.Add("tid", PropertyId);
            result.Add("cid", ClientId);
            result.Add("an", AppName);
            result.Add("av", AppVersion);

            if (AppId != null) result.Add("aid", AppId);
            if (AppInstallerId != null) result.Add("aiid", AppInstallerId);
            if (ScreenName != null) result.Add("cd", ScreenName);
            if (AnonymizeIP) result.Add("aip", "1");
            if (ScreenResolution.HasValue) result.Add("sr", string.Format("{0}x{1}", ScreenResolution.Value.Width, ScreenResolution.Value.Height));
            if (ViewportSize.HasValue) result.Add("vp", string.Format("{0}x{1}", ViewportSize.Value.Width, ViewportSize.Value.Height));
            if (Language != null) result.Add("ul", Language);
            if (ScreenColors.HasValue) result.Add("sd", string.Format("{0}-bits", ScreenColors.Value));

            //if (CampaignName != null) result.Add("cn", CampaignName);
            //if (CampaignSource != null) result.Add("cs", CampaignSource);
            //if (CampaignMedium != null) result.Add("cm", CampaignMedium);
            //if (CampaignKeyword != null) result.Add("ck", CampaignKeyword);
            //if (CampaignContent != null) result.Add("cc", CampaignContent);
            //if (CampaignId != null) result.Add("ci", CampaignId);

            if (Referrer != null) result.Add("dr", Referrer);
            if (Encoding != null) result.Add("de", Encoding);
            //if (GoogleAdWordsId != null) result.Add("gclid", GoogleAdWordsId);
            //if (GoogleDisplayAdsId != null) result.Add("dclid", GoogleDisplayAdsId);
            if (IpOverride != null) result.Add("uip", IpOverride);
            if (UserAgentOverride != null) result.Add("ua", UserAgentOverride);
            if (HostName != null) result.Add("dh", HostName);
            if (Page != null) result.Add("dp", Page);
            if (Title != null) result.Add("dt", Title);
            if (ExperimentId != null) result.Add("xid", ExperimentId);
            if (ExperimentVariant != null) result.Add("xvar", ExperimentVariant);
            if (LocationOverride != null) result.Add("geoid", LocationOverride);
            //if (UserId != null) result.Add("uid", UserId);

            // other params available but not usually used for apps
            //if (DataSource != null) result.Add("ds", DataSource);
            //if (LinkId != null) result.Add("linkid", LinkId);
            //if (DocumentLocationUrl != null) result.Add("dl", DocumentLocationUrl);

            foreach (var item in data.Concat(@params))
            {
                result[item.Key] = item.Value;
            }

            return result;
        }

        /// <summary>
        /// Merges the model values set on this Tracker with params and generates a hit to be sent.
        /// </summary>
        /// <param name="params">Dictionary of hit data to values which are merged with the existing values which are already set (using Set(String, String)). Values in this dictionary will override the values set earlier. The values in this dictionary will not be reused for the subsequent hits. If you need to send a value in multiple hits, you can use the Set(String, String) method.</param>
        /// <remarks>The hit may not be dispatched immediately.</remarks>
        public void Send(IDictionary<string, string> @params)
        {
            if (!string.IsNullOrEmpty(PropertyId))
            {
                if (!IsSampledOut())
                {
                    serviceManager.EnqueueHit(AddRequiredHitData(@params));
                }
            }
        }

        bool IsSampledOut()
        {
            if (SampleRate <= 0.0F)
            {
                return true;
            }
            else if (SampleRate < 100.0F)
            {
                return ((ClientId != null) && (Math.Abs(ClientId.GetHashCode()) % 10000 >= SampleRate * 100.0F));
            }
            else return false;
        }
    }
}
