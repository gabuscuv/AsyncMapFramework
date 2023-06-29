// Copyright 2023 Gabriel Bustillo del Cuvillo

#pragma once

enum class ELoadingMode : uint8;


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/SaveDataStruct.h"
#include "AsyncGameModeComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAsyncGameModeComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASYNCMAPFRAMEWORK_API IAsyncGameModeComponentInterface
{

	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void LoadMap(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade);
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
    bool LoadSaveData(FSaveData savedata);
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void RemoveLoadingMap(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade);
};
