// Copyright 2023 Gabriel Bustillo del Cuvillo

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ELoadingMode : uint8
{
    Startup,
    Regular,
    Prologue,
    MainMenu,
    CustomLocation
};