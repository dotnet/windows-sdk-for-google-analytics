using System;
using System.Text;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

namespace GoogleAnalytics.Sample
{
    public sealed partial class MainPage : Page
    {

        bool isRunningInDebugMode; 
             
        public MainPage()
        {
            this.InitializeComponent();
            isRunningInDebugMode = IsDebugRequest.IsChecked.Value; 
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);

            App.Tracker.ScreenName = "Main";

            // wire up to event for debugging purposes:
            AnalyticsManager.Current.HitSent += AnalyticsManager_HitSent;
            AnalyticsManager.Current.HitMalformed += AnalyticsManager_HitMalformed;
            AnalyticsManager.Current.HitFailed += AnalyticsManager_HitFailed;

        }

        private void AnalyticsManager_HitFailed(object sender, HitFailedEventArgs e)
        {
            Log(e.Hit, $"**Hit Failed** {Environment.NewLine} {e.Error.Message}");

        }

        private void AnalyticsManager_HitMalformed(object sender, HitMalformedEventArgs e)
        {
            Log(e.Hit, $"**Hit Malformed ** {Environment.NewLine} {e.HttpStatusCode.ToString()}");
        }

        private void AnalyticsManager_HitSent(object sender, HitSentEventArgs e)
        {
            Log(e.Hit, e.Response);
        }

        private void ButtonException_Click(object sender, RoutedEventArgs e)
        {
            App.Tracker.Send(HitBuilder.CreateException("oops, something went wrong", false).Build());
        }

        private void ButtonEvent_Click(object sender, RoutedEventArgs e)
        {
            App.Tracker.Send(HitBuilder.CreateCustomEvent("test", "userclick", null, 0).Build());
        }

        private void ButtonView_Click(object sender, RoutedEventArgs e)
        {
            App.Tracker.Send(HitBuilder.CreateScreenView().Build());
        }

        private void ButtonSocial_Click(object sender, RoutedEventArgs e)
        {
            App.Tracker.Send(HitBuilder.CreateSocialInteraction("facebook", "share", "http://googleanalyticssdk.codeplex.com").Build());
        }

        private void ButtonTiming_Click(object sender, RoutedEventArgs e)
        {
            App.Tracker.Send(HitBuilder.CreateTiming("someaction", "loadtime", TimeSpan.FromSeconds(2)).Build());
        }

        private void ButtonThrowException_Click(object sender, RoutedEventArgs e)
        {
            object y = 1;
            string x = (string)y;
        }

        private void IsDebugRequest_Checked(object sender, RoutedEventArgs e)
        {
            isRunningInDebugMode = IsDebugRequest.IsChecked.Value;
            Visibility visibility = isRunningInDebugMode  ? Visibility.Visible : Visibility.Collapsed;
            RequestPanel.Visibility = visibility;
            ResponsePanel.Visibility = visibility;

        }


        void Log(Hit hit, string message)
        {
            if (isRunningInDebugMode)
            {
                if ( AnalyticsManager.Current.DispatchPeriod > TimeSpan.Zero )
                {
                    message = "Using dispatcher-which batches activity. Here is the last hit sent\n" + message ; 
                }

                if (Dispatcher.HasThreadAccess)
                {
                    Results.Text = message;
                    Request.Text = Parse(hit);
                }
                else
                {
                    Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                    {
                        Results.Text = message;
                        Request.Text = Parse(hit);
                    });
                } 
                 
            } 
            //Output to console regardless 
            System.Diagnostics.Debug.WriteLine(message);
        }

        string Parse(Hit hit)
        {
            StringBuilder builder = new StringBuilder();
            if (hit != null)
            {
                foreach (string param in hit.Data.Keys)
                {
                    builder.Append($"{param}:{hit.Data[param]}{Environment.NewLine}");
                }
            }
            return builder.ToString();
        }
    }
}
