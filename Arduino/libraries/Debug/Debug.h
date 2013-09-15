/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
 
 Nice little macros to help with "printf" style debugging
*/

#ifndef __Debug__
#define __Debug__

#include "WProgram.h"

#ifdef __DEBUG__
#define DEBUG_(x) Serial.print(x)
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG_(x)
#define DEBUG(x)
#endif

#define DEBUG2(x1, x2) DEBUG_(x1); DEBUG(x2)
#define DEBUG3(x1, x2, x3) DEBUG_(x1); DEBUG_(x2); DEBUG(x3)
#define DEBUG4(x1, x2, x3, x4) DEBUG_(x1); DEBUG_(x2); DEBUG_(x3); DEBUG(x4)
#define DEBUG5(x1, x2, x3, x4, x5) DEBUG_(x1); DEBUG_(x2); DEBUG_(x3); DEBUG_(x4); DEBUG(x5)
#define DEBUG6(x1, x2, x3, x4, x5, x6) DEBUG_(x1); DEBUG_(x2); DEBUG_(x3); DEBUG_(x4); DEBUG_(x5); DEBUG(x6)

#endif