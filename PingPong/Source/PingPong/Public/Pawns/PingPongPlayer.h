#pragma once

#include "CoreMinimal.h"
#include "Actors/PingPongBall.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "PingPongPlayer.generated.h"

UCLASS()
class PINGPONG_API APingPongPlayer : public APawn
{
	GENERATED_BODY()

protected:
	APingPongPlayer();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupBall(APingPongBall* Ball);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LaunchBall();
};
