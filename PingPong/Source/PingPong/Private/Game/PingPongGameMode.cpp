#include "Game/PingPongGameMode.h"

#include "Actors/PingPongBall.h"
#include "Actors/PingPongGate.h"
#include "Game/PingPongGameState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PingPongPlayer.h"

void APingPongGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitBindToPlayersGate();
	InitFindBall();
}

void APingPongGameMode::InitBindToPlayersGate()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APingPongGate::StaticClass(), FoundActors);
	
	TArray<APingPongGate*> Gates;
	for (AActor* Actor : FoundActors)
	{
		APingPongGate* Gate = Cast<APingPongGate>(Actor);
		if (Gate && Gate->ActorHasTag("GatePlayerOne"))
		{
			GatePlayerOne = Gate;
		}

		if (Gate && Gate->ActorHasTag("GatePlayerTwo"))
		{
			GatePlayerTwo = Gate;
		}
	}

	if (GatePlayerOne.IsValid())
	{
		GatePlayerOne->OnGoal.AddDynamic(this, &ThisClass::OnGoalPlayerOne);
	}

	if (GatePlayerTwo.IsValid())
	{
		GatePlayerTwo->OnGoal.AddDynamic(this, &ThisClass::OnGoalPlayerTwo);
	}
}

void APingPongGameMode::OnGoalPlayerOne()
{
	if (APingPongGameState* PingPongGameState = GetGameState<APingPongGameState>())
	{
		PingPongGameState->UpdatePlayerScoreTwo();
	}

	if (PlayerControllerTwo)
	{
		if (APingPongPlayer* PingPongPlayer = Cast<APingPongPlayer>(PlayerControllerTwo->GetPawn()))
		{
			PingPongPlayer->SetupBall(Ball.Get());
		}
	}
}

void APingPongGameMode::OnGoalPlayerTwo()
{
	if (APingPongGameState* PingPongGameState = GetGameState<APingPongGameState>())
	{
		PingPongGameState->UpdatePlayerScoreOne();
	}

	if (PlayerControllerOne)
	{
		if (APingPongPlayer* PingPongPlayer = Cast<APingPongPlayer>(PlayerControllerOne->GetPawn()))
		{
			PingPongPlayer->SetupBall(Ball.Get());
		}
	}
}

void APingPongGameMode::OnPostLogin(AController* NewPlayer)
{
	//Super::OnPostLogin(NewPlayer);

	if (PlayersCount == 0)
	{
		PlayersCount++;
		PlayerControllerOne = Cast<APlayerController>(NewPlayer);
	}
	else if (PlayersCount == 1)
	{
		PlayersCount++;
		PlayerControllerTwo = Cast<APlayerController>(NewPlayer);
	}

	if (PlayersCount > 1)
	{
		if (APingPongGameState* PingPongGameState = GetGameState<APingPongGameState>())
		{
			PingPongGameState->StartGame();
		}
	}
}

AActor* APingPongGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	
	if (PlayersCount == 0)
	{
		for (AActor* Actor : FoundActors)
		{
			if (Actor->ActorHasTag("PlayerStartOne"))
			{
				return Actor;
			}
		}
	}
	else if (PlayersCount == 1)
	{
		for (AActor* Actor : FoundActors)
		{
			if (Actor->ActorHasTag("PlayerStartTwo"))
			{
				return Actor;
			}
		}
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void APingPongGameMode::InitFindBall()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APingPongBall::StaticClass());
	if (APingPongBall* PingPongBall = Cast<APingPongBall>(FoundActor))
	{
		Ball = TWeakObjectPtr<APingPongBall>(PingPongBall);
	}
}
