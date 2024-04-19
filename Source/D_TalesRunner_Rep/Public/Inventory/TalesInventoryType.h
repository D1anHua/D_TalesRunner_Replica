#pragma once

#include "TalesInventoryType.generated.h"

//! UENUM to display all the entries in the table item
UENUM(BlueprintType)
enum ETalesInventoryItem
{
	//! Sward
	Sward      UMETA(DisplayName = "Sward"),

	//! Shield
	Shield     UMETA(DisplayName = "Shield"),

	//! eatable
	Eatable    UMETA(DisplayName = "Eatable"),
};
