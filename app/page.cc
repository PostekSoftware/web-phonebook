#include <htmcpp.h>

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

SERVER
{
    json data;
    loadDataFromFile(server.bookData, server.boodDataLoaded);
    data["page"] = json(server.bookData);
    data["admin"] = session->loggedIn;
    data["username"] = session->username;
    return data;
}