#include "FoxCoderUtils.h"
#include <string.h>

#include <locale>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <iomanip>

std::string narrow(std::wstring const& s,
    std::locale loc = std::locale())
{
    std::vector<char> result(4 * s.size() + 1);
    wchar_t const* fromNext;
    char* toNext;
    mbstate_t state = { 0 };
    std::codecvt_base::result convResult
        = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t> >(loc)
        .out(state, &s[0], &s[s.size()], fromNext,
            &result[0], &result[result.size()], toNext);

    assert(fromNext == &s[s.size()]);
    assert(toNext != &result[result.size()]);
    assert(convResult == std::codecvt_base::ok);
  //  *toNext = \'\\0\';

        return &result[0];
}

std::wstring widen(std::string const& s,
    std::locale loc = std::locale())
{
    std::vector<wchar_t> result(s.size() + 1);
    char const* fromNext;
    wchar_t* toNext;
    mbstate_t state = { 0 };
    std::codecvt_base::result convResult
        = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t> >(loc)
        .in(state, &s[0], &s[s.size()], fromNext,
            &result[0], &result[result.size()], toNext);

    assert(fromNext == &s[s.size()]);
    assert(toNext != &result[result.size()]);
    assert(convResult == std::codecvt_base::ok);
  //  *toNext = L\'\\0\';

        return &result[0];
}

int main()
{


    wchar_t* dBuf = NULL;


     int dSize = mbstowcs(dBuf, "我最棒", 0);
     std::string ss = strerror(errno);

    dBuf = new wchar_t[dSize];
    wmemset(dBuf, 0, dSize);

   
	return 0;
}