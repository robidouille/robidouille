/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

*/

#include "Arduino.h"
#include "CommandStream.h"

void CommandStream::HandleChar(char cc) {
	if (cc >= '0' && cc <= '9') {
		if (fHasCommand) {
			fValue = 0;
			fHasCommand = false;
		}
		fValue = fValue * 10 + (cc - '0');
	}
	else if ((cc >= 'A' && cc <= 'Z') || (cc >= 'a' && cc <= 'z')) {
		fCommand = cc;
		fHasCommand = true;
	}
}
