// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "RPGPlayer.h"
#include "RPGGameInstance.h"
#include "NextLevelPortal.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Components\SphereComponent.h>
#include <Kismet\GameplayStatics.h>

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(150);
	sphereComp->SetCollisionProfileName(TEXT("Item"));
	sphereComp->SetMobility(EComponentMobility::Static);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(sphereComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/AncientTreasures/Meshes/SM_Chest_02c.SM_Chest_02c'"));
	if (mesh.Succeeded())
	{
		meshComp->SetStaticMesh(mesh.Object);
		meshComp->SetRelativeLocation(FVector(-50, -100, 0));
		meshComp->SetRelativeScale3D(FVector(2));
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		meshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
	}

	ConstructorHelpers::FObjectFinder<UDataTable> item(TEXT("DataTable'/Game/DataTables/ItemTable/ItemTable.ItemTable'"));
	if (item.Succeeded())
	{
		itemTable = item.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> weap(TEXT("DataTable'/Game/DataTables/ItemTable/WeaponeTable.WeaponeTable'"));
	if (weap.Succeeded())
	{
		weaponeTable = weap.Object;
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnDetected);

	sphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetVisibility(false);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::DropItemData()
{
	//아이템 타입 정하기
	bool isWeapone = false;;
	float randf = FMath::FRandRange(0, 100);
	if (randf < 70) isWeapone = false;
	else isWeapone = true;

	//정해진 아이템 타입에 맞는 테이블을 로드한 후 랜덤으로 아이템 데이터 정하기
	//무기 타입이라면 플레이어와 맞는 무기 타입의 무기만 가능
	if (itemTable && weaponeTable)
	{
		if (isWeapone)
		{
			TArray<FWeaponeData*> weaponeDatas;
			weaponeTable->GetAllRows<FWeaponeData>("", weaponeDatas);

			TArray<FWeaponeData*> classWeaponeDatas;

			URPGGameInstance* rpgGameInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (rpgGameInstance)
			{
				switch (rpgGameInstance->currentPlayerData->classData.classType)
				{
				case EClassType::Warrior:
					switch (rpgGameInstance->currentPlayerData->classData.warrior)
					{
					case EWarriorClassType::Berserker:
						for (auto weap : weaponeDatas)
						{
							if (weap->warrior == EWarriorClassType::Berserker)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					case EWarriorClassType::Axeman:
						for (auto weap : weaponeDatas)
						{
							if (weap->warrior == EWarriorClassType::Axeman)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					case EWarriorClassType::Knight:
						for (auto weap : weaponeDatas)
						{
							if (weap->warrior == EWarriorClassType::Knight)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					case EWarriorClassType::Cyborg:
						for (auto weap : weaponeDatas)
						{
							if (weap->warrior == EWarriorClassType::Cyborg)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					}
					break;
				case EClassType::Wizard:
					switch (rpgGameInstance->currentPlayerData->classData.wizard)
					{
					case EWizardClassType::Sorceress:
						for (auto weap : weaponeDatas)
						{
							if (weap->wizard == EWizardClassType::Sorceress)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					case EWizardClassType::Summoner:
						for (auto weap : weaponeDatas)
						{
							if (weap->wizard == EWizardClassType::Summoner)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					}
					break;
				case EClassType::Archer:
					switch (rpgGameInstance->currentPlayerData->classData.archer)
					{
					case EArcherClassType::Bowman:
						for (auto weap : weaponeDatas)
						{
							if (weap->archer == EArcherClassType::Bowman)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					case EArcherClassType::Rifleman:
						for (auto weap : weaponeDatas)
						{
							if (weap->archer == EArcherClassType::Rifleman)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					}
					break;
				case EClassType::Fighter:
					switch (rpgGameInstance->currentPlayerData->classData.fighter)
					{
					case EFighterClassType::Boxer:
						for (auto weap : weaponeDatas)
						{
							if (weap->fighter == EFighterClassType::Boxer)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					case EFighterClassType::StreetFighter:
						for (auto weap : weaponeDatas)
						{
							if (weap->fighter == EFighterClassType::StreetFighter)
							{
								classWeaponeDatas.Add(weap);
							}
						}
						break;
					}
					break;
				}
			}

			int32 index = FMath::RandRange(0, classWeaponeDatas.Num()-1);

			if (index >= classWeaponeDatas.Num())
			{
				UE_LOG(LogTemp, Warning, TEXT("itemp.cpp Line: %d index >= classWeaponeDatas.Num() / index == %d"), __LINE__, index);
				index = classWeaponeDatas.Num() - 1;
			}

			itemData.bReady = true;
			itemData.currentTime = 0;
			itemData.delayTime = 0;

			itemData.itemCount = 1;
			itemData.itemName = classWeaponeDatas[index]->weaponeName;
			itemData.itemType = EItemType::Weapone;
			itemData.tex = classWeaponeDatas[index]->itemTex;
			itemData.addStrength = classWeaponeDatas[index]->strength;
		}
		else
		{
			TArray<FName> itemNames;
			itemNames = itemTable->GetRowNames();

			int32 index = FMath::RandRange(0, itemNames.Num()-1);

			if (index >= itemNames.Num())
			{
				UE_LOG(LogTemp, Warning, TEXT("itemp.cpp Line: %d index >= itemNames.Num() / index == %d"), __LINE__, index);
				index = itemNames.Num() - 1;
			}
			itemData = *itemTable->FindRow<FItemData>(itemNames[index], "");
		}
		
	}
}

void AItem::OnDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Detected"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());

	ARPGPlayer* player = Cast<ARPGPlayer>(OtherActor);
	if (player)
	{
		int cnt = FMath::RandRange(5, 8);

		for (int i = 0; i < cnt; i++)
		{
			DropItemData();
			player->GainItem(itemData);
		}

		URPGGameInstance* rpgGameInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(player->GetController()));
		if (rpgGameInstance)
		{
			rpgGameInstance->currentPlayerData->level++;
			rpgGameInstance->UpdatedCuurentPlayerDataInUserTable();
		}

	}

	ANextLevelPortal* portal = Cast<ANextLevelPortal>(UGameplayStatics::GetActorOfClass(GetWorld(), ANextLevelPortal::StaticClass()));
	if (portal)
	{
		portal->GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapAll"));
		portal->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
		portal->GetParticleComponent()->SetVisibility(true);
	}
	Destroy();
}

