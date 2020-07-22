// Just for debug. Node-gyp uses main.cc file

#include <iostream>
#include <deque>
#include "pretty_print/pretty_print.hpp"
#include <chrono>
#include "group_by.h"

using namespace std;

ostream& operator<<(ostream& out, Item * value) {
    pretty::print(out, *value);
    return out;
};

int main()
{
    cout << std::boolalpha << "-- Debug! --\n" << endl;

    Items items = {
            { { "name", (string)"a" }, {"val", (double)1}, {"other", (bool)true} },
            { { "name", (double)4 }, {"val", (double)2}, {"other", (bool)false} },
            { { "name", (string)"b" }, {"val", (double)3}, {"other", (string)"c"} },
            { { "name", (double)4 }, {"val", (double)3}, {"other", (string)"r"} },
            { { "name", (string)"a" }, {"val", (double)4}, {"other", (string)"d"} },
            { { "name", (string)"b" }, {"val", (double)5}, {"other", (string)"e"} },
    };

    auto groupped = groupBy(items, "name", { "val" });

    pretty::print(std::cout, groupped) << std::endl;

    return 0;
}