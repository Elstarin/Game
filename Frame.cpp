// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Frame.h"

int32 Frame::count = 0;
TMap<FString, Frame::Strata> Frame::StrataMap;
const FString Frame::strataList[5] = {"BACKGROUND", "LOW", "MEDIUM", "HIGH", "OVERLAY"};

TArray<Frame*> Frame::FrameList;
TArray<Frame*> Frame::Level::FrameList;
TArray<Frame::ScriptStruct> Frame::OnUpdateList;
TMap<int32, Frame::Level> Frame::Strata::LevelMap;

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
  name = "Frame_" + FString::FromInt(count);
  // Frame* parent;
  
  bMouseEnabled = false;
  
  // TMap<EventEnum, FuncType> EventMap;
  // EventMap.Emplace(EventEnum::UPDATE, UPDATE);
  
  count++; // Track the total number of frames
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
void Frame::Set_MOUSE_ENTER(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_ENTER))
    EventMap[EventEnum::MOUSE_ENTER] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_ENTER, func);
}

void Frame::Set_MOUSE_EXIT(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_EXIT))
    EventMap[EventEnum::MOUSE_EXIT] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_EXIT, func);
}

void Frame::Set_MOUSE_LEFT_CLICK_DOWN(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_LEFT_CLICK_DOWN))
    EventMap[EventEnum::MOUSE_LEFT_CLICK_DOWN] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_LEFT_CLICK_DOWN, func);
}

void Frame::Set_MOUSE_LEFT_CLICK_UP(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_LEFT_CLICK_UP))
    EventMap[EventEnum::MOUSE_LEFT_CLICK_UP] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_LEFT_CLICK_UP, func);
}

void Frame::Set_MOUSE_RIGHT_CLICK_DOWN(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_RIGHT_CLICK_DOWN))
    EventMap[EventEnum::MOUSE_RIGHT_CLICK_DOWN] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_RIGHT_CLICK_DOWN, func);
}

void Frame::Set_MOUSE_RIGHT_CLICK_UP(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_RIGHT_CLICK_UP))
    EventMap[EventEnum::MOUSE_RIGHT_CLICK_UP] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_RIGHT_CLICK_UP, func);
}

void Frame::Set_UPDATE(FuncType func)
{
  if (EventMap.Contains(EventEnum::UPDATE))
    EventMap[EventEnum::UPDATE] = func;
  else
    EventMap.Emplace(EventEnum::UPDATE, func);
}

void Frame::Set_FRAME_CREATED(FuncType func)
{
  if (EventMap.Contains(EventEnum::FRAME_CREATED))
    EventMap[EventEnum::FRAME_CREATED] = func;
  else
    EventMap.Emplace(EventEnum::FRAME_CREATED, func);
}

void Frame::Set_WINDOW_FOCUS_GAINED(FuncType func)
{
  if (EventMap.Contains(EventEnum::WINDOW_FOCUS_GAINED))
    EventMap[EventEnum::WINDOW_FOCUS_GAINED] = func;
  else
    EventMap.Emplace(EventEnum::WINDOW_FOCUS_GAINED, func);
}

void Frame::Set_WINDOW_FOCUS_LOST(FuncType func)
{
  if (EventMap.Contains(EventEnum::WINDOW_FOCUS_LOST))
    EventMap[EventEnum::WINDOW_FOCUS_LOST] = func;
  else
    EventMap.Emplace(EventEnum::WINDOW_FOCUS_LOST, func);
}

void Frame::Set_KEY_DOWN(FuncType func)
{
  if (EventMap.Contains(EventEnum::KEY_DOWN))
    EventMap[EventEnum::KEY_DOWN] = func;
  else
    EventMap.Emplace(EventEnum::KEY_DOWN, func);
}

void Frame::Set_KEY_UP(FuncType func)
{
  if (EventMap.Contains(EventEnum::KEY_UP))
    EventMap[EventEnum::KEY_UP] = func;
  else
    EventMap.Emplace(EventEnum::KEY_UP, func);
}

void Frame::Set_GAME_START(FuncType func)
{
  if (EventMap.Contains(EventEnum::GAME_START))
    EventMap[EventEnum::GAME_START] = func;
  else
    EventMap.Emplace(EventEnum::GAME_START, func);
}

void Frame::Set_GAME_PAUSE(FuncType func)
{
  if (EventMap.Contains(EventEnum::GAME_PAUSE))
    EventMap[EventEnum::GAME_PAUSE] = func;
  else
    EventMap.Emplace(EventEnum::GAME_PAUSE, func);
}

void Frame::Set_GAME_STOP(FuncType func)
{
  if (EventMap.Contains(EventEnum::GAME_STOP))
    EventMap[EventEnum::GAME_STOP] = func;
  else
    EventMap.Emplace(EventEnum::GAME_STOP, func);
}

void Frame::Set_MOUSE_MOVING(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_MOVING))
    EventMap[EventEnum::MOUSE_MOVING] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_MOVING, func);
}

void Frame::Set_MOUSE_X_DOWN(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_X_DOWN))
    EventMap[EventEnum::MOUSE_X_DOWN] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_X_DOWN, func);
}

void Frame::Set_MOUSE_X_UP(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_X_UP))
    EventMap[EventEnum::MOUSE_X_UP] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_X_UP, func);
}

void Frame::Set_MOUSE_Y_DOWN(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_Y_DOWN))
    EventMap[EventEnum::MOUSE_Y_DOWN] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_Y_DOWN, func);
}

void Frame::Set_MOUSE_Y_UP(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_Y_UP))
    EventMap[EventEnum::MOUSE_Y_UP] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_Y_UP, func);
}

void Frame::Set_MOUSE_SCROLL_DOWN(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_SCROLL_DOWN))
    EventMap[EventEnum::MOUSE_SCROLL_DOWN] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_SCROLL_DOWN, func);
}

void Frame::Set_MOUSE_SCROLL_UP(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_SCROLL_UP))
    EventMap[EventEnum::MOUSE_SCROLL_UP] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_SCROLL_UP, func);
}

void Frame::Set_MOUSE_AXIS_DOWN(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_AXIS_DOWN))
    EventMap[EventEnum::MOUSE_AXIS_DOWN] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_AXIS_DOWN, func);
}

void Frame::Set_MOUSE_AXIS_UP(FuncType func)
{
  if (EventMap.Contains(EventEnum::MOUSE_AXIS_UP))
    EventMap[EventEnum::MOUSE_AXIS_UP] = func;
  else
    EventMap.Emplace(EventEnum::MOUSE_AXIS_UP, func);
}
/*------------------------------------------------------------------------------
		Special script events
------------------------------------------------------------------------------*/
void Frame::OnUpdate(FuncType func)
{
  // Defaults to nullptr if not supplied
  
  // ScriptStruct script;
  // script.frame = this;
  // script.function = func;
  //
  // OnUpdateList.Emplace(script);
  
  OnUpdateFunc = func;
}
/*------------------------------------------------------------------------------
    Get functions
------------------------------------------------------------------------------*/
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
  if (this->EventMap.Contains(event)) // Check if it has the event
  {
    this->EventMap[event](this); // Call the event
  }
}

void Frame::Fire(EventEnum event)
{
  // print("Event:", event);
  switch (event) // Event override behavior can go in here
  {
    case EventEnum::SCORE_UPDATE:
      return;
    // case EventEnum::MOUSE_EXIT:
    //   return;
    // case EventEnum::MOUSE_CLICKED_DOWN:
    //   return;
    // case EventEnum::MOUSE_CLICKED_UP:
    //   return;
    // case EventEnum::UPDATE:
    //   return;
    // case EventEnum::FRAME_CREATED:
    //   return;
    // case EventEnum::GAME_START:
    //   return;
    // case EventEnum::GAME_STOP:
    //   return;
    // case EventEnum::KEY_DOWN:
    //   return;
    // case EventEnum::KEY_UP:
    //   return;
    // case EventEnum::MOUSE_MOVING:
    //   return;
    // case EventEnum::WINDOW_FOCUS_GAINED:
    //   return;
    // case EventEnum::WINDOW_FOCUS_LOST:
    //   return;
    // case EventEnum::DRAWING:
    //   return;
    // case EventEnum::SCORE_UPDATE:
    //   return;
    default: // If there is no special behavior for this event, pass it down here
      for (auto& f : FrameList) // Iterate through every frame
      {
        if (f->EventMap.Contains(event)) // Check if they have the event
        {
          f->EventMap[event](f); // Call the event
        }
      }
  }
}

void Frame::IterateOnUpdateList()
{
  for (auto& script : OnUpdateList)
  {
    script.function(script.frame);
  }
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
  
  // if (nType == "") // Type wasn't given, default to basic frame
  //   std::unique_ptr<Frame*> f(new Frame);
  // else if (nType == "2D") // A 2D frame for the flat user interface
  //   std::unique_ptr<Frame*> f(new Frame2D);
  // else if (nType == "3D") // A 3D frame for displaying in the game world
  //   std::unique_ptr<Frame*> f(new Frame3D);
  // else if (nType == "Actor") // A frame to hook to an actor for events? I dunno
  //   std::unique_ptr<Frame*> f(new FrameActor);

  if (nType == "") // Type wasn't given, default to basic frame
    f = new Frame();
  else if (nType == "2D") // A 2D frame for the flat user interface
    f = new Frame2D();
  else if (nType == "3D") // A 3D frame for displaying in the game world
    f = new Frame3D();
  else if (nType == "Actor") // A frame to hook to an actor for events? I dunno
    f = new FrameActor();
  
  FrameList.Emplace(f);
  
  // delete f; // TODO: Move this to the right place.
  
  // If there wasn't a name passed, give it a default name
  if (nName == "") f->SetName("Frame_" + FString::FromInt(count));
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
  
  print("Creating text object.");
  
  return text;
}
