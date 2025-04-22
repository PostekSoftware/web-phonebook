#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>
#include <md5.h>
#include <nlohmann/json.hpp>
#include <dlfcn.h>
#include <stdlib.h>
#include <httplib.h>
#include <cstdio>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cstdint>

#include "config.h"
#include "global.h"
#include "utils.h"
#include "server.h"

struct page_config
{
    std::string path;
    bool page = false;
    std::string md5page = "";
    bool pageCc = false;
    std::string md5pageCpp = "";
    bool getCc = false;
    std::string md5getCpp = "";
    bool postCc = false;
    std::string md5postCpp = "";
    bool error = false;
    std::string md5error = "";
    bool errorCc = false;
    std::string md5errorCpp = "";
};

enum fileType
{
    PAGE,
    GET,
    POST,
    ERROR
};

struct filePath
{
    fileType type;
    std::string path;
};

std::vector<std::string> includeDirs = CONF_APP_INCLUDES;
std::vector<std::string> linkFiles = CONF_APP_LINK;
std::map<std::pair<fileType, std::string>, std::string> filePaths;
std::map<std::pair<fileType, std::string>, std::string> fileMd5s;

serverData sData;
std::map<std::string, std::pair<int64_t, sessionData *>> sessions;

struct prebuiltPage
{
    std::string obj;
    nlohmann::json (*func)(const httplib::Request &, httplib::Response &, serverData &, sessionData *);
};
std::map<std::pair<fileType, std::string>, prebuiltPage> prebuiltPages;

page_config getPageConfig(const std::string &path);

void buildPage(fileType type, const std::string &path);

nlohmann::json getServerData(fileType type, const std::string &path, const httplib::Request &req, httplib::Response &res, std::string sessionId);

void prebuildAllPages();
void loadPrebuiltPages();