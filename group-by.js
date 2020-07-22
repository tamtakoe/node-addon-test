/**
 * Group items by field
 *
 * items =
 *   [{id: 1, name: 'A', i: 1}, {id: 2, name: 'B', i: 2}, {id: 3, name: 'A', i: 3}, {id: 4, name: 'B', i: 4}, {id: 5, name: 'A', i: 5}]
 *
 * groupByField(items, 'name', ['i']) =
 *   [
 *     {
 *       groupField: 'name',
 *       groupValue: 'A',
 *       items: [{id: 1, name: 'A', i: 1}, {id: 3, name: 'A', i: 3}, {id: 5, name: 'A', i: 5}],
 *       id: 1, //First value
 *       i: 9   //Summed value
 *     }, {
 *       groupField: 'name',
 *       groupValue: 'B',
 *       items: [{id: 2, name: 'B', i: 2}, {id: 4, name: 'B', i: 4}],
 *       id: 2,
 *       i: 6
 *     }
 *   ]
 *
 * @param items - Array<any> - items array
 * @param field - string - field name
 * @param sumFields - Array<string> - array with names of fields which should be summed
 * @returns Array<any>
 */
function groupBy(items, field, sumFields = []) {
    const itemsByField = new Map() // We need correct order of items
    const sumReducer = (fieldName) => (prev, next) => (prev + next[fieldName] || 0);

    for (let i = 0; i < items.length; i++) {
        const item = items[i];
        const groupKey = item[field];
        const groupedCollection = itemsByField.get(groupKey) || [];

        if (!groupedCollection.length) {
            itemsByField.set(groupKey, groupedCollection);
        }

        groupedCollection.push(item);
    }

    const result = [];

    for (const entry of itemsByField) {
        const key = entry[0];
        const items = entry[1];
        const aggregatedItem = { ...items[0] }; // Copy first item

        // Add values which should be summed
        for (let j = 0; j < sumFields.length; j++) {
            const fieldName = sumFields[j];

            aggregatedItem[fieldName] = items.reduce(sumReducer(fieldName), 0)
        }

        aggregatedItem.groupField = field;
        aggregatedItem.groupValue = key;
        aggregatedItem.items = items;

        result.push(aggregatedItem);
    }

    return result;
}

module.exports = groupBy;