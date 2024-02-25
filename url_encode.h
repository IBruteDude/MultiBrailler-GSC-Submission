/** Algorithm taken from libcurl, modified to fit our need
 * link: https://github.com/curl/curl/blob/f274fc5c685ac245d1d2818046bbfb4b97685211/lib/escape.c#L56
 */

#define ISUPPER(x)  (((x) >= 'A') && ((x) <= 'Z'))
#define ISLOWER(x)  (((x) >= 'a') && ((x) <= 'z'))
#define ISDIGIT(x)  (((x) >= '0') && ((x) <= '9'))
#define ISALNUM(x)  (ISDIGIT(x) || ISLOWER(x) || ISUPPER(x))
#define ISURLPUNTCS(x) (((x) == '-') || ((x) == '.') || ((x) == '_') || ((x) == '~'))

#define ISUNRESERVED(x) (ISALNUM(x) || ISURLPUNTCS(x))

#include <string>
std::string url_encode(const std::string& str)
{
    std::string d;
    size_t length = str.size();

    // recommended by chatgpt
    d.reserve((3 * length + 1) * sizeof(char));

    for (unsigned char in: str) {
        if(ISUNRESERVED(in)) {
            d += in;
        } else {
            const char hex[] = "0123456789ABCDEF";
            char out[3] = {'%', hex[in>>4], hex[in & 0xf]};
            d.append(out, 3);
        }
    }
    return d;
}

