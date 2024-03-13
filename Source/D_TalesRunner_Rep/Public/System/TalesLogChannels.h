#pragma once

#include "Logging/LogMacros.h"

class UObject;

D_TALESRUNNER_REP_API DECLARE_LOG_CATEGORY_EXTERN(LogTales, Log, All);
D_TALESRUNNER_REP_API DECLARE_LOG_CATEGORY_EXTERN(LogTalesAbilitySystem, Log, All);

D_TALESRUNNER_REP_API FString GetClientServerContextString(UObject* ContextObject);


