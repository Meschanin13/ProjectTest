#include "PlatformInvocator.h"

void APlatformInvocator::MovePlatform()
{
	bIsOn = !bIsOn;

	if (OnOverlappedInvocator.IsBound())
	{
		OnOverlappedInvocator.Broadcast(bIsOn);
	}
};



