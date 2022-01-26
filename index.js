const groupBy = require('./group-by');
const NativeNapi = require('./addons/native_napi');

function generateData(amount) {
    return Array.from({length: amount}, (v, i) => {
        return {
            id: i,
            group: String.fromCharCode(Math.floor(Math.random() * 25) + 65),
            string: Math.random().toString(36).substring(2),
            number: Math.random() * 100,
            number2: Math.random() * 1000,
            null: null,
            undefined: undefined,
            boolean: true
        }
    })
}

// Generate demo data
const data = generateData(100000)

// Grouping
let grouped;

/**
 * JavaScript
 */
console.log('\n# JavaScript')
console.time('Duration')
grouped = groupBy(data, 'group', ['number', 'number2'])
console.timeEnd('Duration')

/**
 * N-API
 */
console.log('\n# Native addon with N-API')
console.time('Duration')
grouped = NativeNapi.groupBy(data, 'group', ['number', 'number2'])
console.timeEnd('Duration')
// console.log(JSON.stringify(grouped, null, 2));

/**
 * WebAssembly Embind
 */
var EmbindModule = require('./addons/webassembly-embind/group_by.js');

EmbindModule['onRuntimeInitialized'] = function(a) {
    console.log('\n# WebAssembly with Embind binding')
    console.time('Duration')
    grouped = EmbindModule.groupBy(data, 'group', ['number', 'number2']);
    console.timeEnd('Duration')
    // console.log(JSON.stringify(grouped, null, 2));
};

/**
 * WebAssembly WebIdl (WIP)
 */
var WebidlModule = require('./addons/webassembly-webidl/group_by.js');

WebidlModule['onRuntimeInitialized'] = function(a) {
    console.log('\n# WebAssembly with WebIDL binding')
    var CollectionUtils = new WebidlModule.CollectionUtils();

    console.time('Duration')
    var res = CollectionUtils.group_by('Fake result. Work in progress', 42);
    console.timeEnd('Duration')

    // console.log('Result:', res)
};

// const fs = require('fs');
// const wasmBuffer = fs.readFileSync('./addons/webassembly-embind/group_by.wasm');
// WebAssembly.instantiate(wasmBuffer).then(wasmModule => {
//     // Exported function live under instance.exports
//     const groupBy = wasmModule.instance.exports.groupBy;
//     console.log('\n# WebAssembly without emscripten')
//     console.time('Duration')
//     grouped = groupBy(data, 'group', ['number', 'number2']);
//     console.timeEnd('Duration')
// });
