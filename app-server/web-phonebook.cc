#include "web-phonebook.h"

void loadDataFromFile(page &b, bool &bl)
{
    if (bl)
        return;
    std::filesystem::create_directory("db");
    if (!std::filesystem::exists("db/db.json"))
    {
        return;
    }
    std::ifstream stream("db/db.json");
    if (!stream.is_open())
    {
        fprintf(stderr, "Could not open db/db.json!\n");
        exit(1);
    }
    std::string d = "";
    std::string tmp = "";
    while (std::getline(stream, tmp))
    {
        d += tmp;
    }
    stream.close();
    // printf("Loaded file: %s\n\n\n", d.c_str());

    json jsond = json::parse(d);
    // printf("json: %s\n", to_string(jsond).c_str());
    // printf("parsed json from string\n");
    b = (page)jsond;
    // printf("parsed json to int format\n");
    bl = true;
}

void saveDataToFile(const page &b)
{
    json datad = json(b);
    std::string datas = to_string(datad);

    std::filesystem::create_directory("db");
    std::filesystem::remove("db/db.json");
    if (std::filesystem::exists("db/db.json"))
    {
        fprintf(stderr, "Could not delete odl db/db.json file!\n");
        exit(1);
    }

    std::ofstream stream("db/db.json");
    if (!stream.is_open())
    {
        fprintf(stderr, "Could not create db/db.json!\n");
        exit(1);
    }
    stream << datas;
    stream.close();
}

void loadAdminSettings(std::vector<admin> &a, bool &al)
{
    if (al)
        return;
    std::filesystem::create_directory("db");
    if (!std::filesystem::exists("db/admins.json"))
    {
        return;
    }
    std::ifstream stream("db/admins.json");
    if (!stream.is_open())
    {
        fprintf(stderr, "Could not open db/admins.json!\n");
        exit(1);
    }
    std::string d = "";
    std::string tmp = "";
    while (std::getline(stream, tmp))
    {
        d += tmp;
    }
    stream.close();
    // printf("Loaded file: %s\n\n\n", d.c_str());

    json jsond = json::parse(d);
    // printf("json: %s\n", to_string(jsond).c_str());
    // printf("parsed json from string\n");
    a = (std::vector<admin>)jsond;
    // printf("parsed json to int format\n");
    al = true;
}

void loadLanguage(language &lang, bool &ll)
{
    if (ll)
        return;
    std::filesystem::create_directory("db");
    if (!std::filesystem::exists("db/language.json"))
    {
        return;
    }
    std::ifstream stream("db/language.json");
    if (!stream.is_open())
    {
        fprintf(stderr, "Could not open db/language.json!\n");
        exit(1);
    }
    std::string d = "";
    std::string tmp = "";
    while (std::getline(stream, tmp))
    {
        d += tmp;
    }
    stream.close();
    // printf("Loaded file: %s\n\n\n", d.c_str());

    json jsond = json::parse(d);
    // printf("json: %s\n", to_string(jsond).c_str());
    // printf("parsed json from string\n");
    lang = (language)jsond;
    // printf("parsed json to int format\n");
    ll = true;
}

void saveLanguage(const language &lang)
{
    json datad = json(lang);
    std::string datas = to_string(datad);

    std::filesystem::create_directory("db");
    std::filesystem::remove("db/language.json");
    if (std::filesystem::exists("db/language.json"))
    {
        fprintf(stderr, "Could not delete odl db/language.json file!\n");
        exit(1);
    }

    std::ofstream stream("db/language.json");
    if (!stream.is_open())
    {
        fprintf(stderr, "Could not create db/language.json!\n");
        exit(1);
    }
    stream << datas;
    stream.close();
}
