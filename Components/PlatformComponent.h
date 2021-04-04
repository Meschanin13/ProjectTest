#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlatformComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlatormMoved, bool, bIsOn);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MESCHANINPROJECT_API UPlatformComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
	class APlatformInvocator* PlatformInvocator = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Platform Moved")
	FOnPlatormMoved OnPlatformMoved;

	UFUNCTION()
	void OnOverlappedInvocator(bool bIsOn);
	
};
