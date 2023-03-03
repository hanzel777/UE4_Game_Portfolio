// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPawn.h"
#include "SelectCharacterWidget.h"
#include "Components/WidgetComponent.h"
#include <Components\SkeletalMeshComponent.h>
#include <SelectAnimInstance.h>

// Sets default values
ASelectPawn::ASelectPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(skeletalMesh);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	widgetComp->SetupAttachment(skeletalMesh);
	widgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComp->SetRelativeLocationAndRotation(FVector(0, 0, 80), FRotator(0, 90, 0));
	widgetComp->SetDrawAtDesiredSize(true);

	ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_SelectCharacterSlot.WBP_SelectCharacterSlot_C'"));
	if (widget.Succeeded())
	{
		widgetComp->SetWidgetClass(widget.Class);
	}
}

// Called when the game starts or when spawned
void ASelectPawn::BeginPlay()
{
	Super::BeginPlay();

	USelectCharacterWidget* selectWidget = Cast<USelectCharacterWidget>(widgetComp->GetWidget());
	if (selectWidget)
	{
		selectWidget->SetSlotNum(pawnNum);
		UE_LOG(LogTemp, Warning, TEXT("slotNum %d"), selectWidget->GetSlotNum());
	}
}

// Called every frame
void ASelectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASelectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int32 ASelectPawn::GetPawnNum() const
{
	return pawnNum;
}

void ASelectPawn::OnSelected()
{
	USelectAnimInstance* selectAnim = Cast<USelectAnimInstance>(skeletalMesh->GetAnimInstance());
	if (selectAnim)
	{
		selectAnim->PlaySelectMontage();
	}
}

void ASelectPawn::SetMeshAndAnim(USkeletalMesh* mesh, UClass* anim, FString nickName, FString className, int32 level)
{
	skeletalMesh->SetSkeletalMesh(mesh);
	skeletalMesh->SetAnimInstanceClass(anim);

	USelectCharacterWidget* selectCharacter = Cast<USelectCharacterWidget>(widgetComp->GetWidget());
	if (selectCharacter)
	{
		selectCharacter->SetNickName(nickName);
		selectCharacter->SetCharacterClass(className);
		selectCharacter->SetLevel(level);
	}
}

void ASelectPawn::SetSlotColor()
{
	USelectCharacterWidget* selectCharacter = Cast<USelectCharacterWidget>(widgetComp->GetWidget());
	if (selectCharacter)
	{
		selectCharacter->SetColor();
	}
}