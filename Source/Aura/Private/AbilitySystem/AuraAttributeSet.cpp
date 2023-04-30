// Copyright Leslie Apland


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.ContextHandle = Data.EffectSpec.GetContext();
	Props.SourceActor->ASC = Props.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceActor->ASC) && Props.SourceActor->ASC->AbilityActorInfo.IsValid() && Props.SourceActor->ASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceActor->AvatarActor = Props.SourceActor->ASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceActor->Controller = Props.SourceActor->ASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceActor->Controller == nullptr && Props.SourceActor->AvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceActor->AvatarActor))
			{
				Props.SourceActor->Controller = Pawn->GetController();
			}
		}
		if (Props.SourceActor->Controller)
		{
			Props.SourceActor->Character = Cast<ACharacter>(Props.SourceActor->Controller->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetActor->AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetActor->Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetActor->AvatarActor = Cast<ACharacter>(Props.TargetActor->AvatarActor);
		Props.TargetActor->ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetActor->AvatarActor);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& PrevHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, PrevHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& PrevMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, PrevMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& PrevMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, PrevMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& PrevMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, PrevMaxMana);
}
