#include "Game/PingPongGameState.h"

#include "Actors/PingPongBall.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void APingPongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongGameState, PlayerOneScore);
	DOREPLIFETIME(APingPongGameState, PlayerTwoScore);
	DOREPLIFETIME(APingPongGameState, bStartGame);
}

void APingPongGameState::UpdatePlayerScoreOne()
{
	PlayerOneScore++;

	OnScoreChanged.Broadcast(PlayerOneScore, PlayerTwoScore);
}

void APingPongGameState::UpdatePlayerScoreTwo()
{
	PlayerTwoScore++;

	OnScoreChanged.Broadcast(PlayerOneScore, PlayerTwoScore);
}

void APingPongGameState::OnRep_PlayerOneScore()
{
	OnScoreChanged.Broadcast(PlayerOneScore, PlayerTwoScore);
}

void APingPongGameState::OnRep_PlayerTwoScore()
{
	OnScoreChanged.Broadcast(PlayerOneScore, PlayerTwoScore);
}

void APingPongGameState::StartGame()
{
	bStartGame = true;

	ActivateBallOnGameStarted();

	OnGameStart.Broadcast();
}

void APingPongGameState::OnRep_StartGame()
{
	OnGameStart.Broadcast();
}

void APingPongGameState::ActivateBallOnGameStarted()
{
	if (!HasAuthority())
	{
		return;
	}
	
	AActor* FindActor = UGameplayStatics::GetActorOfClass(GetWorld(), APingPongBall::StaticClass());
	if (FindActor)
	{
		FindActor->SetActorTickEnabled(true);
	}
}