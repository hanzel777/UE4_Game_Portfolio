// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemData.h"
#include "PlayerData.h"
#include "RPGPlayer.generated.h"

UCLASS()
class RPGPROJECT_API ARPGPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GainItem(const FItemData data);

	void UsedItem(const FString name);

	void EquipMentItem(FItemData& data, const int32 slotNum);

	void ArtEquipmentItem(FItemData& data, const int32 slotNum);

	void ReleaseItem(FItemData& data, const int32 slotNum);

	void SetSK_Weapone(USkeletalMesh* mesh, const FVector location, const FRotator rotation, const FVector scale);

	void ReleasWeapone();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MpRecovery(const float deltaTime);
	UFUNCTION(BlueprintCallable)
	void GainBerserkerGauge(const float energy);

	UFUNCTION(BlueprintPure)
	float GetAttackSpeed() const;
	UFUNCTION(BlueprintPure)
	float GetJumpDistance() const;
	UFUNCTION(BlueprintPure)
	float GetJumpSpeed() const;
	UFUNCTION(BlueprintPure)
	float GetRunSpeed() const;
	UFUNCTION(BlueprintPure)
	float GetMaxHp() const;
	UFUNCTION(BlueprintPure)
	float GetMaxMp() const;
	UFUNCTION(BlueprintPure)
	float GetCurrentHp() const;
	UFUNCTION(BlueprintPure)
	float GetCurrentMp() const;
	UFUNCTION(BlueprintPure)
	float GetStrength() const;
	UFUNCTION(BlueprintPure)
	float GetDefence() const;
	UFUNCTION(BlueprintPure)
	bool IsBerserkerMode() const;

	UFUNCTION(BlueprintCallable)
	void SetAttackSpeed(const float AttackSpeed);
	UFUNCTION(BlueprintCallable)
	void SetJumpDistance(const float JumpDistance);
	UFUNCTION(BlueprintCallable)
	void SetJumpSpeed(const float JumpSpeed);
	UFUNCTION(BlueprintCallable)
	void SetRunSpeed(const float RunSpeed);
	UFUNCTION(BlueprintCallable)
	void SetMaxHp(const float MaxHp);
	UFUNCTION(BlueprintCallable)
	void SetMaxMp(const float MaxMp);
	UFUNCTION(BlueprintCallable)
	void SetCurrentHp(const float CurrentHp);
	UFUNCTION(BlueprintCallable)
	void SetCurrentMp(const float CurrentMp);
	UFUNCTION(BlueprintCallable)
	void SetStrength(const float Strength);
	UFUNCTION(BlueprintCallable)
	void SetDefence(const float Defence);

	UFUNCTION(BlueprintCallable)
	void AddStrength(const float amount);
	UFUNCTION(BlueprintCallable)
	void SubStrength(const float amount);

	UFUNCTION(BlueprintPure)
	bool IsWeaponeEquip() const;
	UFUNCTION()
	void PunchAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void NormalAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SkillAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SpinAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnDamage(const float damage);

	class UMainUI* GetMainUI() const;
	class UGameOverUI* GetGameOverUI() const { return gameOverUI; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = UI)
	TSubclassOf<class UMainUI> mainWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = UI)
	TSubclassOf<class UGameOverUI> gameOverWidget;

private:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
		class USpringArmComponent* springArm;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
		class UCameraComponent* camera;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* sk_WeaponeComp;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
		class UBoxComponent* punchCollision;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
		class UBoxComponent* weaponeCollision;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
		class UBoxComponent* skillCollision;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = Camera, meta = (AllowPrivateAccess = true))
		class USphereComponent* spinCollision;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UMainUI* mainUI;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UGameOverUI* gameOverUI;

	void OnInventory();
	void OnSkillListView();
	void OnEquipment();

	void OnArtClick();

	void InitStatus(FPlayerData* playerData);

	void DecreaceBerserkerGauge(const float deltaTime);

	void IdentityX();
	void IdentityZ();

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mpRecovertDelayTime = 1;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mpRecovertCurrentTime = 0;

	UDataTable* inventoryTable;
	UDataTable* quiclSlotTable;
	UDataTable* equipSlotTable;
	UDataTable* skillSlotTable;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float attackSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float jumpDistance;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float jumpSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float runSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float maxHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float maxMp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float strength;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float defence;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float currentHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float currentMp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float berserkStrengthMulty = 1.16f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float berserkerGauge;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	float maxBerserkerGauge = 100;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "State", meta = (AllowPrivateAccess = true))
	bool bBerserkerMode;

	bool bWeapone = false;
};
