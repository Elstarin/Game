// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Frame.h"
#include "MainHUD.h"
#include "GameGameMode.h"
#include "GameCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Kismet/GameplayStatics.h"
#include "UtilitySystem.h"
#include "TimerSystem.h"

// void AMainHUD::DrawHUD(){
//   Super::DrawHUD();
//
//   // find center of the Canvas
//   const FVector2D Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
//
//   // const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
//
//   { // Draw chat box
//     float messageH = 20.f;
//     DrawRect(FLinearColor::Black, 0, 0, Canvas->SizeX, messageH);
//   }
//
//   { // Handle print strings
//     int32 count = 0;
//     int32 height = 20;
//     for (auto& str : FStrPrintArray){
//       FVector2D GameOverSize;
//       GetTextSize(str, GameOverSize.X, GameOverSize.Y, HUDFont);
//
//       DrawText(str, FColor::Red, 20, height, HUDFont, 0.75f, true);
//
//       height += (GameOverSize.Y);
//       count++;
//     }
//
//     // for (int32 Index = 0; Index != printStrArray.Num(); ++Index)
//     // {
//     //   // DrawText(TEXT("Drawing2: ") + printStrArray[Index], FColor::Red, 150, 150, HUDFont);
//     //   // GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, *(FString("TEXT2!")));
//     //   // joinedStr += printStrArray[Index];
//     //   // joinedStr += TEXT(" ");
//     //   // DrawText(TEXT("Test text!"), FColor::Red, 150, 150, HUDFont);
//     // }
//   }
//
//   { // Draw very simple crosshair
//     // const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
//
//     float outPutValue = 1.32f;
//
//     // // Get the character and print stuff about it
//     // AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
//     // FString PowerLevelString = FString::Printf(TEXT("%10.1"), FMath::Abs(outPutValue));
//     // // FString PowerLevelString = FString::Printf(TEXT("%10.1"), FMath::Abs(MyCharacter->PowerLevel));
//     // DrawText(PowerLevelString, FColor::Red, 50, 50, HUDFont);
//     // DrawText(TEXT("Test text!"), FColor::Red, 150, 150, HUDFont);
//
//   	AFPSGameMode* MyGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(this));
//
//     // // If the game is over
//     // if (MyGameMode->GetCurrentState() == EGamePlayState::EGameOver){
//     //   // Create a variable for storing the size of printing Game Over
//     //   FVector2d GameOverSize;
//     //   GetTextSize(TEXT("GAME OVER"), GameOverSize.X, GameOverSize.Y, HUDFont);
//     //   DrawText(TEXT("GAME OVER"), FColor::White, (ScreenDimensions.X - GameOverSize.X) / 2.0f, (ScreenDimensions.Y - GameOverSize.Y) / 2.0f, HUDFont);
//     // }
//
//     // offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
//     const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)), (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));
//
//     // draw the crosshair
//     FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
//     TileItem.BlendMode = SE_BLEND_Translucent;
//     Canvas->DrawItem( TileItem );
//   }
// }

#define BUTTONTYPE_MAIN_RESTART 1
#define BUTTONTYPE_MAIN_EXIT 2
 
#define BUTTONTYPE_CONFIRM_YES 1
#define BUTTONTYPE_CONFIRM_NO 2
 
#define CANVAS_WHITE if (Canvas) Canvas->SetDrawColor(FColor_White);
 
//Cursor Draw Offset
// use this to position texture over the point of your cursor,
// if the point is not at exactly 0,0
#define CURSOR_DRAW_OFFSET 3
 
// Static constants ////////////////////////////////////////////////////////////
const FString AMainHUD::S_Title_Main = FString("Joy!");
const FString AMainHUD::S_Title_Confirm = FString("Exit Game?");
 
const FString AMainHUD::S_Button_Restart = FString("Restart");
const FString AMainHUD::S_Button_Exit = FString("Exit");
 
// Colors
const FColor AMainHUD::FColorBlack = FColor(0,0,0,255);
const FColor AMainHUD::FColorRed = FColor(255,0,0,255);
const FColor AMainHUD::FColorYellow = FColor(255,255,0,255);
const FColor AMainHUD::FColorBlue = FColor(0,0,255,255);
const FColor AMainHUD::FColor_White = FColor(255,255,255,255);
// Backgrounds
const FLinearColor AMainHUD::LC_Black = FLinearColor(0, 0, 0, 1);
const FLinearColor AMainHUD::LC_Pink = FLinearColor(1, 0, 1, 1);
const FLinearColor AMainHUD::LC_Red = FLinearColor(1, 0, 0, 1);
const FLinearColor AMainHUD::LC_Yellow = FLinearColor(1, 1, 0, 1);
////////////////////////////////////////////////////////////////////////////////

AMainHUD::AMainHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  // static TArray<Frame> frameList;
  
  // Use the RobotoDistanceField font from the engine
  static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
  HUDFont = HUDFontOb.Object;

  // // Set the crosshair texture
  // static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/crosshair.crosshair'"));
  // CrosshairTex = CrosshairTexObj.Object;
  
	//Draw HUD?
	DontDrawHUD = false;

	//States
	ConfirmDialogOpen = false;
	InMainMenu = true;

  bDrawFrames = true;

  bShowMessages = true;
  
  bShowMainMenu = false;
  
	//Scale
	GlobalHUDMult = 1;
	DefaultFontScale = 0.7;   //scaling down a size 36 font
}
 
//Core
 
void AMainHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
 
	//Establish the PC
	PC = GetOwningPlayerController();
 
	//How to get a ref to your custom PC
	//AYourPCtroller* YourChar = Cast<AYourPCtroller>(PC);
 
	//How to Get The Character
	//AYourCharacterClass* YourChar = Cast<AYourCharacterClass>(GetOwningPawn());
}
 
//===============
// Draw Dialogs
//===============
void AMainHUD::DrawHUD_DrawDialogs()
{
	if (ConfirmDialogOpen) DrawConfirm();
}

FString AMainHUD::EscapeSequences(int32 searchNum, FString str)
{
  auto arr = str.GetCharArray();
  
  int32 index = searchNum + 1;
  
  // Color sequence
  if (arr[index] == 'c'){
    int32 counter = 0;
    char hex[8] = {};
    
    // Go until the next closing sequence
    while ((arr[index] != '|') && (arr[index + 1] != 'r')){
      index++;
    
      // Capture the next 8 characters, they are the hex code
      if (7 >= counter){
        hex[counter] = arr[index];
    
        counter++;
      }
    }
    
    str.RemoveAt(searchNum, (searchNum + 9), true); // Remove the start of the sequence
    str.RemoveAt(index, (index + 1), true); // Remove the end of the sequence
  }
  
  return str;
}

void AMainHUD::addMessage(Message msg)
{
  messages.Add(msg);
}

void AMainHUD::DrawMessageBox()
{
  for (int32 i = messages.Num() - 1; i >= 0; i--)
  {
    float outputWidth, outputHeight, pad = 10.f;
    
    GetTextSize(messages[i].message, outputWidth, outputHeight, HUDFont, 1.f);
    
    float messageH = outputHeight + 2.f * pad;
    float x = 0.f, y = i * messageH;
    
    DrawRect(FLinearColor::Black, x, y, Canvas->SizeX, messageH);
    
    DrawText(
    messages[i].message,
    messages[i].color,
    (x + pad),
    (y + pad),
    HUDFont,
    0.75f,
    false);
    
    messages[i].time -= GetWorld()->GetDeltaSeconds();
    
    // Expired, remove it
    if (messages[i].time <= 0)
    {
      messages.RemoveAt(i);
    }
  }
}

void AMainHUD::DrawPrintBox()
{
  if (!bShowMessages) return; // Only run if true
  
  const float maxX = Canvas->SizeX * 0.5f;
  const float maxY = Canvas->SizeY * 0.28f;
  const float textScale = 0.75f;
  const int32 maxDisplayed = 10; // Total number of strings to be displayed at once
  const int32 maxStored = 20; // Total number of strings that can be stored
  const int32 arrayNum = FStrPrintArray.Num();
  const int32 offsetX = 10;
  const int32 offsetY = 20;
  const int32 startX = 0;
  const int32 startY = Canvas->SizeY - maxY;
  
  DrawRect(FLinearColor(0, 0, 0, 0.15f), startX, startY, maxX, maxY);
  
  { // Handle print strings
    int32 i = arrayNum;
    
    if (arrayNum > 0){ // Only run if it actually has some values
      int32 height = offsetY;
      int32 stop = (arrayNum - maxDisplayed);
      int32 count = 0;
      
      if (0 > stop) stop = 0; // Make sure it's positive
      
      for (i = arrayNum; i >= stop; i--){
        if (FStrPrintArray.IsValidIndex(i)){
          FString str = FStrPrintArray[i];
          
          FVector2D textSize;
          GetTextSize(str, textSize.X, textSize.Y, HUDFont);
          
          textSize.X *= textScale;
          textSize.Y *= textScale;
          
          if ((textSize.X + (offsetX * textScale)) < maxX){ // Check if it fits in the box
            // int32 searchNum = 0;
            // // Has an escape character
            // if (str.FindChar('|', searchNum)){
            //   int32 index = searchNum + 1;
            //
            //   if (str.Len() > 35){
            //     int32 num1 = 4;
            //     int32 num2 = 36;
            //     str.ReplaceEscapedCharWithChar();
            //     // str.Replace(TEXT(" "), TEXT("~"));
            //     // str.RemoveAt(num1, num2, true); // Remove the start of the sequence
            //     str.Replace(TEXT("|"), TEXT("~"));
            //   }
            // }
            
            // Outline
            DrawText(str, FColor(0, 0, 0, 255), (startX + offsetX) + 2, ((startY + maxY) - height) + 2, HUDFont, textScale, false);
            
            // Actual text
            DrawText(str, FColor(100, 175, 255, 255), (startX + offsetX), ((startY + maxY) - height), HUDFont, textScale, false);
            height += (textSize.Y);
          } else { // Doesn't fit, resize it
            float percent = (1 - (maxX / (textSize.X + (offsetX * textScale))) * 1); // How much to cut
            int32 length = str.Len(); // Total number of characters
            int32 splitPoint = floor(length - (length * percent));
            
            // UE_LOG(Main, Warning, TEXT("BEFORE: %s"), *FStrPrintArray[i]);
            FStrPrintArray[i].RemoveAt(splitPoint, (length - splitPoint), true); // Remove the end
            FStrPrintArray.Insert((str.RightChop(splitPoint).Trim()), i); // Insert the extra into the array
            
            str.InsertAt(splitPoint, "\n"); // Insert new line just for use this one time
            DrawText(str, FColor::Blue, (startX + offsetX), ((startY + maxY) - height), HUDFont, textScale, false);
            
            height += (textSize.Y * 2); // Twice as much height
          }
        }
      }
    }
    
    // Wait until there are 10 strings too many, then clear them out
    if (arrayNum > (maxStored + 10)){
      // int32 index;
      
      for (i = (arrayNum - maxStored); i != 0; i--){
        if (FStrPrintArray.IsValidIndex(i)){
          // Run from (total number of elements - maxStored) down to 0, removing each
          // Lowest numbers are the oldest values here
          FStrPrintArray.RemoveAt(i);
        }
      }
    }
  }
  
	// DrawJoyRect(Canvas->SizeX / 2 - 100, Canvas->SizeY / 2 - 50, 200, 100, FLinearColor(0, 0, 1, 0.2333));
}

// template <typename T, typename... A>
// using TestFuncType = std::function<void(Frame*, ...)>;
// using TestFuncType = std::function<void(...)>;
// typedef std::function<void(Frame*)> TestFuncType;

struct CallbackArgs
{
  FString event = "TEST_EVENT";
  // const char* event = "TEST_EVENT";
  
  CallbackArgs(){print("Args constructed");}
  ~CallbackArgs(){print("Args destroyed");}
};

typedef std::function<void(Frame*, CallbackArgs*)> TestFuncType;
// typedef std::function<void(Frame*, std::unique_ptr<CallbackArgs>)> TestFuncType;
// typedef std::function<void(Frame*)> TestFuncType;

void GenerateGUID(char* const GUID, const int32 length)
{
  static const char* const alphabet = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
  static const char* const alphabetL = "abcdefghijklmnopqrstuvwxyz";
  static const char* const alphabetU = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static const char* const numbers = "0123456789";
  
  // const int32 length = 25;
  // char GUID[length]; // The char array that will hold the newly generated GUID
  const int32 split = length / 5;
  
  for (int32 i = 0; i < (length - 1); i++)
  {
    // Every split number of characters, add a '-', but not when it's near the end
    if (((i + split) <= length) && ((i % (split + 1)) == split))
    {
      GUID[i] = '-';
    }
    // If this passes, use a letter. Numbers should be more common, looks better
    else if (FMath::RandRange(0, 2) == 0)
    {
      // If it passes, use a lower case letter. If not, use upper case
      if (FMath::RandRange(0, 1) == 1)
      {
        GUID[i] = alphabetL[FMath::RandRange(0, 25)]; // Pick a random lower case letter
      }
      else
      {
        GUID[i] = alphabetU[FMath::RandRange(0, 25)]; // Pick a random upper case letter
      }
    }
    // Was 1 or 2, use a number instead
    else
    {
      GUID[i] = numbers[FMath::RandRange(0, 9)]; // Pick a random number
    }
  }
  
  GUID[length - 1] = '\0'; // Make sure the last char is the null character
}

void AMainHUD::Startup()
{
  static bool hasRun = false;
  if (hasRun) return;
  hasRun = true;
  
  {
    char GUID[25];
    GenerateGUID(GUID, 25);
    print(GUID);
  }
  
  // UE_LOG(Main, Warning, TEXT("Startup..."));
  
  auto f1 = Frame::CreateFrame("2D", "Frame1", "BACKGROUND", 0);
  f1->SetPosition(200, 200);
  f1->SetSize(50, 50);
  f1->SetColor(1.f, 1.f, 1.f, 1.f);
  f1->SetMouseEnabled(true);
  
  auto text = f1->CreateText();
  
  // f1->OnUpdate([](auto f)
  // {
  //   print("Update");
  // });
  
  // f1->Set_MOUSE_ENTER([](auto f)
  // {
  //   float r = FMath::FRandRange(0, 1);
  //   float g = FMath::FRandRange(0, 1);
  //   float b = FMath::FRandRange(0, 1);
  //
  //   f->SetColor(r, g, b, 1.f);
  // });
  
  // f1->Set_MOUSE_EXIT([](auto f)
  // {
  //   f->SetColor(1.f, 1.f, 1.f, 1.f);
  // });
  
  // auto f2 = Frame::CreateFrame("2D", "Frame2", "BACKGROUND", 0);
  // f2->SetPosition(0, 0);
  // f2->SetSize(250, 100);
  // f2->SetColor(0, 0, 0, 1);
  //
  // f2->SetPoint(Anchors::BOTTOMRIGHT, f1, Anchors::BOTTOMRIGHT, 0, 0);
  
  // auto lambda = [&](auto f, auto args)
  // {
  //   // print(args.event);
  // };
  
  // auto lambda = [&]()
  // {
  //   TUniquePtr<CallbackArgs> args(new CallbackArgs);
  //   print("Called");
  //
  //   return args;
  // };
  //
  // auto passedArgs = lambda();
  // print(&passedArgs);
  // print(passedArgs->event);
  
  // TestFuncType temp;
  
  // CallbackArgs args;
  // print("Size:", sizeof(temp), sizeof(lambda), sizeof(args));
  // temp(f1, args);
  
  // typedef TUniquePtr<CallbackArgs> ArgUniquePtr;
  // static TArray<TUniquePtr<CallbackArgs>> argArray;
  //
  // TimerSystem::SetTicker(0.1, [&](auto ticker)
  // {
  //   static TestFuncType temp = lambda;
  //
  //   // TUniquePtr<CallbackArgs> args(new CallbackArgs);
  //
  //   // CallbackArgs args;
  //   // CallbackArgs* args = new CallbackArgs();
  //   // CallbackArgs* argsPtr;
  //
  //   // argArray.Emplace(args->Release());
  //   // argArray.Emplace(new CallbackArgs);
  //
  //   // temp(f1, args);
  //
  //   // delete args;
  // }, 3);
  //
  // TimerSystem::SetTimer(0.5, [&]()
  // {
  //   argArray[1]->event = "SET EVENT";
  //
  //   print("Running loop");
  //
  //   int32 i = 0;
  //   for (const auto& arg : argArray)
  //   {
  //     i++;
  //     print("Result:", i, arg->event);
  //   }
  // });
}

void AMainHUD::CheckMouseoverFrames(Frame* f, int32 left, int32 right, int32 top, int32 bottom)
{
  // If this passes, mouse is currently within the bounds of the frame
  if ((MouseLocation.X >= left) &&
      (MouseLocation.X <= right) &&
      (MouseLocation.Y >= top) &&
      (MouseLocation.Y <= bottom))
  {
    // If it's false, mouse must have just entered frame, so fire the event
    if (f->GetMouseOver() == false)
    {
      f->SetMouseOver(true);
      f->FireToFrame(EventEnum::MOUSE_ENTER);
    }
    
    // Check if left click just got pressed
    if (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton))
      f->Fire(FRAME_LEFT_CLICK_DOWN);
    
    // Check if left click just got released
    if (PC->WasInputKeyJustReleased(EKeys::LeftMouseButton))
      f->Fire(FRAME_LEFT_CLICK_UP);
    
    // Check if right click just got pressed
    if (PC->WasInputKeyJustPressed(EKeys::RightMouseButton))
      f->Fire(FRAME_RIGHT_CLICK_DOWN);
    
    // Check if right click just got released
    if (PC->WasInputKeyJustReleased(EKeys::RightMouseButton))
      f->Fire(FRAME_RIGHT_CLICK_UP);
  }
  // If statement failed, so mouse is NOT currently within the bounds of this frame
  else
  {
    // If it's true, mouse must have just exited frame, so fire the event
    if (f->GetMouseOver() == true)
    {
      f->FireToFrame(EventEnum::MOUSE_EXIT);
      f->SetMouseOver(false);
    }
  }
}

// float CalculatePoint(Anchors point, Frame* relative, Anchors relativePoint, float frameX, float frameY, float frameW, float frameH)
// {
//   float fX = frameX;
//   float fY = frameY;
//
//   switch (point)
//   {
//     case Anchors::TOPLEFT:
//       fX = relative->GetX();
//       fY = relative->GetY();
//       break;
//     case Anchors::TOP:
//       fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
//       fY = relative->GetY();
//       break;
//     case Anchors::TOPRIGHT:
//       fX = relative->GetX() + relative->GetWidth() - frameW;
//       fY = relative->GetY();
//       break;
//     case Anchors::LEFT:
//       fX = relative->GetX();
//       fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
//       break;
//     case Anchors::CENTER:
//       fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
//       fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
//       break;
//     case Anchors::RIGHT:
//       fX = relative->GetX() + relative->GetWidth() - frameW;
//       fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
//       break;
//     case Anchors::BOTTOMLEFT:
//       fX = relative->GetX();
//       fY = relative->GetY() + relative->GetHeight() - frameH;
//       break;
//     case Anchors::BOTTOM:
//       fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
//       fY = relative->GetY() + relative->GetHeight() - frameH;
//       break;
//     case Anchors::BOTTOMRIGHT:
//       fX = relative->GetX() + relative->GetWidth() - frameW;
//       fY = relative->GetY() + relative->GetHeight() - frameH;
//       break;
//     default:
//       print("Unknown point!");
//       break;
//   }
//
//   switch (relativePoint)
//   {
//     case Anchors::TOPLEFT:
//       fX = fX;
//       fY = fY;
//       break;
//     case Anchors::TOP:
//       fX = fX + (frameW / 2);
//       fY = fY;
//       break;
//     case Anchors::TOPRIGHT:
//       fX = fX + frameW;
//       fY = fY;
//       break;
//     case Anchors::LEFT:
//       fX = fX;
//       fY = fY + (frameH / 2);
//       break;
//     case Anchors::CENTER:
//       fX = fX + (frameW / 2);
//       fY = fY + (frameH / 2);
//       break;
//     case Anchors::RIGHT:
//       fX = fX + frameW;
//       fY = fY + (frameH / 2);
//       break;
//     case Anchors::BOTTOMLEFT:
//       fX = fX;
//       fY = fY + frameH;
//       break;
//     case Anchors::BOTTOM:
//       fX = fX + (frameW / 2);
//       fY = fY + frameH;
//       break;
//     case Anchors::BOTTOMRIGHT:
//       fX = fX + frameW;
//       fY = fY + frameH;
//       break;
//     default:
//       print("Unknown relative point!");
//       break;
//   }
//
//   return fX, fY;
// }

/*------------------------------------------------------------------------------
		Main draw function for frames
------------------------------------------------------------------------------*/
void AMainHUD::DrawFrames()
{
  // TArray<EKeys::Type> GameControlKeys;
  
  TimerSystem::IterateTimerArrays();
  // Frame::IterateOnUpdateList();
  
  // Update the mouse's position
  PC->GetMousePosition(MouseLocation.X, MouseLocation.Y);

  // If false, we don't want to draw any frames, so return
  if (!bDrawFrames) return;
  
  // print("Starting draw cycle");
  
  static double topLeftCorner;
  static double topRightCorner;
  static double bottomLeftCorner;
  static double bottomRightCorner;
  
  static int32 frameX;
  static int32 frameY;
  static int32 frameW;
  static int32 frameH;
  static int32 count; // Total number of frames drawn
  count = 0;

  static float cWidth = Canvas->SizeX;
  static float cHeight = Canvas->SizeY;

  // Run through each strata
  for (auto& strata : Frame::strataList)
  {
    // Has this strata been created?
    if (Frame::StrataMap.Contains(strata))
    {
      // Run through all frame levels for this strata
      for (int32 i = -10; i <= 10; i++)
      {
        // Check if each level contains any frames
        if (Frame::StrataMap[strata].LevelMap.Contains(i))
        {
          // Iterate through every frame in this level
          for (auto& f : Frame::StrataMap[strata].LevelMap[i].FrameList)
          {
            if (f == nullptr) continue; // It's currently null, so skip it
            
            // // If this frame has an OnUpdate set, call it
            // if (f->OnUpdateScript.callback)
            // {
            //   f->OnUpdateScript.callback(f->OnUpdateScript);
            // }
            
            // If not shown, don't draw it (obviously)
            if (true == f->IsShown())
            {
              frameX = f->GetX();
              frameY = f->GetY();
              frameW = f->GetWidth();
              frameH = f->GetHeight();
              
              // If either are zero it won't be visible, so don't waste time drawing it
              if ((frameW > 0) && (frameH > 0))
              {
                // Check if this frame is registered for mouse interactivity, otherwise don't waste time
                if (f->GetMouseEnabled() == true)
                {
                  CheckMouseoverFrames(f, frameX, (frameX + frameW), frameY, (frameY + frameH));
                }
                
                count++;
                // Use a switch in here for category
                
                float fX = frameX;
                float fY = frameY;
                
                // topLeftCorner = 0;
                // topRightCorner = 0;
                // bottomLeftCorner = 0;
                // bottomRightCorner = 0;
                //
                // // Make sure it actually has points set
                // if (f->PointList.Num() > 0)
                // {
                //   // Run through each point
                //   for (auto& point : f->PointList)
                //   {
                //     // Get the frame it is relative to
                //     Frame* relative = point->GetRelativeFrame();
                //
                //     // If it is relative to another frame, recalculate its X and Y
                //     if (relative != nullptr)
                //     {
                //       switch (point->GetAnchorPoint())
                //       {
                //         case Anchors::TOPLEFT:
                //           fX = relative->GetX();
                //           fY = relative->GetY();
                //           break;
                //         case Anchors::TOP:
                //           fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
                //           fY = relative->GetY();
                //           break;
                //         case Anchors::TOPRIGHT:
                //           fX = relative->GetX() + relative->GetWidth() - frameW;
                //           fY = relative->GetY();
                //           break;
                //         case Anchors::LEFT:
                //           fX = relative->GetX();
                //           fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
                //           break;
                //         case Anchors::CENTER:
                //           fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
                //           fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
                //           break;
                //         case Anchors::RIGHT:
                //           fX = relative->GetX() + relative->GetWidth() - frameW;
                //           fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
                //           break;
                //         case Anchors::BOTTOMLEFT:
                //           fX = relative->GetX();
                //           fY = relative->GetY() + relative->GetHeight() - frameH;
                //           break;
                //         case Anchors::BOTTOM:
                //           fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
                //           fY = relative->GetY() + relative->GetHeight() - frameH;
                //           break;
                //         case Anchors::BOTTOMRIGHT:
                //           fX = relative->GetX() + relative->GetWidth() - frameW;
                //           fY = relative->GetY() + relative->GetHeight() - frameH;
                //           break;
                //       }
                //
                //       switch (point->GetRelativePoint())
                //       {
                //         case Anchors::TOPLEFT:
                //           fX = fX;
                //           fY = fY;
                //           break;
                //         case Anchors::TOP:
                //           fX = fX + (frameW / 2);
                //           fY = fY;
                //           break;
                //         case Anchors::TOPRIGHT:
                //           fX = fX + frameW;
                //           fY = fY;
                //           break;
                //         case Anchors::LEFT:
                //           fX = fX;
                //           fY = fY + (frameH / 2);
                //           break;
                //         case Anchors::CENTER:
                //           fX = fX + (frameW / 2);
                //           fY = fY + (frameH / 2);
                //           break;
                //         case Anchors::RIGHT:
                //           fX = fX + frameW;
                //           fY = fY + (frameH / 2);
                //           break;
                //         case Anchors::BOTTOMLEFT:
                //           fX = fX;
                //           fY = fY + frameH;
                //           break;
                //         case Anchors::BOTTOM:
                //           fX = fX + (frameW / 2);
                //           fY = fY + frameH;
                //           break;
                //         case Anchors::BOTTOMRIGHT:
                //           fX = fX + frameW;
                //           fY = fY + frameH;
                //           break;
                //       }
                //     }
                //   }
                // }
                
                Frame* relative = f->GetRelativeFrame();
                
                // If it is relative to another frame, recalculate its X and Y
                if (relative != nullptr)
                {
                  switch (f->GetAnchorPoint())
                  {
                    case Anchors::TOPLEFT:
                      fX = relative->GetX();
                      fY = relative->GetY();
                      break;
                    case Anchors::TOP:
                      fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
                      fY = relative->GetY();
                      break;
                    case Anchors::TOPRIGHT:
                      fX = relative->GetX() + relative->GetWidth() - frameW;
                      fY = relative->GetY();
                      break;
                    case Anchors::LEFT:
                      fX = relative->GetX();
                      fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
                      break;
                    case Anchors::CENTER:
                      fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
                      fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
                      break;
                    case Anchors::RIGHT:
                      fX = relative->GetX() + relative->GetWidth() - frameW;
                      fY = relative->GetY() + (relative->GetHeight() / 2) + (frameY - (frameH / 2));
                      break;
                    case Anchors::BOTTOMLEFT:
                      fX = relative->GetX();
                      fY = relative->GetY() + relative->GetHeight() - frameH;
                      break;
                    case Anchors::BOTTOM:
                      fX = relative->GetX() + (relative->GetWidth() / 2) + (-frameX - (frameW / 2));
                      fY = relative->GetY() + relative->GetHeight() - frameH;
                      break;
                    case Anchors::BOTTOMRIGHT:
                      fX = relative->GetX() + relative->GetWidth() - frameW;
                      fY = relative->GetY() + relative->GetHeight() - frameH;
                      break;
                    default:
                      print("Unknown point!");
                      break;
                  }
                  
                  switch (f->GetRelativePoint())
                  {
                    case Anchors::TOPLEFT:
                      fX = fX;
                      fY = fY;
                      break;
                    case Anchors::TOP:
                      fX = fX + (frameW / 2);
                      fY = fY;
                      break;
                    case Anchors::TOPRIGHT:
                      fX = fX + frameW;
                      fY = fY;
                      break;
                    case Anchors::LEFT:
                      fX = fX;
                      fY = fY + (frameH / 2);
                      break;
                    case Anchors::CENTER:
                      fX = fX + (frameW / 2);
                      fY = fY + (frameH / 2);
                      break;
                    case Anchors::RIGHT:
                      fX = fX + frameW;
                      fY = fY + (frameH / 2);
                      break;
                    case Anchors::BOTTOMLEFT:
                      fX = fX;
                      fY = fY + frameH;
                      break;
                    case Anchors::BOTTOM:
                      fX = fX + (frameW / 2);
                      fY = fY + frameH;
                      break;
                    case Anchors::BOTTOMRIGHT:
                      fX = fX + frameW;
                      fY = fY + frameH;
                      break;
                    default:
                      print("Unknown relative point!");
                      break;
                  }
                }
                
                DrawRect( f->GetColor(),
                          fX,
                          fY,
                          frameW,
                          frameH);
                
                          
                // Check if the frame has any texture widgets
                if (f->TextureList.Num() > 0)
                {
                  for (auto& texture : f->TextureList)
                  {
                    // int32 textX
                    // int32 textY
                    
                    // DrawText( texture->GetText(),
                    //           FLinearColor(0.5f, 0.5f, 0.5f, 1.f),
                    //           frameX, //
                    //           frameY,
                    //           HUDFont,
                    //           1.f,
                    //           false);
                  }
                }
                
                // Check if the frame has any text widgets
                if (f->TextList.Num() > 0)
                {
                  for (auto& text : f->TextList)
                  {
                    // int32 textX
                    // int32 textY
                    
                    DrawText( text->GetText(),
                              FLinearColor(0.5f, 0.5f, 0.5f, 1.f),
                              frameX, //
                              frameY,
                              HUDFont,
                              1.f,
                              false);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
  FString numTimers = FString::Printf(TEXT("Timers: %d"), TimerSystem::TimerList.Num());
  DrawText(numTimers, FColor(100, 175, 255, 255), 50, 50, HUDFont);
  
  FString numTickers = FString::Printf(TEXT("Tickers: %d"), TimerSystem::TickerList.Num());
  DrawText(numTickers, FColor(100, 175, 255, 255), 50, 80, HUDFont);
  
  // FString numFrames = FString::Printf(TEXT("Number of frames: %d"), count);
  // DrawText(numFrames, FColor(100, 175, 255, 255), 50, 50, HUDFont);
}

//Menus
void AMainHUD::DrawMainMenu()
{
  if (!bShowMainMenu) return; // If false, return
  // DrawMaterialSimple(MaterialBackground, 10, 10, 256, 512, 1.3);
  
  const float width = Canvas->SizeX * 0.75f;
  const float height = Canvas->SizeY * 0.5f;
  // const int32 offsetX = Canvas->SizeX * 0.5f;
  // const int32 offsetY = Canvas->SizeY * 0.5f;
  const int32 startX = (Canvas->SizeX * 0.5f) - (width * 0.5f);
  const int32 startY = (Canvas->SizeY * 0.4f) - (height * 0.5f);
  
  DrawRect(FLinearColor(0, 0, 0, 0.6f), startX, startY, width, height);
  
  // DrawRect(FLinearColor::Black, 200, 200, 100.f, 100.f);
 
	//Menu Title
 
	//Draw buttons
	// DrawMainMenuButtons();
}

void AMainHUD::DrawConfirm()
{
	//Blue rect with alpha 50%
	DrawJoyRect(Canvas->SizeX / 2 - 100, Canvas->SizeY / 2 - 50, 200, 100, FLinearColor(0, 0, 1, 0.2333));
 
	//Confirm Title
 
	//Draw buttons
	DrawConfirmButtons();
}
 
//Buttons
void AMainHUD::DrawMainMenuButtons()
{
	//Start Point
	float xStart = 100;
	float yStart = 110;
 
	//Background
	VDrawTile(ButtonBackground, xStart, yStart, 150, 80, FColor(255, 255, 255, 120)); //alpha 120/255
 
	//Text
	DrawJoyText(
		VerdanaFont, "Restart", xStart + 30, yStart + 20,
		LC_Black, DefaultFontScale,
		true,LC_Red
	);
 
	//Struct
	//Add Button If Necessary
	//		could be cleared and need refreshing if using a different menu
	//			clear buttons with ButtonsMain.Empty()
	if (ButtonsMain.Num() < 1 )
	{
		FJoyButtonStruct newButton = FJoyButtonStruct();
		newButton.type 			= BUTTONTYPE_MAIN_RESTART;
		newButton.toolTip		= "Restart the Game!";
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 150;
		newButton.minY 			= yStart;
		newButton.maxY 			= yStart + 80;
 
		//Add to correct array
		ButtonsMain.Add(newButton);
	}
 
 
	xStart = 100;
	yStart = 410;
 
	VDrawTile(ButtonBackground,xStart,yStart,150,80,FColor(255,255,255,120)); //alpha 120/255
 
	//Text
	DrawJoyText(
		VerdanaFont,"Exit",xStart+55,yStart+20,
		LC_Black, DefaultFontScale,
		true,LC_Red
	);
 
	if (ButtonsMain.Num() < 2 )
	{
		FJoyButtonStruct newButton = FJoyButtonStruct();
		newButton.type 			= BUTTONTYPE_MAIN_EXIT;
		newButton.toolTip			= "Exit the Game!";
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 150;
		newButton.minY 			= yStart;
		newButton.maxY 			= yStart + 80;
 
		//Add to correct array
		ButtonsMain.Add(newButton);
	}
}

void AMainHUD::DrawConfirmButtons()
{
	float xStart = Canvas->SizeX/2 - 100;
	float yStart = Canvas->SizeY/2 - 40;
 
	//Highlighted?
	if(ActiveButton_Type == BUTTONTYPE_CONFIRM_YES ) ColorPtr = &LC_Pink;
	else ColorPtr = &LC_Yellow;
 
	//Text
	DrawJoyText(
		VerdanaFont, "Yes", xStart + 30, yStart + 20,
		*ColorPtr, DefaultFontScale,
		true
	);
 
	if (ButtonsConfirm.Num() < 1){
		FJoyButtonStruct newButton = FJoyButtonStruct();
		newButton.type 			= BUTTONTYPE_CONFIRM_YES ;
		newButton.toolTip			= "";
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 75;
		newButton.minY 			= yStart + 20;
		newButton.maxY 			= yStart + 60;
 
		//could use GetTextSize to streamline this
 
		//Add to correct array
		ButtonsConfirm.Add(newButton);
	}
 
	xStart = Canvas->SizeX/2 + 20;
	yStart = Canvas->SizeY/2 - 40;
 
	//Highlighted?
	if(ActiveButton_Type == BUTTONTYPE_CONFIRM_NO) ColorPtr = &LC_Black;
	else ColorPtr = &LC_Yellow;
 
	//Text
	DrawJoyText(
		VerdanaFont,"No",xStart+30,yStart+20,
		*ColorPtr, DefaultFontScale,
		true
	);
 
	if (ButtonsConfirm.Num() < 2 )
	{
		FJoyButtonStruct newButton = FJoyButtonStruct();
		newButton.type 			= BUTTONTYPE_CONFIRM_NO;
		newButton.toolTip			= "";
		newButton.minX 			= xStart;
		newButton.maxX 			= xStart + 75;
		newButton.minY 			= yStart + 20;
		newButton.maxY 			= yStart + 60;
 
		//could use GetTextSize to streamline this
 
		//Add to correct array
		ButtonsConfirm.Add(newButton);
	}
}
 
//===============
// Cursor In Buttons
//===============
int32 AMainHUD::CheckCursorInButton(const TArray<FJoyButtonStruct>& ButtonArray)
{
	for (int32 b = 0; b < ButtonArray.Num(); b++){
		CurCheckButton = &ButtonArray[b];
 
		//check cursor in bounds
		if (CurCheckButton->minX <= MouseLocation.X && MouseLocation.X <= CurCheckButton->maxX &&
			CurCheckButton->minY <= MouseLocation.Y && MouseLocation.Y <= CurCheckButton->maxY){
 
			//Active Button Type
			ActiveButton_Type = CurCheckButton->type;
 
			//Tool Tip
			ActiveButton_Tip = CurCheckButton->toolTip;
 
			//Change Cursor
			CursorHoveringInButton = true;
 
			//Mouse Clicked?
			if (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton)){
				return ActiveButton_Type;
				//no need to check rest of buttons
			}
		}
	}
 
	//No Click Occurred This Tick
	return -1;
}
 
//Check Confirm
void AMainHUD::CheckCursorInButtonsConfirm()
{
	//Check Confirm Buttons
	ClickedButtonType = CheckCursorInButton(ButtonsConfirm); //fills global ActiveButton_Type
 
	if (ClickedButtonType == BUTTONTYPE_CONFIRM_YES){
		PC->ConsoleCommand("Exit");
		return;
	}
  
	if (ClickedButtonType == BUTTONTYPE_CONFIRM_NO){
		ConfirmDialogOpen = false;
		ButtonsConfirm.Empty(); //Buttons not needed anymore
		return;
	}
}
 
//Check Buttons
void AMainHUD::CheckCursorInButtonsMain()
{
	//Check Confirm Buttons
	ClickedButtonType = CheckCursorInButton(ButtonsMain);
 
	if (ClickedButtonType == BUTTONTYPE_MAIN_RESTART){
		PC->ConsoleCommand("RestartLevel");
		return;
	}
  
	if (ClickedButtonType == BUTTONTYPE_MAIN_EXIT){
		ConfirmDialogOpen = true;
		return;
	}
}

void AMainHUD::DrawHUD_CheckCursorInButtons()
{
	if (ConfirmDialogOpen){
		CheckCursorInButtonsConfirm();
 
		//Take Focus Away From All Other buttons
		return;
	}
 
	//Main
	CheckCursorInButtonsMain();
}
 
void AMainHUD::DrawToolTip()
{
	// If mouse is too far to right, draw from left instead
	float xStart = MouseLocation.X + 150;
	float yStart = MouseLocation.Y + 5;
 
	// Out vars
	float RV_xLength;
	float RV_yLength;
  
	// Text Size
	GetTextSize(
		ActiveButton_Tip,
		RV_xLength,
		RV_yLength,
		UE4Font,
		DefaultFontScale * 2
	);
 
	// Decide Draw to Left or to the Right
 
	//Draw to the Left
	if (xStart + RV_xLength >= Canvas->SizeX - 40){
		xStart -= 150 + 140 + 64 + RV_xLength;
 
		//If Text is too long, bring it closer to the cursor
		if (xStart < 33 ) xStart = 33;
	}
 
	//Background
	DrawJoyRect(
		xStart, yStart,
		RV_xLength + 70,
		80,
		FLinearColor(0, 0, 1, 0.7) //alpha 0.7
	);
 
	//Tool Tip
	DrawText(
		ActiveButton_Tip,
		LC_Pink,
		xStart + 32, yStart + 20,
		UE4Font,
		DefaultFontScale * 2,
		false		//scale position of message with HUD scale
	);
}

void AMainHUD::DrawHUD_DrawCursor(){
	//Tool Tip
	if (ActiveButton_Tip != "") DrawToolTip();
 
	//Cursor Hovering in a Button?
	if (CursorHoveringInButton){
		//pointer tex found?
		if (!CursorHoveringButton) return;
    
		DrawFullSizeTile(CursorHoveringButton, MouseLocation.X - CURSOR_DRAW_OFFSET, MouseLocation.Y - CURSOR_DRAW_OFFSET, FColor_White );
	} else {
		//cursor tex found?
		if (!CursorMain) return;
    
		DrawFullSizeTile(CursorMain, MouseLocation.X - CURSOR_DRAW_OFFSET, MouseLocation.Y - CURSOR_DRAW_OFFSET, FColor_White );
	}
}
 
void AMainHUD::PlayerInputChecks()
{
	//check out this tutorial of mine for a list of all EKeys::
	//http://forums.epicgames.com/threads/972861-Tutorials-C-for-UE4-Code-Samples-gt-gt-New-Video-Freeze-Render-When-Tabbed-Out?p=31660286&viewfull=1#post31660286
  
  // FInputKeyManager& FInputKeyManager::Get()
  // EModifierKey::Control
  // EModifierKey::Control, EKeys::Y
  // Click.GetKey() == EKeys::MiddleMouseButton && Click.IsAltDown()
  
  if (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton))
    Frame::Fire(MOUSE_LEFT_CLICK_DOWN);
  if (PC->WasInputKeyJustReleased(EKeys::LeftMouseButton)){
    float time = PC->GetInputKeyTimeDown(EKeys::LeftMouseButton);
    Frame::Fire(MOUSE_LEFT_CLICK_UP);}
    
  if (PC->WasInputKeyJustPressed(EKeys::RightMouseButton))
    Frame::Fire(MOUSE_RIGHT_CLICK_DOWN);
  if (PC->WasInputKeyJustReleased(EKeys::RightMouseButton)){
    float time = PC->GetInputKeyTimeDown(EKeys::RightMouseButton);
    Frame::Fire(MOUSE_RIGHT_CLICK_UP);}

  if (PC->WasInputKeyJustPressed(EKeys::MiddleMouseButton))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::Enter))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::Apostrophe))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::BackSpace))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::SpaceBar))
  {
    print("SpaceBar key");
  }
  if (PC->WasInputKeyJustPressed(EKeys::Down))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::Escape))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::F))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::G))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::A))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::Z))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::X))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::C))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::V))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::B))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::N))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::M))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::J))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::K))
  {
    
  }
  if (PC->WasInputKeyJustPressed(EKeys::L))
  {
    
  }
 
	if (PC->WasInputKeyJustPressed(EKeys::Escape)){
		SetCursorMoveOnly(false);
		return;
	}
  
	if (PC->WasInputKeyJustPressed(EKeys::F)){
		SetCursorMoveOnly(!PC->IsLookInputIgnored());
		return;
	}
  
  if (PC->WasInputKeyJustPressed(EKeys::Tab)){
    bShowMainMenu = !bShowMainMenu;
    print("Tab pressed!", bShowMainMenu);
    
    return;
  }
  
	if (PC->WasInputKeyJustPressed(EKeys::H)){
		DontDrawHUD = !DontDrawHUD;
		return;
	}
 
	//Confirm
	if (ConfirmDialogOpen){
		if (PC->WasInputKeyJustPressed(EKeys::Y)){
			PC->ConsoleCommand("Exit");
			//could replace with function based on confirm context
 
			return;
		}
    
		if(PC->WasInputKeyJustPressed(EKeys::N)){
			ConfirmDialogOpen = false;
			ButtonsConfirm.Empty(); //Buttons not needed anymore
			//Cancel Confirm
 
			return;
		}
	}
}
 
void AMainHUD::DrawHUD_Reset(){
	ActiveButton_Type = -1;
	ActiveButton_Tip = "";
	CursorHoveringInButton = false;
}
 
void AMainHUD::DrawHUD()
{
	// Have PC for Input Checks and Mouse Cursor?
	if (!PC)
  {
		// Attempt to Reacquire PC
		PC = GetOwningPlayerController();
 
		// Failed to find the PC
		if (!PC) return;
	}
 
	// Multiplayer Safety Check
	if (!PC->PlayerInput) return; //not valid for first seconds of a multiplayer client
 
	// Player Input
  CheckAllKeys();
	PlayerInputChecks();
 
	//Draw HUD?
	if (DontDrawHUD) return;
 
	//Super
	Super::DrawHUD();
 
	//No Canvas?
	if (!Canvas) return;
 
	//Reset States
	DrawHUD_Reset();
 
	//Get New Mouse Position
	PC->GetMousePosition(MouseLocation.X, MouseLocation.Y);
 
	//Cursor In Buttons
	DrawHUD_CheckCursorInButtons();
 
	//Draw Dialogs
	DrawHUD_DrawDialogs();
  
  DrawFrames();
  DrawPrintBox();
  DrawMessageBox();
  DrawMainMenu();
  
	//Draw Cursor
	DrawHUD_DrawCursor();
  
  Startup();
}

void GetDisplay()
{
  print("Trying to get display.");
}

APlayerController* AMainHUD::PC;

void CheckMouse(APlayerController* PC)
{
  if (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton))
    Frame::Fire(KEY_DOWN_LeftMouseButton);
  if (PC->WasInputKeyJustReleased(EKeys::LeftMouseButton)){
    float time = PC->GetInputKeyTimeDown(EKeys::LeftMouseButton);
    Frame::Fire(KEY_UP_LeftMouseButton);}
    
  if (PC->WasInputKeyJustPressed(EKeys::RightMouseButton))
    Frame::Fire(KEY_DOWN_RightMouseButton);
  if (PC->WasInputKeyJustReleased(EKeys::RightMouseButton)){
    float time = PC->GetInputKeyTimeDown(EKeys::RightMouseButton);
    Frame::Fire(KEY_UP_RightMouseButton);}
    
  if (PC->WasInputKeyJustPressed(EKeys::MiddleMouseButton))
    Frame::Fire(KEY_DOWN_MiddleMouseButton);
  if (PC->WasInputKeyJustReleased(EKeys::MiddleMouseButton)){
    float time = PC->GetInputKeyTimeDown(EKeys::MiddleMouseButton);
    Frame::Fire(KEY_UP_MiddleMouseButton);}
    
  if (PC->WasInputKeyJustPressed(EKeys::MouseScrollUp))
    Frame::Fire(KEY_DOWN_MouseScrollUp);
  if (PC->WasInputKeyJustReleased(EKeys::MouseScrollUp)){
    float time = PC->GetInputKeyTimeDown(EKeys::MouseScrollUp);
    Frame::Fire(KEY_UP_MouseScrollUp);}
  
  if (PC->WasInputKeyJustPressed(EKeys::MouseScrollDown))
    Frame::Fire(KEY_DOWN_MouseScrollDown);
  if (PC->WasInputKeyJustReleased(EKeys::MouseScrollDown)){
    float time = PC->GetInputKeyTimeDown(EKeys::MouseScrollDown);
    Frame::Fire(KEY_UP_MouseScrollDown);}
  
  if (PC->WasInputKeyJustPressed(EKeys::MouseWheelAxis))
    Frame::Fire(KEY_DOWN_MouseWheelAxis);
  if (PC->WasInputKeyJustReleased(EKeys::MouseWheelAxis)){
    float time = PC->GetInputKeyTimeDown(EKeys::MouseWheelAxis);
    Frame::Fire(KEY_UP_MouseWheelAxis);}
      
  if (PC->WasInputKeyJustPressed(EKeys::ThumbMouseButton))
    Frame::Fire(KEY_DOWN_ThumbMouseButton);
  if (PC->WasInputKeyJustReleased(EKeys::ThumbMouseButton)){
    float time = PC->GetInputKeyTimeDown(EKeys::ThumbMouseButton);
    Frame::Fire(KEY_UP_ThumbMouseButton);}
        
  if (PC->WasInputKeyJustPressed(EKeys::ThumbMouseButton2))
    Frame::Fire(KEY_DOWN_ThumbMouseButton2);
  if (PC->WasInputKeyJustReleased(EKeys::ThumbMouseButton2)){
    float time = PC->GetInputKeyTimeDown(EKeys::ThumbMouseButton2);
    Frame::Fire(KEY_UP_ThumbMouseButton2);}
}

void CheckAlphabet(APlayerController* PC)
{
  if (PC->WasInputKeyJustPressed(EKeys::A))
    Frame::Fire(KEY_DOWN_A);
  if (PC->WasInputKeyJustReleased(EKeys::A)){
    float time = PC->GetInputKeyTimeDown(EKeys::A);
    Frame::Fire(KEY_UP_A);}
  
  if (PC->WasInputKeyJustPressed(EKeys::B))
    Frame::Fire(KEY_DOWN_B);
  if (PC->WasInputKeyJustReleased(EKeys::B)){
    float time = PC->GetInputKeyTimeDown(EKeys::B);
    Frame::Fire(KEY_UP_B);}
  
  if (PC->WasInputKeyJustPressed(EKeys::C))
    Frame::Fire(KEY_DOWN_C);
  if (PC->WasInputKeyJustReleased(EKeys::C)){
    float time = PC->GetInputKeyTimeDown(EKeys::C);
    Frame::Fire(KEY_UP_C);}
  
  if (PC->WasInputKeyJustPressed(EKeys::D))
    Frame::Fire(KEY_DOWN_D);
  if (PC->WasInputKeyJustReleased(EKeys::D)){
    float time = PC->GetInputKeyTimeDown(EKeys::D);
    Frame::Fire(KEY_UP_D);}
  
  if (PC->WasInputKeyJustPressed(EKeys::E))
    Frame::Fire(KEY_DOWN_E);
  if (PC->WasInputKeyJustReleased(EKeys::E)){
    float time = PC->GetInputKeyTimeDown(EKeys::E);
    Frame::Fire(KEY_UP_E);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F))
    Frame::Fire(KEY_DOWN_F);
  if (PC->WasInputKeyJustReleased(EKeys::F)){
    float time = PC->GetInputKeyTimeDown(EKeys::F);
    Frame::Fire(KEY_UP_F);}
  
  if (PC->WasInputKeyJustPressed(EKeys::G))
    Frame::Fire(KEY_DOWN_G);
  if (PC->WasInputKeyJustReleased(EKeys::G)){
    float time = PC->GetInputKeyTimeDown(EKeys::G);
    Frame::Fire(KEY_UP_G);}
  
  if (PC->WasInputKeyJustPressed(EKeys::H))
    Frame::Fire(KEY_DOWN_H);
  if (PC->WasInputKeyJustReleased(EKeys::H)){
    float time = PC->GetInputKeyTimeDown(EKeys::H);
    Frame::Fire(KEY_UP_H);}
  
  if (PC->WasInputKeyJustPressed(EKeys::I))
    Frame::Fire(KEY_DOWN_I);
  if (PC->WasInputKeyJustReleased(EKeys::I)){
    float time = PC->GetInputKeyTimeDown(EKeys::I);
    Frame::Fire(KEY_UP_I);}
  
  if (PC->WasInputKeyJustPressed(EKeys::J))
    Frame::Fire(KEY_DOWN_J);
  if (PC->WasInputKeyJustReleased(EKeys::J)){
    float time = PC->GetInputKeyTimeDown(EKeys::J);
    Frame::Fire(KEY_UP_J);}
  
  if (PC->WasInputKeyJustPressed(EKeys::K))
    Frame::Fire(KEY_DOWN_K);
  if (PC->WasInputKeyJustReleased(EKeys::K)){
    float time = PC->GetInputKeyTimeDown(EKeys::K);
    Frame::Fire(KEY_UP_K);}
  
  if (PC->WasInputKeyJustPressed(EKeys::L))
    Frame::Fire(KEY_DOWN_L);
  if (PC->WasInputKeyJustReleased(EKeys::L)){
    float time = PC->GetInputKeyTimeDown(EKeys::L);
    Frame::Fire(KEY_UP_L);}
  
  if (PC->WasInputKeyJustPressed(EKeys::M))
    Frame::Fire(KEY_DOWN_M);
  if (PC->WasInputKeyJustReleased(EKeys::M)){
    float time = PC->GetInputKeyTimeDown(EKeys::M);
    Frame::Fire(KEY_UP_M);}
  
  if (PC->WasInputKeyJustPressed(EKeys::N))
    Frame::Fire(KEY_DOWN_N);
  if (PC->WasInputKeyJustReleased(EKeys::N)){
    float time = PC->GetInputKeyTimeDown(EKeys::N);
    Frame::Fire(KEY_UP_N);}
  
  if (PC->WasInputKeyJustPressed(EKeys::O))
    Frame::Fire(KEY_DOWN_O);
  if (PC->WasInputKeyJustReleased(EKeys::O)){
    float time = PC->GetInputKeyTimeDown(EKeys::O);
    Frame::Fire(KEY_UP_O);}
  
  if (PC->WasInputKeyJustPressed(EKeys::P))
    Frame::Fire(KEY_DOWN_P);
  if (PC->WasInputKeyJustReleased(EKeys::P)){
    float time = PC->GetInputKeyTimeDown(EKeys::P);
    Frame::Fire(KEY_UP_P);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Q))
    Frame::Fire(KEY_DOWN_Q);
  if (PC->WasInputKeyJustReleased(EKeys::Q)){
    float time = PC->GetInputKeyTimeDown(EKeys::Q);
    Frame::Fire(KEY_UP_Q);}
  
  if (PC->WasInputKeyJustPressed(EKeys::R))
    Frame::Fire(KEY_DOWN_R);
  if (PC->WasInputKeyJustReleased(EKeys::R)){
    float time = PC->GetInputKeyTimeDown(EKeys::R);
    Frame::Fire(KEY_UP_R);}
  
  if (PC->WasInputKeyJustPressed(EKeys::S))
    Frame::Fire(KEY_DOWN_S);
  if (PC->WasInputKeyJustReleased(EKeys::S)){
    float time = PC->GetInputKeyTimeDown(EKeys::S);
    Frame::Fire(KEY_UP_S);}
  
  if (PC->WasInputKeyJustPressed(EKeys::T))
    Frame::Fire(KEY_DOWN_T);
  if (PC->WasInputKeyJustReleased(EKeys::T)){
    float time = PC->GetInputKeyTimeDown(EKeys::T);
    Frame::Fire(KEY_UP_T);}
  
  if (PC->WasInputKeyJustPressed(EKeys::U))
    Frame::Fire(KEY_DOWN_U);
  if (PC->WasInputKeyJustReleased(EKeys::U)){
    float time = PC->GetInputKeyTimeDown(EKeys::U);
    Frame::Fire(KEY_UP_U);}
  
  if (PC->WasInputKeyJustPressed(EKeys::V))
    Frame::Fire(KEY_DOWN_V);
  if (PC->WasInputKeyJustReleased(EKeys::V)){
    float time = PC->GetInputKeyTimeDown(EKeys::V);
    Frame::Fire(KEY_UP_V);}
  
  if (PC->WasInputKeyJustPressed(EKeys::W))
    Frame::Fire(KEY_DOWN_W);
  if (PC->WasInputKeyJustReleased(EKeys::W)){
    float time = PC->GetInputKeyTimeDown(EKeys::W);
    Frame::Fire(KEY_UP_W);}
  
  if (PC->WasInputKeyJustPressed(EKeys::X))
    Frame::Fire(KEY_DOWN_X);
  if (PC->WasInputKeyJustReleased(EKeys::X)){
    float time = PC->GetInputKeyTimeDown(EKeys::X);
    Frame::Fire(KEY_UP_X);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Y))
    Frame::Fire(KEY_DOWN_Y);
  if (PC->WasInputKeyJustReleased(EKeys::Y)){
    float time = PC->GetInputKeyTimeDown(EKeys::Y);
    Frame::Fire(KEY_UP_Y);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Z))
    Frame::Fire(KEY_DOWN_Z);
  if (PC->WasInputKeyJustReleased(EKeys::Z)){
    float time = PC->GetInputKeyTimeDown(EKeys::Z);
    Frame::Fire(KEY_UP_Z);}
}

void CheckNumbers(APlayerController* PC)
{
  if (PC->WasInputKeyJustPressed(EKeys::Zero))
    Frame::Fire(KEY_DOWN_Zero);
  if (PC->WasInputKeyJustReleased(EKeys::Zero)){
    float time = PC->GetInputKeyTimeDown(EKeys::Zero);
    Frame::Fire(KEY_UP_Zero);}
  
  if (PC->WasInputKeyJustPressed(EKeys::One))
    Frame::Fire(KEY_DOWN_One);
  if (PC->WasInputKeyJustReleased(EKeys::One)){
    float time = PC->GetInputKeyTimeDown(EKeys::One);
    Frame::Fire(KEY_UP_One);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Two))
    Frame::Fire(KEY_DOWN_Two);
  if (PC->WasInputKeyJustReleased(EKeys::Two)){
    float time = PC->GetInputKeyTimeDown(EKeys::Two);
    Frame::Fire(KEY_UP_Two);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Three))
    Frame::Fire(KEY_DOWN_Three);
  if (PC->WasInputKeyJustReleased(EKeys::Three)){
    float time = PC->GetInputKeyTimeDown(EKeys::Three);
    Frame::Fire(KEY_UP_Three);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Four))
    Frame::Fire(KEY_DOWN_Four);
  if (PC->WasInputKeyJustReleased(EKeys::Four)){
    float time = PC->GetInputKeyTimeDown(EKeys::Four);
    Frame::Fire(KEY_UP_Four);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Five))
    Frame::Fire(KEY_DOWN_Five);
  if (PC->WasInputKeyJustReleased(EKeys::Five)){
    float time = PC->GetInputKeyTimeDown(EKeys::Five);
    Frame::Fire(KEY_UP_Five);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Six))
    Frame::Fire(KEY_DOWN_Six);
  if (PC->WasInputKeyJustReleased(EKeys::Six)){
    float time = PC->GetInputKeyTimeDown(EKeys::Six);
    Frame::Fire(KEY_UP_Six);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Seven))
    Frame::Fire(KEY_DOWN_Seven);
  if (PC->WasInputKeyJustReleased(EKeys::Seven)){
    float time = PC->GetInputKeyTimeDown(EKeys::Seven);
    Frame::Fire(KEY_UP_Seven);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Eight))
    Frame::Fire(KEY_DOWN_Eight);
  if (PC->WasInputKeyJustReleased(EKeys::Eight)){
    float time = PC->GetInputKeyTimeDown(EKeys::Eight);
    Frame::Fire(KEY_UP_Eight);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Nine))
    Frame::Fire(KEY_DOWN_Nine);
  if (PC->WasInputKeyJustReleased(EKeys::Nine)){
    float time = PC->GetInputKeyTimeDown(EKeys::Nine);
    Frame::Fire(KEY_UP_Nine);}
}

void CheckFKeys(APlayerController* PC)
{
  if (PC->WasInputKeyJustPressed(EKeys::F1))
    Frame::Fire(KEY_DOWN_F1);
  if (PC->WasInputKeyJustReleased(EKeys::F1)){
    float time = PC->GetInputKeyTimeDown(EKeys::F1);
    Frame::Fire(KEY_UP_F1);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F2))
    Frame::Fire(KEY_DOWN_F2);
  if (PC->WasInputKeyJustReleased(EKeys::F2)){
    float time = PC->GetInputKeyTimeDown(EKeys::F2);
    Frame::Fire(KEY_UP_F2);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F3))
    Frame::Fire(KEY_DOWN_F3);
  if (PC->WasInputKeyJustReleased(EKeys::F3)){
    float time = PC->GetInputKeyTimeDown(EKeys::F3);
    Frame::Fire(KEY_UP_F3);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F4))
    Frame::Fire(KEY_DOWN_F4);
  if (PC->WasInputKeyJustReleased(EKeys::F4)){
    float time = PC->GetInputKeyTimeDown(EKeys::F4);
    Frame::Fire(KEY_UP_F4);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F5))
    Frame::Fire(KEY_DOWN_F5);
  if (PC->WasInputKeyJustReleased(EKeys::F5)){
    float time = PC->GetInputKeyTimeDown(EKeys::F5);
    Frame::Fire(KEY_UP_F5);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F6))
    Frame::Fire(KEY_DOWN_F6);
  if (PC->WasInputKeyJustReleased(EKeys::F6)){
    float time = PC->GetInputKeyTimeDown(EKeys::F6);
    Frame::Fire(KEY_UP_F6);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F7))
    Frame::Fire(KEY_DOWN_F7);
  if (PC->WasInputKeyJustReleased(EKeys::F7)){
    float time = PC->GetInputKeyTimeDown(EKeys::F7);
    Frame::Fire(KEY_UP_F7);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F8))
    Frame::Fire(KEY_DOWN_F8);
  if (PC->WasInputKeyJustReleased(EKeys::F8)){
    float time = PC->GetInputKeyTimeDown(EKeys::F8);
    Frame::Fire(KEY_UP_F8);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F9))
    Frame::Fire(KEY_DOWN_F9);
  if (PC->WasInputKeyJustReleased(EKeys::F9)){
    float time = PC->GetInputKeyTimeDown(EKeys::F9);
    Frame::Fire(KEY_UP_F9);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F10))
    Frame::Fire(KEY_DOWN_F10);
  if (PC->WasInputKeyJustReleased(EKeys::F10)){
    float time = PC->GetInputKeyTimeDown(EKeys::F10);
    Frame::Fire(KEY_UP_F10);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F11))
    Frame::Fire(KEY_DOWN_F11);
  if (PC->WasInputKeyJustReleased(EKeys::F11)){
    float time = PC->GetInputKeyTimeDown(EKeys::F11);
    Frame::Fire(KEY_UP_F11);}
  
  if (PC->WasInputKeyJustPressed(EKeys::F12))
    Frame::Fire(KEY_DOWN_F12);
  if (PC->WasInputKeyJustReleased(EKeys::F12)){
    float time = PC->GetInputKeyTimeDown(EKeys::F12);
    Frame::Fire(KEY_UP_F12);}
}

void CheckModKeys(APlayerController* PC)
{
  if (PC->WasInputKeyJustPressed(EKeys::NumLock))
    Frame::Fire(KEY_DOWN_NumLock);
  if (PC->WasInputKeyJustReleased(EKeys::NumLock)){
    float time = PC->GetInputKeyTimeDown(EKeys::NumLock);
    Frame::Fire(KEY_UP_NumLock);}
  
  if (PC->WasInputKeyJustPressed(EKeys::ScrollLock))
    Frame::Fire(KEY_DOWN_ScrollLock);
  if (PC->WasInputKeyJustReleased(EKeys::ScrollLock)){
    float time = PC->GetInputKeyTimeDown(EKeys::ScrollLock);
    Frame::Fire(KEY_UP_ScrollLock);}
  
  if (PC->WasInputKeyJustPressed(EKeys::LeftShift))
    Frame::Fire(KEY_DOWN_LeftShift);
  if (PC->WasInputKeyJustReleased(EKeys::LeftShift)){
    float time = PC->GetInputKeyTimeDown(EKeys::LeftShift);
    Frame::Fire(KEY_UP_LeftShift);}
  
  if (PC->WasInputKeyJustPressed(EKeys::RightShift))
    Frame::Fire(KEY_DOWN_RightShift);
  if (PC->WasInputKeyJustReleased(EKeys::RightShift)){
    float time = PC->GetInputKeyTimeDown(EKeys::RightShift);
    Frame::Fire(KEY_UP_RightShift);}
  
  if (PC->WasInputKeyJustPressed(EKeys::LeftControl))
    Frame::Fire(KEY_DOWN_LeftControl);
  if (PC->WasInputKeyJustReleased(EKeys::LeftControl)){
    float time = PC->GetInputKeyTimeDown(EKeys::LeftControl);
    Frame::Fire(KEY_UP_LeftControl);}
  
  if (PC->WasInputKeyJustPressed(EKeys::RightControl))
    Frame::Fire(KEY_DOWN_RightControl);
  if (PC->WasInputKeyJustReleased(EKeys::RightControl)){
    float time = PC->GetInputKeyTimeDown(EKeys::RightControl);
    Frame::Fire(KEY_UP_RightControl);}
  
  if (PC->WasInputKeyJustPressed(EKeys::LeftAlt))
    Frame::Fire(KEY_DOWN_LeftAlt);
  if (PC->WasInputKeyJustReleased(EKeys::LeftAlt)){
    float time = PC->GetInputKeyTimeDown(EKeys::LeftAlt);
    Frame::Fire(KEY_UP_LeftAlt);}
  
  if (PC->WasInputKeyJustPressed(EKeys::RightAlt))
    Frame::Fire(KEY_DOWN_RightAlt);
  if (PC->WasInputKeyJustReleased(EKeys::RightAlt)){
    float time = PC->GetInputKeyTimeDown(EKeys::RightAlt);
    Frame::Fire(KEY_UP_RightAlt);}
  
  if (PC->WasInputKeyJustPressed(EKeys::LeftCommand))
    Frame::Fire(KEY_DOWN_LeftCommand);
  if (PC->WasInputKeyJustReleased(EKeys::LeftCommand)){
    float time = PC->GetInputKeyTimeDown(EKeys::LeftCommand);
    Frame::Fire(KEY_UP_LeftCommand);}
  
  if (PC->WasInputKeyJustPressed(EKeys::RightCommand))
    Frame::Fire(KEY_DOWN_RightCommand);
  if (PC->WasInputKeyJustReleased(EKeys::RightCommand)){
    float time = PC->GetInputKeyTimeDown(EKeys::RightCommand);
    Frame::Fire(KEY_UP_RightCommand);}
}

void AMainHUD::CheckAllKeys()
{
  if (true) return;
  
  // This function profiles at about 277 times per millisecond with all enabled
  
  // Later, have an array of possible actions and let each one be assigned to
  // any key, then iterate through that array each frame, dropping the EKeys
  // contained into the input checks
  // The cost of the array should easily be made up for by the gain of not
  // checking everything
  
  bool bNumberKeys = true;
  
  bool bExtraKeys = false;
  bool bNumPadKeys = false;
  bool bMathKeys = false;
  bool bArrowKeys = false;
  
  CheckModKeys(PC);
  CheckMouse(PC);
  CheckAlphabet(PC);
  CheckNumbers(PC);
  CheckFKeys(PC);
  
  // if (PC->WasInputKeyJustPressed(EKeys::MouseX))
  // 	Frame::Fire(KEY_DOWN_MouseX);
  // if (PC->WasInputKeyJustReleased(EKeys::MouseX)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::MouseX);
  // 	Frame::Fire(KEY_UP_MouseX);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::MouseY))
  // 	Frame::Fire(KEY_DOWN_MouseY);
  // if (PC->WasInputKeyJustReleased(EKeys::MouseY)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::MouseY);
  // 	Frame::Fire(KEY_UP_MouseY);}
  
  if (PC->WasInputKeyJustPressed(EKeys::BackSpace))
  	Frame::Fire(KEY_DOWN_BackSpace);
  if (PC->WasInputKeyJustReleased(EKeys::BackSpace)){
  	float time = PC->GetInputKeyTimeDown(EKeys::BackSpace);
  	Frame::Fire(KEY_UP_BackSpace);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Tab))
  	Frame::Fire(KEY_DOWN_Tab);
  if (PC->WasInputKeyJustReleased(EKeys::Tab)){
  	float time = PC->GetInputKeyTimeDown(EKeys::Tab);
  	Frame::Fire(KEY_UP_Tab);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Enter))
  	Frame::Fire(KEY_DOWN_Enter);
  if (PC->WasInputKeyJustReleased(EKeys::Enter)){
  	float time = PC->GetInputKeyTimeDown(EKeys::Enter);
  	Frame::Fire(KEY_UP_Enter);}
    
  if (PC->WasInputKeyJustPressed(EKeys::Delete))
    Frame::Fire(KEY_DOWN_Delete);
  if (PC->WasInputKeyJustReleased(EKeys::Delete)){
    float time = PC->GetInputKeyTimeDown(EKeys::Delete);
    Frame::Fire(KEY_UP_Delete);}
  
  if (bExtraKeys)
  {
    if (PC->WasInputKeyJustPressed(EKeys::Insert))
      Frame::Fire(KEY_DOWN_Insert);
    if (PC->WasInputKeyJustReleased(EKeys::Insert)){
      float time = PC->GetInputKeyTimeDown(EKeys::Insert);
      Frame::Fire(KEY_UP_Insert);}
    
    if (PC->WasInputKeyJustPressed(EKeys::Pause))
      Frame::Fire(KEY_DOWN_Pause);
    if (PC->WasInputKeyJustReleased(EKeys::Pause)){
      float time = PC->GetInputKeyTimeDown(EKeys::Pause);
      Frame::Fire(KEY_UP_Pause);}
      
    if (PC->WasInputKeyJustPressed(EKeys::PageUp))
    	Frame::Fire(KEY_DOWN_PageUp);
    if (PC->WasInputKeyJustReleased(EKeys::PageUp)){
    	float time = PC->GetInputKeyTimeDown(EKeys::PageUp);
    	Frame::Fire(KEY_UP_PageUp);}
    
    if (PC->WasInputKeyJustPressed(EKeys::PageDown))
    	Frame::Fire(KEY_DOWN_PageDown);
    if (PC->WasInputKeyJustReleased(EKeys::PageDown)){
    	float time = PC->GetInputKeyTimeDown(EKeys::PageDown);
    	Frame::Fire(KEY_UP_PageDown);}
    
    if (PC->WasInputKeyJustPressed(EKeys::End))
    	Frame::Fire(KEY_DOWN_End);
    if (PC->WasInputKeyJustReleased(EKeys::End)){
    	float time = PC->GetInputKeyTimeDown(EKeys::End);
    	Frame::Fire(KEY_UP_End);}
    
    if (PC->WasInputKeyJustPressed(EKeys::Home))
    	Frame::Fire(KEY_DOWN_Home);
    if (PC->WasInputKeyJustReleased(EKeys::Home)){
    	float time = PC->GetInputKeyTimeDown(EKeys::Home);
    	Frame::Fire(KEY_UP_Home);}
  }
  
  if (PC->WasInputKeyJustPressed(EKeys::CapsLock))
  	Frame::Fire(KEY_DOWN_CapsLock);
  if (PC->WasInputKeyJustReleased(EKeys::CapsLock)){
  	float time = PC->GetInputKeyTimeDown(EKeys::CapsLock);
  	Frame::Fire(KEY_UP_CapsLock);}
  
  if (PC->WasInputKeyJustPressed(EKeys::Escape))
  	Frame::Fire(KEY_DOWN_Escape);
  if (PC->WasInputKeyJustReleased(EKeys::Escape)){
  	float time = PC->GetInputKeyTimeDown(EKeys::Escape);
  	Frame::Fire(KEY_UP_Escape);}
  
  if (PC->WasInputKeyJustPressed(EKeys::SpaceBar))
  	Frame::Fire(KEY_DOWN_SpaceBar);
  if (PC->WasInputKeyJustReleased(EKeys::SpaceBar)){
  	float time = PC->GetInputKeyTimeDown(EKeys::SpaceBar);
  	Frame::Fire(KEY_UP_SpaceBar);}
  
  if (bArrowKeys)
  {
    if (PC->WasInputKeyJustPressed(EKeys::Left))
      Frame::Fire(KEY_DOWN_Left);
    if (PC->WasInputKeyJustReleased(EKeys::Left)){
      float time = PC->GetInputKeyTimeDown(EKeys::Left);
      Frame::Fire(KEY_UP_Left);}
      
    if (PC->WasInputKeyJustPressed(EKeys::Up))
      Frame::Fire(KEY_DOWN_Up);
    if (PC->WasInputKeyJustReleased(EKeys::Up)){
      float time = PC->GetInputKeyTimeDown(EKeys::Up);
      Frame::Fire(KEY_UP_Up);}
      
    if (PC->WasInputKeyJustPressed(EKeys::Right))
      Frame::Fire(KEY_DOWN_Right);
    if (PC->WasInputKeyJustReleased(EKeys::Right)){
      float time = PC->GetInputKeyTimeDown(EKeys::Right);
      Frame::Fire(KEY_UP_Right);}
        
    if (PC->WasInputKeyJustPressed(EKeys::Down))
      Frame::Fire(KEY_DOWN_Down);
    if (PC->WasInputKeyJustReleased(EKeys::Down)){
      float time = PC->GetInputKeyTimeDown(EKeys::Down);
      Frame::Fire(KEY_UP_Down);}
  }
  
  if (bNumPadKeys)
  {
    if (PC->WasInputKeyJustPressed(EKeys::NumPadZero))
      Frame::Fire(KEY_DOWN_NumPadZero);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadZero)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadZero);
      Frame::Fire(KEY_UP_NumPadZero);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadOne))
      Frame::Fire(KEY_DOWN_NumPadOne);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadOne)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadOne);
      Frame::Fire(KEY_UP_NumPadOne);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadTwo))
      Frame::Fire(KEY_DOWN_NumPadTwo);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadTwo)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadTwo);
      Frame::Fire(KEY_UP_NumPadTwo);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadThree))
      Frame::Fire(KEY_DOWN_NumPadThree);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadThree)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadThree);
      Frame::Fire(KEY_UP_NumPadThree);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadFour))
      Frame::Fire(KEY_DOWN_NumPadFour);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadFour)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadFour);
      Frame::Fire(KEY_UP_NumPadFour);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadFive))
      Frame::Fire(KEY_DOWN_NumPadFive);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadFive)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadFive);
      Frame::Fire(KEY_UP_NumPadFive);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadSix))
      Frame::Fire(KEY_DOWN_NumPadSix);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadSix)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadSix);
      Frame::Fire(KEY_UP_NumPadSix);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadSeven))
      Frame::Fire(KEY_DOWN_NumPadSeven);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadSeven)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadSeven);
      Frame::Fire(KEY_UP_NumPadSeven);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadEight))
      Frame::Fire(KEY_DOWN_NumPadEight);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadEight)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadEight);
      Frame::Fire(KEY_UP_NumPadEight);}
    
    if (PC->WasInputKeyJustPressed(EKeys::NumPadNine))
      Frame::Fire(KEY_DOWN_NumPadNine);
    if (PC->WasInputKeyJustReleased(EKeys::NumPadNine)){
      float time = PC->GetInputKeyTimeDown(EKeys::NumPadNine);
      Frame::Fire(KEY_UP_NumPadNine);}
  }
  
  if (bMathKeys)
  {
    if (PC->WasInputKeyJustPressed(EKeys::Multiply))
    	Frame::Fire(KEY_DOWN_Multiply);
    if (PC->WasInputKeyJustReleased(EKeys::Multiply)){
    	float time = PC->GetInputKeyTimeDown(EKeys::Multiply);
    	Frame::Fire(KEY_UP_Multiply);}
    
    if (PC->WasInputKeyJustPressed(EKeys::Add))
    	Frame::Fire(KEY_DOWN_Add);
    if (PC->WasInputKeyJustReleased(EKeys::Add)){
    	float time = PC->GetInputKeyTimeDown(EKeys::Add);
    	Frame::Fire(KEY_UP_Add);}
    
    if (PC->WasInputKeyJustPressed(EKeys::Subtract))
    	Frame::Fire(KEY_DOWN_Subtract);
    if (PC->WasInputKeyJustReleased(EKeys::Subtract)){
    	float time = PC->GetInputKeyTimeDown(EKeys::Subtract);
    	Frame::Fire(KEY_UP_Subtract);}
    
    if (PC->WasInputKeyJustPressed(EKeys::Decimal))
    	Frame::Fire(KEY_DOWN_Decimal);
    if (PC->WasInputKeyJustReleased(EKeys::Decimal)){
    	float time = PC->GetInputKeyTimeDown(EKeys::Decimal);
    	Frame::Fire(KEY_UP_Decimal);}
    
    if (PC->WasInputKeyJustPressed(EKeys::Divide))
    	Frame::Fire(KEY_DOWN_Divide);
    if (PC->WasInputKeyJustReleased(EKeys::Divide)){
    	float time = PC->GetInputKeyTimeDown(EKeys::Divide);
    	Frame::Fire(KEY_UP_Divide);}
  }
  
  
  // if (PC->WasInputKeyJustPressed(EKeys::Semicolon))
  // 	Frame::Fire(KEY_DOWN_Semicolon);
  // if (PC->WasInputKeyJustReleased(EKeys::Semicolon)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Semicolon);
  // 	Frame::Fire(KEY_UP_Semicolon);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Equals))
  // 	Frame::Fire(KEY_DOWN_Equals);
  // if (PC->WasInputKeyJustReleased(EKeys::Equals)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Equals);
  // 	Frame::Fire(KEY_UP_Equals);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Comma))
  // 	Frame::Fire(KEY_DOWN_Comma);
  // if (PC->WasInputKeyJustReleased(EKeys::Comma)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Comma);
  // 	Frame::Fire(KEY_UP_Comma);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Underscore))
  // 	Frame::Fire(KEY_DOWN_Underscore);
  // if (PC->WasInputKeyJustReleased(EKeys::Underscore)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Underscore);
  // 	Frame::Fire(KEY_UP_Underscore);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Hyphen))
  // 	Frame::Fire(KEY_DOWN_Hyphen);
  // if (PC->WasInputKeyJustReleased(EKeys::Hyphen)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Hyphen);
  // 	Frame::Fire(KEY_UP_Hyphen);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Period))
  // 	Frame::Fire(KEY_DOWN_Period);
  // if (PC->WasInputKeyJustReleased(EKeys::Period)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Period);
  // 	Frame::Fire(KEY_UP_Period);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Slash))
  // 	Frame::Fire(KEY_DOWN_Slash);
  // if (PC->WasInputKeyJustReleased(EKeys::Slash)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Slash);
  // 	Frame::Fire(KEY_UP_Slash);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Tilde))
  // 	Frame::Fire(KEY_DOWN_Tilde);
  // if (PC->WasInputKeyJustReleased(EKeys::Tilde)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Tilde);
  // 	Frame::Fire(KEY_UP_Tilde);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::LeftBracket))
  // 	Frame::Fire(KEY_DOWN_LeftBracket);
  // if (PC->WasInputKeyJustReleased(EKeys::LeftBracket)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::LeftBracket);
  // 	Frame::Fire(KEY_UP_LeftBracket);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::LeftParantheses))
  // 	Frame::Fire(KEY_DOWN_LeftParantheses);
  // if (PC->WasInputKeyJustReleased(EKeys::LeftParantheses)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::LeftParantheses);
  // 	Frame::Fire(KEY_UP_LeftParantheses);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Backslash))
  // 	Frame::Fire(KEY_DOWN_Backslash);
  // if (PC->WasInputKeyJustReleased(EKeys::Backslash)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Backslash);
  // 	Frame::Fire(KEY_UP_Backslash);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::RightBracket))
  // 	Frame::Fire(KEY_DOWN_RightBracket);
  // if (PC->WasInputKeyJustReleased(EKeys::RightBracket)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::RightBracket);
  // 	Frame::Fire(KEY_UP_RightBracket);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::RightParantheses))
  // 	Frame::Fire(KEY_DOWN_RightParantheses);
  // if (PC->WasInputKeyJustReleased(EKeys::RightParantheses)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::RightParantheses);
  // 	Frame::Fire(KEY_UP_RightParantheses);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Apostrophe))
  // 	Frame::Fire(KEY_DOWN_Apostrophe);
  // if (PC->WasInputKeyJustReleased(EKeys::Apostrophe)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Apostrophe);
  // 	Frame::Fire(KEY_UP_Apostrophe);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Quote))
  // 	Frame::Fire(KEY_DOWN_Quote);
  // if (PC->WasInputKeyJustReleased(EKeys::Quote)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Quote);
  // 	Frame::Fire(KEY_UP_Quote);}
  //
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Asterix))
  // 	Frame::Fire(KEY_DOWN_Asterix);
  // if (PC->WasInputKeyJustReleased(EKeys::Asterix)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Asterix);
  // 	Frame::Fire(KEY_UP_Asterix);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Ampersand))
  // 	Frame::Fire(KEY_DOWN_Ampersand);
  // if (PC->WasInputKeyJustReleased(EKeys::Ampersand)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Ampersand);
  // 	Frame::Fire(KEY_UP_Ampersand);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Caret))
  // 	Frame::Fire(KEY_DOWN_Caret);
  // if (PC->WasInputKeyJustReleased(EKeys::Caret)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Caret);
  // 	Frame::Fire(KEY_UP_Caret);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Dollar))
  // 	Frame::Fire(KEY_DOWN_Dollar);
  // if (PC->WasInputKeyJustReleased(EKeys::Dollar)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Dollar);
  // 	Frame::Fire(KEY_UP_Dollar);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Exclamation))
  // 	Frame::Fire(KEY_DOWN_Exclamation);
  // if (PC->WasInputKeyJustReleased(EKeys::Exclamation)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Exclamation);
  // 	Frame::Fire(KEY_UP_Exclamation);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Colon))
  // 	Frame::Fire(KEY_DOWN_Colon);
  // if (PC->WasInputKeyJustReleased(EKeys::Colon)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Colon);
  // 	Frame::Fire(KEY_UP_Colon);}
  
  
  // if (PC->WasInputKeyJustPressed(EKeys::A_AccentGrave))
  // 	Frame::Fire(KEY_DOWN_A_AccentGrave);
  // if (PC->WasInputKeyJustReleased(EKeys::A_AccentGrave)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::A_AccentGrave);
  // 	Frame::Fire(KEY_UP_A_AccentGrave);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::E_AccentGrave))
  // 	Frame::Fire(KEY_DOWN_E_AccentGrave);
  // if (PC->WasInputKeyJustReleased(EKeys::E_AccentGrave)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::E_AccentGrave);
  // 	Frame::Fire(KEY_UP_E_AccentGrave);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::E_AccentAigu))
  // 	Frame::Fire(KEY_DOWN_E_AccentAigu);
  // if (PC->WasInputKeyJustReleased(EKeys::E_AccentAigu)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::E_AccentAigu);
  // 	Frame::Fire(KEY_UP_E_AccentAigu);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::C_Cedille))
  // 	Frame::Fire(KEY_DOWN_C_Cedille);
  // if (PC->WasInputKeyJustReleased(EKeys::C_Cedille)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::C_Cedille);
  // 	Frame::Fire(KEY_UP_C_Cedille);}
  //
  // if (PC->WasInputKeyJustPressed(EKeys::Section))
  // 	Frame::Fire(KEY_DOWN_Section);
  // if (PC->WasInputKeyJustReleased(EKeys::Section)){
  // 	float time = PC->GetInputKeyTimeDown(EKeys::Section);
  // 	Frame::Fire(KEY_UP_Section);}
}

// void AMainHUD::CheckGamepadButtons()
// {
//   // Ensure that the Gamepad_ names match those in GenericApplication.cpp
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftX))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftX);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftX)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftX);
//     Frame::Fire(KEY_UP_Gamepad_LeftX);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftY))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftY);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftY)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftY);
//     Frame::Fire(KEY_UP_Gamepad_LeftY);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightX))
//     Frame::Fire(KEY_DOWN_Gamepad_RightX);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightX)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightX);
//     Frame::Fire(KEY_UP_Gamepad_RightX);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightY))
//     Frame::Fire(KEY_DOWN_Gamepad_RightY);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightY)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightY);
//     Frame::Fire(KEY_UP_Gamepad_RightY);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftTriggerAxis))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftTriggerAxis);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftTriggerAxis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftTriggerAxis);
//     Frame::Fire(KEY_UP_Gamepad_LeftTriggerAxis);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightTriggerAxis))
//     Frame::Fire(KEY_DOWN_Gamepad_RightTriggerAxis);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightTriggerAxis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightTriggerAxis);
//     Frame::Fire(KEY_UP_Gamepad_RightTriggerAxis);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftThumbstick))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftThumbstick);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftThumbstick)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftThumbstick);
//     Frame::Fire(KEY_UP_Gamepad_LeftThumbstick);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightThumbstick))
//     Frame::Fire(KEY_DOWN_Gamepad_RightThumbstick);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightThumbstick)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightThumbstick);
//     Frame::Fire(KEY_UP_Gamepad_RightThumbstick);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_Special_Left))
//     Frame::Fire(KEY_DOWN_Gamepad_Special_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_Special_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_Special_Left);
//     Frame::Fire(KEY_UP_Gamepad_Special_Left);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_Special_Right))
//     Frame::Fire(KEY_DOWN_Gamepad_Special_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_Special_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_Special_Right);
//     Frame::Fire(KEY_UP_Gamepad_Special_Right);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Bottom))
//     Frame::Fire(KEY_DOWN_Gamepad_FaceButton_Bottom);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_FaceButton_Bottom)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_FaceButton_Bottom);
//     Frame::Fire(KEY_UP_Gamepad_FaceButton_Bottom);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right))
//     Frame::Fire(KEY_DOWN_Gamepad_FaceButton_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_FaceButton_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_FaceButton_Right);
//     Frame::Fire(KEY_UP_Gamepad_FaceButton_Right);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Left))
//     Frame::Fire(KEY_DOWN_Gamepad_FaceButton_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_FaceButton_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_FaceButton_Left);
//     Frame::Fire(KEY_UP_Gamepad_FaceButton_Left);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Top))
//     Frame::Fire(KEY_DOWN_Gamepad_FaceButton_Top);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_FaceButton_Top)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_FaceButton_Top);
//     Frame::Fire(KEY_UP_Gamepad_FaceButton_Top);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftShoulder))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftShoulder);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftShoulder)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftShoulder);
//     Frame::Fire(KEY_UP_Gamepad_LeftShoulder);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightShoulder))
//     Frame::Fire(KEY_DOWN_Gamepad_RightShoulder);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightShoulder)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightShoulder);
//     Frame::Fire(KEY_UP_Gamepad_RightShoulder);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftTrigger))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftTrigger);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftTrigger)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftTrigger);
//     Frame::Fire(KEY_UP_Gamepad_LeftTrigger);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightTrigger))
//     Frame::Fire(KEY_DOWN_Gamepad_RightTrigger);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightTrigger)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightTrigger);
//     Frame::Fire(KEY_UP_Gamepad_RightTrigger);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Up))
//     Frame::Fire(KEY_DOWN_Gamepad_DPad_Up);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_DPad_Up)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_DPad_Up);
//     Frame::Fire(KEY_UP_Gamepad_DPad_Up);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Down))
//     Frame::Fire(KEY_DOWN_Gamepad_DPad_Down);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_DPad_Down)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_DPad_Down);
//     Frame::Fire(KEY_UP_Gamepad_DPad_Down);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Right))
//     Frame::Fire(KEY_DOWN_Gamepad_DPad_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_DPad_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_DPad_Right);
//     Frame::Fire(KEY_UP_Gamepad_DPad_Right);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Left))
//     Frame::Fire(KEY_DOWN_Gamepad_DPad_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_DPad_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_DPad_Left);
//     Frame::Fire(KEY_UP_Gamepad_DPad_Left);}
//
//
//   // Virtual key codes used for input axis button press/release emulation
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Up))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftStick_Up);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftStick_Up)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftStick_Up);
//     Frame::Fire(KEY_UP_Gamepad_LeftStick_Up);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Down))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftStick_Down);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftStick_Down)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftStick_Down);
//     Frame::Fire(KEY_UP_Gamepad_LeftStick_Down);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Right))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftStick_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftStick_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftStick_Right);
//     Frame::Fire(KEY_UP_Gamepad_LeftStick_Right);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Left))
//     Frame::Fire(KEY_DOWN_Gamepad_LeftStick_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_LeftStick_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_LeftStick_Left);
//     Frame::Fire(KEY_UP_Gamepad_LeftStick_Left);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightStick_Up))
//     Frame::Fire(KEY_DOWN_Gamepad_RightStick_Up);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightStick_Up)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightStick_Up);
//     Frame::Fire(KEY_UP_Gamepad_RightStick_Up);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightStick_Down))
//     Frame::Fire(KEY_DOWN_Gamepad_RightStick_Down);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightStick_Down)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightStick_Down);
//     Frame::Fire(KEY_UP_Gamepad_RightStick_Down);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightStick_Right))
//     Frame::Fire(KEY_DOWN_Gamepad_RightStick_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightStick_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightStick_Right);
//     Frame::Fire(KEY_UP_Gamepad_RightStick_Right);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gamepad_RightStick_Left))
//     Frame::Fire(KEY_DOWN_Gamepad_RightStick_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::Gamepad_RightStick_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gamepad_RightStick_Left);
//     Frame::Fire(KEY_UP_Gamepad_RightStick_Left);}
// }
//
// void AMainHUD::CheckGestures()
// {
//   if (PC->WasInputKeyJustPressed(EKeys::Tilt))
//     Frame::Fire(KEY_DOWN_Tilt);
//   if (PC->WasInputKeyJustReleased(EKeys::Tilt)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Tilt);
//     Frame::Fire(KEY_UP_Tilt);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::RotationRate))
//     Frame::Fire(KEY_DOWN_RotationRate);
//   if (PC->WasInputKeyJustReleased(EKeys::RotationRate)){
//     float time = PC->GetInputKeyTimeDown(EKeys::RotationRate);
//     Frame::Fire(KEY_UP_RotationRate);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gravity))
//     Frame::Fire(KEY_DOWN_Gravity);
//   if (PC->WasInputKeyJustReleased(EKeys::Gravity)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gravity);
//     Frame::Fire(KEY_UP_Gravity);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Acceleration))
//     Frame::Fire(KEY_DOWN_Acceleration);
//   if (PC->WasInputKeyJustReleased(EKeys::Acceleration)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Acceleration);
//     Frame::Fire(KEY_UP_Acceleration);}
//
//   // Fingers
//   const FKey EKeys::TouchKeys[NUM_TOUCH_KEYS] =
//   {
//     FKey("Touch1"),
//     FKey("Touch2"),
//     FKey("Touch3"),
//     FKey("Touch4"),
//     FKey("Touch5"),
//     FKey("Touch6"),
//     FKey("Touch7"),
//     FKey("Touch8"),
//     FKey("Touch9"),
//     FKey("Touch10")
//   };
//
//   // Gestures
//   if (PC->WasInputKeyJustPressed(EKeys::Gesture_Pinch))
//     Frame::Fire(KEY_DOWN_Gesture_Pinch);
//   if (PC->WasInputKeyJustReleased(EKeys::Gesture_Pinch)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gesture_Pinch);
//     Frame::Fire(KEY_UP_Gesture_Pinch);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Gesture_Flick))
//     Frame::Fire(KEY_DOWN_Gesture_Flick);
//   if (PC->WasInputKeyJustReleased(EKeys::Gesture_Flick)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Gesture_Flick);
//     Frame::Fire(KEY_UP_Gesture_Flick);}
// }
//
// void AMainHUD::CheckControllerKeys()
// {
//   // Motion Controllers
//   //		Left Controller
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton1))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton1);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton1)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton1);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton1);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton2))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton2);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton2)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton2);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton2);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton3))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton3);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton3)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton3);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton3);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton4))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton4);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton4)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton4);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton4);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton5))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton5);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton5)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton5);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton5);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton6))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton6);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton6)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton6);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton6);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton7))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton7);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton7)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton7);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton7);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton8))
//     Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton8);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton8)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_FaceButton8);
//     Frame::Fire(KEY_UP_MotionController_Left_FaceButton8);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Shoulder))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Shoulder);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Shoulder)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Shoulder);
//     Frame::Fire(KEY_UP_MotionController_Left_Shoulder);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Trigger))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Trigger);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Trigger)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Trigger);
//     Frame::Fire(KEY_UP_MotionController_Left_Trigger);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Grip1))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Grip1);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Grip1)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Grip1);
//     Frame::Fire(KEY_UP_MotionController_Left_Grip1);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Grip2))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Grip2);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Grip2)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Grip2);
//     Frame::Fire(KEY_UP_MotionController_Left_Grip2);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Thumbstick);
//     Frame::Fire(KEY_UP_MotionController_Left_Thumbstick);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Up))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Up);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Up)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Thumbstick_Up);
//     Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Up);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Down))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Down);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Down)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Thumbstick_Down);
//     Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Down);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Left))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Thumbstick_Left);
//     Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Left);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Right))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Thumbstick_Right);
//     Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Right);}
//
//
//   //		Right Controller
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton1))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton1);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton1)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton1);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton1);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton2))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton2);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton2)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton2);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton2);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton3))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton3);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton3)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton3);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton3);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton4))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton4);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton4)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton4);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton4);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton5))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton5);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton5)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton5);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton5);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton6))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton6);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton6)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton6);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton6);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton7))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton7);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton7)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton7);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton7);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton8))
//     Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton8);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton8)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_FaceButton8);
//     Frame::Fire(KEY_UP_MotionController_Right_FaceButton8);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Shoulder))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Shoulder);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Shoulder)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Shoulder);
//     Frame::Fire(KEY_UP_MotionController_Right_Shoulder);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Trigger))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Trigger);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Trigger)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Trigger);
//     Frame::Fire(KEY_UP_MotionController_Right_Trigger);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Grip1))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Grip1);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Grip1)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Grip1);
//     Frame::Fire(KEY_UP_MotionController_Right_Grip1);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Grip2))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Grip2);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Grip2)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Grip2);
//     Frame::Fire(KEY_UP_MotionController_Right_Grip2);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Thumbstick);
//     Frame::Fire(KEY_UP_MotionController_Right_Thumbstick);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Up))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Up);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Up)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Thumbstick_Up);
//     Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Up);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Down))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Down);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Down)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Thumbstick_Down);
//     Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Down);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Left))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Thumbstick_Left);
//     Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Left);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Right))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Thumbstick_Right);
//     Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Right);}
//
//   // Motion Controllers
//   //		Left Controller
//   if (!bKeyDown_MotionController_Left_FaceButton1)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton1))
//     {
//       bKeyDown_MotionController_Left_FaceButton1 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton1);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton1))
//     {
//       bKeyDown_MotionController_Left_FaceButton1 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton1)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton1);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_FaceButton2)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton2))
//     {
//       bKeyDown_MotionController_Left_FaceButton2 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton2);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton2))
//     {
//       bKeyDown_MotionController_Left_FaceButton2 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton2)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton2);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_FaceButton3)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton3))
//     {
//       bKeyDown_MotionController_Left_FaceButton3 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton3);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton3))
//     {
//       bKeyDown_MotionController_Left_FaceButton3 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton3)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton3);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_FaceButton4)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton4))
//     {
//       bKeyDown_MotionController_Left_FaceButton4 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton4);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton4))
//     {
//       bKeyDown_MotionController_Left_FaceButton4 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton4)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton4);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_FaceButton5)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton5))
//     {
//       bKeyDown_MotionController_Left_FaceButton5 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton5);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton5))
//     {
//       bKeyDown_MotionController_Left_FaceButton5 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton5)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton5);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_FaceButton6)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton6))
//     {
//       bKeyDown_MotionController_Left_FaceButton6 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton6);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton6))
//     {
//       bKeyDown_MotionController_Left_FaceButton6 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton6)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton6);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_FaceButton7)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton7))
//     {
//       bKeyDown_MotionController_Left_FaceButton7 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton7);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton7))
//     {
//       bKeyDown_MotionController_Left_FaceButton7 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton7)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton7);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_FaceButton8)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_FaceButton8))
//     {
//       bKeyDown_MotionController_Left_FaceButton8 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_FaceButton8);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_FaceButton8))
//     {
//       bKeyDown_MotionController_Left_FaceButton8 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_FaceButton8)
//       Frame::Fire(KEY_UP_MotionController_Left_FaceButton8);
//     }
//   }
//
//
//   if (!bKeyDown_MotionController_Left_Shoulder)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Shoulder))
//     {
//       bKeyDown_MotionController_Left_Shoulder = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Shoulder);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Shoulder))
//     {
//       bKeyDown_MotionController_Left_Shoulder = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Shoulder)
//       Frame::Fire(KEY_UP_MotionController_Left_Shoulder);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_Trigger)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Trigger))
//     {
//       bKeyDown_MotionController_Left_Trigger = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Trigger);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Trigger))
//     {
//       bKeyDown_MotionController_Left_Trigger = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Trigger)
//       Frame::Fire(KEY_UP_MotionController_Left_Trigger);
//     }
//   }
//
//
//   if (!bKeyDown_MotionController_Left_Grip1)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Grip1))
//     {
//       bKeyDown_MotionController_Left_Grip1 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Grip1);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Grip1))
//     {
//       bKeyDown_MotionController_Left_Grip1 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Grip1)
//       Frame::Fire(KEY_UP_MotionController_Left_Grip1);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_Grip2)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Grip2))
//     {
//       bKeyDown_MotionController_Left_Grip2 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Grip2);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Grip2))
//     {
//       bKeyDown_MotionController_Left_Grip2 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Grip2)
//       Frame::Fire(KEY_UP_MotionController_Left_Grip2);
//     }
//   }
//
//
//   if (!bKeyDown_MotionController_Left_Thumbstick)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick))
//     {
//       bKeyDown_MotionController_Left_Thumbstick = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick))
//     {
//       bKeyDown_MotionController_Left_Thumbstick = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Thumbstick)
//       Frame::Fire(KEY_UP_MotionController_Left_Thumbstick);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_Thumbstick_Up)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Up))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Up = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Up);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Up))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Up = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Thumbstick_Up)
//       Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Up);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_Thumbstick_Down)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Down))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Down = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Down);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Down))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Down = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Thumbstick_Down)
//       Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Down);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_Thumbstick_Left)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Left))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Left = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Left);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Left))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Left = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Thumbstick_Left)
//       Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Left);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Left_Thumbstick_Right)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Right))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Right = true;
//       Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Right);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Right))
//     {
//       bKeyDown_MotionController_Left_Thumbstick_Right = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Left_Thumbstick_Right)
//       Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Right);
//     }
//   }
//
//
//   //		Right Controller
//   if (!bKeyDown_MotionController_Right_FaceButton1)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton1))
//     {
//       bKeyDown_MotionController_Right_FaceButton1 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton1);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton1))
//     {
//       bKeyDown_MotionController_Right_FaceButton1 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton1)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton1);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_FaceButton2)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton2))
//     {
//       bKeyDown_MotionController_Right_FaceButton2 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton2);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton2))
//     {
//       bKeyDown_MotionController_Right_FaceButton2 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton2)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton2);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_FaceButton3)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton3))
//     {
//       bKeyDown_MotionController_Right_FaceButton3 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton3);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton3))
//     {
//       bKeyDown_MotionController_Right_FaceButton3 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton3)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton3);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_FaceButton4)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton4))
//     {
//       bKeyDown_MotionController_Right_FaceButton4 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton4);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton4))
//     {
//       bKeyDown_MotionController_Right_FaceButton4 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton4)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton4);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_FaceButton5)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton5))
//     {
//       bKeyDown_MotionController_Right_FaceButton5 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton5);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton5))
//     {
//       bKeyDown_MotionController_Right_FaceButton5 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton5)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton5);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_FaceButton6)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton6))
//     {
//       bKeyDown_MotionController_Right_FaceButton6 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton6);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton6))
//     {
//       bKeyDown_MotionController_Right_FaceButton6 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton6)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton6);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_FaceButton7)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton7))
//     {
//       bKeyDown_MotionController_Right_FaceButton7 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton7);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton7))
//     {
//       bKeyDown_MotionController_Right_FaceButton7 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton7)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton7);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_FaceButton8)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_FaceButton8))
//     {
//       bKeyDown_MotionController_Right_FaceButton8 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_FaceButton8);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_FaceButton8))
//     {
//       bKeyDown_MotionController_Right_FaceButton8 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_FaceButton8)
//       Frame::Fire(KEY_UP_MotionController_Right_FaceButton8);
//     }
//   }
//
//
//   if (!bKeyDown_MotionController_Right_Shoulder)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Shoulder))
//     {
//       bKeyDown_MotionController_Right_Shoulder = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Shoulder);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Shoulder))
//     {
//       bKeyDown_MotionController_Right_Shoulder = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Shoulder)
//       Frame::Fire(KEY_UP_MotionController_Right_Shoulder);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_Trigger)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Trigger))
//     {
//       bKeyDown_MotionController_Right_Trigger = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Trigger);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Trigger))
//     {
//       bKeyDown_MotionController_Right_Trigger = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Trigger)
//       Frame::Fire(KEY_UP_MotionController_Right_Trigger);
//     }
//   }
//
//
//   if (!bKeyDown_MotionController_Right_Grip1)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Grip1))
//     {
//       bKeyDown_MotionController_Right_Grip1 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Grip1);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Grip1))
//     {
//       bKeyDown_MotionController_Right_Grip1 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Grip1)
//       Frame::Fire(KEY_UP_MotionController_Right_Grip1);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_Grip2)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Grip2))
//     {
//       bKeyDown_MotionController_Right_Grip2 = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Grip2);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Grip2))
//     {
//       bKeyDown_MotionController_Right_Grip2 = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Grip2)
//       Frame::Fire(KEY_UP_MotionController_Right_Grip2);
//     }
//   }
//
//
//   if (!bKeyDown_MotionController_Right_Thumbstick)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick))
//     {
//       bKeyDown_MotionController_Right_Thumbstick = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick))
//     {
//       bKeyDown_MotionController_Right_Thumbstick = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Thumbstick)
//       Frame::Fire(KEY_UP_MotionController_Right_Thumbstick);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_Thumbstick_Up)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Up))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Up = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Up);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Up))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Up = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Thumbstick_Up)
//       Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Up);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_Thumbstick_Down)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Down))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Down = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Down);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Down))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Down = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Thumbstick_Down)
//       Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Down);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_Thumbstick_Left)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Left))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Left = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Left);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Left))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Left = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Thumbstick_Left)
//       Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Left);
//     }
//   }
//
//   if (!bKeyDown_MotionController_Right_Thumbstick_Right)
//   {
//     if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Right))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Right = true;
//       Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Right);
//     }
//   }
//   else
//   {
//     if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Right))
//     {
//       bKeyDown_MotionController_Right_Thumbstick_Right = false;
//       float time = PC->GetInputKeyTimeDown(MotionController_Right_Thumbstick_Right)
//       Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Right);
//     }
//   }
// }
//
// void AMainHUD::CheckMotionControllerKeys()
// {
//   //   Motion Controller Axes
//   //		Left Controller
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_X))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_X);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_X)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Thumbstick_X);
//     Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_X);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Y))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Thumbstick_Y);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Thumbstick_Y)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Thumbstick_Y);
//     Frame::Fire(KEY_UP_MotionController_Left_Thumbstick_Y);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_TriggerAxis))
//     Frame::Fire(KEY_DOWN_MotionController_Left_TriggerAxis);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_TriggerAxis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_TriggerAxis);
//     Frame::Fire(KEY_UP_MotionController_Left_TriggerAxis);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Grip1Axis))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Grip1Axis);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Grip1Axis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Grip1Axis);
//     Frame::Fire(KEY_UP_MotionController_Left_Grip1Axis);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Left_Grip2Axis))
//     Frame::Fire(KEY_DOWN_MotionController_Left_Grip2Axis);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Left_Grip2Axis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Left_Grip2Axis);
//     Frame::Fire(KEY_UP_MotionController_Left_Grip2Axis);}
//
//
//   //		Right Controller
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_X))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_X);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_X)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Thumbstick_X);
//     Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_X);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Thumbstick_Y))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Thumbstick_Y);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Thumbstick_Y)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Thumbstick_Y);
//     Frame::Fire(KEY_UP_MotionController_Right_Thumbstick_Y);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_TriggerAxis))
//     Frame::Fire(KEY_DOWN_MotionController_Right_TriggerAxis);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_TriggerAxis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_TriggerAxis);
//     Frame::Fire(KEY_UP_MotionController_Right_TriggerAxis);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Grip1Axis))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Grip1Axis);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Grip1Axis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Grip1Axis);
//     Frame::Fire(KEY_UP_MotionController_Right_Grip1Axis);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::MotionController_Right_Grip2Axis))
//     Frame::Fire(KEY_DOWN_MotionController_Right_Grip2Axis);
//   if (PC->WasInputKeyJustReleased(EKeys::MotionController_Right_Grip2Axis)){
//     float time = PC->GetInputKeyTimeDown(EKeys::MotionController_Right_Grip2Axis);
//     Frame::Fire(KEY_UP_MotionController_Right_Grip2Axis);}
// }
//
// void AMainHUD::CheckPS4SpecificKeys()
// {
//   // PS4-specific
//   if (PC->WasInputKeyJustPressed(EKeys::PS4_Special))
//     Frame::Fire(KEY_DOWN_PS4_Special);
//   if (PC->WasInputKeyJustReleased(EKeys::PS4_Special)){
//     float time = PC->GetInputKeyTimeDown(EKeys::PS4_Special);
//     Frame::Fire(KEY_UP_PS4_Special);}
// }
//
// void AMainHUD::CheckSteamControllerKeys()
// {
//   // Steam Controller Specific
//   if (PC->WasInputKeyJustPressed(EKeys::Steam_Touch_0))
//     Frame::Fire(KEY_DOWN_Steam_Touch_0);
//   if (PC->WasInputKeyJustReleased(EKeys::Steam_Touch_0)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Steam_Touch_0);
//     Frame::Fire(KEY_UP_Steam_Touch_0);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Steam_Touch_1))
//     Frame::Fire(KEY_DOWN_Steam_Touch_1);
//   if (PC->WasInputKeyJustReleased(EKeys::Steam_Touch_1)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Steam_Touch_1);
//     Frame::Fire(KEY_UP_Steam_Touch_1);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Steam_Touch_2))
//     Frame::Fire(KEY_DOWN_Steam_Touch_2);
//   if (PC->WasInputKeyJustReleased(EKeys::Steam_Touch_2)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Steam_Touch_2);
//     Frame::Fire(KEY_UP_Steam_Touch_2);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Steam_Touch_3))
//     Frame::Fire(KEY_DOWN_Steam_Touch_3);
//   if (PC->WasInputKeyJustReleased(EKeys::Steam_Touch_3)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Steam_Touch_3);
//     Frame::Fire(KEY_UP_Steam_Touch_3);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Steam_Back_Left))
//     Frame::Fire(KEY_DOWN_Steam_Back_Left);
//   if (PC->WasInputKeyJustReleased(EKeys::Steam_Back_Left)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Steam_Back_Left);
//     Frame::Fire(KEY_UP_Steam_Back_Left);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Steam_Back_Right))
//     Frame::Fire(KEY_DOWN_Steam_Back_Right);
//   if (PC->WasInputKeyJustReleased(EKeys::Steam_Back_Right)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Steam_Back_Right);
//     Frame::Fire(KEY_UP_Steam_Back_Right);}
// }
//
// void AMainHUD::CheckXBoxOneGlobalSpeechCommands()
// {
//   // Xbox One global speech commands
//   if (PC->WasInputKeyJustPressed(EKeys::Global_Menu))
//     Frame::Fire(KEY_DOWN_Global_Menu);
//   if (PC->WasInputKeyJustReleased(EKeys::Global_Menu)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Global_Menu);
//     Frame::Fire(KEY_UP_Global_Menu);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Global_View))
//     Frame::Fire(KEY_DOWN_Global_View);
//   if (PC->WasInputKeyJustReleased(EKeys::Global_View)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Global_View);
//     Frame::Fire(KEY_UP_Global_View);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Global_Pause))
//     Frame::Fire(KEY_DOWN_Global_Pause);
//   if (PC->WasInputKeyJustReleased(EKeys::Global_Pause)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Global_Pause);
//     Frame::Fire(KEY_UP_Global_Pause);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Global_Play))
//     Frame::Fire(KEY_DOWN_Global_Play);
//   if (PC->WasInputKeyJustReleased(EKeys::Global_Play)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Global_Play);
//     Frame::Fire(KEY_UP_Global_Play);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Global_Back))
//     Frame::Fire(KEY_DOWN_Global_Back);
//   if (PC->WasInputKeyJustReleased(EKeys::Global_Back)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Global_Back);
//     Frame::Fire(KEY_UP_Global_Back);}
// }
//
// void AMainHUD::CheckAndroidKeys()
// {
//   // Android-specific
//   if (PC->WasInputKeyJustPressed(EKeys::Android_Back))
//     Frame::Fire(KEY_DOWN_Android_Back);
//   if (PC->WasInputKeyJustReleased(EKeys::Android_Back)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Android_Back);
//     Frame::Fire(KEY_UP_Android_Back);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Android_Volume_Up))
//     Frame::Fire(KEY_DOWN_Android_Volume_Up);
//   if (PC->WasInputKeyJustReleased(EKeys::Android_Volume_Up)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Android_Volume_Up);
//     Frame::Fire(KEY_UP_Android_Volume_Up);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Android_Volume_Down))
//     Frame::Fire(KEY_DOWN_Android_Volume_Down);
//   if (PC->WasInputKeyJustReleased(EKeys::Android_Volume_Down)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Android_Volume_Down);
//     Frame::Fire(KEY_UP_Android_Volume_Down);}
//
//   if (PC->WasInputKeyJustPressed(EKeys::Android_Menu))
//     Frame::Fire(KEY_DOWN_Android_Menu);
//   if (PC->WasInputKeyJustReleased(EKeys::Android_Menu)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Android_Menu);
//     Frame::Fire(KEY_UP_Android_Menu);}
//
//
//   if (PC->WasInputKeyJustPressed(EKeys::Invalid))
//     Frame::Fire(KEY_DOWN_Invalid);
//   if (PC->WasInputKeyJustReleased(EKeys::Invalid)){
//     float time = PC->GetInputKeyTimeDown(EKeys::Invalid);
//     Frame::Fire(KEY_UP_Invalid);}
// }

TArray<FString> Frame::EventList;
// const FString EventList[100] = {
//   "GAME_START",
//   "GAME_LOAD",
//   "LOAD_FILE",
//   "DRAWING",
//   "UPDATE",
//   "FRAME_CREATED",
//   "ENTITY_CREATED",
//   "ENTITY_DAMAGED",
//   "ENTITY_DESTROYED",
//   "ENTITY_FALLING",
//   "ENTITY_SMOKE",
//   "ENTITY_HIT_GROUND",
//   "ENTITY_OFF_SCREEN_X",
//   "ENTITY_OFF_SCREEN_Y",
//   "MOUSE_DOWN",
//   "WINDOW_FOCUS",
//   "WINDOW_VISIBLE",
//   "KEY_DOWN",
//   "KEY_UP",
//   "GAME_QUIT",
//   "SCORE_UPDATE",
//   "MOUSE_ENTER",
//   "MOUSE_EXIT",
//   "MOUSE_CLICKED_DOWN",
//   "MOUSE_CLICKED_UP",
//   "ZEPPLIN_FALLING",
//   "MOUSE_MOVEMENT",
// };

/*
const FKey EKeys::(.+)\(.+\);

if (PC->WasInputKeyJustPressed(EKeys::$1))\n\tFrame::Fire(KEY_DOWN_$1);\nif (PC->WasInputKeyJustReleased(EKeys::$1)){\n\tfloat time = PC->GetInputKeyTimeDown(EKeys::$1);\n\tFrame::Fire(KEY_UP_$1);\n}\n
*/
