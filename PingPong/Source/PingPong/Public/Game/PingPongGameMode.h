#pragma once

#include "CoreMinimal.h"
#include "Actors/PingPongBall.h"
#include "Actors/PingPongGate.h"
#include "GameFramework/GameMode.h"
#include "PingPongGameMode.generated.h"

UCLASS()
class PINGPONG_API APingPongGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	void InitBindToPlayersGate();

	void InitFindBall();
	
	UFUNCTION()
	void OnGoalPlayerOne();

	UFUNCTION()
	void OnGoalPlayerTwo();

	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	TWeakObjectPtr<APingPongGate> GatePlayerOne = nullptr;
	TWeakObjectPtr<APingPongGate> GatePlayerTwo = nullptr;
	TWeakObjectPtr<APingPongBall> Ball = nullptr;

	TObjectPtr<APlayerController> PlayerControllerOne = nullptr;
	TObjectPtr<APlayerController> PlayerControllerTwo = nullptr;

	int PlayersCount = 0;
};
