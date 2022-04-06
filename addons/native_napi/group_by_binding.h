#include "group_by.h"
//#include "pretty_print/pretty_print.hpp"
#include <napi.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <chrono>

using namespace std;

using Key = std::string;
using Value = variant<std::string, double, bool>;
using Item = unordered_map<Key, Value>;
using Items = vector<Item>;
using GroupValue = variant<std::string, double, bool, vector<Item*>>;
using Group = unordered_map<Key, GroupValue>;
using Groups = vector<Group>;

// For pretty_print
//ostream& operator<<(ostream& out, Item * value) {
//    pretty::print(out, *value);
//    return out;
//};

Value primitiveJsToValue(Napi::Value jsValue) {
    Value value;

    if (jsValue.IsNumber()) {
        value = (double)jsValue.As<Napi::Number>().DoubleValue();

    } else if (jsValue.IsBoolean()) {
        value = (bool)jsValue.As<Napi::Boolean>();

    } else if (jsValue.IsString()) {
        value = (string)jsValue.As<Napi::String>();
    }

    return value;
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
    vector<string> sumFields;

    // Convert sumFields to vector<string>
    Napi::Array jsSumFields = info[2].IsArray() ? info[2].As<Napi::Array>() : Napi::Array::New(env);

    for (int i = 0; i < jsSumFields.Length(); i++) {
        string sumField = (string)((Napi::Value)jsSumFields[i]).As<Napi::String>();
        sumFields.push_back(sumField);
    }

    // Convert items to vector<string, variant<std::string, double, bool>>
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

            item[key] = primitiveJsToValue(jsValue);
        }

        items.push_back(item);
    }

    auto t1 = std::chrono::system_clock::now();
    cout << "- Convert JS input data to C++ data in ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << endl;

    /////////////////////////////////////////////
    // Do work
    Groups result = groupBy(items, groupField, sumFields);
    // pretty::print(std::cout, result) << std::endl;

    auto t2 = std::chrono::system_clock::now();
    cout << "- C++ groupping in ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << endl;
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

                    vector<Item*> items = std::get<vector<Item*>>(pair.second);

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
    cout << "- Convert C++ output data to JS data in ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << endl;

    return jsResult;
}
