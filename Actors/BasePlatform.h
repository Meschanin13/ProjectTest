#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BasePlatform.generated.h"

/** BasePlatform is the base class for Platforms that can be placed or spawned in a level.
*
*/

/* EPlatformBehavior has Platform's behavior types like loop or moving on overlap collision. */
UENUM(BlueprintType)
enum class EPlatformBehavior : uint8
{
	OnDemand = 0,
	Loop
};

UCLASS()
class MESCHANINPROJECT_API ABasePlatform : public AActor
{
	GENERATED_BODY()

	ABasePlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* PlatformMovingCurve = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	EPlatformBehavior PlatformBehavior = EPlatformBehavior::OnDemand;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget))
	FVector EndLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	FVector StartLocation = FVector::ZeroVector;

	UFUNCTION(BlueprintCallable)
	void  PlatformHasFinished();

	/*Time of immobility Platform on StartLocation*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float PauseTimeOnStart = 1.0f;

	/*Time of immobility Platform on EndLocation*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float PauseTimeOnEnd = 1.0f;

	UFUNCTION(BlueprintCallable)
	void Move();
	
	UFUNCTION()
	void  UpdateMoving(float Value);

private:
	/*Direction is true if Platform is moving from StartLocation to EndLocation.*/
	bool bDirection = true;

	FTimeline MovingTimeline;
	FTimerHandle TimerHandle;

};

