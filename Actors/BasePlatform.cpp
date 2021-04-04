#include "BasePlatform.h"

ABasePlatform::ABasePlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DefaultPlatformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Platform root"));
	RootComponent = DefaultPlatformRoot;

	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlatformMeshComponent->SetupAttachment(DefaultPlatformRoot);
}

void ABasePlatform::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = PlatformMeshComponent->GetRelativeLocation();

	if (IsValid(PlatformMovingCurve))
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("UpdateMoving"));
		MovingTimeline.AddInterpFloat(PlatformMovingCurve, TimelineCallback);
	}

	if (PlatformBehavior == EPlatformBehavior::Loop)
	{
		FOnTimelineEvent FinishedTimeline;
		FinishedTimeline.BindUFunction(this, FName("PlatformHasFinished"));
		MovingTimeline.SetTimelineFinishedFunc(FinishedTimeline);
	}
}

void ABasePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovingTimeline.TickTimeline(DeltaTime);
}

void ABasePlatform::UpdateMoving(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	PlatformMeshComponent->SetRelativeLocation(NewLocation);
}

void ABasePlatform::PlatformHasFinished()
{
	if (bDirection)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABasePlatform::Move, PauseTimeOnStart);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABasePlatform::Move, PauseTimeOnEnd);
	}
}

void ABasePlatform::Move()
{
	if (bDirection)
	{
		MovingTimeline.Play();
	}
	else
	{
		MovingTimeline.Reverse();
	}

	bDirection = !bDirection;
}
