//
// Product.h
// Declaration of the Product class.
//

#pragma once

namespace GoogleAnalytics
{
	namespace Ecommerce
	{
		public enum class PromotionAction
		{
			Click,
			View
		};

		public ref class Promotion sealed
		{

		internal:

			static Platform::String^ GetAction(PromotionAction action)
			{
				switch (action)
				{
				case PromotionAction::Click:
					return "click";
				case PromotionAction::View:
					return "view";
				default:
					throw ref new Platform::NotImplementedException();
				}
			}

		public:

			property Platform::String^ Creative;

			property Platform::String^ Id;

			property Platform::String^ Name;

			property Platform::String^ Position;

		};

	}
}