#include <napi.h>
#include <iostream>
#include "pretty_print/pretty_print.hpp"
#include <algorithm>
#include <deque>
#include <map>
#include <string>
#include <variant>
#include <chrono>

using namespace std;

using Key = std::string;
using Value = variant<std::string, double, bool>;
using Item = map<Key, Value>;
using Items = deque<Item>;
using GroupValue = variant<std::string, double, bool, deque<Item*>>;
using Group = map<Key, GroupValue>;
using Groups = deque<Group>;

// For pretty_print
ostream& operator<<(ostream& out, Item * value) {
    pretty::print(out, *value);
    return out;
};

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

// Wrap/Unwrap by N-api
Napi::Value GroupBy(const Napi::CallbackInfo &info) {
    // Unwrap C++ from JS
    auto t0 = std::chrono::system_clock::now();

    Napi::Env env = info.Env();
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "WrongNumber of arguments.").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Arguments
    Items items;
    string groupField = info[1].As<Napi::String>();
    deque<string> sumFields;

    // Convert sumFields to deque<string>
    Napi::Array jsSumFields = info[2].IsArray() ? info[2].As<Napi::Array>() : Napi::Array::New(env);

    for (int i = 0; i < jsSumFields.Length(); i++) {
        string sumField = (string)((Napi::Value)jsSumFields[i]).As<Napi::String>();
        sumFields.push_back(sumField);
    }

    // Convert items to deque<string, variant<std::string, double, bool>>
    Napi::Array jsItems = info[0].As<Napi::Array>();

    for (int i = 0; i < jsItems.Length(); i++) {
        Napi::Object jsItem = ((Napi::Value)jsItems[i]).As<Napi::Object>();
        Napi::Array jsKeys = jsItem.GetPropertyNames();

        Item item;

        for (int j = 0; j < jsKeys.Length(); j++) {
            Value value;
            Napi::Value jsKey = jsKeys[j];

            string key = (string)jsKey.As<Napi::String>();
            Napi::Value jsValue = jsItem.Get(jsKey);

            if (jsValue.IsNumber()) {
                value = (double)jsValue.As<Napi::Number>().DoubleValue();

            } else if (jsValue.IsBoolean()) {
                value = (bool)jsValue.As<Napi::Boolean>();

            } else if (jsValue.IsString()) {
                value = (string)jsValue.As<Napi::String>();
            }

            item[key] = value;
        }

        items.push_back(item);
    }

    auto t1 = std::chrono::system_clock::now();
    cout << "Convert JS -> C++ data: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << endl;

    /////////////////////////////////////////////
    // Do work
    Groups result = groupBy(items, groupField, sumFields);
    // pretty::print(std::cout, result) << std::endl;

    auto t2 = std::chrono::system_clock::now();
    cout << "C++ Groupping: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << endl;
    /////////////////////////////////////////////

    // Wrap C++ to JS
    Napi::Array jsResult = Napi::Array::New(env);

    int n = 0;

    for_each(begin(result), end(result), [&](Group& group) {
        Napi::Object resultGroup = Napi::Object::New(env);

        for (auto& pair : group) {
            string key = Napi::String::New(env, pair.first);

            switch (pair.second.index()) {
                case 0: resultGroup.Set(key, Napi::String::New(env, std::get<string>(pair.second)));
                    break;
                case 1: resultGroup.Set(key, Napi::Number::New(env, std::get<double>(pair.second)));
                    break;
                case 2: resultGroup.Set(key, Napi::Boolean::New(env, std::get<bool>(pair.second)));
                    break;
                case 3:
                    Napi::Array jsItems = Napi::Array::New(env);
                    int m = 0;

                    deque<Item*> items = std::get<deque<Item*>>(pair.second);

                    for_each(begin(items), end(items), [&](Item*& pItem) {
                        Item& item = *pItem;
                        Napi::Object resultItem = Napi::Object::New(env);

                        for (auto& itemPair : item) {
                            string itemKey = Napi::String::New(env, itemPair.first);
                            switch (itemPair.second.index()) {
                                case 0: resultItem.Set(itemKey, Napi::String::New(env, std::get<string>(itemPair.second)));
                                    break;
                                case 1: resultItem.Set(itemKey, Napi::Number::New(env, std::get<double>(itemPair.second)));
                                    break;
                                case 2: resultItem.Set(itemKey, Napi::Boolean::New(env, std::get<bool>(itemPair.second)));
                                    break;
                            }
                        }
                        jsItems[m] = resultItem;
                        m++;
                    });

                    resultGroup.Set(key, jsItems);
                    break;
            }
        }

        jsResult[n] = resultGroup;
        n++;
    });

    auto t3 = std::chrono::system_clock::now();
    cout << "Convert C++ -> JS data: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << endl;

    return jsResult;
}
