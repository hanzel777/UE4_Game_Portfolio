// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NextLevelPortal.generated.h"

UCLASS()
class RPGPROJECT_API ANextLevelPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANextLevelPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UCapsuleComponent* GetCapsuleComponent() const { return capsuleComp; }
	class UParticleSystemComponent* GetParticleComponent() const { return particleComp; }

private:
	UPROPERTY(EditAnyWhere)
	class UCapsuleComponent* capsuleComp;
	UPROPERTY(EditAnyWhere)
	class UParticleSystemComponent* particleComp;

	UFUNCTION()
	void OnNextLevel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
