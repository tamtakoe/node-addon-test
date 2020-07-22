# node-addon-test

#### Install
Install node.js 12+
```sh
npm install 
#Use it also for rebuild C++ code
```

#### Run as node-addon
```sh
npm start
```

#### Run as C++ application
Change `add_executable(cl_project main.cc)` to `add_executable(cl_project main.cpp)` in `CMakeList.txt`  
Build and run it by CLion etc.


#### Files
`group-by.js` - JS algorithm   
`group_by.h` - C++ algorithm   
`group_by_napi.h` - JS/C++ wrappers (need totally optimization)

#### Example
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