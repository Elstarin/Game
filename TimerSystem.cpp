// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "TimerSystem.h"

// TimerSystem::TimerSystem()
// {
// }
//
// TimerSystem::~TimerSystem()
// {
// }

// FTimespan timeSpan = FTimespan(/*Hours*/0,/*Minutes*/0, realtimeSeconds /*Seconds*/);

inline double TimerSystem::GetTime()
{
  return FPlatformTime::Seconds();
}

inline uint32 TimerSystem::GetCycles()
{
  return FPlatformTime::Cycles();
}

inline int32 TimerSystem::GetTodayTime()
{
  return FDateTime::Now().GetTimeOfDay().GetTotalMilliseconds();
}

inline int64 TimerSystem::GetTodayTicks()
{
  return FDateTime::Now().GetTimeOfDay().GetTicks();
}

TArray<TimerSystem::TimerStruct*> TimerSystem::TimerList;
TArray<TimerSystem::TickerStruct*> TimerSystem::TickerList;

void TimerSystem::SetTimer(double time, void (*func)())
{
  TimerStruct* timer = new TimerStruct();
  
  timer->remaining = time + GetTime();
  timer->callback = func;
  
  TimerList.Emplace(timer);
}

void TimerSystem::SetTicker(double time, void (*func)(), int32 numTicks)
{
  TickerStruct* ticker = new TickerStruct();

  ticker->current = time + GetTime();
  ticker->interval = time;
  ticker->callback = func;
  ticker->ticks = numTicks; // Defaults to -1, which means it won't automatically stop
  ticker->cancelled = false;
  ticker->arg = false;

  TickerList.Emplace(ticker);
}

// Overload function that will pass the ticker
void TimerSystem::SetTicker(double time, void (*func)(TickerStruct*), int32 numTicks)
{
  TickerStruct* ticker = new TickerStruct();

  ticker->current = time + GetTime();
  ticker->interval = time;
  ticker->callbackArg = func; // callbackArg so the iterator knows to pass ticker
  ticker->ticks = numTicks; // Defaults to -1, which means it won't automatically stop
  ticker->cancelled = false;
  ticker->arg = true;

  TickerList.Emplace(ticker);
}

void TimerSystem::IterateTimerArrays()
{
  auto cTime = GetTime(); // Time since game started
  
  // Run through every timer backwards
  for (int32 i = TimerList.Num() - 1; i >= 0; --i)
  {
    if (cTime >= TimerList[i]->remaining) // Timer is up
    {
      TimerList[i]->callback(); // Call its function
      delete TimerList[i];
      TimerList.RemoveAtSwap(i); // Swap because I don't care about the order
    }
  }
  
  // Run through every ticker backwards
  for (int32 i = TickerList.Num() - 1; i >= 0; --i)
  {
    if (true == TickerList[i]->cancelled) // Was cancelled, remove it
    {
      delete TickerList[i];
      TickerList.RemoveAtSwap(i);
    }
    else if (cTime >= TickerList[i]->current) // Make it tick once
    {
      // Call its function
      if (TickerList[i]->arg) TickerList[i]->callbackArg(TickerList[i]);
      else TickerList[i]->callback();

      TickerList[i]->current += TickerList[i]->interval; // Set it for the next tick
      TickerList[i]->ticks--; // Take 1 off its remaining ticks
  
      if (TickerList[i]->ticks == 0) // No ticks left, remove it
      {
        delete TickerList[i];
        TickerList.RemoveAtSwap(i); // Swap because I don't care about the order
      }
    }
  }
}
