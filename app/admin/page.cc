#include <htmcpp.h>

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

SERVER
{
    json data;
    if (!session->loggedIn)
    {
        res.set_redirect("/login", 302);
        return data;
    }
    data["username"] = session->username;
    return data;
}