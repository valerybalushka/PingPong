#include "Actors/PingPongGate.h"
#include "Actors/PingPongBall.h"

APingPongGate::APingPongGate()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetupAttachment(RootComponent);
}

void APingPongGate::BeginPlay()
{
	Super::BeginPlay();
	
	InitBindOnBeginOvelap();
}

void APingPongGate::InitBindOnBeginOvelap()
{
	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxBeginOverlap);
	}
}

void APingPongGate::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(APingPongBall::StaticClass()))
	{
		OnGoal.Broadcast();
	}
}
