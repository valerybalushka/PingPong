#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "PingPongBall.generated.h"

UCLASS()
class PINGPONG_API APingPongBall : public AActor
{
	GENERATED_BODY()

protected:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	APingPongBall();
	
	virtual void BeginPlay() override;
	
	void InitRandomBallDirection();

	void UpdateForwardMovement();

	void HandleOnBallBounce(const FHitResult& OutSweepHitResult);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh; 

	UPROPERTY(EditAnywhere)
	float BallSpeed = 2000.0f;

	UPROPERTY(EditAnywhere)
	float DefaultBallSpeed = 2000.0f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> NiagaraSystemForBounce;
	
	UPROPERTY(ReplicatedUsing = OnRep_BouncePoint)
	FTransform BouncePoint;

	UFUNCTION()
	void OnRep_BouncePoint();
	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RestartBallSpeed();
};
