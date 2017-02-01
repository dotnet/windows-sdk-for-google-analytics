//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace GoogleAnalytics_Sample_Native
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();



	private:
		bool isRunningInDebugMode;
		void ButtonEvent_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonView_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonSocial_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonTiming_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonException_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonThrowException_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void IsDebugRequest_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);




		void AnalyticsManager_HitFailed (Object ^ unused,  GoogleAnalytics::HitFailedEventArgs ^args);
		void AnalyticsManager_HitMalformed (Object ^ unused, GoogleAnalytics::HitMalformedEventArgs  ^args);
		void AnalyticsManager_HitSent(Object ^ unused, GoogleAnalytics::HitSentEventArgs  ^args);

		 
	protected: 
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override; 
		void Log(GoogleAnalytics::Hit^ hit, Platform::String^ message); 
		Platform::String^ Parse(GoogleAnalytics::Hit ^ hit);  


	};
}
