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
    loadAdminSettings(server.admins, server.adminsLoaded);
    std::string u = "";
    for (const admin &a : server.admins)
    {
        printf("checking username: %s, param: %s\n", a.username.c_str(), req.get_param_value("username").c_str());
        if (a.username == req.get_param_value("username") && a.password == req.get_param_value("password"))
        {
            u = a.username;
            break;
        }
    }
    if (u == "")
    {
        res.set_redirect("/login?invalid=true");
        data["error"] = "Invalid username or password";
        return data;
    }
    session->loggedIn = true;
    session->username = u;
    printf("User logged in: %s\n", u.c_str());
    res.set_redirect("/admin", 302);

    data["info"] = "Logged in as " + session->username;
    return data;
}