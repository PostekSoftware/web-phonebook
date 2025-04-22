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
    if (req.get_param_value("logout") != "")
    {
        session->loggedIn = false;
        session->username = "";
        res.set_redirect("/login");
    }
    return data;
}