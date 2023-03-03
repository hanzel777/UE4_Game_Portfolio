// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Item.generated.h"

UCLASS()
class RPGPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnyWhere, meta = (AllowPrivateAccess = true))
	FItemData itemData;

	UPROPERTY(EditAnyWhere, meta = (AllowPrivateAccess = true))
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnyWhere, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* meshComp;

	UDataTable* itemTable;
	UDataTable* weaponeTable;

public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DropItemData();

	UFUNCTION()
	void OnDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class USphereComponent* GetSphereComponent() const { return sphereComp; }
	class UStaticMeshComponent* GetMeshComponent() const { return meshComp; }
};