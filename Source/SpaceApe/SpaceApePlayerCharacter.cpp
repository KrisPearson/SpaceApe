// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceApePlayerCharacter.h"
#include "SpaceApeProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Enemy.h"
#include "Sound/SoundBase.h"
#include "Engine.h"
#include "EngineUtils.h"

const FName ASpaceApePlayerCharacter::MoveForwardBinding("MoveForward");
const FName ASpaceApePlayerCharacter::MoveRightBinding("MoveRight");
const FName ASpaceApePlayerCharacter::FireForwardBinding("FireForward");
const FName ASpaceApePlayerCharacter::FireRightBinding("FireRight");




ASpaceApePlayerCharacter::ASpaceApePlayerCharacter() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));

	//RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = RootComp;

	TiltComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Tilt Component"));
	TiltComponent->SetupAttachment(RootComponent);

	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent->SetupAttachment(TiltComponent);

	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Cache our sound effect
	//static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/KKIIDDZZ_00018"));
	FireSound = FireAudio.Object;

	// Movement
	MoveSpeed = 500.0f;
	// Weapon
	GunOffset = FVector(0.f, 0.f, -30.f);

	MaxAngle = 15.f;
	FireRate = 0.12f;
	bCanFire = true;

	PrimaryActorTick.bCanEverTick = true;
}

void ASpaceApePlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	World = GetWorld();

}


void ASpaceApePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ASpaceApePlayerCharacter, ShipMeshComponent->GetComponentTransform());

	//DOREPLIFETIME(AOnlineGameCharacter, Health);

	DOREPLIFETIME(ASpaceApePlayerCharacter, CurrentScore);
}

void ASpaceApePlayerCharacter::ServerFire_Implementation(FVector _FireDirection) {
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (_FireDirection.SizeSquared() > 0.0f)
		{

			const FRotator FireRotation = _FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);


			if (World != NULL) {
				// spawn the projectile
				ASpaceApeProjectile* NewProjectile = World->SpawnActor<ASpaceApeProjectile>(SpawnLocation, FireRotation);

				NewProjectile->OnEnemyHit.AddDynamic(this, &ASpaceApePlayerCharacter::DealDamage);

				bCanFire = false;
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceApePlayerCharacter::ShotTimerExpired, FireRate);

				// try and play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}
			}

			bCanFire = false;
		}
	}
}

bool ASpaceApePlayerCharacter::ServerFire_Validate(FVector _FireDirection) {
	return true;
}

void ASpaceApePlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void ASpaceApePlayerCharacter::HandleMovement(float deltaSeconds) {
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);


	//FVector movementDirection;                     // Direction the mesh is moving in

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * deltaSeconds;



	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{

		//UE_LOG(LogTemp, Log, TEXT("Your message %f"), Movement.SizeSquared());

		const FRotator NewRotation = FRotator(0, 0, 0);//Movement.Rotation();
		FHitResult Hit(1.f);
		//RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		AddMovementInput(MoveDirection, 1);

		float speedFraction;                           // Speed at which the mesh is moving, as a fraction of "full throttle"
		//UE_LOG(LogTemp, Log, TEXT("Valuee is %f"), MoveDirection.X);
		if (ForwardValue == 0.0f && RightValue == 0.0f)
		{
			// Guard against divide-by-zero, just in case
			MoveDirection = FVector::ZeroVector;
			speedFraction = 0.0f;

		}
		else
		{
			// The movement direction we can get simply by looking at the normal vector in the same direction as the input
			FVector rawMovementVector(ForwardValue, RightValue, 0.0f);
			MoveDirection = rawMovementVector;
			MoveDirection.Normalize();

			// The speed can be determined by looking at the ratio between the actual input and largest conceiveable input in this direction.
			// The largest input in this direction will be when one of the axis is +/-1.  Thus, by taking the axis that is farther from 0 and
			// scaling the movement direction by 1 / (axis value), we can get the largest possible input in this direction.
			FVector maxInput = MoveDirection;
			float largerAxis = FMath::Max(FMath::Abs(maxInput.X), FMath::Abs(maxInput.Y));
			maxInput /= largerAxis;

			speedFraction = rawMovementVector.Size() / maxInput.Size();
		}



		//FVector tiltVector = FVector::CrossProduct( ( MoveDirection.GetSafeNormal() ), GetActorUpVector());

		// Use the speed to determine the proper tilt
		FRotator tilt(MaxAngle * speedFraction, 0, 0.0f); // < perhaps this needs attention?
		tilt = tilt * -1;

		//UE_LOG(LogTemp, Log, TEXT("Yaw = %f"), tilt.Yaw);
		//UE_LOG(LogTemp, Log, TEXT("Pitch =  %f"), tilt.Pitch);
		//UE_LOG(LogTemp, Log, TEXT("Roll =  %f"), tilt.Roll);


		// Use the direction of travel to determine the direction of tilt
		FQuat rotation = FQuat::FindBetween(FVector(1, 0, 0), MoveDirection);

		// Use the tilt and rotation to determine the actual rotation we need to apply
		FQuat tiltRotation = rotation * tilt.Quaternion();


		//FQuat tiltRotation = MoveDirection.ToOrientationQuat() * tilt.Quaternion();


		// Apply this rotation to our mesh
		FRotator previousAngle;
		// ...



		//this->SetActorRotation(tiltRotation * previousAngle.Quaternion());

		//ShipMeshComponent->SetWorldRotation(tiltRotation * previousAngle.Quaternion());

		TiltComponent->SetWorldRotation(tiltRotation);





		//calculating Pitch 
		float fPitch = MaxAngle * ((ForwardValue * GetActorForwardVector().X * -1)
			+ (RightValue * GetActorRightVector().X));

		//calculating Roll
		float fRoll = MaxAngle * ((RightValue * GetActorForwardVector().X)
			+ (ForwardValue * GetActorRightVector().X));

		FRotator dumbTilt = FRotator(fPitch, fRoll, 0);

	}
}


void ASpaceApePlayerCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.0f);

	ServerFire(FireDirection);

	// Try and fire a shot
	//FireShot(FireDirection);

	


	HandleMovement(DeltaSeconds);
	/*
	FString CameraName = "MainGameCameraActor";

	//               =================================
	if (GetWorld()) {
		for (TActorIterator<AActor>ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Access the subclass instance with the * or -> operators.
			if (ActorItr->GetName().Contains(CameraName)) {
				FViewTargetTransitionParams Params;
				APlayerController* Controller = Cast<APlayerController>(GetController());
				if (Controller != nullptr) {
					Controller->SetViewTarget(*ActorItr, Params);
					UE_LOG(LogTemp, Warning, TEXT("SetViewTarget"));
				}
			}
		}
	}
	//          ===============================================
	*/

	//rotate the mesh
	FRotator Rotation = FRotator(0, 20, 0);
	ShipMeshComponent->AddLocalRotation(Rotation * DeltaSeconds);

	/*
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;


	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{

	UE_LOG(LogTemp, Log, TEXT("Your message %f"), Movement.SizeSquared());

	const FRotator NewRotation = FRotator(0, 0, 0);//Movement.Rotation();
	FHitResult Hit(1.f);
	RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
	const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
	const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
	RootComponent->MoveComponent(Deflection, NewRotation, true);

	//RootComponent->SetWorldLocation(Deflection, true); <=== colliding with shots?
	}
	}


	#pragma Zone https://answers.unrealengine.com/questions/399482/math-help-pitch-and-roll-a-character-in-the-contro.html

	// Determine the speed and direction of travel
	FVector movementDirection;                     // Direction the mesh is moving in
	float speedFraction;                           // Speed at which the mesh is moving, as a fraction of "full throttle"

	if (ForwardValue == 0.0f && RightValue == 0.0f)
	{
	// Guard against divide-by-zero, just in case
	movementDirection = FVector::ZeroVector;
	speedFraction = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("Values are 0"));
	}
	else
	{
	// The movement direction we can get simply by looking at the normal vector in the same direction as the input
	FVector rawMovementVector(ForwardValue, RightValue, 0.0f);
	movementDirection = rawMovementVector;
	movementDirection.Normalize();

	// The speed can be determined by looking at the ratio between the actual input and largest conceiveable input in this direction.
	// The largest input in this direction will be when one of the axis is +/-1.  Thus, by taking the axis that is farther from 0 and
	// scaling the movement direction by 1 / (axis value), we can get the largest possible input in this direction.
	FVector maxInput = movementDirection;
	float largerAxis = FMath::Max(FMath::Abs(maxInput.X), FMath::Abs(maxInput.Y));
	maxInput /= largerAxis;

	speedFraction = rawMovementVector.Size() / maxInput.Size();
	}

	// Use the speed to determine the proper tilt
	FRotator tilt(MaxAngle * speedFraction, 0.0f, 0.0f);

	// Use the direction of travel to determine the direction of tilt
	FQuat rotation = FQuat::FindBetween(FVector(0, 1, 0), movementDirection);

	// Use the tilt and rotation to determine the actual rotation we need to apply
	FQuat tiltRotation = rotation * tilt.Quaternion();

	// Apply this rotation to our mesh
	FRotator previousAngle;
	// ...

	//this->SetActorRotation(tiltRotation * previousAngle.Quaternion());

	ShipMeshComponent->SetWorldRotation(tiltRotation * previousAngle.Quaternion());


	#pragma endZone

	*/





}

void ASpaceApePlayerCharacter::FireShot(FVector FireDirection) { // DEPRECATED


	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			if (World != NULL)
			{
				// spawn the projectile
				ASpaceApeProjectile* NewProjectile = World->SpawnActor<ASpaceApeProjectile>(SpawnLocation, FireRotation);

				NewProjectile->OnEnemyHit.AddDynamic(this, &ASpaceApePlayerCharacter::DealDamage);

				if (NewProjectile->OnEnemyHit.IsBound()) {
					UE_LOG(LogTemp, Log, TEXT("Is bound"));
				}
				else UE_LOG(LogTemp, Log, TEXT("Not bound"));

				bCanFire = false;
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceApePlayerCharacter::ShotTimerExpired, FireRate);

				// try and play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}

				bCanFire = false;
			}
		}
	}
}

void ASpaceApePlayerCharacter::DealDamage(AActor* _Enemy) {

	if (AEnemy* Enemy = Cast<AEnemy>(_Enemy)) {

		const int ScoreValue = Enemy->GetScoreValue();
			
			// Inform the enemy of damage and check whether the enemy died as a result.
		if (Enemy->ReceiveDamage(PlayerProjectileDamage)) {
			CurrentScore += ScoreValue;
		}
		else CurrentScore += 1; 
	}

}

void ASpaceApePlayerCharacter::ShotTimerExpired() {
	bCanFire = true;
}

