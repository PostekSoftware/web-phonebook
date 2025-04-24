#include <htmcpp.h>

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

SERVER
{
    json data;
    loadDataFromFile(server.bookData, server.boodDataLoaded);
    loadLanguage(server.lang, server.langLoaded);
    data["page"] = json(server.bookData);
    data["lang"] = json(server.lang);
    data["admin"] = session->loggedIn;
    data["username"] = session->username;
    return data;
}