#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformInvocator.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOverlappedInvocatorSignature, bool)

UCLASS()
class MESCHANINPROJECT_API APlatformInvocator : public AActor
{
	GENERATED_BODY()

public:
	FOnOverlappedInvocatorSignature OnOverlappedInvocator;

protected:
	UFUNCTION(BlueprintCallable)
	void MovePlatform();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsOn = true;
};
