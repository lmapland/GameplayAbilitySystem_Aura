// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interface/MouseOver.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IMouseOver
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
