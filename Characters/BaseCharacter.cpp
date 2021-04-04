// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Components/MovementComponents/BaseCharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	BaseCharacterMovementComponent = StaticCast<UBaseCharacterMovementComponent*>(GetCharacterMovement());
}

void ABaseCharacter::StandUp()
{
	if (GetCharacterMovement()->IsCrouching() && !BaseCharacterMovementComponent->IsSwimming())
	{
		UnCrouch();
	}
	else if (!GetCharacterMovement()->IsCrouching() && bIsProned && !BaseCharacterMovementComponent->IsSwimming())
	{
		UnProne();
	}
}

void ABaseCharacter::ChangeCrouchState()
{
	if (!GetCharacterMovement()->IsCrouching() && !bIsProned)
	{
		Crouch();
	}
}

void ABaseCharacter::ChangeProneState()
{
	if (GetCharacterMovement()->IsCrouching() && !bIsProned)
	{
		Prone();
	}

	else if (!GetCharacterMovement()->IsCrouching() && bIsProned)
	{
		UnProne();
	}
}

void ABaseCharacter::StartSprint()
{
	bIsSprintRequested = true;

	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void ABaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState(DeltaTime);
	
	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, GetIKOffsetForASocket(RightFootSocketName), DeltaTime, IKInterpSpeed);
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, GetIKOffsetForASocket(LeftFootSocketName), DeltaTime, IKInterpSpeed);
	IKPelvisOffset = FMath::FInterpTo(IKPelvisOffset, CalculateIKPelvisOffset(), DeltaTime, IKInterpSpeed);

	if (!(BaseCharacterMovementComponent->IsSprinting()))
	{
		CurrentStamina += StaminaRestoreVelocity * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}

	if (CurrentStamina < MaxStamina)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Stamina: %.2f"), CurrentStamina));
	}

	if (CurrentStamina == 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseCharacter::OnTimerFired, TimerRate, false);
		bIsOutOfStaminaRequested = true;
		BaseCharacterMovementComponent->SetIsOutOfStamina(bIsOutOfStaminaRequested);
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString::Printf(TEXT("Stamina: %.2f"), CurrentStamina));
	}

	if (CurrentStamina == MaxStamina)
	{
		bIsOutOfStaminaRequested = false;
		BaseCharacterMovementComponent->SetIsOutOfStamina(bIsOutOfStaminaRequested);
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentStamina = MaxStamina;
	TimerRate = MaxStamina / StaminaRestoreVelocity;
}

void ABaseCharacter::Prone(bool bClientSimulation)
{
	if (BaseCharacterMovementComponent)
	{
		if (CanProne())
		{
			BaseCharacterMovementComponent->SetWantsToProne(true);
		}
	}
}

void ABaseCharacter::UnProne(bool bClientSimulation)
{
	if (BaseCharacterMovementComponent)
	{
		if (BaseCharacterMovementComponent)
		{
			BaseCharacterMovementComponent->SetWantsToProne(false);
		}
	}
}

bool ABaseCharacter::CanProne() const
{
	return !bIsProned && BaseCharacterMovementComponent && GetRootComponent() && !GetRootComponent()->IsSimulatingPhysics();
}

void ABaseCharacter::OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	const ACharacter* DefaultCharacter = GetDefault<ACharacter>(GetClass());
	if (GetMesh() && DefaultCharacter->GetMesh())
	{
		FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
		MeshRelativeLocation.Z = DefaultCharacter->GetMesh()->GetRelativeLocation().Z + HalfHeightAdjust;
		BaseTranslationOffset.Z = MeshRelativeLocation.Z;
	}
	else
	{
		BaseTranslationOffset.Z = DefaultCharacter->GetBaseTranslationOffset().Z + HalfHeightAdjust;
	}

	K2_OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ABaseCharacter::OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	const ACharacter* DefaultCharacter = GetDefault<ACharacter>(GetClass());
	if (GetMesh() && DefaultCharacter->GetMesh())
	{
		FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
		MeshRelativeLocation.Z = DefaultCharacter->GetMesh()->GetRelativeLocation().Z + HalfHeightAdjust;
		BaseTranslationOffset.Z = MeshRelativeLocation.Z;
	}
	else
	{
		BaseTranslationOffset.Z = DefaultCharacter->GetBaseTranslationOffset().Z + HalfHeightAdjust;
	}

	K2_OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ABaseCharacter::OnSprintStart_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ABaseCharacter::OnSprintStart_Implementation"));
}

void ABaseCharacter::OnSprintEnd_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnSprintEnd_Implementation"));
}

bool ABaseCharacter::CanSprint()
{	
	return !IsOutOfStamina() && !bIsProned && !BaseCharacterMovementComponent->IsSwimming();
}

void ABaseCharacter::OnTimerFired()
{
	BaseCharacterMovementComponent->ChangingSpeed();
}

void ABaseCharacter::TryChangeSprintState(float DeltaTime)
{
	if (bIsSprintRequested && !GetBaseCharacterMovementComponent()->IsSprinting() && CanSprint())
	{
		BaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}

	if (!bIsSprintRequested && GetBaseCharacterMovementComponent()->IsSprinting())
	{
		BaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}

	if (BaseCharacterMovementComponent->IsSprinting())
	{
		CurrentStamina -= SprintStaminaConsumptionVelocity * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}
}

float ABaseCharacter::GetIKOffsetForASocket(const FName& SocketName)
{
	float Result = 0.0f;

	float ScaledCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	FVector TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z);
	FVector TraceEnd = TraceStart - (ScaledCapsuleHalfHeight + IKTraceExtendDistance) * FVector::UpVector;
	
	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	
	/*Calculate Value for Player's foot position on surface. Returns the z-axis value in Player's local coordinate system */
	const FVector FootSizeBox = FVector(1.f, 11.f, 7.f);
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEnd, FootSizeBox, GetMesh()->GetSocketRotation(SocketName), TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		float CharacterBottom = TraceStart.Z - ScaledCapsuleHalfHeight;
		Result = (CharacterBottom - HitResult.Location.Z);
	}
	
	/*Excludes collisions with object higher than MaxHighForFootIK*/
	if (FMath::Abs(Result) < MaxHighForFootIK)
	{
		return Result;
	}

	return 0;
}

float ABaseCharacter::CalculateIKPelvisOffset()
{
	return  -FMath::Abs(IKRightFootOffset - IKLeftFootOffset);
}