
#pragma once

enum class ELoadingMode : uint8;


#include "CoreMinimal.h"
#include "UObject/Interface.h"
/// #include "Enums/LoadingMode.h"
#include "MapInterface.generated.h"
/** Please add a class description */
UINTERFACE(MinimalAPI)
class UMapInterface : public UInterface
{
	GENERATED_BODY()

};

class ASYNCMAPFRAMEWORK_API IMapInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void SetPrologueMode(ELoadingMode True);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void SetLoadingWidgetVisibility(bool Visible);
};
