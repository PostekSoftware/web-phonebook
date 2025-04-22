#include "router.h"

std::vector<std::string> getPath(const std::string &path)
{
    std::string tmp;
    std::stringstream ss(path);
    std::vector<std::string> vPath;
    while (getline(ss, tmp, '/'))
    {
        if (tmp != "")
            vPath.push_back(tmp);
    }
    return vPath;
}

std::string getStaticPageContent(const std::string &path)
{
    std::ifstream sp(CONF_APP_DIR + path + "/page.html");
    if (!sp.is_open())
    {
        fprintf(stderr, "Could not open file: %s\n", (CONF_APP_DIR + path + "/page.html").c_str());
        return "";
    }
    std::string tmp = "";
    std::string content = "";
    while (std::getline(sp, tmp))
        content += tmp + "\n";
    sp.close();

    return content;
}

std::string getPage(const std::string &path, const httplib::Request &req, httplib::Response &res)
{
    std::vector<std::string> vPath = getPath(path);
    std::string checkedPath = "";
    bool is_404 = false;
    for (int i = 0; i < vPath.size(); i++)
    {
        if (!std::filesystem::exists(CONF_APP_DIR + checkedPath + "/" + vPath[i]) || !std::filesystem::is_directory(CONF_APP_DIR + checkedPath + "/" + vPath[i]) || std::filesystem::is_empty(CONF_APP_DIR + checkedPath + "/" + vPath[i]))
        {
            fprintf(stderr, "Path: '%s' does not exist! Returning a 404 page.\n", path.c_str());
            is_404 = true;
            break;
        }
        if (i != vPath.size() - 1)
            continue;

        checkedPath += "/" + vPath[i];
    }

    if (is_404)
    {
        // handle a 404
        res.status = 404;
        return "404";
    }

    std::string sessionId = "";
    auto cookie = req.get_header_value("Cookie");
    printf("Cookie: %s\n", cookie.c_str());
    auto cookies = splitString(cookie, "; ");
    for (const auto &cookie : cookies)
    {
        auto c = splitString(cookie, "=");
        if (c.size() != 2)
            continue;
        if (c.at(0) == "sessionId")
        {
            sessionId = c.at(1);
        }
    }

    auto p = getPageConfig(checkedPath);
    if (!p.page && !p.getCc && !p.postCc)
    {
        res.status = 500;
        return "error";
    }

    if (p.page && !p.pageCc && req.method == "GET")
    {
        res.status = 200;
        return getStaticPageContent(checkedPath);
    }
    else if (p.page && p.pageCc && req.method == "GET")
    {
        res.status = 200;
        buildPage(fileType::PAGE, checkedPath);
        std::string ret = "";
        try
        {
            ret = inja::render(getStaticPageContent(checkedPath), getServerData(fileType::PAGE, checkedPath, req, res, sessionId));
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return ret;
    }
    else if (p.getCc && req.method == "GET")
    {
        res.status = 200;
        buildPage(fileType::GET, checkedPath);
        return to_string(getServerData(fileType::GET, checkedPath, req, res, sessionId));
    }
    else if (p.postCc && req.method == "POST")
    {
        res.status = 200;
        buildPage(fileType::POST, checkedPath);
        return to_string(getServerData(fileType::POST, checkedPath, req, res, sessionId));
    }

    // handle page prerendering
    res.status = 500;
    return "Internal server error";
}
