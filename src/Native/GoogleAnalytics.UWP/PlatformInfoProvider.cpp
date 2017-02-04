//
// PlatformInfoProvider.cpp
// Implementation of the PlatformInfoProvider class.
//

#include "pch.h"
#include "PlatformInfoProvider.h"
#include <string>
#include <collection.h>

using namespace GoogleAnalytics;
using namespace Platform;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Graphics::Display;


IBox<Dimensions>^ GetDimensions(IBox<Windows::Foundation::Size>^ size)
{
	if (size != nullptr)
	{
		Dimensions result;
		result.Width = size->Value.Width;
		result.Height = size->Value.Height;
		return result;
	}
	return nullptr;
}


IBox<Dimensions>^ ParseDimensionsUpdate(IBox<Dimensions>^currentDimentions,
	IBox<Windows::Foundation::Size>^ newSize,
	bool &hasChanged)
{
	Dimensions valDimensions;
	if (newSize != nullptr)
	{
		valDimensions.Width = newSize->Value.Width;
		valDimensions.Height = newSize->Value.Height;
		if (currentDimentions == nullptr)
		{
			hasChanged = true;
		}
		else if (currentDimentions->Value.Height != newSize->Value.Height ||
			currentDimentions->Value.Width != newSize->Value.Width)
		{
			hasChanged = true;
		}
		return valDimensions;
	}
	else
	{
		if (currentDimentions != nullptr)
		{
			hasChanged = true;
		}
		return nullptr;
	}
}


String^ PlatformInfoProvider::Key_AnonymousClientId = "GoogleAnaltyics.AnonymousClientId";

PlatformInfoProvider::PlatformInfoProvider() :
	windowInitialized(false),
	viewPortResolution(nullptr),
	screenResolution(nullptr)
{
	InitializeWindow();
}

PlatformInfoProvider::~PlatformInfoProvider()
{
	UninitializeWindow();
}

void PlatformInfoProvider::UninitializeWindow()
{
	if (windowInitialized)
	{
		coreWindow->SizeChanged -= sizeChangedEventToken;
		coreWindow = nullptr;
		windowInitialized = false;
	}
}

void PlatformInfoProvider::InitializeWindow()
{
	try
	{
		coreWindow = Windows::UI::Core::CoreWindow::GetForCurrentThread();
		if (coreWindow.Get())
		{
			auto bounds = coreWindow->Bounds;

			float w = bounds.Width;
			float h = bounds.Height;
			auto displayInfo = DisplayInformation::GetForCurrentView();
			switch (displayInfo->ResolutionScale)
			{
			case ResolutionScale::Scale120Percent:
				w = std::floorf(.5f + w * 1.2f);
				w = std::floorf(.5f + w * 1.2f);
				break;
			case ResolutionScale::Scale140Percent:
				w = std::floorf(.5f + w * 1.4f);
				w = std::floorf(.5f + w * 1.4f);
				break;
			case ResolutionScale::Scale150Percent:
				w = std::floorf(.5f + w * 1.5f);
				w = std::floorf(.5f + w * 1.5f);
				break;
			case ResolutionScale::Scale160Percent:
				w = std::floorf(.5f + w * 1.6f);
				w = std::floorf(.5f + w * 1.6f);
				break;
			case ResolutionScale::Scale180Percent:
				w = std::floorf(.5f + w * 1.8f);
				w = std::floorf(.5f + w * 1.8f);
				break;
			case ResolutionScale::Scale225Percent:
				w = std::floorf(.5f + w * 2.25f);
				w = std::floorf(.5f + w * 2.25f);
				break;
			}

			if ((displayInfo->CurrentOrientation & DisplayOrientations::Landscape) == DisplayOrientations::Landscape)
			{
				SetScreenResolution(Size(w, h));
			}
			else // portrait
			{
				SetScreenResolution(Size(h, w));
			}
			SetViewPortResolution(Size(bounds.Width, bounds.Height)); // leave viewport at the scale unadjusted size
			sizeChangedEventToken = coreWindow->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &PlatformInfoProvider::Window_SizeChanged);
			windowInitialized = true;
		}
	}
	catch (const std::exception) { /* ignore, CoreWindow may not be ready yet */ }
}

void PlatformInfoProvider::Window_SizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ e)
{
	SetViewPortResolution(e->Size);
}

void PlatformInfoProvider::OnTracking()
{
	if (!windowInitialized)
	{
		InitializeWindow();
	}
}

String^ PlatformInfoProvider::AnonymousClientId::get()
{
	if (anonymousClientId) return anonymousClientId;
	auto appSettings = ApplicationData::Current->LocalSettings;
	if (!appSettings->Values->HasKey(Key_AnonymousClientId))
	{
		GUID guid;
		CoCreateGuid(&guid);
		std::wstring str(Guid(guid).ToString()->Data());
		auto result = ref new String(str.substr(1, str.length() - 2).data());
		appSettings->Values->Insert(Key_AnonymousClientId, result);
		return result;
	}
	else
	{
		return (String^)appSettings->Values->Lookup(Key_AnonymousClientId);
	}
}

void PlatformInfoProvider::AnonymousClientId::set(String^ value)
{
	anonymousClientId = value;
}

IBox<Dimensions>^ PlatformInfoProvider::ViewPortResolution::get()
{
	return viewPortResolution;
}

void PlatformInfoProvider::SetViewPortResolution(IBox<Size>^ value)
{
	bool hasChanged = false;
	viewPortResolution = ParseDimensionsUpdate(viewPortResolution, value, hasChanged);
	if (hasChanged)
	{
		ViewPortResolutionChanged(this, nullptr);
	}
}

IBox<Dimensions>^ PlatformInfoProvider::ScreenResolution::get()
{
	return screenResolution;
}

void PlatformInfoProvider::SetScreenResolution(IBox<Size>^ value)
{
	bool hasChanged = false;
	screenResolution = ParseDimensionsUpdate(screenResolution, value, hasChanged);
	if (hasChanged)
		ScreenResolutionChanged(this, nullptr);
}

String^ PlatformInfoProvider::UserLanguage::get()
{
	return Windows::Globalization::ApplicationLanguages::Languages->GetAt(0);
}

IBox<int>^ PlatformInfoProvider::ScreenColors::get()
{
	return nullptr;
}

Platform::String^ PlatformInfoProvider::UserAgent::get()
{
	return ConstructUserAgent();
}

String^ PlatformInfoProvider::ConstructUserAgent()
{
	auto sysInfo = ref new Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation();
	auto package = Windows::ApplicationModel::Package::Current;
	auto ai = Windows::System::Profile::AnalyticsInfo::VersionInfo;
	String^ sv = ai->DeviceFamilyVersion;

	unsigned long long v = wcstoull(sv->Data(), 0, 10);
	unsigned long long v1 = (v & 0xFFFF000000000000L) >> 48;
	unsigned long long v2 = (v & 0x0000FFFF00000000LL) >> 32;
	String^ systemVersion = v1.ToString() + "." + v2.ToString();

	if (ai->DeviceFamily == "Windows.Desktop") {
		String^ uaArchitecture;
		switch (package->Id->Architecture)
		{
		case ProcessorArchitecture::X64:
			uaArchitecture = "Win64; X64";
			break;
		case ProcessorArchitecture::X86:
			uaArchitecture = "Win32; X86";
			break;
		case ProcessorArchitecture::Arm:
			uaArchitecture = "ARM";
			break;
		default:
			uaArchitecture = "Win64; X64";
			break;
		}
		return "Mozilla/5.0 (Windows NT " + systemVersion + "; " + uaArchitecture + "; " + sysInfo->SystemManufacturer + "; " + sysInfo->SystemProductName + ")";
	}
	else if (ai->DeviceFamily == "Windows.Mobile") {
		return "Mozilla/5.0 (Windows Phone " + systemVersion + "; " + sysInfo->SystemManufacturer + "; " + sysInfo->SystemProductName + ")";
	}
	else if (ai->DeviceFamily == "Windows.Xbox") {
		return "Mozilla/5.0 (Windows NT " + systemVersion + "; Win64; X64; Xbox; " + sysInfo->SystemProductName + ")";
	}
	else {
		return "Mozilla/5.0 (Windows " + systemVersion + "; " + package->Id->Architecture.ToString() + "; " + sysInfo->SystemManufacturer + "; " + sysInfo->SystemProductName + ")";
	}
}