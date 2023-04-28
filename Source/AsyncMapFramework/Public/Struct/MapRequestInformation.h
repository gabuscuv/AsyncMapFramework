#pragma once

#include "CoreMinimal.h"

enum class ELoadingMode : uint8;

//USTRUCT(BlueprintType)
struct FMapRequestInformation
{
   // GENERATED_USTRUCT_BODY()
    FMapRequestInformation()
    {
        levelName = FName("");
        loadingMode = (ELoadingMode)0;
        IgnoreFade =  false;
    }


    FMapRequestInformation(FName _levelName, ELoadingMode _loadingMode, bool _IgnoreFade)
    {
        levelName = _levelName;
        loadingMode = _loadingMode;
        IgnoreFade =  _IgnoreFade;
    }

    FName levelName;
    ELoadingMode loadingMode;
    bool IgnoreFade;
};