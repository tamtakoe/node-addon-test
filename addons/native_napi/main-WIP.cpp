/* WIP */
// TODO Change const expression to runtime expressions

//#include <Windows.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <variant>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>
#include <boost/hana.hpp>
#include <boost/hana/ext/std.hpp>
#include <boost/hana/experimental/printable.hpp>
//#include "pretty_print/pretty_print.hpp"

using namespace std::literals;
using sv = std::string_view;
namespace hana = boost::hana;
using namespace hana::literals;

// ?? ??????
// const items =
// [
//     {id: 1, name: 'A', val: 1, smth: 'F'},
//     {id: 2, name: 'B', val: 2, smth: 444},
//     {id: 3, name: 'A', val: 3, smth: true},
//     {id: 4, name: 'B', val: 4, smth: false},
//     {id: 5, name: 'A', val: 5, smth: null}
// ]
// ???????? ?????:
// [
//     {
//         groupField: 'name',
//         groupValue: 'A',
//         // Items in the group 'A'
//         items: [
//             {id: 1, name: 'A', val: 1, smth: 'F'},
//             {id: 3, name: 'A', val: 3, smth: true},
//             {id: 5, name: 'A', val: 5, smth: null}
//         ],
//         // Summed values (from fields described by sumFields)
//         val: 9, // 1 + 3 + 5
//         // Other fields of first item of group
//         name: 'A',
//         id: 1,
//         smth: 'F'
//     }, {
//         groupField: 'name',
//         groupValue: 'B',
//         items: [
//             {id: 2, name: 'B', val: 2, smth: 444},
//             {id: 4, name: 'B', val: 4, smth: false},
//         ],
//         val: 6, // 2 + 4
//         name: 'B',
//         id: 2,
//         smth: 444
//     }
// ]


constexpr struct {} null;
using any = std::variant<sv, bool, double, decltype(null)>;


using hana::_;

//using Item = hana::tuple<double, sv, double, any>;
//using Items = std::vector<Item>;
//
//using T = typename std::remove_reference_t<Item>;
////using String = std::pair<sv, std::tuple<std::vector<std::reference_wrapper<T>>, double>>;
////using String = hana::placeholder_detail::subscript<std::pair<sv, std::tuple<std::vector<std::reference_wrapper<T>>, double>>>;
//using String = hana::placeholder_detail::subscript<std::pair<sv, std::tuple<std::vector<std::reference_wrapper<hana::tuple<double, sv, double, any>>>, double>>>;


template<typename T> void print() {
//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
  std::cerr << __FUNCTION__ << std::endl;
}

//void print_auto(auto) {
////  std::cerr << __PRETTY_FUNCTION__ << std::endl;
//  std::cerr << __FUNCTION__ << std::endl;
//}



//auto group_by(const auto & items, auto key_field, auto sum_field) {
auto group_by = [](const auto & items, auto key_field, auto sum_field) {
  using T = typename std::remove_reference_t<decltype(items[0])>;


  std::unordered_map<sv, std::tuple<std::vector<std::reference_wrapper<T>>, double>> map;


  for(auto && item: items) {
    map.try_emplace(key_field(item));
    auto && [items, sum] = map.at(key_field(item));
    items.emplace_back(std::ref(item));
    sum += sum_field(item);
  }

  return map;
};

decltype(auto) operator<<(std::ostream & o, const decltype(null) &) {
  return o << "null";
}

template<typename ... Ts> decltype(auto) operator<<(std::ostream & o, const std::variant<Ts...> & v) {
  return std::visit([&](auto && value) -> decltype(auto) {
    return o << value;
  }, v);
}

int main() {
//    SetConsoleCP(866); SetConsoleOutputCP(866);
    //system("chcp 1251");
//    system("chcp 65001");
//    SetConsoleOutputCP(CP_UTF8);

    constexpr size_t amount = 10;
//   {id: 1, name: 'A', val: 1, smth: 'F'},
//  auto id = _[0_c];
  auto name = _[1_c];
//    int i = 1;
//    auto name = _[1_c];
//    using T = typename std::remove_reference_t<decltype(items[0])>;

//    auto name = [](auto tuple) {
//        return tuple[1];
//    };
//    hana::tuple<double, sv, double, any>
//
//    std::unordered_map<sv, std::tuple<std::vector<std::reference_wrapper<T>>

    //    auto v = hana::int_c<1>;
//    auto name = _[v];
//    auto name = [](auto tuple) {
//        return tuple[1_c];
//    };
//  using T1 = typename std::remove_reference_t<decltype(_[0_c])>;
  auto val = _[2_c];
//  auto n = std::vector<T1> {};
//  n.push_back(_[1_c]);
//  n.push_back(_[2_c]);
  //auto n = []{_[1_c], _[2_c]};
//  auto smth = _[3_c];

//  for (string name : fieldNames) {
//
//  }

  std::vector<hana::tuple<double, sv, double, any>> items{
//  auto items{
    {1, "A", 1, "F"},
    {2, "B", 2, 444.},
    {3, "A", 3, true},
    {4, "B", 4, false},
    {5, "A", 5, null}
  };

  auto size = std::size(items);

  for (size_t i = 0; i < amount; i++) {
    items.push_back(items[rand() % size]);
  }

    auto t0 = std::chrono::system_clock::now();

  auto map = group_by(items, name, val);

    auto t1 = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

//    std::cout << "Grouping time, ms: "sv << ms << "\n";

    std::cout << "Grouping time, ms: " << ms << "\n";

    std::cerr << map.size() << "\n";

   for(auto && [key, value]: map) {
     auto && [items, sum] = value;
//     pretty::print(std::cerr, key) << "{";

//     std::cerr << key << "{";
//     for(auto item: items) pretty::print(std::cerr, item.get()) << ",";
//     for(auto item: items) std::cerr << hana::experimental::print(item.get()) << ",";

//     std::cerr << "} " << sum << '\n';
   }


}
