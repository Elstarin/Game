// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

class GAME_API TimerSystem
{
  private:
    typedef std::function<void()> CallbackType;
  public:
    struct LightTimerStruct
    {
      double remaining;
			void (*callback)();
    };
    
    struct TimerStruct
    {
      double remaining;
			// void (*callback)();
      CallbackType callback;
    };
    
    struct TickerStruct
    {
      double current;
      double interval;
      // void (*callback)();
      // void (*callbackArg)(TickerStruct*);
      // CallbackType callback;
      // std::function<void()> callback;
      std::function<void(TickerStruct*)> callbackArg;
      int32 ticks;
      bool cancelled;
      bool arg;
      
      // Set it to cancelled; it will be removed next iteration
      inline void Cancel(){this->cancelled = true;}
    };
    // typedef std::function<void(TickerStruct*)> CallbackArgType;
		
		TimerSystem();
		~TimerSystem();
		
		static double GetTime();
		static uint32 GetCycles();
		static int32 GetTodayTime();
		static int64 GetTodayTicks();
    
    static TArray<LightTimerStruct*> LightTimerList;
    static TArray<TimerStruct*> TimerList;
    static TArray<TickerStruct*> TickerList;
    
    static void SetLightTimer(double time, void (*func)());
    
    static void SetTimer(double time, CallbackType func);
    
    // static void SetTicker(double time, std::function<void()> func, int32 numTicks = -1);
    
    // Overload function that will pass the ticker
    static void SetTicker(double time, std::function<void(TickerStruct*)> func, int32 numTicks = -1);
    
    static void IterateTimerArrays();
};
