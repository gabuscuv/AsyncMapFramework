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