#include "Game/PingPongPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/PingPongPlayer.h"

void APingPongPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongPlayerController, PawnLocation);
}

void APingPongPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APingPongPlayerController::MoveInput);
			EnhancedInput->BindAction(LaunchAction, ETriggerEvent::Started, this, &APingPongPlayerController::LaunchInput);
		}
	}
}

void APingPongPlayerController::MoveInput(const FInputActionValue& Value)
{
	const float Input = Value.Get<float>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector Direction(0.0f, 1.0f, 0.0f);
	Direction.Y = Direction.Y * Input * PawnSpeed * DeltaTime;

	UpdateMovmentInput(Direction);
}

void APingPongPlayerController::UpdateMovmentInput(const FVector& Direction)
{
	if (HasAuthority())
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			ControlledPawn->AddActorLocalOffset(Direction, true);
			PawnLocation = ControlledPawn->GetActorLocation();
			
			return;
		}
	}

	UpdateMovmentInput_Server(Direction);
}

void APingPongPlayerController::OnRep_PawnLocation()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->SetActorLocation(PawnLocation);
	}
}

void APingPongPlayerController::UpdateMovmentInput_Server_Implementation(const FVector& Direction)
{
	UpdateMovmentInput(Direction);
}

void APingPongPlayerController::LaunchInput(const FInputActionValue& Value)
{
	if (APingPongPlayer* PingPongPlaye = Cast<APingPongPlayer>(GetPawn()))
	{
		PingPongPlaye->LaunchBall();
	}
}
