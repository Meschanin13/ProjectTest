#include "PlatformComponent.h"
#include "../Actors/PlatformInvocator.h"

void UPlatformComponent::OnOverlappedInvocator(bool bIsOn)
{
	if (OnPlatformMoved.IsBound())
	{
		OnPlatformMoved.Broadcast(bIsOn);
	}
};


// Called when the game starts
void UPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PlatformInvocator))
	{
		PlatformInvocator->OnOverlappedInvocator.AddUObject(this, &UPlatformComponent::OnOverlappedInvocator);
	}
}
