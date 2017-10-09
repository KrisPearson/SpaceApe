// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEAPE_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

	void FillPool(TSubclassOf<AActor> _ActorToSpawn, int _Count);


	AActor* GetReusableReference();

	void ReturnReusableReference(AActor* _Ref);

	

	TArray<AActor*>* GetArrayPointer() { return &PooledObjects; }

	//TSubclassOf<AActor> GetPooledObjectsType();
	//void AssignDelegateToPooledObjects(void(*PoolObjectFunc)(), void(*OtherClassFunc)());

protected:
	TArray<AActor*> PooledObjects;

private:



	TSubclassOf<AActor> PooledObjectsType;


	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnActorForPool(TSubclassOf<AActor> _ActorToSpawn);
	void SpawnActorForPool_Implementation(TSubclassOf<AActor> _ActorToSpawn);
	bool SpawnActorForPool_Validate(TSubclassOf<AActor> _ActorToSpawn);
};
