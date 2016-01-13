// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

// UCLASS()
// class GAME_API AMainHUD : public AHUD{
// 	GENERATED_BODY()
//
// 	AMainHUD(const FObjectInitializer& ObjectInitializer);
//
// 	// Variable for storing the font
// 	UPROPERTY()
// 	UFont* HUDFont;
//
// 	// Primary draw call for the HUD
// 	virtual void DrawHUD() override;
//
// 	private:
// 		// Crosshair asset pointer
// 		UTexture2D* CrosshairTex;
// };

struct Message
{
	FString message;
	float time;
	FColor color;
	Message()
	{
		time = 5.f;
		color = FColor::White;
	}
	
	Message(FString nMessage, float nTime, FColor nColor)
	{
		message = nMessage;
		time = nTime;
		color = nColor;
	}
};

USTRUCT()
struct FJoyButtonStruct
{
	GENERATED_USTRUCT_BODY()
 
	//Vars
	int32 		type;
	FString	toolTip;
	float 		minX;
	float 		maxX;
	float 		minY;
	float 		maxY;
 
	//default properties
	FJoyButtonStruct(){
		type 	= -1;
		toolTip = "";
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
	}
};
 
UCLASS()
class GAME_API AMainHUD : public AHUD
{
	GENERATED_UCLASS_BODY()
	
	// Variable for storing the font
	UPROPERTY()
	UFont* HUDFont;
	
	// // Crosshair asset pointer
	// UTexture2D* CrosshairTex;
	
	// Array of messages
	TArray<Message> messages;
	
	//
	void addMessage(Message msg);
	
	// AMainHUD(const FObjectInitializer& ObjectInitializer);
 
	// Font
	// I recommend creating the font at a high resolution / size like 36
	// then you can scale down the font as needed to any size of your choice
 
	/** Verdana */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MainHUD)
	UFont* VerdanaFont;
 
	/** Put Roboto Here */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MainHUD)
	UFont* UE4Font;
	
	/** Font Scaling Used By Your HUD Code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MainHUD)
	float DefaultFontScale;
 
	/** HUD Scaling */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MainHUD)
	float GlobalHUDMult;
 
	// T2D
	/** Cursor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=T2D)
	UTexture2D* CursorMain;
 
	/** Hovering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=T2D)
	UTexture2D* CursorHoveringButton;
 
	/** Button */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=T2D)
	UTexture2D* ButtonBackground;
 
	// Materials
	/** Events */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Materials)
	UMaterialInterface* MaterialBackground;
 
	/** Draw frames? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Options)
	bool bDrawFrames;
 
	/* Draw Hud? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Options)
	bool DontDrawHUD;
	
	/* Show message box? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Options)
	bool bShowMessages;
	
	/** Show the menu? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Options)
	bool bShowMainMenu;
 
//Cursor
public:
	FVector2D MouseLocation;
	void DrawHUD_DrawCursor();
 
//Buttons
public:
	TArray<FJoyButtonStruct> ButtonsMain;
	TArray<FJoyButtonStruct> ButtonsConfirm;
 
	//Cursor In buttons
	void DrawHUD_CheckCursorInButtons();
	void CheckCursorInButtonsMain();
	void CheckCursorInButtonsConfirm();
 
	const FJoyButtonStruct* CurCheckButton;
	int32 CheckCursorInButton(const TArray<FJoyButtonStruct>& ButtonArray);
	int32 ClickedButtonType;
	//States
	bool ConfirmDialogOpen;
	bool InMainMenu;
 
	int32 		ActiveButton_Type;
	FString 	ActiveButton_Tip;
	bool CursorHoveringInButton;
//Colors
public:
	const FLinearColor * ColorPtr;
 
	//Colors
	static const FColor FColorBlack;
	static const FColor FColorRed;
	static const FColor FColorYellow;
	static const FColor FColorBlue;
	static const FColor FColor_White;
 
	static const FLinearColor LC_Black;
	static const FLinearColor LC_Pink;
	static const FLinearColor LC_Red;
	static const FLinearColor LC_Yellow;
//FString
public:
 
	//`Titles
	static const FString S_Title_Main;
	static const FString S_Title_Confirm;
	//`Button Text
	static const FString S_Button_Restart;
	static const FString S_Button_Exit;
 
// Utility
 
//Stop Camera From Moving With Mouse
FORCEINLINE void SetCursorMoveOnly(bool CursorOnly){
	if (!PC) return;
	//
	PC->SetIgnoreLookInput(CursorOnly);
}
 
//DrawLine
FORCEINLINE void DrawJoyLine(
	const FVector2D& Start,
	const FVector2D& End,
	const FLinearColor& TheColor,
	const float& Thick){
		
	if (!Canvas) return;

	FCanvasLineItem NewLine(Start,End);
	NewLine.SetColor(TheColor);
	NewLine.LineThickness = Thick;
	Canvas->DrawItem(NewLine);
}
 
FORCEINLINE void DrawJoyRect(
	float X, float Y,
	float Width, float Height,
	const FLinearColor& Color){
		
	if (!Canvas) return;
 
	FCanvasTileItem RectItem(
		FVector2D(X, Y),
		FVector2D( Width, Height ),
		Color
	);
 
  RectItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(RectItem);
}
 
//DrawText
FORCEINLINE void DrawJoyText(
	UFont*	TheFont,
	const FString& TheStr,
	const float& X, const float& Y,
	const FLinearColor& TheColor,
	const float& TheScale,
	bool DrawOutline=false,
	const FLinearColor OutlineColor=FLinearColor(0,0,0,1)){
	if(!Canvas) return;
	//
 
	//Text and Font
	FCanvasTextItem NewText(
		FVector2D(X,Y),
		FText::FromString(TheStr),
		TheFont,
		TheColor
	);
 
	//Text Scale
	NewText.Scale.Set(TheScale,TheScale);
 
	//Outline gets its alpha from the main color
	NewText.bOutlined = true;
	NewText.OutlineColor = OutlineColor;
	NewText.OutlineColor.A = TheColor.A * 2;
 
	//Draw
	Canvas->DrawItem(NewText);
}

//Draw Full Size Tile
FORCEINLINE void DrawFullSizeTile(UTexture2D* tex, float x, float y, const FColor& Color){
	if (!Canvas) return;
	if (!tex) return;
	//~~
 
	Canvas->SetDrawColor(Color);
 
	//Draw
	Canvas->DrawTile(
		tex, x, y, //z pos
		tex->GetSurfaceWidth(), //screen width
		tex->GetSurfaceHeight(),  //screen height
		0, //texture start width
		0, //texture start height
		tex->GetSurfaceWidth(), //texture width from start
		tex->GetSurfaceHeight(), //texture height from start
		BLEND_Translucent
	);
}
 
FORCEINLINE void VDrawTile(UTexture2D* tex, float x, float y, float screenX, float screenY, const FColor& TheColor){
	if (!Canvas) return;
	if (!tex) return;
	//~
 
	Canvas->SetDrawColor(TheColor);
 
	//Draw
	Canvas->DrawTile(
		tex, x, y, //z pos
		screenX, //screen width
		screenY,  //screen height
		0, //texture start width
		0, //texture start height
		tex->GetSurfaceWidth(), //texture width from start
		tex->GetSurfaceHeight(), //texture height from start
		BLEND_Translucent
	);
}
 
//~
 
// Draw
public:
	void DrawHUD_DrawDialogs();
	
	void CheckMouseoverFrames(Frame* f, int32 left, int32 right, int32 top, int32 bottom);
	void DrawFrames();
	void DrawMessageBox();
	void DrawPrintBox();
	FString EscapeSequences(int32 searchNum, FString str);
	void Startup();
	// void CreateFrame(FString nName, FString nStrata, int32 nLevel);
 
	//Menus
	void DrawMainMenu();
	void DrawConfirm();
 
	//Buttons
	void DrawMainMenuButtons();
	void DrawConfirmButtons();
public:
	void DrawToolTip();
 
//Core
public:
	static APlayerController* PC;
	void PlayerInputChecks();
	static void CheckAllKeys();
	// void CheckGamepadButtons();
	// void CheckGestures();
	// void CheckControllerKeys();
	// void CheckMotionControllerKeys();
	// void CheckPS4SpecificKeys();
	// void CheckSteamControllerKeys();
	// void CheckXBoxOneGlobalSpeechCommands();
	// void CheckAndroidKeys();
protected:
	//Draw HUD
	void DrawHUD_Reset();
	// virtual void DrawHUD() OVERRIDE;
	virtual void DrawHUD();
 
	/** after all game elements are created */
	// virtual void PostInitializeComponents() OVERRIDE;
	virtual void PostInitializeComponents();
};
