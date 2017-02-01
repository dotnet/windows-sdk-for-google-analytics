//
// PlatformInfoProvider.h
// Declaration of the PlatformInfoProvider class.
//

#pragma once

#include <agile.h>
#include "IPlatformInfoProvider.h"

namespace GoogleAnalytics
{
	/// <summary>
	/// Windows 10, Universal Platform implementation of <see cref="IPlatformInfoProvider"/>.
	/// </summary>
	public ref class PlatformInfoProvider sealed : IPlatformInfoProvider
	{
	private:

		Platform::Agile<Windows::UI::Core::CoreWindow^> coreWindow;

		Windows::Foundation::EventRegistrationToken sizeChangedEventToken;

		static Platform::String^ Key_AnonymousClientId;

		Platform::String^ anonymousClientId;

		bool windowInitialized;

		void InitializeWindow();

		void UninitializeWindow();

		void Window_SizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ e);

		Platform::IBox<Dimensions>^ viewPortResolution;

		Platform::IBox<Dimensions>^ screenResolution;

		void SetViewPortResolution(Platform::IBox<Windows::Foundation::Size>^ value);
		void SetScreenResolution(Platform::IBox<Windows::Foundation::Size>^ value);



	internal:

		static Platform::String^ ConstructUserAgent();

	public:

		PlatformInfoProvider();

		virtual ~PlatformInfoProvider();

		/// <inheritdoc/>
		virtual event Windows::Foundation::EventHandler<Platform::Object^>^ ViewPortResolutionChanged;

		/// <inheritdoc/>
		virtual event Windows::Foundation::EventHandler<Platform::Object^>^ ScreenResolutionChanged;

		/// <inheritdoc/>
		virtual void OnTracking();

		/// <inheritdoc/>
		property Platform::String^ AnonymousClientId
		{
			virtual Platform::String^ get();
			void set(Platform::String^ value);
		}

		/// <inheritdoc/>
		virtual property Platform::IBox<Dimensions>^ ViewPortResolution
		{
			Platform::IBox<Dimensions>^ get();
		}

		/// <inheritdoc/>
		virtual property Platform::IBox<Dimensions>^ ScreenResolution
		{
			Platform::IBox<Dimensions>^ get();
		}

		/// <inheritdoc/>
		virtual property Platform::String^ UserLanguage
		{
			Platform::String^ get();
		}

		/// <inheritdoc/>
		virtual property Platform::IBox<int>^ ScreenColors
		{
			Platform::IBox<int>^ get();
		}

		virtual property Platform::String^ UserAgent
		{
			Platform::String^ get();
		}

	};
}
