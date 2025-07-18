#include "Actors/PingPongBall.h"
#include "NiagaraFunctionLibrary.h"
#include "Game/PingPongGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/PingPongPlayer.h"

void APingPongBall::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongBall, BouncePoint);
}

APingPongBall::APingPongBall()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	SetActorTickEnabled(false);
}

void APingPongBall::BeginPlay()
{
	Super::BeginPlay();

	InitRandomBallDirection();
}

void APingPongBall::InitRandomBallDirection()
{
	float RandomYaw = UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f);
	FRotator NewRotation = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, RandomYaw);

	SetActorRotation(NewRotation);
}

void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateForwardMovement();
}

void APingPongBall::RestartBallSpeed()
{
	BallSpeed = DefaultBallSpeed;
}

void APingPongBall::UpdateForwardMovement()
{
	if (!HasAuthority())
	{
		return;
	}

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector MovementDirection = GetActorForwardVector() * BallSpeed * DeltaTime;

	FHitResult OutSweepHitResult;
	AddActorWorldOffset(MovementDirection, true, &OutSweepHitResult);
	
	HandleOnBallBounce(OutSweepHitResult);
}

void APingPongBall::HandleOnBallBounce(const FHitResult& OutSweepHitResult)
{
	if (!OutSweepHitResult.bBlockingHit)
	{
		return;
	}

	BallSpeed += 250.0f;
	
	FRotator ReflectRotation;
	
	AActor* HitActor = OutSweepHitResult.GetActor();
	if (Cast<APingPongPlayer>(HitActor))
	{
		FVector Direction = (GetActorLocation() - HitActor->GetActorLocation());
		Direction.Normalize();
		ReflectRotation = UKismetMathLibrary::Conv_VectorToRotator(Direction);
	} else
	{
		FVector Direction = GetActorForwardVector();
		FVector SurfaceNormal = OutSweepHitResult.ImpactNormal;
		FVector ReflectionVector = UKismetMathLibrary::GetReflectionVector(Direction, SurfaceNormal);
		ReflectRotation = UKismetMathLibrary::Conv_VectorToRotator(ReflectionVector);
	}
	
	ReflectRotation.Roll = 0.0f;
	ReflectRotation.Pitch = 0.0f;

	SetActorRotation(ReflectRotation);
	
	if (NiagaraSystemForBounce)
	{
		FRotator RotFromSurface = UKismetMathLibrary::Conv_VectorToRotator(OutSweepHitResult.ImpactNormal);
		BouncePoint = FTransform(RotFromSurface, OutSweepHitResult.ImpactPoint, FVector(1.0f, 1.0f, 1.0f));
	}
}

void APingPongBall::OnRep_BouncePoint()
{
	if (NiagaraSystemForBounce)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraSystemForBounce,
			BouncePoint.GetLocation(),
			BouncePoint.Rotator(),
			FVector(1.f),
			true,
			true,
			ENCPoolMethod::ManualRelease,
			true
		);
	}
}
