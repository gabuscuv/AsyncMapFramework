// Copyright 2023 Gabriel Bustillo del Cuvillo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AsyncPlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAsyncPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ASYNCMAPFRAMEWORK_API IAsyncPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void TeleportToPlayerStart(FName LevelName);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void TeleportToLastTransform();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	bool IsMenuPawn();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	bool ReSpawn(bool MenuPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void SaveData(FName LastLoadedMap, ELoadingMode LoadingMode);
};
