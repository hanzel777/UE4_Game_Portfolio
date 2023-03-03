// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillData.h"

SkillData::SkillData()
{
}

SkillData::~SkillData()
{
}

void FSkillData::RemoveData()
{
	skillName = "";
	Texture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Textures/Textured-Black-Tile-BY6001G-.Textured-Black-Tile-BY6001G-'"));
	coolTime = 0;
	currentTime = 0;
	bSkillReady = true;
	skillType = ESkillType::Normal;
}