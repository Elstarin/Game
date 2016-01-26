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

// // Main log
// DEFINE_LOG_CATEGORY_STATIC(Main, Warning, All);

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

// namespace // Unnamed namespace
// {
//   inline FString convert(){
//     return "NONE";
//   }
//
//   inline FString convert(FString x){
//     return x;
//   }
//
//   inline FString* convert(FString* x){
//     return x;
//   }
//
//   inline FString convert(const char* x){
//   	return x;
//   }
//
//   // inline FString convert(std::string x){
//   //   return x.c_str();
//   // }
//
//   inline FString convert(bool x){
//   	if (x){
//   		return TEXT("true");
//   	} else {
//   		return TEXT("false");
//   	}
//   }
//
//   inline FString convert(int32 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(int8 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(int16 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(int64 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(uint8 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(uint16 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(uint32 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(uint64 x){
//     return FString::FromInt(x);
//   }
//
//   inline FString convert(double x){
//     return FString::SanitizeFloat(x);
//   }
//
//   inline FString convert(float x){
//     return FString::SanitizeFloat(x);
//   }
//
//   inline FString convert(FVector x){
//     return x.ToString();
//   }
//
//   inline FString convert(FVector2D x){
//     return x.ToString();
//   }
//
//   inline FString convert(FRotator x){
//     return x.ToString();
//   }
//
//   inline FString convert(FLinearColor x){
//     return x.ToString();
//   }
//
//   inline FString convert(UObject x){
//     return x.GetName();
//   }
//
//   inline FString convert(AActor x)
//   {
//     return x.GetName();
//   }
//
//   inline FString convert(AActor* x)
//   {
//     return x->GetName();
//   }
//
//   inline FString convert(void* x)
//   {
//     return "Pointer: " + FString::Printf(TEXT("%p"), x);
//     // return "Pointer: " + FString::Printf(TEXT("%d"), x);
//   }
// }

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
extern inline FString convert(Frame& x);
extern inline FString convert(Frame* x);

// static FString printStr;
extern FString printStr;

template <typename T>
void print(T value)
{
  printStr += convert(value);
  // UE_LOG(Main, Warning, TEXT("Print 1: %s"), *printStr);
	
	UtilitySystem::finishPrint(printStr);

	// Empty the FString
  printStr = "";
}

template <typename T, typename... Args>
void print(T value, Args... args)
{
	// Catch each value and store it up in printStr
  printStr += (convert(value) + FString(" "));
  // UE_LOG(Main, Warning, TEXT("Print 2: %s"), *printStr);

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

// As the name implies, these two functions are used for iterating over a tuple
// call it with a lambda, like: IterateTuple(tuple, [](const auto& x){}
template<class F, class...Ts, std::size_t...Is>
void IterateTuple(const std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>){
  using expander = int[];
  (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template<class F, class...Ts>
void IterateTuple(const std::tuple<Ts...> & tuple, F func){
  IterateTuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
}
