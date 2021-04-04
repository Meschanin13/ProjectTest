// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UBaseCharacterMovementComponent;
UCLASS(Abstract, NotBlueprintable)
class MESCHANINPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};
	virtual void StandUp();
	virtual void ChangeCrouchState();
	virtual void ChangeProneState();
	
	virtual void SwimForward(float Value) {};
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};

	virtual void StartSprint();
	virtual void StopSprint();
	virtual bool CanSprint();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	FORCEINLINE UBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() { return BaseCharacterMovementComponent; }

	float GetIKRightFootOffset() const { return IKRightFootOffset; }
	float GetIKLeftFootOffset() const { return IKLeftFootOffset; }
	float GetIKPelvisOffset() const { return IKPelvisOffset; }

	bool IsOutOfStamina() { return bIsOutOfStaminaRequested; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void K2_OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);
	virtual void OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);

	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void K2_OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);
	virtual void OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);

	UPROPERTY(BlueprintReadOnly, Category = Character)
	uint32 bIsProned : 1;

protected:
	UFUNCTION(BlueprintCallable, Category = "Character | Movement")
	virtual void Prone(bool bClientSimulation = false);

	UFUNCTION(BlueprintCallable, Category = "Character | Movement")
	virtual	void UnProne(bool bClientSimulation = false);

	UFUNCTION(BlueprintCallable, Category = "Character | Movement")
	virtual bool CanProne() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName RightFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName LeftFootSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | IK settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float IKTraceExtendDistance = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | IK settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float IKInterpSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | IK settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHighForFootIK = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Sprinting settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxStamina = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Sprinting settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float StaminaRestoreVelocity = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Sprinting settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintStaminaConsumptionVelocity = 5.0f;

	UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

private:
	FTimerHandle TimerHandle;
	void OnTimerFired();
	float TimerRate = 10.0f;

	void TryChangeSprintState(float DeltaTime);
	bool bIsSprintRequested = false;
	bool bIsOutOfStaminaRequested = false;
	float CurrentStamina = 0.0f;
	
	float GetIKOffsetForASocket(const FName& SocketName);
	float CalculateIKPelvisOffset();
	float IKRightFootOffset = 0.0f;
	float IKLeftFootOffset = 0.0f;
	float IKPelvisOffset = 0.0f;
};
