// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "../BaseCharacter.h"
#include "../../Components/MovementComponents/BaseCharacterMovementComponent.h"

void UBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<ABaseCharacter>(), TEXT("UBaseCharacterAnimInstance::NativeBeginPlay() UBaseCharacterAnimInstance can be used only with ABaseCharacter"));
	CachedBaseCharacter = StaticCast<ABaseCharacter*>(TryGetPawnOwner());
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsProning = CharacterMovement->IsProning();
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsSwimming = CharacterMovement->IsSwimming();
	bIsOutOfStamina = CharacterMovement->IsOutOfStamina();

	RightFootEffectorLocation = FVector((CachedBaseCharacter->GetIKRightFootOffset() + CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);
	LeftFootEffectorLocation = FVector(-(CachedBaseCharacter->GetIKLeftFootOffset() + CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);

	CrouchRightFootEffectorLocation = FVector((CachedBaseCharacter->GetIKRightFootOffset() + IKPelvisLengthScale * CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);
	CrouchLeftFootEffectorLocation = FVector(-(CachedBaseCharacter->GetIKLeftFootOffset() + IKPelvisLengthScale * CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);

	IKPelvisBoneOffset = FVector(0.0f, 0.0f, CachedBaseCharacter->GetIKPelvisOffset());
}
