/* Copyright (c) 2002-2012 Croteam Ltd. All rights reserved. */

#include <windows.h>

unsigned long DetermineDesktopWidth(void)
{
  return((unsigned long) ::GetSystemMetrics(SM_CXSCREEN));
}


