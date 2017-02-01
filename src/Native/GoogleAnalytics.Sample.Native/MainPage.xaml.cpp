//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace GoogleAnalytics_Sample_Native;
using namespace GoogleAnalytics; 

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;



// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	isRunningInDebugMode = this->IsDebugRequest->IsChecked->Value; 
}



void GoogleAnalytics_Sample_Native::MainPage::OnNavigatedTo( NavigationEventArgs^ args )
{
	App::Tracker->ScreenName = "MainPage"; 		 
	AnalyticsManager::Current->HitSent += ref new Windows::Foundation::EventHandler<GoogleAnalytics::HitSentEventArgs ^>(this, &GoogleAnalytics_Sample_Native::MainPage::AnalyticsManager_HitSent);
	AnalyticsManager::Current->HitMalformed += ref new Windows::Foundation::EventHandler<GoogleAnalytics::HitMalformedEventArgs ^>(this, &GoogleAnalytics_Sample_Native::MainPage::AnalyticsManager_HitMalformed);
	AnalyticsManager::Current->HitFailed += ref new Windows::Foundation::EventHandler<GoogleAnalytics::HitFailedEventArgs ^>(this, &GoogleAnalytics_Sample_Native::MainPage::AnalyticsManager_HitFailed);

	 
}


void GoogleAnalytics_Sample_Native::MainPage::ButtonEvent_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	App::Tracker->Send ( HitBuilder::CreateCustomEvent("test", "userclick", nullptr, 0)->Build());
}


void GoogleAnalytics_Sample_Native::MainPage::ButtonSocial_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	App::Tracker->Send( HitBuilder::CreateSocialInteraction("facebook", "share", "http://googleanalyticssdk.codeplex.com")->Build());
}

void GoogleAnalytics_Sample_Native::MainPage::ButtonView_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	App::Tracker->Send(HitBuilder::CreateScreenView()->Build());
}


void GoogleAnalytics_Sample_Native::MainPage::ButtonTiming_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{ 
	TimeSpan timeSpan;  
	timeSpan.Duration = 20000000L; //  2 seconds
	App::Tracker->Send(HitBuilder::CreateTiming("loadtime", "currentPage", timeSpan, "")->Build()) ; 
}
 


void GoogleAnalytics_Sample_Native::MainPage::ButtonException_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	App::Tracker->Send(HitBuilder::CreateException("oops, something went wrong", false)->Build());
}


void MainPage::ButtonThrowException_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Object^ y = 1;
	String^ x = (String^)y;
}

void MainPage::IsDebugRequest_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	
	isRunningInDebugMode = IsDebugRequest->IsChecked->Value; 
	Windows::UI::Xaml::Visibility visibility = isRunningInDebugMode ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;

	RequestPanel->Visibility = visibility;
	ResponsePanel->Visibility = visibility;

}



void MainPage::AnalyticsManager_HitSent(  Object^ unused, HitSentEventArgs^ e  )
{
	Log(e->Hit, e->Response ); 
	 
}

void MainPage::AnalyticsManager_HitFailed(Object^ unused, HitFailedEventArgs ^ e)
{

	Log(e->Hit, e->Error );
}

void MainPage::AnalyticsManager_HitMalformed(Object^ unused, HitMalformedEventArgs ^ e)
{

	String ^error = "Hit malformed. Http code=" + e->HttpStatusCode.ToString(); 
	Log(e->Hit, error );
}

void MainPage::Log (GoogleAnalytics::Hit^ hit, String^ message)
{
	 
	if ( isRunningInDebugMode )
	{
		if (AnalyticsManager::Current->DispatchPeriod.Duration > 0)
			message = "Using Dispatcher- which batches activity. Here is the last hit sent\n" + message; 

		if (Dispatcher->HasThreadAccess)
		{			
			Results->Text = message;
			Request->Text = Parse(hit);
		} 
		else 
			Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
				ref new Windows::UI::Core::DispatchedHandler([this, message, hit]()
		{
			Results->Text = message;
			Request->Text = Parse(hit);
		}));
	}
	// Output to console regardless 
	::OutputDebugString(message->Data());
}


String^ MainPage::Parse(Hit^ hit)
{
	String^ retVal = ""; 

	if (hit != nullptr )
	{		
		for ( IKeyValuePair<String^, String^>^ item : hit->Data )  
		{
			retVal += item->Key + ":" + item->Value + "\n";  
		}
		 
	} 
	return retVal;

}

/* 
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

*/


