// Gabriel Bustillo del Cuvillo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AsyncGameModeComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ASYNCMAPFRAMEWORK_API UAsyncGameModeComponentInterface : public UInterface
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
	void LoadMap();
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
    bool LoadSaveData();
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void RemoveLoadingMap(FName LevelName);
};
