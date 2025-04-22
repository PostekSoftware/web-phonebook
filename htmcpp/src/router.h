#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include <httplib.h>
#include <inja.hpp>

#include "config.h"
#include "compiler.h"

std::vector<std::string> getPath(const std::string &path);
std::string getStaticPageContent(const std::string &path);
std::string getPage(const std::string &path, const httplib::Request &req, httplib::Response &res);