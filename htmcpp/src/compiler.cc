#include "compiler.h"

page_config getPageConfig(const std::string &path)
{
    page_config conf = {path};

    if (std::filesystem::exists(CONF_APP_DIR + path + "/page.html") && std::filesystem::is_regular_file(CONF_APP_DIR + path + "/page.html"))
        conf.page = true;
    if (std::filesystem::exists(CONF_APP_DIR + path + "/page.cc") && std::filesystem::is_regular_file(CONF_APP_DIR + path + "/page.cc"))
        conf.pageCc = true;
    if (std::filesystem::exists(CONF_APP_DIR + path + "/get.cc") && std::filesystem::is_regular_file(CONF_APP_DIR + path + "/get.cc"))
        conf.getCc = true;
    if (std::filesystem::exists(CONF_APP_DIR + path + "/post.cc") && std::filesystem::is_regular_file(CONF_APP_DIR + path + "/post.cc"))
        conf.postCc = true;
    if (std::filesystem::exists(CONF_APP_DIR + path + "/error.html") && std::filesystem::is_regular_file(CONF_APP_DIR + path + "/error.html"))
        conf.error = true;
    if (std::filesystem::exists(CONF_APP_DIR + path + "/error.cc") && std::filesystem::is_regular_file(CONF_APP_DIR + path + "/error.cc"))
        conf.errorCc = true;

    if (conf.page && (conf.getCc))
    {
        fprintf(stderr, "Path '%s' is set up as both a page and an api endpoint!\n", path.c_str());
        conf.path = "";
        return conf;
    }

    return conf;
}

void buildPage(fileType type, const std::string &path)
{
    if (G_isDev)
    {
        if (!std::filesystem::exists("out/pages-dev"))
        {
            std::filesystem::create_directory("out/pages-dev");
        }
        if (!std::filesystem::is_directory("out/pages-dev"))
        {
            std::filesystem::remove("out/pages-dev");
            std::filesystem::create_directory("out/pages-dev");
        }

        std::string pathToFile = CONF_APP_DIR + path;
        switch (type)
        {
        case fileType::PAGE:
            pathToFile += "/page.cc";
            break;
        case fileType::GET:
            pathToFile += "/get.cc";
            break;
        case fileType::POST:
            pathToFile += "/post.cc";
            break;
        case fileType::ERROR:
            pathToFile += "/error.cc";
            break;
        }

        std::ifstream sp(pathToFile);
        if (!sp.is_open())
        {
            fprintf(stderr, "Could not open file: %s\n", (CONF_APP_DIR + pathToFile).c_str());
        }
        std::string tmp = "";
        std::string content = "";
        while (std::getline(sp, tmp))
            content += tmp + "\n";
        sp.close();

        std::string fileMd5 = md5(content);

        if (filePaths.count({type, path}) && fileMd5s.at({type, path}) == fileMd5)
        {
            return;
        }

        // char strUuid[37];
        // gen_uuid4(strUuid, sizeof(strUuid));
        std::string uuid = getUuid();

        std::string compileCommand = "clang++ -g -O0 -Wno-return-type-c-linkage -shared -fPIC -std=c++20 " + pathToFile + " out/httplib/httplib.so out/app/app.so -Iapp-server -Ihtmcpp/include -Ihtmcpp/external/json -Ihtmcpp/external/httplib ";
        for (auto l : linkFiles)
        {
            if (l != "")
                compileCommand += l + " ";
        }
        for (auto i : includeDirs)
        {
            if (i != "")
                compileCommand += "-I" + i + " ";
        }
        compileCommand += "-o out/pages-dev/" + uuid + ".so";

        printf("Running: %s\n", compileCommand.c_str());
        system(compileCommand.c_str());

        if (filePaths.count({type, path}))
            if (std::filesystem::exists("out/pages-dev/" + filePaths.at({type, path}) + ".so"))
                std::filesystem::remove("out/pages-dev/" + filePaths.at({type, path}) + ".so");

        printf("Path: %s; File %s created; md5: %s\n", path.c_str(), uuid.c_str(), fileMd5.c_str());

        filePaths.erase({type, path});
        fileMd5s.erase({type, path});
        filePaths.insert({{type, path}, uuid});
        fileMd5s.insert({{type, path}, fileMd5});
    }
    else
    {
        // todo: probably nothing actually
        return;
    }
}

nlohmann::json getServerData(fileType type, const std::string &path, const httplib::Request &req, httplib::Response &res, std::string sessionId)
{
    auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    int64_t nowInt = now.time_since_epoch().count();
    sessionData *sessData = new sessionData{};
    if (sessionId == "" || !sessions.count(sessionId))
    {
        printf("Session '' or does not exist on server\n");
        auto uuid = getUuid();
        sessions.insert({uuid, {nowInt, new sessionData{}}});
        sessData = sessions.at(uuid).second;
        sessionId = uuid;
        res.headers.insert({"Set-Cookie", "sessionId=" + uuid + (std::string) "; SameSite=Strict"});
    }
    else
    {
        auto s = sessions.at(sessionId);
        if (s.first > nowInt - 60 * 60)
        {
            delete s.second;
            sessions.erase(sessionId);
            auto uuid = getUuid();
            sessions.insert({uuid, {nowInt, sessData}});
        }
        else
        {
            delete sessData;
            sessData = s.second;
            s.first = nowInt;
        }
    }
    printf("Session: %s\n", sessionId.c_str());

    if (G_isDev)
    {
        std::string fileUuid = filePaths.at({type, path});
        void *pLib = dlopen(("out/pages-dev/" + fileUuid + ".so").c_str(), RTLD_LAZY);
        if (!pLib)
        {
            throw std::runtime_error("'out/pages-dev/" + fileUuid + ".so' could not open file!\n");
            return "";
        }
        auto server = (nlohmann::json (*)(const httplib::Request &, httplib::Response &, serverData &, sessionData *))dlsym(pLib, "srv");
        if (!server)
        {
            throw std::runtime_error("'out/pages-dev/" + fileUuid + ".so' does not have a valid definition!\n");
            return "";
        }
        nlohmann::json d = server(req, res, sData, sessData);
        dlclose(pLib);

        return d;
    }
    else
    {
        if (!prebuiltPages.count({type, path}))
        {
            fprintf(stderr, "PAGE NOT PREBUILT!!! %s; %s\n", path.c_str(), type == fileType::PAGE ? "page" : (type == fileType::GET ? "get" : "post"));
            exit(1);
        }

        prebuiltPage p = prebuiltPages.at({type, path});

        return p.func(req, res, sData, sessData);
    }
}

void preBuildPage(fileType type, const std::string &path, std::map<std::pair<fileType, std::string>, std::string> &files)
{
    std::string pathToFile = CONF_APP_DIR + path;
    switch (type)
    {
    case fileType::PAGE:
        pathToFile += "page.cc";
        break;
    case fileType::GET:
        pathToFile += "get.cc";
        break;
    case fileType::POST:
        pathToFile += "post.cc";
        break;
    case fileType::ERROR:
        break;
    }

    // char strUuid[37];
    // gen_uuid4(strUuid, sizeof(strUuid));
    std::string uuid = getUuid();

    printf("Building page '%s': %s\n", pathToFile.c_str(), uuid.c_str());

    std::string compileCommand = "clang++ -Wno-return-type-c-linkage -shared -fPIC -std=c++20 " + pathToFile + " out/httplib/httplib.so out/app/app.so -Iapp-server -Ihtmcpp/include -Ihtmcpp/external/json -Ihtmcpp/external/httplib ";
    for (auto l : linkFiles)
    {
        if (l != "")
            compileCommand += l + " ";
    }
    for (auto i : includeDirs)
    {
        if (i != "")
            compileCommand += "-I" + i + " ";
    }
    compileCommand += "-o out/prebuilt/" + uuid + ".so";

    printf("Running: %s\n", compileCommand.c_str());
    if (system(compileCommand.c_str()) != 0)
    {
        fprintf(stderr, "Error while building page: %s\n", pathToFile.c_str());
        exit(1);
    }

    std::string tmpp = path;
    tmpp.pop_back();
    files.insert({{type, tmpp}, uuid});
}

void iterateDir(const std::string &path, std::map<std::pair<fileType, std::string>, std::string> &files)
{
    printf("Chekcing path: app%s\n", path.c_str());
    auto p = getPageConfig(path);

    if (p.pageCc)
    {
        preBuildPage(fileType::PAGE, path, files);
    }
    if (p.getCc)
    {
        preBuildPage(fileType::GET, path, files);
    }
    if (p.postCc)
    {
        preBuildPage(fileType::POST, path, files);
    }

    for (const auto &obj : std::filesystem::directory_iterator(CONF_APP_DIR + path))
    {
        if (obj.is_directory())
        {
            iterateDir(path + obj.path().filename().string() + "/", files);
        }
    }
}

void prebuildAllPages()
{
    if (!std::filesystem::exists("out/prebuilt"))
    {
        std::filesystem::create_directory("out/prebuilt");
    }
    else if (!std::filesystem::is_directory("out/prebuilt"))
    {
        std::filesystem::remove("out/prebuilt");
        std::filesystem::create_directory("out/prebuilt");
    }
    else
    {
        std::filesystem::remove_all("out/prebuilt");
        std::filesystem::create_directory("out/prebuilt");
    }

    std::map<std::pair<fileType, std::string>, std::string> files;

    iterateDir("/", files);

    nlohmann::json data;
    for (auto &file : files)
    {
        nlohmann::json obj;
        obj["type"] = (file.first.first == fileType::PAGE ? "page" : (file.first.first == fileType::GET ? "get" : "post"));
        obj["path"] = file.first.second;
        obj["obj"] = file.second;
        data.emplace_back(obj);
    }

    std::string dataStr = nlohmann::to_string(data);
    printf("\nBuild metadata: %s\n", dataStr.c_str());

    std::ofstream of("out/prebuilt/metadata.json");
    if (!of.is_open())
    {
        fprintf(stderr, "Could not write to file 'out/prebuilt/metadata.json'\n");
        exit(1);
    }
    of << dataStr;
    of.close();
}

void loadPrebuiltPages()
{
    printf("Loading metadata...\n\n");
    std::ifstream sp("out/prebuilt/metadata.json");
    if (!sp.is_open())
    {
        fprintf(stderr, "Could not open file: out/prebuilt/metadata.json\n");
        exit(1);
    }
    std::string tmp = "";
    std::string metadataStr = "";
    while (std::getline(sp, tmp))
        metadataStr += tmp + "\n";
    sp.close();

    nlohmann::json metadata;
    metadata = nlohmann::json::parse(metadataStr, nullptr, true, false);

    if (!metadata.is_array() || metadata.empty())
    {
        printf("Error loading 'out/prebuilt/metadata.json'\n");
        exit(1);
    }

    for (const auto &obj : metadata)
    {
        std::string ob = obj.at("obj");
        std::string path = obj.at("path");
        fileType type = (obj.at("type") == "page" ? fileType::PAGE : obj.at("type") == "get" ? fileType::GET
                                                                                             : fileType::POST);

        if (ob == "" || (type != fileType::PAGE && type != fileType::GET && type != fileType::POST))
        {
            fprintf(stderr, "Error parsing metadata!\n");
            exit(1);
        }

        std::string fileUuid = ob;
        void *pLib = dlopen(("out/prebuilt/" + fileUuid + ".so").c_str(), RTLD_LAZY);
        if (!pLib)
        {
            fprintf(stderr, "'out/prebuilt/%s.so' could not open file!\n", fileUuid.c_str());
            exit(1);
        }
        auto server = (nlohmann::json (*)(const httplib::Request &, httplib::Response &, serverData &, sessionData *))dlsym(pLib, "srv");
        if (!server)
        {
            fprintf(stderr, "'out/prebuilt/%s.so' does not have a valid definition!\n", fileUuid.c_str());
            exit(1);
        }

        prebuiltPages.insert({{type, path}, {ob, server}});
        printf("Page: %s; Type: %s; Prebuilt: %s\n", path == "" ? "/" : path.c_str(), ((std::string)obj.at("type")).c_str(), ob.c_str());
    }

    printf("\nAll prebuilt pages loaded.\nStarting the server...\n\n");
}
