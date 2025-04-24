#pragma once

#include "web-phonebook.h"

struct serverData
{
    page bookData = page();
    bool boodDataLoaded = false;
    std::vector<admin> admins = std::vector<admin>();
    bool adminsLoaded = false;
    language lang = language();
    bool langLoaded = false;
};

struct sessionData
{
    int loggedIn = false;
    std::string username = "";
};