const groupBy = require('./group-by');
const collectionUtils = require('./addons/collection_utils');

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
console.log('\n-- JavaScript --')
console.time('Duration')
grouped = groupBy(data, 'group', ['number', 'number2'])
console.timeEnd('Duration')

console.log('\n-- Native addon --')
console.time('Duration')
grouped = collectionUtils.groupBy(data, 'group', ['number', 'number2'])
console.timeEnd('Duration')

// console.log(JSON.stringify(grouped, null, 2));


var EmbindModule = require('./webassembly/group_by.js');

EmbindModule['onRuntimeInitialized'] = function(a) {
    console.log('\n-- WebAssembly with Embind binding --')
    // var res = Module.group_by(data, 'group', ['number', 'number2']);
    // var res = Module.group_by([{a:1}, {b:2}, {c:3}], 'group');
    console.time('Duration')
    var res = EmbindModule.group_by('Work in progress', 7);
    console.timeEnd('Duration')

    console.log('Result:', res)
};


var WebidlModule = require('./webassembly-webidl/group_by.js');

WebidlModule['onRuntimeInitialized'] = function(a) {
    console.log('\n-- WebAssembly with WebIDL binding --')
    var CollectionUtils = new WebidlModule.CollectionUtils();

    console.time('Duration')
    var res = CollectionUtils.group_by('Work in progress', 7);
    console.timeEnd('Duration')

    console.log('Result:', res)
};
