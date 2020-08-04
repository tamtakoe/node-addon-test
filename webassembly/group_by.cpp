/* Embind example */

#include <string>
#include <emscripten/bind.h>

using namespace std;

double group_by(string text, int value) {
  const char* str = text.c_str();
  printf("Text: '%s' Number: %d\n", str, value);
  return value * -1.7;
}

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("group_by", &group_by);
}


/* WIP */
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <unordered_map>
//#include <string>
//#include <variant>
//#include <emscripten/bind.h>
//
//using namespace std;
//
//template <class... Args>
//struct variant_cast_proxy
//{
//    std::variant<Args...> v;
//
//    template <class... ToArgs>
//    operator std::variant<ToArgs...>() const
//    {
//        return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return arg; }, v);
//    }
//};
//
//template <class... Args>
//auto variantCast(const std::variant<Args...>& v) -> variant_cast_proxy<Args...>
//{
//    return { v };
//}
//
//using Key = std::string;
//using Value = variant<std::string, double, bool>;
//using Item = unordered_map<Key, Value>;
//using Items = vector<Item>;
//using GroupValue = variant<std::string, double, bool, vector<Item*>>;
//using Group = unordered_map<Key, GroupValue>;
//using Groups = vector<Group>;
//
//EMSCRIPTEN_BINDINGS(my_module) {
////     Register vector bindings
////        emscripten::register_vector<Item>("VectorItem");
//
//
//    emscripten::function("group_by", emscripten::optional_override([](emscripten::val itemsJS, std::string groupField) {
////            Items itemsV = emscripten::vecFromJSArray<Item>(items);
////           Items itemsV = to_vector<Item>(items);
////            Items itemsV = items.as<Items>();
//
//        auto l = itemsJS["length"].as<unsigned>();
//
//        std::vector<Item> items;
//        for (unsigned i = 0; i < l; ++i) {
//            auto itemJS = itemsJS[i];
//            Item item;
//
//            for (auto itemPair : itemJS) {
//                emscripten::val keyJS = itemPair.first;
//                emscripten::val valueJS = itemPair.second;
//                string key = keyJS.as<string>();
//                variant<string, double, bool> value;
//
//                if (valueJS.isNumber()) {
//                    value = valueJS.as<double>();
//                } else if (valueJS.isString()) {
//                    value = valueJS.as<string>();
//                } else if (valueJS.isTrue() || valueJS.isFalse()) {
//                    value = valueJS.as<bool>();
//                }
//
//                item[key] = value;
//            }
//
//            items.push_back(item);
//        }
//
//        return items;
//
//
//        printf("groupBy!\n");
//        cout << groupField << endl;
//        return groupField;
//    }));
//}