// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayer.h"
#include "MainUI.h"
#include "GameOverUI.h"
#include "RPGGameInstance.h"
#include "PlayerAnim.h"
#include "RPGPlayerController.h"
#include "EnemyBaseCharacter.h"
#include "Enemy_Frost_Giant.h"
#include <GameFramework\CharacterMovementComponent.h>
#include <GameFramework\SpringArmComponent.h>
#include <Camera\CameraComponent.h>
#include <Components\CapsuleComponent.h>
#include <Components\BoxComponent.h>
#include <Components\SphereComponent.h>
#include <Kismet\GameplayStatics.h>

// Sets default values
ARPGPlayer::ARPGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(85);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	//캐릭터 스켈레탈 매쉬 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Test/FemailMesh.FemailMesh'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -85.0f), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(1));
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	sk_WeaponeComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Weapone Component"));
	sk_WeaponeComp->SetupAttachment(GetMesh(), FName(TEXT("hand_rSocket")));

	//캐릭터 공격 콜라이더 설정
	punchCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Punch Collision"));
	punchCollision->SetupAttachment(GetRootComponent());
	punchCollision->SetRelativeLocation(FVector(80, 0 ,20));
	punchCollision->SetBoxExtent(FVector(50));

	punchCollision->SetGenerateOverlapEvents(true);
	punchCollision->SetCollisionProfileName(TEXT("NoCollision"));
	punchCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;

	weaponeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapone Collision"));
	weaponeCollision->SetupAttachment(GetRootComponent());
	weaponeCollision->SetRelativeLocation(FVector(120, 0, 20));
	weaponeCollision->SetBoxExtent(FVector(100, 50, 50));

	weaponeCollision->SetGenerateOverlapEvents(true);
	weaponeCollision->SetCollisionProfileName(TEXT("NoCollision"));
	weaponeCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;

	skillCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Collision"));
	skillCollision->SetupAttachment(GetRootComponent());
	skillCollision->SetRelativeLocation(FVector(170, 0, 25));
	skillCollision->SetBoxExtent(FVector(150, 32, 32));

	skillCollision->SetGenerateOverlapEvents(true);
	skillCollision->SetCollisionProfileName(TEXT("NoCollision"));
	skillCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;


	spinCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Spin Collision"));
	spinCollision->SetupAttachment(GetRootComponent());
	spinCollision->SetSphereRadius(350);

	spinCollision->SetGenerateOverlapEvents(true);
	spinCollision->SetCollisionProfileName(TEXT("NoCollision"));
	spinCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;

	//스프링암 컴포넌트 설정
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 1600;
	springArm->SetUsingAbsoluteRotation(true);
	springArm->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	springArm->bUsePawnControlRotation = false;
	springArm->bDoCollisionTest = false;

	//카메라 컴포넌트 설정
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
	camera->bUsePawnControlRotation = false;

	//캐릭터 회전 설정
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//캐릭터 무브먼드 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 640.0f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	ConstructorHelpers::FClassFinder<UMainUI> tempUI(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_MainUI.WBP_MainUI_C'"));
	if (tempUI.Succeeded())
	{
		mainWidget = tempUI.Class;
	}

	ConstructorHelpers::FClassFinder<UGameOverUI> tempGameOverUI(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_GaeOverUI.WBP_GaeOverUI_C'"));
	if (tempGameOverUI.Succeeded())
	{
		gameOverWidget = tempGameOverUI.Class;
	}
}

// Called when the game starts or when spawned
void ARPGPlayer::BeginPlay()
{
	Super::BeginPlay();

	//메인UI 클래스를 로드하는데 성공하면 UI출력
	if (mainWidget)
	{
		auto mainUserWidget = CreateWidget(GetWorld(), mainWidget);
		mainUI = Cast<UMainUI>(mainUserWidget);
		if (mainUI)
		{
			mainUI->AddToViewport();
		}
	}

	if (gameOverWidget)
	{
		gameOverUI = CreateWidget<UGameOverUI>(GetWorld(), gameOverWidget);
	}

	 URPGGameInstance* instance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(GetController()));
	 if (instance)
	 {
		 GetMesh()->SetSkeletalMesh(instance->currentPlayerData->classData.mesh);
		 GetMesh()->SetAnimInstanceClass(instance->currentPlayerData->classData.basicAnim);
		 UPlayerAnim* anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		 anim->Init();

		 inventoryTable = instance->currentPlayerData->inventoryTable;
		 quiclSlotTable = instance->currentPlayerData->quickSlotTable;
		 equipSlotTable = instance->currentPlayerData->equipSlotTable;
		 skillSlotTable = instance->currentPlayerData->skillSlotTable;

		 InitStatus(instance->currentPlayerData);
	 }
	
	//아이템 테이블을 로드하는데 성공하면 아이템 테이블의 정보를 받아서 인벤토리에 전달
	if (inventoryTable)	mainUI->GetInventory()->InitInventory(inventoryTable);
	if (quiclSlotTable) mainUI->GetQuickSlot()->InitQuickSlot(quiclSlotTable);
	if (equipSlotTable)	mainUI->GetEquipment()->InitEquipment(equipSlotTable);
	if (skillSlotTable)	mainUI->GetSkillSlot()->InitSkillSlot(skillSlotTable);

	punchCollision->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayer::PunchAttack);
	weaponeCollision->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayer::NormalAttack);
	skillCollision->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayer::SkillAttack);
	spinCollision->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayer::SpinAttack);
}

void ARPGPlayer::PunchAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float damage = strength;

	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(OtherActor);
	if (enemy)
	{
		enemy->OnDamage(damage, this);
	}

	AEnemy_Frost_Giant* frostGiant = Cast<AEnemy_Frost_Giant>(OtherActor);
	if (frostGiant)
	{
		frostGiant->OnDamage(damage);
	}
}

void ARPGPlayer::NormalAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float damage = strength;

	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(OtherActor);
	if (enemy)
	{
		enemy->OnDamage(damage, this);
		GainBerserkerGauge(1);
	}

	AEnemy_Frost_Giant* frostGiant = Cast<AEnemy_Frost_Giant>(OtherActor);
	if (frostGiant)
	{
		frostGiant->OnDamage(damage);
		GainBerserkerGauge(1);
	}
}

void ARPGPlayer::SkillAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float damage = strength * 2;

	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(OtherActor);
	if (enemy)
	{
		enemy->OnDamage(damage, this);
		GainBerserkerGauge(5);
	}

	AEnemy_Frost_Giant* frostGiant = Cast<AEnemy_Frost_Giant>(OtherActor);
	if (frostGiant)
	{
		frostGiant->OnDamage(damage);
		GainBerserkerGauge(5);
	}
}

void ARPGPlayer::SpinAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float damage = strength * 1.5f;

	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(OtherActor);
	if (enemy)
	{
		enemy->OnDamage(damage, this);
		GainBerserkerGauge(5);
	}

	AEnemy_Frost_Giant* frostGiant = Cast<AEnemy_Frost_Giant>(OtherActor);
	if (frostGiant)
	{
		frostGiant->OnDamage(damage);
		GainBerserkerGauge(5);
	}
}

void ARPGPlayer::OnDamage(const float damage)
{
	if (currentHp <= 0) return;

	(damage <= defence) ? currentHp -= 1 : currentHp -= damage - defence;

	if (currentHp <= 0)
	{
		currentHp = 0;
		if (gameOverUI)
		{
			UPlayerAnim* anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
			if (anim)
			{
				anim->OnDie();
			}
		}
	}
	else
	{

	}
}

void ARPGPlayer::InitStatus(FPlayerData* playerData)
{
	IdentityX();
	attackSpeed = 1;
	jumpDistance = playerData->classData.JumpDistance;
	jumpSpeed = playerData->classData.JumpSpeed;
	runSpeed = playerData->classData.RunSpeed;
	strength = playerData->classData.Strength;
	defence = playerData->classData.Defence;
	maxHp = playerData->classData.MaxHp;
	maxMp = playerData->classData.MaxMp;
	currentHp = playerData->currentHp;
	currentMp = playerData->currentMp;
	maxBerserkerGauge = 100;

	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void ARPGPlayer::MpRecovery_Implementation(const float deltaTime)
{
	if (currentMp >= maxMp)
	{
		mpRecovertCurrentTime = 0;
		return;
	}

	mpRecovertCurrentTime += deltaTime;
	if (mpRecovertCurrentTime >= mpRecovertDelayTime)
	{
		currentMp += maxMp / 10.0f;
		if (currentMp > maxMp)
		{
			currentMp = maxMp;
		}
		mpRecovertCurrentTime -= mpRecovertDelayTime;
	}
}

void ARPGPlayer::DecreaceBerserkerGauge(const float deltaTime)
{
	if (bBerserkerMode)
	{
		berserkerGauge -= maxBerserkerGauge / 60 * deltaTime;

		if (berserkerGauge <= 0)
		{
			berserkerGauge = 0;
			IdentityX();
		}
	}
}

// Called every frame
void ARPGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MpRecovery(DeltaTime);
	DecreaceBerserkerGauge(DeltaTime);;
}

// Called to bind functionality to input
void ARPGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ARPGPlayer::OnInventory);
	PlayerInputComponent->BindAction(TEXT("SkillList"), IE_Pressed, this, &ARPGPlayer::OnSkillListView);
	PlayerInputComponent->BindAction(TEXT("Equipment"), IE_Pressed, this, &ARPGPlayer::OnEquipment);

	PlayerInputComponent->BindAction(TEXT("IdentityX"), IE_Pressed, this, &ARPGPlayer::IdentityX);
	PlayerInputComponent->BindAction(TEXT("IdentityZ"), IE_Pressed, this, &ARPGPlayer::IdentityZ);


	PlayerInputComponent->BindKey(EKeys::LeftAlt, IE_Pressed, this, &ARPGPlayer::OnArtClick);
	PlayerInputComponent->BindKey(EKeys::LeftAlt, IE_Released, this, &ARPGPlayer::OnArtClick);
}

void ARPGPlayer::OnInventory()
{
	if (mainUI->GetInventory()->IsVisible())
	{
		mainUI->GetInventory()->SetVisibility(ESlateVisibility::Hidden);
	}
	else 
	{
		mainUI->GetInventory()->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPGPlayer::OnSkillListView()
{
	if (mainUI->GetSkillListView()->IsVisible())
	{
		mainUI->GetSkillListView()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		mainUI->GetSkillListView()->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPGPlayer::OnEquipment()
{
	if (mainUI->GetEquipment()->IsVisible())
	{
		mainUI->GetEquipment()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		mainUI->GetEquipment()->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPGPlayer::IdentityX()
{
	UPlayerAnim* anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	if (anim && bBerserkerMode)
	{
		bBerserkerMode = false;
		anim->OnBerserkEnd();

		attackSpeed -= 1;
		strength /= berserkStrengthMulty;
		mainUI->GetEquipment()->SetStrength(strength);
	}
}

void ARPGPlayer::IdentityZ()
{
	UPlayerAnim* anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(GetController());
	if (anim && rpgController && !rpgController->GetAttack() && rpgController->GetControl() && !rpgController->GetRush() && !bBerserkerMode && berserkerGauge > 0)
	{
		bBerserkerMode = true;
		anim->OnBerserk();

		attackSpeed += 1;
		strength *= berserkStrengthMulty;
		mainUI->GetEquipment()->SetStrength(strength);
	}
}

void ARPGPlayer::OnArtClick()
{
	UE_LOG(LogTemp, Warning, TEXT("ARPGPlayer::OnArtClick"));

	for (auto slot : mainUI->GetInventory()->GetSlots())
	{
		if (slot->bArt)
		{
			slot->bArt = false;
		}
		else
		{
			slot->bArt = true;
		}
	}
}

void ARPGPlayer::GainItem(const FItemData data)
{
	mainUI->GainItem(data);
}

void ARPGPlayer::UsedItem(const FString name)
{
	mainUI->UsedItem(name);
}

void ARPGPlayer::EquipMentItem(FItemData& data, const int32 slotNum)
{
	UE_LOG(LogTemp, Warning, TEXT("ARPGPlayer::EquipMentItem"));
	switch (data.itemType)
	{
		case EItemType::Access_Ear: case EItemType::Access_Neck: case EItemType::Access_Ring:
		case EItemType::Equip_Body: case EItemType::Equip_Hand: case EItemType::Equip_Head: case EItemType::Equip_Leg: case EItemType::Equip_Shoulder: case EItemType::Weapone:
			mainUI->GetEquipment()->SetSlot(data, slotNum);
			break;
	}
}

void ARPGPlayer::ArtEquipmentItem(FItemData& data, const int32 slotNum)
{
	UE_LOG(LogTemp, Warning, TEXT("ARPGPlayer::ArtEquipmentItem"));
	mainUI->GetEquipment()->ArtSetSlot(data, slotNum);
}

void ARPGPlayer::ReleaseItem(FItemData& data, const int32 slotNum)
{
	for (auto slot : mainUI->GetInventory()->GetSlots())
	{
		if (slot->GetItemData().itemName == "")
		{
			mainUI->GetInventory()->InsertItem(data, slot->GetSlotNum());
			mainUI->GetEquipment()->RemoveItem(slotNum);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ARPGPlayer::ReleaseItem : Inventory Full"));
}

void ARPGPlayer::SetSK_Weapone(USkeletalMesh* mesh, const FVector location, const FRotator rotation, const FVector scale)
{
	UE_LOG(LogTemp, Warning, TEXT("SetSK_Weapone"));

	sk_WeaponeComp->SetSkeletalMesh(mesh);
	sk_WeaponeComp->SetRelativeLocationAndRotation(location, rotation);
	sk_WeaponeComp->SetRelativeScale3D(scale);

	bWeapone = true;
}

void ARPGPlayer::ReleasWeapone()
{
	sk_WeaponeComp->SetSkeletalMesh(nullptr);

	bWeapone = false;
}

UMainUI* ARPGPlayer::GetMainUI() const
{
	return mainUI;
}

void ARPGPlayer::GainBerserkerGauge(const float energy)
{
	if (berserkerGauge >= maxBerserkerGauge || bBerserkerMode)return;

	berserkerGauge += energy;
	if (berserkerGauge > maxBerserkerGauge)
	{
		berserkerGauge = maxBerserkerGauge;
	}
}

float ARPGPlayer::GetAttackSpeed() const
{
	return attackSpeed;
}
float ARPGPlayer::GetJumpDistance() const
{
	return jumpDistance;
}
float ARPGPlayer::GetJumpSpeed() const
{
	return jumpSpeed;
}
float ARPGPlayer::GetRunSpeed() const
{
	return runSpeed;
}
float ARPGPlayer::GetMaxHp() const
{
	return maxHp;
}
float ARPGPlayer::GetMaxMp() const
{
	return maxMp;
}
float ARPGPlayer::GetCurrentHp() const
{
	return currentHp;
}
float ARPGPlayer::GetCurrentMp() const
{
	return currentMp;
}
float ARPGPlayer::GetStrength() const
{
	return strength;
}
float ARPGPlayer::GetDefence() const
{
	return defence;
}

void ARPGPlayer::SetAttackSpeed(const float AttackSpeed)
{
	attackSpeed = AttackSpeed;
}
void ARPGPlayer::SetJumpDistance(const float JumpDistance)
{
	jumpDistance = JumpDistance;
}
void ARPGPlayer::SetJumpSpeed(const float JumpSpeed)
{
	jumpSpeed = JumpSpeed;
}
void ARPGPlayer::SetRunSpeed(const float RunSpeed)
{
	runSpeed = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}
void ARPGPlayer::SetMaxHp(const float MaxHp)
{
	maxHp = MaxHp;
}
void ARPGPlayer::SetMaxMp(const float MaxMp)
{
	maxMp = MaxMp;
}
void ARPGPlayer::SetCurrentHp(const float CurrentHp)
{
	currentHp = CurrentHp;
}
void ARPGPlayer::SetCurrentMp(const float CurrentMp)
{
	currentMp = CurrentMp;
}
void ARPGPlayer::SetStrength(const float Strength)
{
	strength = Strength;
}
void ARPGPlayer::SetDefence(const float Defence)
{
	defence = Defence;
}

bool ARPGPlayer::IsBerserkerMode() const
{
	return bBerserkerMode;
}

bool ARPGPlayer::IsWeaponeEquip() const
{
	return bWeapone;
}

void ARPGPlayer::AddStrength(const float amount)
{
	if (bBerserkerMode)
	{
		strength += amount * berserkStrengthMulty;
	}
	else
	{
		strength += amount;
	}
}
void ARPGPlayer::SubStrength(const float amount)
{
	if (bBerserkerMode)
	{
		strength -= amount * berserkStrengthMulty;
	}
	else
	{
		strength -= amount;
	}
}