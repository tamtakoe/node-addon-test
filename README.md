# node-addon-test
Playground with C++ addons, WebAssembly etc.

#### Install
- Install node.js 12+
- [Optional. Just for `main-WIP.cpp`] Install [Boost C++ library](https://www.boost.org/) 1.71.0+ (Put to `addons/native_napi/boost` folder)
```sh
npm install
```

#### Run
```sh
npm start # runs ./index.js
```

## JavaScript
- `group-by.js` - JS algorithm   

## Native node.js addon ([N-API](https://nodejs.org/api/n-api.html))
- `binding.gyp` - node-gyp compiler settings
- `addons/native_napi/main.cc` - entry point  
- `addons/native_napi/group_by.h` - C++ algorithm   
- `addons/native_napi/group_by_binding.h` - JS/C++ wrappers (need totally optimization)
- `addons/native_napi/index.js` - exports addon to node.js module
- `addons/native_napi/CMakeLists.txt` - CMake file for manual testing (without node-gyp)
- `addons/native_napi/main.cpp` - entry point for manual testing

##### Compile as node-addon
```sh
npm install 
# Yes, it also rebuilds C++ code
```
Note: check "MACOSX_DEPLOYMENT_TARGET" in your binding.gyp has quite differences or equivalent of your version of MacOS.
If an error try to compile code with C version:
```sh
export CXXFLAGS="--std=c++17" && npm install
```

##### Compile and run as C++ application
Build and run `CMakeLists.txt` by CLion etc.


## Webassembly module ([Embind binder](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html))

- `webassembly-embind/group_by.cpp` - c++ example
- Other files are generated

Install [emsdk](https://emscripten.org/docs/getting_started/downloads.html)

Compile WASM (Run it from emsdk directory)
```sh
em++ --bind -std=c++1z -s ALLOW_MEMORY_GROWTH=1 <path_to_addon>/group_by_binding.cpp -o <path_to_addon>/group_by.js
```

ALLOW_MEMORY_GROWTH=1 needs just for huge amount of data

- `webassembly-embind/group_by.js` - result module


## Webassembly module ([WebIDL binder](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html))

- `webassembly-webidl/group_by.idl` - interface declaration
- `webassembly-webidl/group_by.cpp` - c++ example
- `webassembly-webidl/glue_wrapper.cpp` - represents classes for binding
- Other files are generated

Generate glue files
```sh
python <emsdk-path>/upstream/emscripten/tools/webidl_binder.py group_by.idl glue
```

Compile wasm
```sh
em++ -std=c++1z glue_wrapper.cpp --post-js glue.js -o group_by.js
```

- `webassembly-webidl/group_by.js` - result module

## Example
Group items by field
```js
// Source data
const items =
[
    {id: 1, name: 'A', val: 1, smth: 'F'},
    {id: 2, name: 'B', val: 2, smth: 444},
    {id: 3, name: 'A', val: 3, smth: true},
    {id: 4, name: 'B', val: 4, smth: false},
    {id: 5, name: 'A', val: 5, smth: null}
]

// Name of field which is base for grouping
const groupField = 'name'

// Fields whose values are to be summed
const sumFields = ['val']


groupBy(items, groupField, sumFields) 
```
Result:
```js
[
    {
        groupField: 'name',
        groupValue: 'A',
        // Items in the group 'A'
        items: [
            {id: 1, name: 'A', val: 1, smth: 'F'},
            {id: 3, name: 'A', val: 3, smth: true}, 
            {id: 5, name: 'A', val: 5, smth: null}
        ],
        // Summed values (from fields described by sumFields)
        val: 9, // 1 + 3 + 5
        // Other fields of first item of group
        name: 'A',
        id: 1,
        smth: 'F'
    }, {
        groupField: 'name',
        groupValue: 'B',
        items: [
            {id: 2, name: 'B', val: 2, smth: 444},
            {id: 4, name: 'B', val: 4, smth: false}, 
        ],
        val: 6, // 2 + 4
        name: 'B',
        id: 2,
        smth: 444
    }
]
```
