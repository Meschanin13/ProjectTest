// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

/**
 * 
 */
class ABaseCharacter;
UCLASS()
class MESCHANINPROJECT_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UBaseCharacterMovementComponent();

	FORCEINLINE bool IsSprinting() { return bIsSprinting; }
	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	virtual bool IsProning() const;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaTime) override;
	virtual bool CanProneInCurrentState() const;
	virtual void Prone(bool bClientSimulation = false);
	virtual void UnProne(bool bClientSimulation = false);
	
	void SetIsOutOfStamina(bool bIsOutOfStamina_In);
	bool IsOutOfStamina() { return bIsOutOfStamina; }

	void SetWantsToProne(bool bWantsToProne_In) { bWantsToProne = bWantsToProne_In; };

	/**
	 * If true, prouning should keep the base of the capsule in place by lowering the center of the shrunken capsule. If false, the base of the capsule moves up and the center stays in place.
	 * The same behavior applies when the character unprones: if true, the base is kept in the same location and the center moves up. If false, the capsule grows and only moves up if the base impacts something.
	 */
	UPROPERTY(Category = "Character Movement (General Settings)", VisibleInstanceOnly, BlueprintReadWrite, AdvancedDisplay)
	bool bProneMaintainsBaseLocation = true;

	void ChangingSpeed();

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(Category = "Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleRadius = 50.0f;

	UPROPERTY(Category = "Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleHalfHeight = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Prone", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float ProneCapsuleRadus = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Prone", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float ProneCapsuleHalfHeight = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Prone", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float ProneSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Prone", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float OutOfStaminaSpeed = 100.0f;

	ABaseCharacter* BaseCharacterOwner;

private:
	bool bWantsToProne = false;
	bool bIsSprinting = false;
	bool bIsOutOfStamina = false;
	float WalkSpeed = 0.0f;
};
