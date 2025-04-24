#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>

using namespace nlohmann;

struct number
{
    std::string location = "";
    std::string title = "";
    std::string name = "";
    std::string email = "";
    std::string phoneInt = "";
    std::string phoneExt = "";
};
inline void to_json(json &j, const number &n)
{
    j = json{{"location", n.location}, {"title", n.title}, {"name", n.name}, {"email", n.email}, {"phoneInt", n.phoneInt}, {"phoneExt", n.phoneExt}};
};
inline void from_json(const json &j, number &n)
{
    try
    {
        j.at("location").get_to(n.location);
        j.at("title").get_to(n.title);
        j.at("name").get_to(n.name);
        j.at("email").get_to(n.email);
        j.at("phoneInt").get_to(n.phoneInt);
        j.at("phoneExt").get_to(n.phoneExt);
    }
    catch (const nlohmann::json_abi_v3_11_3::detail::type_error &e)
    {
        std::cerr << "Parsing number: " << e.what() << '\n';
    }
};
// inline void from_json(const json &j, std::vector<number> &n)
// {
//     if (!j.is_array())
//     {
//         fprintf(stderr, "Error reading json!\n");
//         return;
//     }
//     for (const auto &jj : j)
//     {
//         n.push_back((number)jj);
//     }
// }

struct group
{
    std::string name = "";
    std::vector<number> numbers = std::vector<number>();
};
inline void to_json(json &j, const group &g)
{
    j = json{{"name", g.name}, {"numbers", g.numbers}};
}
inline void from_json(const json &j, group &g)
{
    try
    {
        j.at("name").get_to(g.name);
        j.at("numbers").get_to(g.numbers);
    }
    catch (const nlohmann::json_abi_v3_11_3::detail::type_error &e)
    {
        std::cerr << "Parsing group: " << e.what() << '\n';
    }
}
// inline void from_json(const json &j, std::vector<group> &g)
// {
//     if (!j.is_array())
//     {
//         fprintf(stderr, "Error reading json!\n");
//         return;
//     }
//     for (const auto &jj : j)
//     {
//         g.push_back((group)jj);
//     }
// }

struct book
{
    std::string name = "";
    std::vector<number> numbers = std::vector<number>();
    std::vector<group> groups = std::vector<group>();
};
inline void to_json(json &j, const book &b)
{
    j = json{{"name", b.name}, {"numbers", b.numbers}, {"groups", b.groups}};
}
inline void from_json(const json &j, book &b)
{
    try
    {
        j.at("name").get_to(b.name);
        j.at("numbers").get_to(b.numbers);
        j.at("groups").get_to(b.groups);
    }
    catch (const nlohmann::json_abi_v3_11_3::detail::type_error &e)
    {
        std::cerr << "Parsing book: " << e.what() << '\n';
    }
}
// inline void from_json(const json &j, std::vector<book> &b)
// {
//     if (!j.is_array())
//     {
//         fprintf(stderr, "Error reading json!\n");
//         return;
//     }
//     for (const auto &jj : j)
//     {
//         b.push_back((book)jj);
//     }
// }

struct page
{
    std::string name = "";
    std::vector<book> books = std::vector<book>();
};
inline void to_json(json &j, const page &p)
{
    j = json{{"name", p.name}, {"books", p.books}};
}
inline void from_json(const json &j, page &p)
{
    try
    {
        printf("Parse page name\n");
        j.at("name").get_to(p.name);
        printf("Parse page books\n");
        j.at("books").get_to(p.books);
    }
    catch (const nlohmann::json_abi_v3_11_3::detail::type_error &e)
    {
        std::cerr << "Parsing page: " << e.what() << '\n';
    }
}

void loadDataFromFile(page &b, bool &bl);
void saveDataToFile(const page &b);

struct admin
{
    std::string username = "";
    std::string password = "";
};
inline void to_json(json &j, const admin &a)
{
    j = json{{"username", a.username}, {"password", a.password}};
}
inline void from_json(const json &j, admin &a)
{
    try
    {
        j.at("username").get_to(a.username);
        printf("Loaded admin: %s\n", a.username.c_str());
        j.at("password").get_to(a.password);
    }
    catch (const nlohmann::json_abi_v3_11_3::detail::type_error &e)
    {
        std::cerr << "Parsing admin: " << e.what() << '\n';
    }
}

void loadAdminSettings(std::vector<admin> &a, bool &al);