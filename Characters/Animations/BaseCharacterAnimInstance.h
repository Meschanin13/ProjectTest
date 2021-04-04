// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MESCHANINPROJECT_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsProning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSwimming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOutOfStamina = false;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character | IK Settings")
	FVector RightFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character | IK Settings")
	FVector LeftFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character | IK Settings")
	FVector CrouchRightFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character | IK Settings")
	FVector CrouchLeftFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character | IK Settings")
	FVector IKPelvisBoneOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character | IK Settings")
	float IKPelvisLengthScale = -0.5f;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character | Prone")
	float PronePelvisOffset = 70.0f;

private:
	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacter;	
};
