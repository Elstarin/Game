// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __GAME_H__
#define __GAME_H__

#include "Engine.h"

// Current Class Name + Function Name where this is called!
#define CURRENT_CLASS_AND_FUNC (FString(__FUNCTION__))
 
// Current Class where this is called!
#define CURRENT_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )
 
// Current Function Name where this is called!
#define CURRENT_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
 
// Current Line Number in the code where this is called!
#define CURRENT_LINE (FString::FromInt(__LINE__))
 
// Current Function Signature where this is called!
#define CURRENT_FUNCSIG (FString(__FUNCSIG__))
 
// Final output
#define PRINT_DATA (__FUNCTION__ + "(" + __FUNCSIG__ + ")")

#define CURRENT_CLASS_AND_LINE ("[" + FString(__FUNCTION__) + " (" + FString::FromInt(__LINE__) + ")] ")
#define FUNCTION_DATA ("[" + CURRENT_CLASS + " " + FString::FromInt(__LINE__) + "] " + FString(__FUNCSIG__))

// Define the log category
DEFINE_LOG_CATEGORY_STATIC(L, Warning, All);
// Define a macro wrapper for the log to make it more convenient and useful
// Valid log verbosity values are: Fatal, Error, Warning, Display, Log, Verbose, or VeryVerbose.
#define LOG(str, ...) UE_LOG(L, Warning, TEXT("[%s (%s)] %s"), *CURRENT_CLASS_AND_FUNC, *CURRENT_LINE, *FString::Printf(TEXT(str), ##__VA_ARGS__ ) )

// This macro makes sure that a given bit of code is only run once
// It can be called as: if (!FIRST_RUN) return; or as: if (FIRST_RUN){}
#define FIRST_RUN ([] { \
    static bool is_first_time = true; \
    auto was_first_time = is_first_time; \
    is_first_time = false; \
    return was_first_time; } ())

#include "TimerSystem.h"
#include "Frame.h"
#include "UtilitySystem.h"

#endif
