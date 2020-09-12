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
#include "Components/PlayerWeaponComponent.h"
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
	TiltComponent->SetIsReplicated(true);
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent->SetupAttachment(TiltComponent);

	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);


	//EquippedWeaponComponent = CreateDefaultSubobject<UPlayerWeaponComponent>(TEXT("WUP"));



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

	SetOwner(GetController());

	PrimaryActorTick.bCanEverTick = true;
}

void ASpaceApePlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void ASpaceApePlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	World = GetWorld();

	//UPlayerWeaponComponent* NewWeapon = Cast<UPlayerWeaponComponent>(DefaultWeaponComponent);
	ChangeWeapon(DefaultWeaponComponent);
}


void ASpaceApePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ASpaceApePlayerCharacter, ShipMeshComponent->GetComponentTransform());

	//DOREPLIFETIME(AOnlineGameCharacter, Health);

	DOREPLIFETIME(ASpaceApePlayerCharacter, CurrentScore); // Does this need replicating anymore? Now we use PlayerState
}


void ASpaceApePlayerCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.0f);


	if (EquippedWeaponComponent != nullptr) {
		if (FireDirection.SizeSquared() > 0.0f) {
			if (GetLocalRole() == ROLE_AutonomousProxy) {
				ServerFire(FireDirection);

				EquippedWeaponComponent->Shoot(FireDirection);
			}
			else if (GetLocalRole() == ROLE_Authority) {
				Fire(FireDirection);

				EquippedWeaponComponent->Shoot(FireDirection);
			}
		}
	}


	//rotate the ship mesh
	FRotator Rotation = FRotator(0, 20, 0);
	ShipMeshComponent->AddLocalRotation(Rotation * DeltaSeconds);

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

}

void ASpaceApePlayerCharacter::HandleMovement(float deltaSeconds) {
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * deltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f) {

		//UE_LOG(LogTemp, Log, TEXT("Your message %f"), Movement.SizeSquared());

		const FRotator NewRotation = FRotator(0, 0, 0);//Movement.Rotation();
		FHitResult Hit(1.f);
		//RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		AddMovementInput(MoveDirection, 1);

		float speedFraction;                           // Speed at which the mesh is moving, as a fraction of "full throttle"

		if (ForwardValue == 0.0f && RightValue == 0.0f) {
			// Guard against divide-by-zero, just in case
			MoveDirection = FVector::ZeroVector;
			speedFraction = 0.0f;

		}
		else {
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


		// Points forwards in the direction of movement, using the input axis. Rotation Yaw doesn't matter yet.
		FVector moveForwardVector = (ForwardValue * FVector::ForwardVector) + (RightValue * FVector::RightVector);
		moveForwardVector.Normalize(); // size = 1

		//Calculates a relative right vector using the cross product of the direction of movement and up vector. 
		//the ^ symbol is the FVector cross product operator, which returns a value perpendicular to the plane spanned by the input vectors
		FVector moveRightVector = moveForwardVector ^ FVector::UpVector;

		//tiltAngle will be something between 0 degrees and MaxTiltAngle, according to the size of the value read from input
		float tiltAngle = FMath::Max(FMath::Abs(ForwardValue), FMath::Abs(RightValue)) * MaxAngle;

		//Calculates the tilt rotation based on movement Right vector
		FQuat tilt = FQuat::MakeFromEuler(tiltAngle * moveRightVector);

		//Interpolate the rotation to the calculated tilt, by converting it into a rotator
		TiltComponent->SetWorldRotation(FMath::RInterpTo(TiltComponent->GetComponentRotation(), tilt.Rotator(), deltaSeconds, 10.f));

	}
}


void ASpaceApePlayerCharacter::ServerFire_Implementation(FVector _FireDirection) {
	if (bCanFire == true) {
		const FRotator FireRotation = _FireDirection.Rotation();

		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		if (World != NULL) {
			ASpaceApeProjectile* NewProjectile = World->SpawnActor<ASpaceApeProjectile>(SpawnLocation, FireRotation);

			NewProjectile->OnEnemyHit.AddDynamic(this, &ASpaceApePlayerCharacter::DealDamage);

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceApePlayerCharacter::ShotTimerExpired, FireRate);

			if (FireSound != nullptr) {
				MulticastPlayFireSound();
			}
		}
	}
}

bool ASpaceApePlayerCharacter::ServerFire_Validate(FVector _FireDirection) {
	return true;
}

void ASpaceApePlayerCharacter::Fire(FVector _FireDirection) {
	if (bCanFire == true) {
		// If we are pressing fire stick in a direction

		const FRotator FireRotation = _FireDirection.Rotation();
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);


		if (World != NULL) {
			// spawn the projectile
			ASpaceApeProjectile* NewProjectile = World->SpawnActor<ASpaceApeProjectile>(SpawnLocation, FireRotation);

			NewProjectile->OnEnemyHit.AddDynamic(this, &ASpaceApePlayerCharacter::DealDamage);

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceApePlayerCharacter::ShotTimerExpired, FireRate);

			if (FireSound != nullptr) {
				MulticastPlayFireSound();
			}
		}
	}
}

void ASpaceApePlayerCharacter::MulticastPlayFireSound_Implementation() {
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);	
		//UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

/*
This will be used primarily for weapon pickups.
Replaces the current weapon witha  new one.
Need to destroy old component and perhaps perform some kind of check.
This method should eventually be made private/ protected, and some kind of public condition check method should handle weapon changes.
*/
void ASpaceApePlayerCharacter::ChangeWeapon(TSubclassOf<UPlayerWeaponComponent> _NewWeapon) {
	//EquippedWeaponComponent = ConstructObject<UPlayerWeaponComponent>(_NewWeapon, this, *_NewWeapon->GetName()/*TEXT("InitialWeapon")*/);
	EquippedWeaponComponent = NewObject<UPlayerWeaponComponent>(this, _NewWeapon, *_NewWeapon->GetName()/*TEXT("InitialWeapon")*/);
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


	GetController()->PlayerState->Score = CurrentScore; // needs storing. Could pass this to the server to validate

	//NOTE: It isn't safe to store score on the actor, as it could potentially be cheated.

}

void ASpaceApePlayerCharacter::ShotTimerExpired() {
	bCanFire = true;
}

