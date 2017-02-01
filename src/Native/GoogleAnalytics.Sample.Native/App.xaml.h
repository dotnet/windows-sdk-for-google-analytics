//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

namespace GoogleAnalytics_Sample_Native
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	internal:
		App();

		//STEP01: declare a tracker for our app. 
		static GoogleAnalytics::Tracker^ tracker; 

		

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
		void Log (GoogleAnalytics::Hit^ hit, Platform::String^ message);


	public :
		static property GoogleAnalytics::Tracker^ Tracker
		{
			GoogleAnalytics::Tracker^ get() { return tracker;  }
		}
		void OnUnhandledException(Platform::Object ^sender, Windows::UI::Xaml::UnhandledExceptionEventArgs ^e);
	};
}
