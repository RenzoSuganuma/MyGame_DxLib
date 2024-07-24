#pragma once
#include "iostream"

extern int pigeon_se;
extern unsigned int windowWidth_;
extern unsigned int windowHeigth_;
extern unsigned int refreshRate_;
extern bool changeWindowMode_;
extern int quitKey_;
//DWORD currentTime;

extern void const SetScreenSize(unsigned int width, unsigned int height);

extern  std::pair< unsigned int, unsigned int >
const GetScreenSize();

extern void const SetChangeWindowMode(bool cond);

extern  const bool const GetChangeWindowMode();

extern  void const SetRefreshRate(unsigned int rate);

extern  const int const GetQuitKey();

extern  void const SetQuitKey(int keyCode);

// extern  const DWORD const GetCurrentSystemTime();

