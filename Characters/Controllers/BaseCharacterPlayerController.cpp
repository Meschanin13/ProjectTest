// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterPlayerController.h"
#include "../BaseCharacter.h"

void ABaseCharacterPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<ABaseCharacter>(InPawn);
}

void ABaseCharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ABaseCharacterPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABaseCharacterPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ABaseCharacterPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ABaseCharacterPlayerController::LookUp);
	InputComponent->BindAxis("TurnAtRate", this, &ABaseCharacterPlayerController::Turn);
	InputComponent->BindAxis("LookUpAtRate", this, &ABaseCharacterPlayerController::LookUp);
	InputComponent->BindAxis("SwimForward", this, &ABaseCharacterPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &ABaseCharacterPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &ABaseCharacterPlayerController::SwimUp);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ABaseCharacterPlayerController::ChangeCrouchState);
	InputComponent->BindAction("StandUp", EInputEvent::IE_Pressed, this, &ABaseCharacterPlayerController::StandUp);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ABaseCharacterPlayerController::Jump);
	InputComponent->BindAction("Prone", EInputEvent::IE_Pressed, this, &ABaseCharacterPlayerController::ChangeProneState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ABaseCharacterPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ABaseCharacterPlayerController::StopSprint);
}

void ABaseCharacterPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void ABaseCharacterPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void ABaseCharacterPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

void ABaseCharacterPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void ABaseCharacterPlayerController::TurnAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TurnAtRate(Value);
	}
}

void ABaseCharacterPlayerController::LookUpAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUpAtRate(Value);
	}
}

void ABaseCharacterPlayerController::StandUp()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StandUp();
	}
}

void ABaseCharacterPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}


void ABaseCharacterPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void ABaseCharacterPlayerController::ChangeProneState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeProneState();
	}
}

void ABaseCharacterPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}


void ABaseCharacterPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void ABaseCharacterPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void ABaseCharacterPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void ABaseCharacterPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}
