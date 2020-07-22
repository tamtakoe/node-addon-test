const { performance } = require('perf_hooks');
const groupBy = require('./group-by');
const collectionUtils = require('bindings')('collection_utils');

// Generate demo data
const amount = 100000;
const data = Array.from({length: amount}, (v, i) => {
    return {
        id: i,
        group: String.fromCharCode(Math.floor(Math.random() * 25) + 65),
        string: Math.random().toString(36).substring(2),
        number: Math.random() * 100,
        null: null,
        undefined: undefined,
        boolean: true
    }
})

// Grouping
let grouped;
var t0 = performance.now();
grouped = collectionUtils.groupBy(data, 'group', ['number'])
var t1 = performance.now();
grouped = groupBy(data, 'group', ['number'])
var t2 = performance.now();

console.log('----')
console.log('C++ addon duration, ms:', t1 - t0);
console.log('JS duration, ms:', t2 - t1);

// console.log(JSON.stringify(grouped, null, 2));
