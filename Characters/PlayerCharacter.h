// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MESCHANINPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitialize);

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void Turn(float Value) override;
	virtual void LookUp(float Value) override;
	virtual void TurnAtRate(float Value) override;
	virtual void LookUpAtRate(float Value) override;
	virtual void Jump() override;
	virtual void ChangeCrouchState() override;
	virtual void ChangeProneState() override;

	virtual void SwimForward(float Value) override;
	virtual void SwimRight(float Value) override;
	virtual void SwimUp(float Value) override;

	virtual void OnStartCrouch(float HeightAdjust, float ScaledHeightAdjust) override;
	virtual void OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust) override;

	virtual void OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual bool CanJumpInternal_Implementation() const override;
	virtual void OnJumped_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	class USpringArmComponent* SpringArmComponent;	

	float SpringArmOffset = 0.0f;
};
