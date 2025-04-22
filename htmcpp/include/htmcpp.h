#pragma once

#include <nlohmann/json.hpp>
#include <httplib.h>
#include <server.h>

using namespace nlohmann;

#define SERVER \
    extern "C" nlohmann::json srv(const httplib::Request &req, httplib::Response &res, serverData &server, sessionData *session) // nlohmann::json srv(const httplib::Request &req, httplib::Response &res)
