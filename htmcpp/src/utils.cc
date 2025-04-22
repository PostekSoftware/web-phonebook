#include "utils.h"

uint32_t rand32()
{
    return ((rand() & 0x3) << 30) | ((rand() & 0x7fff) << 15) | (rand() & 0x7fff);
}
bool gen_uuid4(char dst[37], size_t len)
{
    srand(time(NULL));
    int n = snprintf(dst, len, "%08x-%04x-%04x-%04x-%04x%08x",
                     rand32(),                       // Generates a 32-bit Hex number
                     rand32() & 0xffff,              // Generates a 16-bit Hex number
                     ((rand32() & 0x0fff) | 0x4000), // Generates a 16-bit Hex number of the form 4xxx (4 indicates the UUID version)
                     (rand32() & 0x3fff) + 0x8000,   // Generates a 16-bit Hex number in the range [0x8000, 0xbfff]
                     rand32() & 0xffff, rand32());   // Generates a 48-bit Hex number

    return n >= 0 && n < len; // Success only when snprintf result is a positive number and the provided buffer was large enough.
}

std::string getUuid()
{
    char strUuid[37];
    gen_uuid4(strUuid, sizeof(strUuid));
    return (std::string)strUuid;
}

std::vector<std::string> splitString(const std::string &str, const std::string &token)
{
    std::vector<std::string> result;
    std::string tmp = str;
    while (tmp.size())
    {
        int index = tmp.find(token);
        if (index != std::string::npos)
        {
            result.push_back(tmp.substr(0, index));
            tmp = tmp.substr(index + token.size());
            if (tmp.size() == 0)
                result.push_back(tmp);
        }
        else
        {
            result.push_back(tmp);
            tmp = "";
        }
    }
    return result;
}