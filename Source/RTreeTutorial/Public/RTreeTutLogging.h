// Copyright Reflection Tree 2025, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRTreeTut, Log, All);

#define RTREETUT_LOGFMT(Verbosity, Format, ...) \
UE_LOGFMT(LogRTreeTut, Verbosity, "{0}: " Format, TEXT(__FUNCTION__), ##__VA_ARGS__)
