#pragma once
#include <iostream>
#include <algorithm>
#include <deque>
#include <map>
#include <string>
#include <variant>

using namespace std;

using Key = std::string;
using Value = variant<std::string, double, bool>;
using Item = map<Key, Value>;
using Items = deque<Item>;
using GroupValue = variant<std::string, double, bool, deque<Item*>>;
using Group = map<Key, GroupValue>;
using Groups = deque<Group>;

template <class... Args>
struct variant_cast_proxy
{
    std::variant<Args...> v;

    template <class... ToArgs>
    operator std::variant<ToArgs...>() const
    {
        return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return arg; }, v);
    }
};

template <class... Args>
auto variantCast(const std::variant<Args...>& v) -> variant_cast_proxy<Args...>
{
    return { v };
}

Groups groupBy(Items& items, const Key groupField, const deque<Key>& sumFields) {
    // Sort items by groupField
    std::sort(items.begin(), items.end(), [&groupField](Item& a, Item& b) { return a[groupField] < b[groupField]; });

    // Group items
    Value currentGroupName;
    Group* pCurrentGroup = nullptr;
    Groups groups;
    deque<Item*>* pCurrentGroupItems = nullptr;
    for (Item& item : items) {

        // Create new group
        if (currentGroupName != item[groupField]) {
            currentGroupName = item[groupField];
            groups.push_back({});
            pCurrentGroup = &groups.back();

            // Copy item
            for (const auto& pair : item) {
                (*pCurrentGroup)[pair.first] = variantCast(pair.second);
            }

            // Add extra fields
            (*pCurrentGroup)["groupField"] = groupField;
            (*pCurrentGroup)["groupValue"] = variantCast(currentGroupName);

            pCurrentGroupItems = &get<deque<Item*>>((*pCurrentGroup)["items"] = deque<Item*>{});

            // Set sumFields in `0`
            for (const auto& sf : sumFields) {
                (*pCurrentGroup)[sf] = (double)0;
            }
        }

        // Add item to the group item collection
        pCurrentGroupItems->push_back(&item);

        // Aggregate values from sumFields
        for (auto& sf : sumFields) {
            if (item.find(sf) != item.end()) {
                Value& val = item[sf];

                if (val.index() == 1) {
                    (*pCurrentGroup)[sf] = get<double>((*pCurrentGroup)[sf]) + get<double>(val);
                }
            }
        }
    }

    return groups;
}
