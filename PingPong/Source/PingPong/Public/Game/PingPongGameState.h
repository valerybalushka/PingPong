#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PingPongGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged, int, PlayerOne, int, PlayerTwo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);

UCLASS()
class PINGPONG_API APingPongGameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	
	void UpdatePlayerScoreOne();
	
	void UpdatePlayerScoreTwo();

	void StartGame();

protected:
	void ActivateBallOnGameStarted();

public:
	
	FORCEINLINE bool GetIsGameStarted() const { return bStartGame; };

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGameStart OnGameStart;
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerOneScore)
	int PlayerOneScore;

	UFUNCTION()
	void OnRep_PlayerOneScore();
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerTwoScore)
	int PlayerTwoScore;

	UFUNCTION()
	void OnRep_PlayerTwoScore();

	UPROPERTY(ReplicatedUsing = OnRep_StartGame)
	bool bStartGame = false;

	UFUNCTION()
	void OnRep_StartGame();
};
