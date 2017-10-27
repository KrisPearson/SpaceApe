// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectPoolComponent.h"
#include "SpaceApeProjectile.h"


// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{

}

void UObjectPoolComponent::FillPool(TSubclassOf<AActor> _ActorToSpawn, int _Count) {
	PooledObjects.Reserve(_Count); // Reserves memory for the array

	PooledObjectsType = _ActorToSpawn;

	if (UWorld* World = GetWorld()) {

		for (int i = 0; i < _Count; i++) {

			SpawnActorForPool(PooledObjectsType);
			//AActor* NewObject = World->SpawnActor< AActor >(_ActorToSpawn);
			//PooledObjects.Add(NewObject);

			//NewObject->OnEnemyHit.AddDynamic(this, &ASpaceApePlayerCharacter::DealDamage); // if needed, this could be added in an override method		
			//NewProjectile->ToggleEnabled(false);

			UE_LOG(LogTemp, Warning, TEXT(" Adding Object to Pool"));

		}
	}


}

AActor* UObjectPoolComponent::GetReusableReference() {

	if (PooledObjects.Num() <= 0) {
		/* Commented out functionality would add another object to the pool. The issue is that new objects wouldn't be bound to functions etc.
		UE_LOG(LogTemp, Warning, TEXT(" Adding new obj to pool"));
		SpawnActorForPool(PooledObjectsType);

		//if (UWorld* World = GetWorld()) { AActor* newObject = World->SpawnActor< AActor >(PooledObjectsType); }
		//PooledObjects.Add(newObject);
		//newObject->SetActorLocation(FVector(0, 200, 300));
		//return newObject;
		*/
		return nullptr;
	}
	//UE_LOG(LogTemp, Warning, TEXT(" Get object from pool %s"), *PooledObjects.Top()->GetName() );
	AActor* returnActor = PooledObjects.Top();

	PooledObjects.Pop(); 
	CurrentNumberOfPooledObjectsInUse++;

	return returnActor;
}

void UObjectPoolComponent::ReturnReusableReference(AActor * _Ref) {
	if (NumberOfObjectsToReplaceWithDuplicates <= 0) {

		UE_LOG(LogTemp, Warning, TEXT("Return object to pool %s"), *_Ref->GetName());
		_Ref->Reset();
		PooledObjects.Push(_Ref);

		CurrentNumberOfPooledObjectsInUse--;

	}
	else{
		_Ref->Destroy();
		if (PooledObjects.Num() > 0) {
			AActor* newobject = DuplicateObject(PooledObjects[0], PooledObjects[0]->GetOuter());

			CurrentNumberOfPooledObjectsInUse--;
			NumberOfObjectsToReplaceWithDuplicates--;
		}
		else {
			//delay add object
		}
	}
}

void UObjectPoolComponent::ReplaceInUseObjectsWithDuplicates() {
	int bufferValue = 10; // the buffer value is used to decrease the liklihood that objects slip through the net and are added back to the pool.
	NumberOfObjectsToReplaceWithDuplicates = CurrentNumberOfPooledObjectsInUse + bufferValue;
	UE_LOG(LogTemp, Warning, TEXT("NumberOfObjectsToReplaceWithDuplicates = %f"), NumberOfObjectsToReplaceWithDuplicates);
}



void UObjectPoolComponent::SpawnActorForPool_Implementation(TSubclassOf<AActor> _ActorToSpawn) {
	AActor* NewObject = GetWorld()->SpawnActor< AActor >(_ActorToSpawn);
	PooledObjects.Add(NewObject);
}

bool UObjectPoolComponent::SpawnActorForPool_Validate(TSubclassOf<AActor> _ActorToSpawn) {
	return true;
}

