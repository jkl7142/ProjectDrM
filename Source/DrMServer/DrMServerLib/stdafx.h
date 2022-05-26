#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAM_AND_MEAN		// Exclude rarely-used stuff from Windows headers


#include <Ws2tcpip.h>
#include <winsock2.h>
#include <tchar.h>

#include <assert.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cstdint>