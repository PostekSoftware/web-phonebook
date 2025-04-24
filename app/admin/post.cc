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
    else if (req.get_param_value("update") != "")
    {
        json jsond = json::parse(req.get_param_value("update"));
        page b = (page)jsond;
        server.bookData = b;
        saveDataToFile(server.bookData);
        res.set_redirect("/");
    }
    else if (req.get_param_value("lang") != "")
    {
        json jsond = json::parse(req.get_param_value("lang"));
        language b = (language)jsond;
        server.lang = b;
        saveLanguage(server.lang);
        res.set_redirect("/");
    }
    return data;
}