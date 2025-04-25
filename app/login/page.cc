#include <htmcpp.h>

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

SERVER
{
    json data;
    if (session->loggedIn)
    {
        res.set_redirect("/admin", 302);
        return data;
    }
    loadDataFromFile(server.bookData, server.boodDataLoaded);
    data["page"] = json(server.bookData);
    data["invalid"] = req.has_param("invalid");
    return data;
}