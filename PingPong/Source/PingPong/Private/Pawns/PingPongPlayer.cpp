#include "Pawns/PingPongPlayer.h"

APingPongPlayer::APingPongPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	Mesh->SetupAttachment(Box);
}

void APingPongPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}
