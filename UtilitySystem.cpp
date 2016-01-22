// Fill out your copyright notice in the Description page of Project Settings.

// KEYWORDS: AActor, UPrimitiveComponent, FHitResult, UParticleSystem, UParticleSystemComponent, TSubobjectPtr, NULL, AActor

// A way to allocate space for a TArray when creating it. Just note that a TArray with a different allocator is a different type, so this one couldn't be passed as TArray<Shape*>
/*
  TArray<Shape*, TInlineAllocator<16>> MyShapeArray;
*/

// To solve the above, they mention using a template
/*
  template<typename AllocatorType>
  int32 GetNumShapes(TArray<Shape*, AllocatorType>& ShapeArray)
  {
    return ShapeArray.Num();
  }
  
  void TestCompile()
  {
  	TArray<Shape*> HeapShapeArray;
  	TArray<Shape*, TInlineAllocator<16>> InlineShapeArray;
  	
  	FillShapeArray(HeapShapeArray);
  	FillShapeArray(InlineShapeArray);
  	
  	// ok, compiler infers template type of allocator as FDefaultAllocator
  	const int32 A = GetNumShapes(HeapShapeArray);
  	
  	// ok, compiler infers template type of allocator as TInlineAllocator<16>
  	const int32 B = GetNumShapes(InlineShapeArray);
  	
  	// ok, explicit with template type, but not really necessary
  	// or very reusable with hard-coded "16"
  	const int32 C = GetNumShapes<TInlineAllocator<16>>(InlineShapeArray);
  }
*/

// UE4 recommendation for iterating over a TMap
/*
  TMap<AActor*, FMyStruct> ActorStructs;
  for (const auto& Entry : ActorStructs)
  {
    AActor* Actor = Entry.Key;
    const FMyStruct& Data = Entry.Value;
  }
*/

// UE4 recommendation for iterating over a TArray, they specifically mention that you usually want to capture the element as either a pointer or a reference to avoid it copying
/*
  for (auto* MyShape : MyShapeArray)
  {
    MyShape->Draw();
  }
*/

// Reserve memory before doing a loop to add elements to an array to avoid it having to increase the amount of reserved memory multiple times
/*
  Result.Reserve(Result.Num() + N);
  for (int32 Index=0; Index < N; Index++)
  {
    Result.Add(CharToCopy);
  }
*/

// Always pass a TArray by reference, otherwise it will copy the whole array. Not positive this example is correct, I wrote it.
/*
  void ArrayFunc(TArray<char> &Result){};
*/

// Removing values from an array more efficiently
/*
  void RemoveEvenValues(TArray<int32>& Numbers)
  {
  	for (int32 Index = 0; Index < Numbers.Num(); Index++)
  	{
  		if (Numbers[Index] % 2 == 0)
  		{
  			Numbers.RemoveAtSwap(Index, 1, false);
  			Index--;   // since we need to process this index again!
  		}
  	}
  	
  	// Optional: reclaim empty space after values have been removed
  	Numbers.Shrink();
  }
*/

// Initializing a list
/*
  int32 tableinit[] = {4, 8, 15, 16, 23, 42};
  TArray<int32> table;
  table.Append(tableinit, ARRAY_COUNT(tableinit));
*/

//
/*

*/

#include "Game.h"
#include "UtilitySystem.h"
#include "TimerSystem.h"
// #include "MainHUD.h"

// Extern globals
TArray<FString> FStrPrintArray;
FString printStr;

UtilitySystem::UtilitySystem()
{
}

UtilitySystem::~UtilitySystem()
{
}
 
/*------------------------------------------------------------------------------
		Utility System
------------------------------------------------------------------------------*/
UtilitySystem::UtilitySystem(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
  // static TArray<FString> globalArray; // Make the array
  // static TArray<FString>* globalArrayPtr = &globalArray;
  // globalArray.SetNum(8);
  
  // globalArray.Emplace(TEXT("STRING"));
  // printStrArrayPtr = &printStrArray; // Create the pointer
  // TArray<FString>* printStrArrayPtr = &printStrArray; // Create the pointer
  // TArray<FString>& strArray = *printStrArrayPtr;
  // strArray.Emplace(TEXT("%d"));
  // strArray.Emplace(TEXT("%d"));
  
  // UE_LOG(Main, Warning, TEXT("StaticLib Pointer: %d"), globalArrayPtr);
  // UE_LOG(Main, Warning, TEXT("StaticLib num: %d"), (*globalArrayPtr).Num());
  // UE_LOG(Main, Warning, TEXT("StaticLib constructor: %d"), printStrArray.Num());
}
 
int32 UtilitySystem::ComplicatedGameDataAnalysis(){
	//Do lots of stuff that requires the entire Game's compiled header source,
	//  involving lots of your custom project-specific classes,
	//    classes that have not been compiled in the .h of this library
	//    since they depend on it.
        return 9000;
}

// TArray<Frame*> TestFrameList;
void CreateFrameList()
{
  for (int32 i = 1; i <= 10000; i++)
  {
    auto frame = Frame::CreateFrame("", "", "BACKGROUND", 0);
  }
}

// void IterateFrameList()
// {
//   for (int32 i = 0; i < TestFrameList.Num(); i++)
//   {
//     auto* f = TestFrameList[i];
//   }
//
//   // for (auto& f : TestFrameList)
//   // {
//   //
//   // }
// }

// void UtilitySystem::profileCode()
void UtilitySystem::profileCode(UWorld* const World)
{
  // APlayerController* PlayerCon = UObject::GetOwningPlayerController();
  // auto PlayerCon = UGameplayStatics::GetPlayerController(World, 0);
  // auto PlayerCon = GetWorld()->GetFirstPlayerController();
  
  // print("Creating frame list", Frame::GetNumFrames());
  // CreateFrameList();

  if (false) // Toggle this to easily disable profiling
  {
    TimerSystem::SetTimer(1, [](){
      int32 loopNum = 1;
      // loopNum = 100;
      // loopNum = 1000; // 1k
      // loopNum = 10000; // 10k
      // loopNum = 100000; // 100k
      // loopNum = 500000; // 500k
      loopNum = 1000000; // 1m
      // loopNum = 10000000; // 10m
      // loopNum = 100000000; // 100m
      
      struct CallbackArgs
      {
        FString event = "TEST_EVENT";
        // FString event1 = "TEST_EVENT";
        // FString event2 = "TEST_EVENT";
        // FString event3 = "TEST_EVENT";
        // FString event4 = "TEST_EVENT";
        // FString event5 = "TEST_EVENT";
        // FString event6 = "TEST_EVENT";
        // const char* event = "TEST_EVENT";
        
        // CallbackArgs(){print("Args constructed");}
        // ~CallbackArgs(){print("Args destroyed");}
      };
      
      // static TArray<TUniquePtr<CallbackArgs>> argArray;
      // static TArray<CallbackArgs*> argArray;
      
      // Code to test
      double start = TimerSystem::GetTime();
      for (int32 i = 1; i <= loopNum; i++)
      {
        // argArray.Emplace(new CallbackArgs);
        // argArray.Emplace(new CallbackArgs());
        // Frame::Fire(EventEnum::GAME_PAUSE);
      }
      double stop = TimerSystem::GetTime();
      
      double totalMS = (stop - start) * 1000.f; // Total time in milliseconds
      double MSper = totalMS / loopNum; // Milliseconds per iteration
      double oneSecond = 1000.f / MSper; // How many iterations can be done in one second
      double oneMS = oneSecond / 1000.f; // How many iterations can be done in one millisecond
      
      print("Total MS:", totalMS);
      print("MS per:", MSper);
      print("In 1 second:", oneSecond);
      print("In 1 MS:", oneMS);
    });
  }
}

void UtilitySystem::finishPrint(FString str)
{
  str.Trim();
  str.Shrink();
  FStrPrintArray.Emplace(str);
}

inline FString convert(){
  return "NONE";
}

inline FString convert(FString x){
  return x;
}

inline FString* convert(FString* x){
  return x;
}

inline FString convert(const char* x){
  return x;
}

// inline FString convert(std::string x){
//   return x.c_str();
// }

inline FString convert(bool x){
  if (x){
    return TEXT("true");
  } else {
    return TEXT("false");
  }
}

inline FString convert(int32 x){
  return FString::FromInt(x);
}

inline FString convert(int8 x){
  return FString::FromInt(x);
}

inline FString convert(int16 x){
  return FString::FromInt(x);
}

inline FString convert(int64 x){
  return FString::FromInt(x);
}

inline FString convert(uint8 x){
  return FString::FromInt(x);
}

inline FString convert(uint16 x){
  return FString::FromInt(x);
}

inline FString convert(uint32 x){
  return FString::FromInt(x);
}

inline FString convert(uint64 x){
  return FString::FromInt(x);
}

inline FString convert(double x){
  return FString::SanitizeFloat(x);
}

inline FString convert(float x){
  return FString::SanitizeFloat(x);
}

inline FString convert(FVector x)
{
  return x.ToString();
}

inline FString convert(FVector2D x)
{
  return x.ToString();
}

inline FString convert(FRotator x)
{
  return x.ToString();
}

inline FString convert(FLinearColor x)
{
  return x.ToString();
}

inline FString convert(UObject x)
{
  return x.GetName();
}

inline FString convert(AActor x)
{
  return x.GetName();
}

inline FString convert(AActor* x)
{
  return x->GetName();
}

inline FString convert(Frame& x)
{
  return FString::Printf(TEXT("%s (%p)"), *(x.GetName()), &x);
}

inline FString convert(Frame* x)
{
  return FString::Printf(TEXT("%s (%p)"), *(x->GetName()), x);
}

inline FString convert(void* x)
{
  return "Pointer: " + FString::Printf(TEXT("%p"), x);
}
