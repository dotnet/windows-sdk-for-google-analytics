# Windows SDK for Google Analytics&trade;
The Windows SDK for Google Analytics makes it easy to connect your Universal Windows Apps to Google Analytics Mobile App accounts.  

The SDK uses Google's [measurement protocol](https://developers.google.com/analytics/devguides/collection/protocol/) to send HTTP requests with user interaction data to Google's [Universal Analytics](https://support.google.com/analytics/answer/2790010) Services. The SDK also supports the [debug](https://developers.google.com/analytics/devguides/collection/protocol/v1/validating-hits) endpoint which will allows developers to test and validate their hits.

The SDK supports tracking for the following interaction ([Hit](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#t)) types:

- Screen Views 
- Actions (aka [Events](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#events))
- [Social Interactions](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#social) 
- [Exceptions](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#exception)
- [Enhanced commerce](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#enhanced-ecomm) 
- [User Timings](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#timing) 
- [Custom Dimensions & Metrics](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#customs) 
 
The implementation supports:

- [Session Management](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sc) 
- [Cache Busting](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#z)  
- [Dispatching (periodic and manual)](https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#qt)  
 

## Getting Started 
Please read our [Getting Started ](https://github.com/dotnet/windows-sdk-for-google-analytics/wiki/Getting-Started) page for detailed information on pre-requisites, samples, and getting the nuget packages for the SDK.  

 
## Release notes/Update history:
**v1.5.0.0. Feb 2017**. This is our initial release, though our SDK is an iteration on this prior project on [codeplex](http://googleanalyticssdk.codeplex.com/). 
- Please see supported features above.  
- Converged the APIs so that the C# and WinRT SDKs would be seamless. In the convergence, we deprecated a few features from old SDK (in codeplex), so look at our FAQ, for porting your code from older SDK to this one. 
- We tweaked the APIs so they mapped closer to the Google Analytics Android SDK. This way folks familiar with the API on other platform could more easily reuse their knowledge with our SDK. 
 
## Useful links 
- [SDK Documentation](https://github.com/dotnet/windows-sdk-for-google-analytics/wiki/Documentation)
- [Future roadmap page](https://github.com/dotnet/windows-sdk-for-google-analytics/wiki/Roadmap)
- [FAQs](https://github.com/dotnet/windows-sdk-for-google-analytics/wiki/FAQ) 
- [Learn about Google Analytics](https://developers.google.com/analytics/)

## About this SDK
This SDK is intended to have feature parity as well as API similarity (adjusted for .NET and WinRT conventions) with the official Google Analytics SDK for Android.  

### Feedback and Requests 
Please use the [issues](https://github.com/dotnet/windows-sdk-for-google-analytics/issues) page to submit any bugs, comments or questions, and feature requests.  

### Contributing 
We accept contributions to code, samples, and docs. Please submit pull requests.
 
### .NET foundation
This project is supported by the [.NET Foundation](https://dotnetfoundation.org/)
