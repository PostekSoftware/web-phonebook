#pragma once

#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <vector>
#include <sstream>

std::string getUuid();
std::vector<std::string> splitString(const std::string &str, const std::string &token);
