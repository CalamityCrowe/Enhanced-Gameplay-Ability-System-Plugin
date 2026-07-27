// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "EnhancedAssetManager.generated.h"
 
/**
 * 
 */
 /* 
 	we set this in the project settings under the asset manager. 
	this is so we can load all the relevant tags in the enhanced gameplay tags singleton within the start initial loading function
 */
 
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public: 
	UEnhancedAssetManager();
	
	static UEnhancedAssetManager& Get();
	
	virtual void StartInitialLoading() override;
};
