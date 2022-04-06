// Just for debug. Node-gyp uses main.cc file

#include <iostream>
#include <deque>
#include <vector>
//#include "pretty_print/pretty_print.hpp"
#include <chrono>
#include "group_by.h"

using namespace std;

// For pretty_print
//ostream& operator<<(ostream& out, Item * value) {
//    pretty::print(out, *value);
//    return out;
//};

int main()
{
    cout << std::boolalpha << "-- Debug! --\n" << endl;
    int amount = 100000;
    Items items = {};

    vector<Item> baseItems = {
            { { "name", (string)"a" }, {"val", (double)1}, {"other", (bool)true} },
            { { "name", (double)4.1 }, {"val", (double)2}, {"other", (bool)false} },
            { { "name", (string)"b" }, {"val", (double)3}, {"other", (string)"c"} },
            { { "name", (double)2 }, {"val", (double)4}, {"other", (double)8} },
            { { "name", (string)"d" }, {"val", (double)5}, {"other", (string)"d"} },
            { { "name", (string)"c" }, {"val", (double)6}, {"other", (bool)true} },
            { { "name", (string)"b" }, {"val", (double)7}, {"other", (string)"c"} },
            { { "name", (double)4.9 }, {"val", (double)8}, {"other", (double)1} },
            { { "name", (string)"a" }, {"val", (double)9}, {"other", (double)7} },
            { { "name", (string)"b" }, {"val", (double)0}, {"other", (string)"e"} },
    };

    for (int i = 0; i < amount; i++) {
        items.push_back(baseItems[rand()%10]);
    }

    auto t0 = std::chrono::system_clock::now();

    auto groupped = groupBy(items, "name", { "val" });

    auto t1 = std::chrono::system_clock::now();
    cout << "Grouping time, ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << endl;

    cout << std::boolalpha;
//    pretty::print(std::cout, groupped) << std::endl;
    system("pause");

    return 0;
}
