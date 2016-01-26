// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Frame.h"

int32 Frame::frameCount = 0;
TMap<FString, Frame::Strata> Frame::StrataMap;
const FString Frame::strataList[5] = {"BACKGROUND", "LOW", "MEDIUM", "HIGH", "OVERLAY"};

TArray<Frame*> Frame::FrameList;
TArray<Frame*> Frame::Level::FrameList;
TArray<Frame::ScriptStruct> Frame::OnUpdateList;
TMap<int32, Frame::Level> Frame::Strata::LevelMap;
TMap<EventEnum, TArray<CallbackEventUPtrType>> Frame::EventMap;
// TMultiMap<EventEnum, EventCallbackHolder> Frame::EventMap;

Frame::Frame()
{
  w = 0.f;
  h = 0.f;
  x = 0.f;
  y = 0.f;
  r = 0.f;
  g = 0.f;
  b = 0.f;
  a = 0.f;
  shown = true;
  mouseOver = false;
  scale = 1.f;
  level = 0.f;
  strata = "BACKGROUND";
  name = "Frame_" + FString::FromInt(frameCount);
  // Frame* parent;
  
  bMouseEnabled = false;
  
  // TMap<EventEnum, FuncType> EventMap;
  // EventMap.Emplace(EventEnum::UPDATE, UPDATE);
  
  frameCount++; // Track the total number of frames
}

Frame::~Frame()
{
  // FrameList.Remove(this);
  // this->OnUpdate(); // Set it to nullptr
  // for (auto& f : FrameList) // Iterate through every frame
  // {
  //   if (f == nullptr)
  //   {
  //
  //   }
  // }
  
  frameCount--;
}

EventCallbackHolder::EventCallbackHolder(const CallbackEventFuncType& func, Frame* f, EventEnum event) :
  callback(func), // Set the callback to the passed function
  frame(f), // Set the pointer to the frame
  time(0.f) // Default the time to 0
{
  // If we want to set any extra arguments to specific events, that can be done in this switch statement
  switch (event)
  {
    case EventEnum::TESTING_EVENT:
      this->event = "TESTING_EVENT";
      break;
    case EventEnum::MOUSE_ENTER:
      this->event = "MOUSE_ENTER";
      break;
    case EventEnum::MOUSE_EXIT:
      this->event = "MOUSE_EXIT";
      break;
    case EventEnum::MOUSE_LEFT_CLICK_DOWN:
      this->event = "MOUSE_LEFT_CLICK_DOWN";
      break;
    case EventEnum::MOUSE_LEFT_CLICK_UP:
      this->event = "MOUSE_LEFT_CLICK_UP";
      break;
    case EventEnum::MOUSE_RIGHT_CLICK_DOWN:
      this->event = "MOUSE_RIGHT_CLICK_DOWN";
      break;
    case EventEnum::MOUSE_RIGHT_CLICK_UP:
      this->event = "MOUSE_RIGHT_CLICK_UP";
      break;
    case EventEnum::UPDATE:
      this->event = "UPDATE";
      break;
    case EventEnum::FRAME_CREATED:
      this->event = "FRAME_CREATED";
      break;
    case EventEnum::GAME_START:
      this->event = "GAME_START";
      break;
    case EventEnum::GAME_STOP:
      this->event = "GAME_STOP";
      break;
    case EventEnum::KEY_DOWN:
      this->event = "KEY_DOWN";
      break;
    case EventEnum::KEY_UP:
      this->event = "KEY_UP";
      break;
    case EventEnum::MOUSE_MOVING:
      this->event = "MOUSE_MOVING";
      break;
    case EventEnum::WINDOW_FOCUS_GAINED:
      this->event = "WINDOW_FOCUS_GAINED";
      break;
    case EventEnum::WINDOW_FOCUS_LOST:
      this->event = "WINDOW_FOCUS_LOST";
      break;
    case EventEnum::DRAWING:
      this->event = "DRAWING";
      break;
    case EventEnum::SCORE_UPDATE:
      this->event = "SCORE_UPDATE";
      break;
    default:
      print("Need an event string for:", event);
      this->event = "EVENT_STRING_NOT_SET";
      break;
  }
}

void Frame::DeleteFrame()
{
  // Take it out of the frame list
  FrameList.Remove(this);
  FrameList.Shrink();
  this->OnUpdate(); // Set it to nullptr
}

class FrameActor : public Frame
{
	
};

class Frame3D : public Frame
{
	
};

class Frame2D : public Frame
{
	
};

class FrameButton2D : public Frame
{
	
};

class FrameText : public Frame
{
  private:
    FString text;
    bool outline;
    bool wrap;
    int32 lineSpacing;
    bool shown;
  public:
    /*--------------------------------------------------------------------------
    		Get Functions
    --------------------------------------------------------------------------*/
    FString GetText() const {return text;}
    bool GetOutline() const {return outline;}
    bool GetWrap() const {return wrap;}
    int32 GetNumLines(){return wrap;} // Find the number of \n in the FString
    int32 GetLineSpacing() const {return lineSpacing;}
    /*--------------------------------------------------------------------------
    		Set Functions
    --------------------------------------------------------------------------*/
    void SetText(FString nText){text = nText;}
    void SetOutline(bool nOutline){outline = nOutline;}
    void SetWrap(bool nWrap){wrap = nWrap;}
    void SetLineSpacing(int32 nLineSpacing){lineSpacing = nLineSpacing;}
    void Show(){shown = true;}
    void Hide(){shown = false;}
};
/*------------------------------------------------------------------------------
		Event setting functions
------------------------------------------------------------------------------*/
void Frame::SetEvent(EventEnum event, CallbackEventFuncType func)
{
  // If the check passes, then no frame has registered for this event before,
  // and the array to hold the EventCallbackHolder objects needs to be created
  if (!EventMap.Contains(event))
  {
    TArray<CallbackEventUPtrType> arr; // Create a new array of unique pointers
    // TArray<CallbackEventUPtrType> arr;
    EventMap.Emplace(event, std::move(arr)); // Add it to the map
    // EventMap.Emplace(event, arr); // Add it to the map
  }
  else
  {
    // Iterate through every holder for this event
    for (auto& holder : EventMap[event])
    {
      // Check if this frame is already registered for this event
      if (holder->frame == this)
      {
        holder->callback = func; // Replace the old callback with the new one
        return; // Return to avoid creating and adding another holder object for the same frame
      }
    }
  }
  
  // Create a unique_ptr callback holder and pass the values to its constructor
  EventMap[event].Emplace(std::make_unique<EventCallbackHolder>(func, this, event));
}

/*------------------------------------------------------------------------------
		Special script events
------------------------------------------------------------------------------*/
void Frame::OnUpdate(FuncType func)
{
  // std::function<void(Frame*)>
  OnUpdateFunc = func;
}
/*------------------------------------------------------------------------------
    Get functions
------------------------------------------------------------------------------*/
int32 Frame::GetNumFrames(){return frameCount;}

float Frame::GetWidth() const {return w;}
float Frame::GetHeight() const {return h;}
float Frame::GetSize() const {return w, h;}
float Frame::GetX() const {return x;}
float Frame::GetY() const {return y;}
float Frame::GetPosition() const {return x, y;}
float Frame::GetColorInt() const {return r, g, b, a;}
FLinearColor Frame::GetColor() const {return FLinearColor(r, g, b, a);}
float Frame::GetAlpha() const {return a;}
float Frame::GetScale() const {return scale;}
int32 Frame::GetLevel() const {return level;}
bool Frame::IsShown() const {return shown;}
bool Frame::GetMouseOver() const {return mouseOver;}
bool Frame::GetMouseEnabled() const {return bMouseEnabled;}
FString Frame::GetType() const {return type;}
FString Frame::GetStrata() const {return strata;}
FString Frame::GetName() const {return name;}
FString Frame::GetUniqueID() const {return uniqueID;}
Frame* Frame::GetParent() const {return parent;}
Anchors Frame::GetAnchorPoint() const {return anchorPoint;}
Frame* Frame::GetRelativeFrame() const {return relativeTo;}
Anchors Frame::GetRelativePoint() const {return relativePoint;}
// Frame& Frame::GetPtr(){return *f;}
/*------------------------------------------------------------------------------
    END get functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Set functions
------------------------------------------------------------------------------*/
void Frame::Show(){shown = true;}
void Frame::Hide(){shown = false;}
void Frame::SetWidth(float nW){w = nW;}
void Frame::SetHeight(float nH){h = nH;}
void Frame::SetSize(float nW, float nH){w = nW; h = nH;}
void Frame::SetX(float nX){x = nX;}
void Frame::SetY(float nY){y = nY;}
void Frame::SetPosition(float nX, float nY){x = nX; y = nY;}
void Frame::SetScale(float nScale){scale = nScale;}
void Frame::SetName(FString nName){name = nName;}
void Frame::SetType(FString nType){type = nType;}
void Frame::SetShown(bool nVisibility){shown = nVisibility;}
void Frame::SetMouseOver(bool nMouseOver){mouseOver = nMouseOver;}
void Frame::SetMouseEnabled(bool nMouseEnabled){bMouseEnabled = nMouseEnabled;}

void Frame::SetColor(float nR, float nG, float nB, float nA)
{
  // All of these default to 1.f if they are not supplied.
  
  if ((nR >= 0.f) && (1.f >= nR)) r = nR; // It is between 0 and 1
  else if (0.f > nR) r = 0.f; // Less than 0, set it to 0
  else if (nR > 1.f) r = 1.f; // More than 1, set it to 1
  
  if ((nG >= 0.f) && (1.f >= nG)) g = nG;
  else if (0.f > nG) g = 0.f;
  else if (nG > 1.f) g = 1.f;
  
  if ((nB >= 0.f) && (1.f >= nB)) b = nB;
  else if (0.f > nB) b = 0.f;
  else if (nB > 1.f) b = 1.f;
  
  if ((nA >= 0.f) && (1.f >= nA)) a = nA;
  else if (0.f > nA) a = 0.f;
  else if (nA > 1.f) a = 1.f;
}

void Frame::SetAlpha(float nA)
{
  // Defaults to 1.f if not supplied
  
  if ((nA >= 0.f) && (1.f >= nA)) a = nA; // It is between 0 and 1
  else if (0.f > nA) a = 0.f; // Less than 0, set it to 0
  else if (nA > 1.f) a = 1.f; // More than 1, set it to 1
}

void Frame::SetLevel(int32 nLevel)
{
  if ((nLevel >= -10.f) && (nLevel <= 10.f))
  {
    level = nLevel;
  }
  else
  {
    print("Invalid level:", nLevel);
  }
}

void Frame::SetStrata(FString nStrata)
{
  // Make sure it's a valid strata
  if (
      (nStrata == "BACKGROUND") ||
      (nStrata == "LOW") ||
      (nStrata == "MEDIUM") ||
      (nStrata == "HIGH") ||
      (nStrata == "OVERLAY")
    )
  {
    strata = nStrata;
  }
  else // Not valid
  {
    print("Invalid strata: ", nStrata);
  }
}

void Frame::SetParent(Frame* nParent)
{
  parent = nParent;
}

void Frame::SetPoint(Anchors nAnchor, Frame* nRelativeTo, Anchors nRelative, float nX, float nY)
{
  anchorPoint = nAnchor; // Defaults to CENTER
  relativeTo = nRelativeTo; // Defaults to nullptr
  relativePoint = nRelative; // Defaults to CENTER
  x = nX; // Defaults to 0
  y = nY; // Defaults to 0
}
/*------------------------------------------------------------------------------
    END Set functions
------------------------------------------------------------------------------*/

// bool bEventList[] = {
//   MOUSE_ENTER,
//   MOUSE_EXIT,
//   GAME_START,
//   GAME_LOAD,
//   LOAD_FILE,
//   DRAWING,
//   UPDATE,
//   FRAME_CREATED,
//   MOUSE_DOWN,
//   WINDOW_FOCUS,
//   WINDOW_VISIBLE,
//   KEY_DOWN,
//   KEY_UP,
//   GAME_QUIT,
//   SCORE_UPDATE,
//   MOUSE_CLICKED_DOWN,
//   MOUSE_CLICKED_UP,
//   ZEPPLIN_FALLING,
//   MOUSE_MOVEMENT,
//
//   // Key events
//   KEY_DOWN_MouseX,
//   KEY_UP_MouseX,
//   KEY_DOWN_MouseY,
//   KEY_UP_MouseY,
//   KEY_DOWN_MouseScrollUp,
//   KEY_UP_MouseScrollUp,
//   KEY_DOWN_MouseScrollDown,
//   KEY_UP_MouseScrollDown,
//   KEY_DOWN_MouseWheelAxis,
//   KEY_UP_MouseWheelAxis,
//
//   KEY_DOWN_LeftMouseButton,
//   KEY_UP_LeftMouseButton,
//   KEY_DOWN_RightMouseButton,
//   KEY_UP_RightMouseButton,
//   KEY_DOWN_MiddleMouseButton,
//   KEY_UP_MiddleMouseButton,
//   KEY_DOWN_ThumbMouseButton,
//   KEY_UP_ThumbMouseButton,
//   KEY_DOWN_ThumbMouseButton2,
//   KEY_UP_ThumbMouseButton2,
//
//   KEY_DOWN_BackSpace,
//   KEY_UP_BackSpace,
//   KEY_DOWN_Tab,
//   KEY_UP_Tab,
//   KEY_DOWN_Enter,
//   KEY_UP_Enter,
//   KEY_DOWN_Pause,
//   KEY_UP_Pause,
//
//   KEY_DOWN_CapsLock,
//   KEY_UP_CapsLock,
//   KEY_DOWN_Escape,
//   KEY_UP_Escape,
//   KEY_DOWN_SpaceBar,
//   KEY_UP_SpaceBar,
//   KEY_DOWN_PageUp,
//   KEY_UP_PageUp,
//   KEY_DOWN_PageDown,
//   KEY_UP_PageDown,
//   KEY_DOWN_End,
//   KEY_UP_End,
//   KEY_DOWN_Home,
//   KEY_UP_Home,
//
//   KEY_DOWN_Left,
//   KEY_UP_Left,
//   KEY_DOWN_Up,
//   KEY_UP_Up,
//   KEY_DOWN_Right,
//   KEY_UP_Right,
//   KEY_DOWN_Down,
//   KEY_UP_Down,
//
//   KEY_DOWN_Insert,
//   KEY_UP_Insert,
//   KEY_DOWN_Delete,
//   KEY_UP_Delete,
//
//   KEY_DOWN_Zero,
//   KEY_UP_Zero,
//   KEY_DOWN_One,
//   KEY_UP_One,
//   KEY_DOWN_Two,
//   KEY_UP_Two,
//   KEY_DOWN_Three,
//   KEY_UP_Three,
//   KEY_DOWN_Four,
//   KEY_UP_Four,
//   KEY_DOWN_Five,
//   KEY_UP_Five,
//   KEY_DOWN_Six,
//   KEY_UP_Six,
//   KEY_DOWN_Seven,
//   KEY_UP_Seven,
//   KEY_DOWN_Eight,
//   KEY_UP_Eight,
//   KEY_DOWN_Nine,
//   KEY_UP_Nine,
//
//   KEY_DOWN_A,
//   KEY_UP_A,
//   KEY_DOWN_B,
//   KEY_UP_B,
//   KEY_DOWN_C,
//   KEY_UP_C,
//   KEY_DOWN_D,
//   KEY_UP_D,
//   KEY_DOWN_E,
//   KEY_UP_E,
//   KEY_DOWN_F,
//   KEY_UP_F,
//   KEY_DOWN_G,
//   KEY_UP_G,
//   KEY_DOWN_H,
//   KEY_UP_H,
//   KEY_DOWN_I,
//   KEY_UP_I,
//   KEY_DOWN_J,
//   KEY_UP_J,
//   KEY_DOWN_K,
//   KEY_UP_K,
//   KEY_DOWN_L,
//   KEY_UP_L,
//   KEY_DOWN_M,
//   KEY_UP_M,
//   KEY_DOWN_N,
//   KEY_UP_N,
//   KEY_DOWN_O,
//   KEY_UP_O,
//   KEY_DOWN_P,
//   KEY_UP_P,
//   KEY_DOWN_Q,
//   KEY_UP_Q,
//   KEY_DOWN_R,
//   KEY_UP_R,
//   KEY_DOWN_S,
//   KEY_UP_S,
//   KEY_DOWN_T,
//   KEY_UP_T,
//   KEY_DOWN_U,
//   KEY_UP_U,
//   KEY_DOWN_V,
//   KEY_UP_V,
//   KEY_DOWN_W,
//   KEY_UP_W,
//   KEY_DOWN_X,
//   KEY_UP_X,
//   KEY_DOWN_Y,
//   KEY_UP_Y,
//   KEY_DOWN_Z,
//   KEY_UP_Z,
// };

void Frame::FireToFrame(EventEnum event)
{
  if (this && EventMap.Contains(event)) // Make sure the frame exists and that the event has been registered
  {
    // Iterate through each holder for this event
    for (auto& holder : EventMap[event])
    {
      // Find the holder for this particular frame
      if (holder->frame == this)
      {
        holder->time = TimerSystem::GetTime(); // Update the time to current time
        holder->callback(*holder); // Call the event and pass the holder object
        return; // Return to avoid wasting time
      }
    }
  }
}

void Frame::Fire(EventEnum event)
{
  // Check if this event is registered
  if (EventMap.Contains(event))
  {
    auto cTime = TimerSystem::GetTime();
    
    // Iterate through each holder that is set for this event
    for (auto& holder : EventMap[event])
    {
      holder->time = cTime; // Update the time to current time
      holder->callback(*holder); // Call the event and pass the holder object
    }
  }
}

void Frame::GenerateUniqueID(char* const ID, const int32 length)
{
  // In here, I'm not bothering to track every ID and make sure the new one is unique,
  // but as long as it's a decent length, like 15 - 25 characterss, getting the
  // same string twice should be very, very unlikely, basically impossible.
  
  static const char* const alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static const char* const numbers = "0123456789";
  static const int32 split = length / 5;

  for (int32 i = 0; i < (length - 1); i++)
  {
    // Every split number of characters, add a '-', but not when it's near the end
    if (((i + split) <= length) && ((i % (split + 1)) == split))
    {
      ID[i] = '-';
    }
    // If this passes, use a number. Numbers should be more common, looks better
    else if (FMath::RandRange(0, 100) >= 70)
    {
      ID[i] = numbers[FMath::RandRange(0, 9)]; // Pick a random number
    }
    // Otherwise, use a letter
    else
    {
      ID[i] = alphabet[FMath::RandRange(0, 51)]; // Pick a random letter
    }
  }

  ID[length - 1] = '\0'; // Make sure the last char is the null character
}

Frame* Frame::CreateFrame(
  FString nType = "",
  FString nName = "",
  FString nStrata = "BACKGROUND",
  int32 nLevel = 0)
{
  // std::unique_ptr<Frame*> f;
  Frame* f; // The variable to hold the new frame
  // Frame test; // The variable to hold the new frame
  // std::unique_ptr<Frame> test(new Frame);
  
  // int32 num = FrameList.Num()
  // FrameList.Emplace(new Frame());
  // auto& f = FrameList[FrameList.Num() - 1]; // Grab a reference to the one that was just added
  
  if (nType == "") // Type wasn't given, default to basic frame
    f = new Frame();
  else if (nType == "2D") // A 2D frame for the flat user interface
    f = new Frame2D();
  else if (nType == "3D") // A 3D frame for displaying in the game world
    f = new Frame3D();
  else if (nType == "Actor") // A frame to hook to an actor for events? I dunno
    f = new FrameActor();
  
  FrameList.Emplace(f);
  
  // If there wasn't a name passed, give it a default name
  if (nName == "") f->SetName("Frame_" + FString::FromInt(frameCount));
  else f->SetName(nName);
  
  f->type = nType;
  f->strata = nStrata;
  f->level = nLevel;

  if (!StrataMap.Contains(f->strata)) // Hasn't been added before
  {
    Strata strataObj;
    StrataMap.Emplace(f->strata, strataObj); // Add the new strata to the list
  }

  if (!StrataMap[f->strata].LevelMap.Contains(f->level)) // Hasn't been added before
  {
    Level levelObj;
    StrataMap[f->strata].LevelMap.Emplace(f->level, levelObj); // Add the new level to the list
    // StrataMap[strata].LevelMap[level].Sort(); // Sort them
  }

  StrataMap[f->strata].LevelMap[f->level].FrameList.Emplace(f); // Store it
  
  char ID[25];
  GenerateUniqueID(ID, 25);
  f->uniqueID = ID;
  
  return f;
}

Frame::TextureWidget* Frame::CreateTexture()
{
  TextureWidget* texture = new TextureWidget; // Create the texture object
  TextureList.Emplace(texture); // Add the new texture to the list to draw
  
  return texture;
}

Frame::TextWidget* Frame::CreateText()
{
  TextWidget* text = new TextWidget; // Create the text object
  TextList.Emplace(text); // Add the new text to the list to draw
  
  text->SetText("Some text!");
  text->SetOutline(false);
  text->SetWrap(false);
  text->SetLineSpacing(10);
  text->Show();
  
  // print("Creating text object.");
  
  return text;
}

Frame* Frame::FindFrame(FString searchString)
{
  for (const auto& frame : FrameList)
  {
    if (searchString == frame->GetName())
    {
      return frame; // Found a frame with this name
    }
  }
  
  return nullptr; // Couldn't find a frame with this name, returning nullptr
}
