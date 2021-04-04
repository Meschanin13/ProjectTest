// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../../Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"

UBaseCharacterMovementComponent::UBaseCharacterMovementComponent()
{
	BaseCharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());
	WalkSpeed = MaxWalkSpeed;
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}
	return Result;
}

void UBaseCharacterMovementComponent::StartSprint()
{
	bIsSprinting = true;
	bForceMaxAccel = 1;
}

void UBaseCharacterMovementComponent::StopSprint()
{
	bIsSprinting = false;
	bForceMaxAccel = 0;
}

bool UBaseCharacterMovementComponent::IsProning() const
{
	return BaseCharacterOwner && BaseCharacterOwner->bIsProned;
}

bool UBaseCharacterMovementComponent::CanProneInCurrentState() const
{
	return (IsFalling() || IsMovingOnGround()) && UpdatedComponent && !UpdatedComponent->IsSimulatingPhysics();
}

void UBaseCharacterMovementComponent::Prone(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (!bClientSimulation && !CanProneInCurrentState())
	{
		return;
	}

	// See if collision is already at desired size.
	if (CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == ProneCapsuleHalfHeight)
	{
		if (!bClientSimulation)
		{
			BaseCharacterOwner->bIsProned =true;
			bWantsToCrouch = false;
			BaseCharacterOwner->bIsCrouched = false;
		}
		BaseCharacterOwner->OnStartProne(0.f, 0.f);
		return;
	}

	// Change collision size to proning dimensions
	const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
	const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float OldUnscaledRadius = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	// Height is not allowed to be smaller than radius.
	const float ClampedPronedHalfHeight = FMath::Max3(0.f, ProneCapsuleRadus, ProneCapsuleHalfHeight);
	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(ProneCapsuleRadus, ClampedPronedHalfHeight);
	float HalfHeightAdjust = (OldUnscaledHalfHeight - ClampedPronedHalfHeight);
	float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

	if (!bClientSimulation)
	{
		// Proning to a larger height? (this is rare)
		if (ClampedPronedHalfHeight > CrouchedHalfHeight)
		{
			FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(ProneTrace), false, CharacterOwner);
			FCollisionResponseParams ResponseParam;
			InitCollisionParams(CapsuleParams, ResponseParam);
			const bool bEncroached = GetWorld()->OverlapBlockingTestByChannel(UpdatedComponent->GetComponentLocation() - FVector(0.f, 0.f, ScaledHalfHeightAdjust), FQuat::Identity,
			UpdatedComponent->GetCollisionObjectType(), GetPawnCapsuleCollisionShape(SHRINK_None), CapsuleParams, ResponseParam);

			// If encroached, cancel
			if (bEncroached)
			{
				CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(OldUnscaledRadius, CrouchedHalfHeight);
				return;
			}
		}

		// Intentionally not using MoveUpdatedComponent, where a horizontal plane constraint would prevent the base of the capsule from staying at the same spot.
		if (bProneMaintainsBaseLocation)
		{
			UpdatedComponent->MoveComponent(FVector(0.f, 0.f, -ScaledHalfHeightAdjust), UpdatedComponent->GetComponentQuat(), true, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
		}

		BaseCharacterOwner->bIsProned = true;
		bWantsToCrouch = false;
		BaseCharacterOwner->bIsCrouched = false;
	}

	bForceNextFloorCheck = true;

	// OnStartProne takes the change from the Default size, not the current one (though they are usually the same).
	ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
	HalfHeightAdjust = (DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - ClampedPronedHalfHeight);
	ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

	BaseCharacterOwner->OnStartProne(HalfHeightAdjust, ScaledHalfHeightAdjust);
	MaxWalkSpeed = ProneSpeed;
}

void UBaseCharacterMovementComponent::UnProne(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
	
	// See if collision is already at desired size.
	if (CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == CrouchedHalfHeight)
	{
		if (!bClientSimulation)
		{
			BaseCharacterOwner->bIsProned = false;
			bWantsToCrouch = true;
			BaseCharacterOwner->bIsCrouched = true;
		}
		BaseCharacterOwner->OnEndProne(0.f, 0.f);
		return;
	}

	const float CurrentPronedHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
	const float OldUnscaledHalfHeight = CrouchedHalfHeight;
	const float HalfHeightAdjust = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - OldUnscaledHalfHeight;
	const float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;
	const FVector PawnLocation = UpdatedComponent->GetComponentLocation();

	check(CharacterOwner->GetCapsuleComponent());

	if (!bClientSimulation)
	{
		const UWorld* MyWorld = GetWorld();
		const float SweepInflation = KINDA_SMALL_NUMBER * 10.f;
		FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(ProneTrace), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(CapsuleParams, ResponseParam);

		const FCollisionShape StandingCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, -SweepInflation - ScaledHalfHeightAdjust);
		const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
		bool bEncroached = true;

		if (!bProneMaintainsBaseLocation)
		{
			bEncroached = MyWorld->OverlapBlockingTestByChannel(PawnLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);

			if (bEncroached)
			{
				if (ScaledHalfHeightAdjust > 0.f)
				{
					float PawnRadius, PawnHalfHeight;
					CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleSize(PawnRadius, PawnHalfHeight);
					const float ShrinkHalfHeight = PawnHalfHeight - PawnRadius;
					const float TraceDist = PawnHalfHeight - ShrinkHalfHeight;
					const FVector Down = FVector(0.f, 0.f, -TraceDist);

					FHitResult Hit(1.f);
					const FCollisionShape ShortCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, ShrinkHalfHeight);
					const bool bBlockingHit = MyWorld->SweepSingleByChannel(Hit, PawnLocation, PawnLocation + Down, FQuat::Identity, CollisionChannel, ShortCapsuleShape, CapsuleParams);
					if (Hit.bStartPenetrating)
					{
						bEncroached = true;
					}
					else
					{
						const float DistanceToBase = (Hit.Time * TraceDist) + ShortCapsuleShape.Capsule.HalfHeight;
						const FVector NewLoc = FVector(PawnLocation.X, PawnLocation.Y, PawnLocation.Z - DistanceToBase + StandingCapsuleShape.Capsule.HalfHeight + SweepInflation + MIN_FLOOR_DIST / 2.f);
						bEncroached = MyWorld->OverlapBlockingTestByChannel(NewLoc, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
						if (!bEncroached)
						{
							UpdatedComponent->MoveComponent(NewLoc - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
						}
					}
				}
			}
		}
		else
		{
			// Expand while keeping base location the same.
			FVector StandingLocation = PawnLocation + FVector(0.f, 0.f, StandingCapsuleShape.GetCapsuleHalfHeight() - CurrentPronedHalfHeight);
			bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);

			if (bEncroached)
			{
				if (IsMovingOnGround())
				{
					const float MinFloorDist = KINDA_SMALL_NUMBER * 10.f;
					if (CurrentFloor.bBlockingHit && CurrentFloor.FloorDist > MinFloorDist)
					{
						StandingLocation.Z -= CurrentFloor.FloorDist - MinFloorDist;
						bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
					}
				}
			}

			if (!bEncroached)
			{
				UpdatedComponent->MoveComponent(StandingLocation - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
				bForceNextFloorCheck = true;
			}
		}

		if (bEncroached)
		{
			return;
		}

		BaseCharacterOwner->bIsProned = false;
		bWantsToCrouch = true;
		BaseCharacterOwner->bIsCrouched = true;
	}
	else
	{
		bShrinkProxyCapsule = true;
	}

	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), CrouchedHalfHeight, true);

	BaseCharacterOwner->OnEndProne(HalfHeightAdjust, ScaledHalfHeightAdjust);
	MaxWalkSpeed = WalkSpeed;
}

void UBaseCharacterMovementComponent::SetIsOutOfStamina(bool bIsOutOfStamina_In)
{
		bIsOutOfStamina = bIsOutOfStamina_In;

		if (bIsOutOfStamina)
		{
			StopSprint();
			MaxWalkSpeed = OutOfStaminaSpeed;
		}
}

void UBaseCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaTime)
{
	Super::UpdateCharacterStateBeforeMovement(DeltaTime);

	if (BaseCharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		const bool bIsProning = IsProning();
		if (bIsProning && (!bWantsToProne || !CanProneInCurrentState()))
		{
			UnProne(false);
		}
		else if (!bIsProning && bWantsToProne && CanProneInCurrentState())
		{
			Prone(false);
		}
	}
}

void UBaseCharacterMovementComponent::ChangingSpeed()
{
	MaxWalkSpeed = WalkSpeed;
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (MovementMode == MOVE_Swimming)
	{
		if (IsProning())
		{	
			bWantsToProne = false;
			UnProne(false);
		}

		if (IsCrouching())
		{
			bWantsToCrouch = false;
			UnCrouch();
		}

		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfHeight);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);
	}
}