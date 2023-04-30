// Copyright Leslie Apland


#include "UI\WidgetController\OverlayWidgetController.h"
#include "AbilitySystem\AuraAttributeSet.h"
#include "AbilitySystem\AuraAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AAS = Cast<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AAS->GetHealth());
	OnMaxHealthChanged.Broadcast(AAS->GetMaxHealth());

	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AAS->GetHealthAttribute()).AddUObject();

	OnManaChanged.Broadcast(AAS->GetMana());
	OnMaxManaChanged.Broadcast(AAS->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AAS = Cast<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AAS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AAS->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AAS->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// Only broadcast if the tag is "Message"-type tag (aka a "sub-tag" of "Message")
				if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}
