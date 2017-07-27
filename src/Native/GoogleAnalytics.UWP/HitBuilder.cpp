//
// HitBuilder.cpp
// Implementation of the HitBuilder class.
//

#include "pch.h"
#include "HitBuilder.h"
#include <collection.h>
#include "TimeSpanHelper.h"



// #include "Ecommerce/Product.h" // xmldocs 
// #include "Ecommerce/ProductAction.h" // xmldocs 

using namespace GoogleAnalytics;
using namespace GoogleAnalytics::Ecommerce;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

String^ HitBuilder::HitType_Screenview = "screenview";
String^ HitBuilder::HitType_Event = "event";
String^ HitBuilder::HitType_Exception = "exception";
String^ HitBuilder::HitType_SocialNetworkInteraction = "social";
String^ HitBuilder::HitType_UserTiming = "timing";

HitBuilder::HitBuilder()
{
	lineage = ref new Vector<HitBuilder^>();
	lineage->Append(this);

	data = ref new Map<String^, String^>();

	Product^ p; 
}

HitBuilder::HitBuilder(IMap<String^, String^>^ data)
{
	lineage = ref new Vector<HitBuilder^>();
	lineage->Append(this);

	this->data = ref new Map<String^, String^>();
	for each (auto kvp in data)
	{
		this->data->Insert(kvp->Key, kvp->Value);
	}
}

HitBuilder::HitBuilder(IVector<HitBuilder^>^ lineage, IMap<String^, String^>^ data)
{
	this->lineage = ref new Vector<HitBuilder^>();
	for each (auto hitBuilder in lineage)
	{
		this->lineage->Append(hitBuilder);
	}
	this->lineage->Append(this);

	this->data = ref new Map<String^, String^>();
	for each (auto kvp in data)
	{
		this->data->Insert(kvp->Key, kvp->Value);
	}
}

HitBuilder^ HitBuilder::CreateScreenView()
{
	auto data = ref new Map<String^, String^>();
	data->Insert("t", HitBuilder::HitType_Screenview);
	return ref new HitBuilder(data);
}

HitBuilder^ HitBuilder::CreateScreenView(String^ screenName)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("t", HitBuilder::HitType_Screenview);
	if (screenName) data->Insert("cd", screenName);
	return ref new HitBuilder(data);
}

HitBuilder^ HitBuilder::CreateCustomEvent(String^ category, String^ action, String^ label, long long value)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("t", HitBuilder::HitType_Event);
	data->Insert("ec", category);
	data->Insert("ea", action);
	if (label) data->Insert("el", label);
	if (value) data->Insert("ev", value.ToString());
	return ref new HitBuilder(data);
}

HitBuilder^ HitBuilder::CreateException(String^ description, bool isFatal)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("t", HitBuilder::HitType_Exception);
	if (description) data->Insert("exd", description);
	if (!isFatal) data->Insert("exf", "0");
	return ref new HitBuilder(data);
}

HitBuilder^ HitBuilder::CreateSocialInteraction(String^ network, String^ action, String^ target)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("t", HitBuilder::HitType_SocialNetworkInteraction);
	data->Insert("sn", network);
	data->Insert("sa", action);
	data->Insert("st", target);
	return ref new HitBuilder(data);
}

HitBuilder^ HitBuilder::CreateTiming(String^ category, String^ variable, IBox<TimeSpan>^ time, String^ label)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("t", HitBuilder::HitType_UserTiming);
	if (category) data->Insert("utc", category);
	if (variable) data->Insert("utv", variable);
	if (time != nullptr) data->Insert("utt", std::floor(0.5 + TimeSpanHelper::GetTotalMilliseconds(time->Value)).ToString());
	if (label) data->Insert("utl", label);
	return ref new HitBuilder(data);
}

String^ HitBuilder::Get(String^ paramName)
{
	return data->Lookup(paramName);
}

HitBuilder^ HitBuilder::Set(String^ paramName, String^ paramValue)
{
	if (data->HasKey(paramName))
	{
		data->Remove(paramName);
	}
	data->Insert(paramName, paramValue);
	return ref new HitBuilder(lineage, data);
}

HitBuilder^ HitBuilder::SetAll(IMap<String^, String^>^ params)
{
	return ref new HitBuilder(lineage, params);
}

HitBuilder^ HitBuilder::SetCustomDimension(int index, Platform::String^ dimension)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("cd" + index, dimension);
	return ref new HitBuilder(lineage, data);
}

HitBuilder^ HitBuilder::SetCustomMetric(int index, long long metric)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("cd" + index, metric.ToString());
	return ref new HitBuilder(lineage, data);
}

HitBuilder^ HitBuilder::SetNewSession()
{
	auto data = ref new Map<String^, String^>();
	data->Insert("sc", "start");
	return ref new HitBuilder(lineage, data);
}

HitBuilder^ HitBuilder::SetNonInteraction()
{
	auto data = ref new Map<String^, String^>();
	data->Insert("ni", "1");
	return ref new HitBuilder(lineage, data);
}

HitBuilder^ HitBuilder::AddProduct(Product^ product)
{
	int index(ProductCount + 1);

	auto data = ref new Map<String^, String^>();
	if (product->Id) data->Insert("pr" + index + "id", product->Id);
	if (product->Name) data->Insert("pr" + index + "nm", product->Name);
	if (product->Brand) data->Insert("pr" + index + "br", product->Brand);
	if (product->Category) data->Insert("pr" + index + "ca", product->Category);
	if (product->Variant) data->Insert("pr" + index + "va", product->Variant);
	if (product->Price) data->Insert("pr" + index + "pr", product->Price->Value.ToString());
	if (product->Quantity) data->Insert("pr" + index + "qt", product->Quantity->Value.ToString());
	if (product->CouponCode) data->Insert("pr" + index + "cc", product->CouponCode);
	if (product->Position) data->Insert("pr" + index + "ps", product->Position->Value.ToString());

	for each (auto kvp in product->CustomDimensions)
	{
		data->Insert("pr" + index + "cd" + kvp->Key, kvp->Value);
	}
	for each (auto kvp in product->CustomDimensions)
	{
		data->Insert("pr" + index + "cm" + kvp->Key, kvp->Value);
	}
	
	auto result = ref new HitBuilder(lineage, data);
	result->ProductCount = index;
	return result;
}


HitBuilder^ HitBuilder::AddPromotion(Promotion^ promotion)
{
	int index(PromotionCount + 1);

	auto data = ref new Map<String^, String^>();
	if (promotion->Id) data->Insert("promo" + index + "id", promotion->Id);
	if (promotion->Name) data->Insert("promo" + index + "nm", promotion->Name);
	if (promotion->Creative) data->Insert("promo" + index + "cr", promotion->Creative);
	if (promotion->Position) data->Insert("promo" + index + "ps", promotion->Position);

	auto result = ref new HitBuilder(lineage, data);
	result->PromotionCount = index;
	return result;
}

HitBuilder^ HitBuilder::SetProductAction(ProductAction^ action)
{
	auto data = ref new Map<String^, String^>();
	if (action->Action) data->Insert("pa", action->Action);
	if (action->TransactionId) data->Insert("ti", action->TransactionId);
	if (action->TransactionAffiliation) data->Insert("ta", action->TransactionAffiliation);
	if (action->TransactionRevenue) data->Insert("tr", action->TransactionRevenue->Value.ToString());
	if (action->TransactionTax) data->Insert("tt", action->TransactionTax->Value.ToString());
	if (action->TransactionShipping) data->Insert("ts", action->TransactionShipping->Value.ToString());
	if (action->TransactionCouponCode) data->Insert("tcc", action->TransactionCouponCode);
	if (action->ProductActionList) data->Insert("pal", action->ProductActionList);
	if (action->CheckoutStep) data->Insert("cos", action->CheckoutStep->Value.ToString());
	if (action->CheckoutOptions) data->Insert("col", action->CheckoutOptions);
	return ref new HitBuilder(lineage, data);
}

HitBuilder^ HitBuilder::SetPromotionAction(PromotionAction action)
{
	auto data = ref new Map<String^, String^>();
	data->Insert("pa", Promotion::GetAction(action));
	return ref new HitBuilder(lineage, data);
}

IMap<String^, String^>^ HitBuilder::Build()
{
	auto data = ref new Map<String^, String^>();
	for each (auto hitBuilder in lineage)
	{
		for each (auto kvp in hitBuilder->Data)
		{
			if (data->HasKey(kvp->Key)) 
			{
				data->Remove(kvp->Key);
			}
			data->Insert(kvp->Key, kvp->Value);
		}
	}
	return data;
}