// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorFiles/EnhancedAssetManager.h"

#include "AbilitySystemGlobals.h"

#include "EditorFiles/EnhancedGameplayTags.h"


UEnhancedAssetManager::UEnhancedAssetManager()
{
}

UEnhancedAssetManager& UEnhancedAssetManager::Get()
{
	check(GEngine);
	
	if (UEnhancedAssetManager* Singleton = Cast<UEnhancedAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager ClassName in DefaultEngine.ini. it must be set to EnhancedAssetManager"))
	
	return *NewObject<UEnhancedAssetManager>();
}

void UEnhancedAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FEnhancedGameplayTags::InitializeNativeTags(); 
	UAbilitySystemGlobals::Get().InitGlobalData(); 
}
