// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

enum EventEnum
{
	MOUSE_ENTER,
	MOUSE_EXIT,
	MOUSE_CLICKED_DOWN,
	MOUSE_CLICKED_UP,
	UPDATE,
	FRAME_CREATED,
	GAME_START,
	GAME_PAUSE,
	GAME_STOP,
	WINDOW_FOCUS_GAINED,
	WINDOW_FOCUS_LOST,
	KEY_DOWN,
	KEY_UP,
	DRAWING,
	SCORE_UPDATE,
	MOUSE_MOVING,
	
	MOUSE_X_DOWN,
	MOUSE_X_UP,
	MOUSE_Y_DOWN,
	MOUSE_Y_UP,
	MOUSE_SCROLL_DOWN,
	MOUSE_SCROLL_UP,
	MOUSE_AXIS_DOWN,
	MOUSE_AXIS_UP,

	// Key events
	KEY_DOWN_MouseX,
	KEY_UP_MouseX,
	KEY_DOWN_MouseY,
	KEY_UP_MouseY,
	KEY_DOWN_MouseScrollUp,
	KEY_UP_MouseScrollUp,
	KEY_DOWN_MouseScrollDown,
	KEY_UP_MouseScrollDown,
	KEY_DOWN_MouseWheelAxis,
	KEY_UP_MouseWheelAxis,

	KEY_DOWN_LeftMouseButton,
	KEY_UP_LeftMouseButton,
	KEY_DOWN_RightMouseButton,
	KEY_UP_RightMouseButton,
	KEY_DOWN_MiddleMouseButton,
	KEY_UP_MiddleMouseButton,
	KEY_DOWN_ThumbMouseButton,
	KEY_UP_ThumbMouseButton,
	KEY_DOWN_ThumbMouseButton2,
	KEY_UP_ThumbMouseButton2,

	KEY_DOWN_BackSpace,
	KEY_UP_BackSpace,
	KEY_DOWN_Tab,
	KEY_UP_Tab,
	KEY_DOWN_Enter,
	KEY_UP_Enter,
	KEY_DOWN_Pause,
	KEY_UP_Pause,

	KEY_DOWN_CapsLock,
	KEY_UP_CapsLock,
	KEY_DOWN_Escape,
	KEY_UP_Escape,
	KEY_DOWN_SpaceBar,
	KEY_UP_SpaceBar,
	KEY_DOWN_PageUp,
	KEY_UP_PageUp,
	KEY_DOWN_PageDown,
	KEY_UP_PageDown,
	KEY_DOWN_End,
	KEY_UP_End,
	KEY_DOWN_Home,
	KEY_UP_Home,

	KEY_DOWN_Left,
	KEY_UP_Left,
	KEY_DOWN_Up,
	KEY_UP_Up,
	KEY_DOWN_Right,
	KEY_UP_Right,
	KEY_DOWN_Down,
	KEY_UP_Down,

	KEY_DOWN_Insert,
	KEY_UP_Insert,
	KEY_DOWN_Delete,
	KEY_UP_Delete,

	KEY_DOWN_Zero,
	KEY_UP_Zero,
	KEY_DOWN_One,
	KEY_UP_One,
	KEY_DOWN_Two,
	KEY_UP_Two,
	KEY_DOWN_Three,
	KEY_UP_Three,
	KEY_DOWN_Four,
	KEY_UP_Four,
	KEY_DOWN_Five,
	KEY_UP_Five,
	KEY_DOWN_Six,
	KEY_UP_Six,
	KEY_DOWN_Seven,
	KEY_UP_Seven,
	KEY_DOWN_Eight,
	KEY_UP_Eight,
	KEY_DOWN_Nine,
	KEY_UP_Nine,

	KEY_DOWN_A,
	KEY_UP_A,
	KEY_DOWN_B,
	KEY_UP_B,
	KEY_DOWN_C,
	KEY_UP_C,
	KEY_DOWN_D,
	KEY_UP_D,
	KEY_DOWN_E,
	KEY_UP_E,
	KEY_DOWN_F,
	KEY_UP_F,
	KEY_DOWN_G,
	KEY_UP_G,
	KEY_DOWN_H,
	KEY_UP_H,
	KEY_DOWN_I,
	KEY_UP_I,
	KEY_DOWN_J,
	KEY_UP_J,
	KEY_DOWN_K,
	KEY_UP_K,
	KEY_DOWN_L,
	KEY_UP_L,
	KEY_DOWN_M,
	KEY_UP_M,
	KEY_DOWN_N,
	KEY_UP_N,
	KEY_DOWN_O,
	KEY_UP_O,
	KEY_DOWN_P,
	KEY_UP_P,
	KEY_DOWN_Q,
	KEY_UP_Q,
	KEY_DOWN_R,
	KEY_UP_R,
	KEY_DOWN_S,
	KEY_UP_S,
	KEY_DOWN_T,
	KEY_UP_T,
	KEY_DOWN_U,
	KEY_UP_U,
	KEY_DOWN_V,
	KEY_UP_V,
	KEY_DOWN_W,
	KEY_UP_W,
	KEY_DOWN_X,
	KEY_UP_X,
	KEY_DOWN_Y,
	KEY_UP_Y,
	KEY_DOWN_Z,
	KEY_UP_Z,

	KEY_DOWN_NumPadZero,
	KEY_UP_NumPadZero,
	KEY_DOWN_NumPadOne,
	KEY_UP_NumPadOne,
	KEY_DOWN_NumPadTwo,
	KEY_UP_NumPadTwo,
	KEY_DOWN_NumPadThree,
	KEY_UP_NumPadThree,
	KEY_DOWN_NumPadFour,
	KEY_UP_NumPadFour,
	KEY_DOWN_NumPadFive,
	KEY_UP_NumPadFive,
	KEY_DOWN_NumPadSix,
	KEY_UP_NumPadSix,
	KEY_DOWN_NumPadSeven,
	KEY_UP_NumPadSeven,
	KEY_DOWN_NumPadEight,
	KEY_UP_NumPadEight,
	KEY_DOWN_NumPadNine,
	KEY_UP_NumPadNine,

	KEY_DOWN_Multiply,
	KEY_UP_Multiply,
	KEY_DOWN_Add,
	KEY_UP_Add,
	KEY_DOWN_Subtract,
	KEY_UP_Subtract,
	KEY_DOWN_Decimal,
	KEY_UP_Decimal,
	KEY_DOWN_Divide,
	KEY_UP_Divide,

	KEY_DOWN_F1,
	KEY_UP_F1,
	KEY_DOWN_F2,
	KEY_UP_F2,
	KEY_DOWN_F3,
	KEY_UP_F3,
	KEY_DOWN_F4,
	KEY_UP_F4,
	KEY_DOWN_F5,
	KEY_UP_F5,
	KEY_DOWN_F6,
	KEY_UP_F6,
	KEY_DOWN_F7,
	KEY_UP_F7,
	KEY_DOWN_F8,
	KEY_UP_F8,
	KEY_DOWN_F9,
	KEY_UP_F9,
	KEY_DOWN_F10,
	KEY_UP_F10,
	KEY_DOWN_F11,
	KEY_UP_F11,
	KEY_DOWN_F12,
	KEY_UP_F12,

	KEY_DOWN_NumLock,
	KEY_UP_NumLock,

	KEY_DOWN_ScrollLock,
	KEY_UP_ScrollLock,

	KEY_DOWN_LeftShift,
	KEY_UP_LeftShift,
	KEY_DOWN_RightShift,
	KEY_UP_RightShift,
	KEY_DOWN_LeftControl,
	KEY_UP_LeftControl,
	KEY_DOWN_RightControl,
	KEY_UP_RightControl,
	KEY_DOWN_LeftAlt,
	KEY_UP_LeftAlt,
	KEY_DOWN_RightAlt,
	KEY_UP_RightAlt,
	KEY_DOWN_LeftCommand,
	KEY_UP_LeftCommand,
	KEY_DOWN_RightCommand,
	KEY_UP_RightCommand,

	KEY_DOWN_Semicolon,
	KEY_UP_Semicolon,
	KEY_DOWN_Equals,
	KEY_UP_Equals,
	KEY_DOWN_Comma,
	KEY_UP_Comma,
	KEY_DOWN_Underscore,
	KEY_UP_Underscore,
	KEY_DOWN_Hyphen,
	KEY_UP_Hyphen,
	KEY_DOWN_Period,
	KEY_UP_Period,
	KEY_DOWN_Slash,
	KEY_UP_Slash,
	KEY_DOWN_Tilde,
	KEY_UP_Tilde,
	KEY_DOWN_LeftBracket,
	KEY_UP_LeftBracket,
	KEY_DOWN_LeftParantheses,
	KEY_UP_LeftParantheses,
	KEY_DOWN_Backslash,
	KEY_UP_Backslash,
	KEY_DOWN_RightBracket,
	KEY_UP_RightBracket,
	KEY_DOWN_RightParantheses,
	KEY_UP_RightParantheses,
	KEY_DOWN_Apostrophe,
	KEY_UP_Apostrophe,
	KEY_DOWN_Quote,
	KEY_UP_Quote,

	KEY_DOWN_Asterix,
	KEY_UP_Asterix,
	KEY_DOWN_Ampersand,
	KEY_UP_Ampersand,
	KEY_DOWN_Caret,
	KEY_UP_Caret,
	KEY_DOWN_Dollar,
	KEY_UP_Dollar,
	KEY_DOWN_Exclamation,
	KEY_UP_Exclamation,
	KEY_DOWN_Colon,
	KEY_UP_Colon,

	KEY_DOWN_A_AccentGrave,
	KEY_UP_A_AccentGrave,
	KEY_DOWN_E_AccentGrave,
	KEY_UP_E_AccentGrave,
	KEY_DOWN_E_AccentAigu,
	KEY_UP_E_AccentAigu,
	KEY_DOWN_C_Cedille,
	KEY_UP_C_Cedille,
	KEY_DOWN_Section,
	KEY_UP_Section,
};

// class FrameText
// {
//   private:
//     int32 w, h;
//     int32 x, y;
//     int32 r, g, b, a;
//     int32 scale;
//     bool shown;
//     FString type;
//     FString strata;
//     FString name;
//     // Frame* parent;
//     static int32 count;
//
//   public:
// };

/**
 *
 */
class GAME_API Frame
{
	// Sockets
	private:
		typedef void (*FuncType)(Frame*);
	public:
		TMap<EventEnum, FuncType> EventMap;
		
		void Set_MOUSE_ENTER(FuncType func);
		void Set_MOUSE_EXIT(FuncType func);
		void Set_MOUSE_CLICKED_DOWN(FuncType func);
		void Set_MOUSE_CLICKED_UP(FuncType func);
		void Set_UPDATE(FuncType func);
		void Set_FRAME_CREATED(FuncType func);
		void Set_WINDOW_FOCUS_GAINED(FuncType func);
		void Set_WINDOW_FOCUS_LOST(FuncType func);
		void Set_KEY_DOWN(FuncType func);
		void Set_KEY_UP(FuncType func);
		void Set_GAME_START(FuncType func);
		void Set_GAME_PAUSE(FuncType func);
		void Set_GAME_STOP(FuncType func);
		void Set_MOUSE_MOVING(FuncType func);
		void Set_MOUSE_X_DOWN(FuncType func);
		void Set_MOUSE_X_UP(FuncType func);
		void Set_MOUSE_Y_DOWN(FuncType func);
		void Set_MOUSE_Y_UP(FuncType func);
		void Set_MOUSE_SCROLL_DOWN(FuncType func);
		void Set_MOUSE_SCROLL_UP(FuncType func);
		void Set_MOUSE_AXIS_DOWN(FuncType func);
		void Set_MOUSE_AXIS_UP(FuncType func);
		
		// KEY_DOWN_MouseX,
		// KEY_UP_MouseX,
		// KEY_DOWN_MouseY,
		// KEY_UP_MouseY,
		// KEY_DOWN_MouseScrollUp,
		// KEY_UP_MouseScrollUp,
		// KEY_DOWN_MouseScrollDown,
		// KEY_UP_MouseScrollDown,
		// KEY_DOWN_MouseWheelAxis,
		// KEY_UP_MouseWheelAxis,
	private:
		enum AnchorPoints
		{
			TOPLEFT,
			TOP,
			TOPRIGHT,
			LEFT,
			CENTER,
			RIGHT,
			BOTTOMLEFT,
			BOTTOM,
			BOTTOMRIGHT,
		};
		
		float w, h;
		float x, y;
		float r, g, b, a;
		float scale;
		int32 level;
		bool shown;
		bool mouseOver;
		FString type;
		FString strata;
		FString name;
		Frame* parent;
		static int32 count;
		static TArray<Frame*> FrameList; // All frames are stored in here
		
		AnchorPoints anchorPoint;
		Frame* relativeTo;
		AnchorPoints relativePoint;
		int32 offsetX;
		int32 offsetY;
		
		struct Level
		{
			static TArray<Frame*> FrameList;
			Level(){}
		};
		
		struct Strata // The basic array
		{
			static TMap<int32, Level> LevelMap;
			Strata(){}
		};
		
		struct ScriptStruct
		{
			Frame* frame;
			void (*function)(Frame*);
		};
		
		static TArray<ScriptStruct> OnUpdateList;
		static TArray<FString> EventList;
	protected:
		
	public:
		void SetPoint(AnchorPoints nAnchor, int32 nX, int32 nY, int32 nOffsetX, int32 nOffsetY, Frame* relativeTo);
		
    static const FString strataList[5];
    
    static TMap<FString, Strata> StrataMap;
    // static TArray<FrameText> TextList;
		
		void OnUpdate(FuncType func);
    
    Frame();
    ~Frame();
    
		/*--------------------------------------------------------------------------
				Get functions
		--------------------------------------------------------------------------*/
    float GetWidth() const;
    float GetHeight() const;
    float GetSize() const;
    float GetX() const;
    float GetY() const;
    float GetPosition() const;
    float GetColorInt() const;
    FLinearColor GetColor() const;
    float GetAlpha() const;
    float GetScale() const;
    int32 GetLevel() const;
    bool IsShown() const;
    bool GetMouseOver() const;
    FString GetType() const;
    FString GetStrata() const;
    FString GetName() const;
    Frame* GetParent() const;
		    
		/*--------------------------------------------------------------------------
				Set functions
		--------------------------------------------------------------------------*/
		void Show();
		void Hide();
    void SetWidth(float nW);
    void SetHeight(float nH);
    void SetSize(float nW, float nH);
    void SetX(float nX);
    void SetY(float nY);
    void SetPosition(float nX, float nY);
    void SetScale(float nScale);
    void SetName(FString nName);
    void SetType(FString nType);
    void SetShown(bool nVisibility);
    void SetMouseOver(bool nMouseOver);
    void SetColor(float nR = 1.f, float nG = 1.f, float nB = 1.f, float nA = 1.f);
    void SetAlpha(float nA = 1.f);
    void SetLevel(int32 nLevel);
    void SetStrata(FString nStrata);
    void SetParent(Frame* nParent);
		
		/*--------------------------------------------------------------------------
				Misc frame functions
		--------------------------------------------------------------------------*/
    static Frame* CreateFrame(FString nType, FString nName, FString nStrata, int32 nLevel);
    static void IterateOnUpdateList();
		static void Fire(EventEnum event);
		void FireToFrame(EventEnum event);
};
