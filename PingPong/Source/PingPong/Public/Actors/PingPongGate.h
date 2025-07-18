#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PingPongGate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoal);

UCLASS()
class PINGPONG_API APingPongGate : public AActor
{
	GENERATED_BODY()
	
public:	
	APingPongGate();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	void InitBindOnBeginOvelap();

	UFUNCTION()
	void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
public:	
	FOnGoal OnGoal;
};
