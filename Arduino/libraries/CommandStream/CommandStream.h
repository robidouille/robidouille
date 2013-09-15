/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

*/

#ifndef __CommandStream__
#define __CommandStream__

#include "Arduino.h"

class CommandStream {
public:
	CommandStream() {
		fValue = 0;
		fCommand = 0;
		fHasCommand = false;
	}

	void HandleChar(char cc);

	bool HasCommand() const {return fHasCommand;}
	char GetCommand() const {return fCommand;}
	unsigned long GetValue() const {return fValue;}

private:
	unsigned long fValue;
	char fCommand;
	bool fHasCommand;
};

#endif
