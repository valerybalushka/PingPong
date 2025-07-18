#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "PingPongPlayerController.generated.h"

UCLASS()
class PINGPONG_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void MoveInput(const FInputActionValue& Value);

	void UpdateMovmentInput(const FVector& Direction);

	UFUNCTION(Server, Reliable)
	void UpdateMovmentInput_Server(const FVector& Direction);

	void LaunchInput(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> LaunchAction;
	
	UPROPERTY(EditDefaultsOnly)
	float PawnSpeed = 2000.0f;

	UPROPERTY(ReplicatedUsing = OnRep_PawnLocation)
	FVector PawnLocation;

	UFUNCTION()
	void OnRep_PawnLocation();
};
