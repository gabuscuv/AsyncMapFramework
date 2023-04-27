// Copyright 2023 Gabriel Bustillo del Cuvillo

#pragma once

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