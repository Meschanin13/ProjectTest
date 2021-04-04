// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseCharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MESCHANINPROJECT_API ABaseCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void SetupInputComponent() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void Jump();
	void StandUp();
	void ChangeCrouchState();
	void ChangeProneState();

	void StartSprint();
	void StopSprint();

	void SwimForward(float Value);
	void SwimRight(float Value);
	void SwimUp(float Value);

	TSoftObjectPtr<class ABaseCharacter> CachedBaseCharacter;

	
};
