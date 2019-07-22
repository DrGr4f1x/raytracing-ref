#pragma once

// TODO: reference additional headers your program requires here

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

// Standard headers
#include <atomic>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <stdint.h>
#include <sstream>
#include <vector>

// Core headers
#include "timer.h"
#include "vec3.h"