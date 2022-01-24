/* Embind example */
#include "group_by.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <emscripten/bind.h>

using namespace std;
using Key = std::string;
using Value = variant<std::string, double, bool>;
using Item = unordered_map<Key, Value>;
using Items = vector<Item>;
using GroupValue = variant<std::string, double, bool, vector<Item*>>;
using Group = unordered_map<Key, GroupValue>;
using Groups = vector<Group>;

Value primitiveJsToValue(emscripten::val jsValue) {
    std::string jsType = jsValue.typeof().as<std::string>();
    Value value;

    if (jsType == "string") {
        value = jsValue.as<std::string>();
    }
    if (jsType == "number") {
        value = jsValue.as<double>();
    }
    if (jsType == "boolean") {
        value = jsValue.as<bool>();
    }
    return value;
}

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("groupBy", emscripten::optional_override([](emscripten::val itemsJs, std::string groupField, emscripten::val sumFieldsJs) {
      // Unwrap C++ from JS
      auto t0 = std::chrono::system_clock::now();

      auto itemsLength = itemsJs["length"].as<unsigned>();
      auto sumFields = emscripten::vecFromJSArray<Key>(sumFieldsJs);
      Items items;

      for (unsigned i = 0; i < itemsLength; i++) {
          auto itemJs = itemsJs[i];
          auto itemJsKeys = emscripten::val::global("Object").call<emscripten::val>("keys", itemJs);
          int keysLength = itemJsKeys["length"].as<int>();

          Item item;

          for (int j = 0; j < keysLength; j++) {
            auto key = itemJsKeys[j].as<std::string>();
            auto jsValue = itemJs[key];

            item[key] = primitiveJsToValue(jsValue);
          }

          items.push_back(item);
      }

      auto t1 = std::chrono::system_clock::now();
      cout << "- Convert JS input data to C++ data in ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << endl;

      /////////////////////////////////////////////
      // Do work
      Groups result = groupBy(items, groupField, sumFields);

      auto t2 = std::chrono::system_clock::now();
      cout << "- C++ groupping in ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << endl;
      /////////////////////////////////////////////

      // Wrap C++ to JS
      auto jsResult = emscripten::val::array();
      int n = 0;

      for_each(begin(result), end(result), [&](Group& group) {
        auto jsResultGroup = emscripten::val::object();

          for (auto& pair : group) {
              auto key = (std::string)pair.first;

              switch (pair.second.index()) {
                  case 0: jsResultGroup.set(key, std::get<string>(pair.second));
                      break;
                  case 1: jsResultGroup.set(key, std::get<double>(pair.second));
                      break;
                  case 2: jsResultGroup.set(key, std::get<bool>(pair.second));
                      break;
                  case 3: {
                      auto jsItems = emscripten::val::array();
                      int m = 0;

                      vector<Item*> items = std::get<vector<Item*>>(pair.second);

                      for_each(begin(items), end(items), [&](Item*& pItem) {
                          Item& item = *pItem;
                          auto jsResultItem = emscripten::val::object();

                          for (auto& itemPair : item) {
                              string itemKey = (std::string)itemPair.first;
                              switch (itemPair.second.index()) {
                                  case 0: jsResultItem.set(itemKey, std::get<string>(itemPair.second));
                                      break;
                                  case 1: jsResultItem.set(itemKey, std::get<double>(itemPair.second));
                                      break;
                                  case 2: jsResultItem.set(itemKey, std::get<bool>(itemPair.second));
                                      break;
                              }
                          }
                          jsItems.set(m, jsResultItem);
                          m++;
                      });

                      jsResultGroup.set(key, jsItems);
                      break;
                  }
              }
          }

          jsResult.set(n, jsResultGroup);
          n++;
      });

      auto t3 = std::chrono::system_clock::now();
      cout << "- Convert C++ output data to JS data in ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << endl;

      return jsResult;
  }));
}
