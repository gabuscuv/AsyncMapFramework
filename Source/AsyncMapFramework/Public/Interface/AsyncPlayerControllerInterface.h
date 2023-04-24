// Gabriel Bustillo del Cuvillo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AsyncPlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ASYNCMAPFRAMEWORK_API  UAsyncPlayerControllerInterface : public UInterface
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
	void TeleportToLastTransform(FVector x);
    UFUNCTION(BlueprintPure, Category = "AsyncMapHelper")
    bool IsMenuPawn();
};
