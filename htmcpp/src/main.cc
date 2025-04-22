#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "config.h"
#include "router.h"
#include "global.h"

void quit(const std::string &arg)
{
    fprintf(stderr, "Invalid argument '%s'!\nArguments: dev, build, start\n", arg.c_str());
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        quit("");
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "dev")
        {
            G_isDev = true;
            break;
        }
        else if (arg == "build")
        {
            G_build = true;
            break;
        }
        else if (arg == "start")
        {
            G_run = true;
            break;
        }
        else
        {
            quit(arg);
        }
    }

    if (G_build)
    {
        prebuildAllPages();
        exit(0);
    }

    if (G_run)
    {
        loadPrebuiltPages();
    }

    if (G_isDev)
        printf("Starting in dev mode!\n");

    httplib::Server srv;

    srv.set_mount_point("/static", "static");

    srv.set_error_handler([](const httplib::Request &req, httplib::Response &res)
                          { 
                            if(req.path.rfind("/static/", 0) != 0)
                                res.set_content(getPage(req.path, req, res), "text/html"); });

    srv.set_logger([](const httplib::Request &req, auto &res)
                   { std::cout << req.remote_addr << ":" << req.remote_port << " - " << res.status << "; " << req.path << "; " << req.version << ";\n"; });

    std::printf("Running the server on %s:%i\n", CONF_IP, CONF_PORT);
    srv.listen(CONF_IP, CONF_PORT);
    return 0;
}