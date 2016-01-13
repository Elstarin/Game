// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UtilitySystem.generated.h"

//Current Class Name + Function Name where this is called!
#define JOYSTR_CUR_CLASS_FUNC (FString(__FUNCTION__))
 
//Current Class where this is called!
#define JOYSTR_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )
 
//Current Function Name where this is called!
#define JOYSTR_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
 
//Current Line Number in the code where this is called!
#define JOYSTR_CUR_LINE  (FString::FromInt(__LINE__))
 
//Current Class and Line Number where this is called!
#define JOYSTR_CUR_CLASS_LINE (JOYSTR_CUR_CLASS + "(" + JOYSTR_CUR_LINE + ")")
 
//Current Function Signature where this is called!
#define JOYSTR_CUR_FUNCSIG (FString(__FUNCSIG__))

// // General Log
// DECLARE_LOG_CATEGORY_EXTERN(YourLog, Log, All);

// Main log
DEFINE_LOG_CATEGORY_STATIC(Main, Warning, All);

extern TArray<FString> FStrPrintArray;
extern double Time;
double GetTime();

/*------------------------------------------------------------------------------
		General global functions
------------------------------------------------------------------------------*/
void GetDisplay();
 
UCLASS()
class GAME_API UtilitySystem : public UObject
{
	GENERATED_UCLASS_BODY()
	
	public:
		UtilitySystem();
		~UtilitySystem();
	 
		//FORCEINLNE function
		static FORCEINLINE bool IsValid(AActor* TheActor)
		{
			if(!TheActor) return false;
			if(!TheActor->IsValidLowLevel()) return false;
			return true;
		}
		
		TArray<int32> IntArray;
		TArray<int32>* Pointer = &IntArray;
		const TArray<int32>& Value = *Pointer;
	 
		//cpp function
		static int32 ComplicatedGameDataAnalysis();
		
    // static void profileCode();
		static void profileCode(UWorld* const World);
		
		static void finishPrint(FString str);
};

/*------------------------------------------------------------------------------
		Convert to FString overload functions
------------------------------------------------------------------------------*/
extern inline FString convert();
extern inline FString convert(FString x);
extern inline FString* convert(FString* x);
extern inline FString convert(const char* x);
// extern inline FString convert(std::string x);
extern inline FString convert(bool x);
extern inline FString convert(int32 x);
extern inline FString convert(int8 x);
extern inline FString convert(int16 x);
extern inline FString convert(int64 x);
extern inline FString convert(uint8 x);
extern inline FString convert(uint16 x);
extern inline FString convert(uint32 x);
extern inline FString convert(uint64 x);
extern inline FString convert(double x);
extern inline FString convert(float x);
extern inline FString convert(FVector x);
extern inline FString convert(FVector2D x);
extern inline FString convert(FRotator x);
extern inline FString convert(FLinearColor x);
extern inline FString convert(UObject x);
extern inline FString convert(AActor x);
extern inline FString convert(AActor* x);
extern inline FString convert(void* x);

static FString printStr;

template <typename T>
void print(T value){
  printStr += convert(value);
	
	UtilitySystem::finishPrint(printStr);
	
  // GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, *(printStr + "\n"));
	
	// Add the FString to the TArray
	// FStrPrintArray.Emplace(printStr);
	
	// // Add the timer to the TArray
	// printTimerArray.Emplace(15.f);

	// Empty the FString
	printStr.Reset();
}

template <typename T, typename... Args>
void print(T value, Args... args){ // Recursive variadic function
  // static const unsigned short int32 size = sizeof...(Arguments);
  // <Args...>::name()

	// Catch each value and store it up in printStr
  printStr += (convert(value) + FString(" "));

  print(args...);
}

#define print(...) (print(JOYSTR_CUR_CLASS_LINE + ": ", __VA_ARGS__))

// template <typename Last>
// std::string type_name(){
//   return std::string(typeid(Last).name());
// }
//
// template <typename First, typename Second, typename ...Rest>
// std::string type_name(){
//   return type_name<First>() + " " + type_name<Second, Rest...>();
// }
