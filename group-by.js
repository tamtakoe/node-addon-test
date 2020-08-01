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
    const groupsMap = {};

    for (let i = 0; i < items.length; i++) {
        const item = items[i];
        const groupValue = item[field];
        let group = groupsMap[groupValue];

        if (!group) {
            group = {items: [], groupField: field, groupValue: groupValue, ...item};
            groupsMap[groupValue] = group;
        }

        // Add values which should be summed
        for (let j = 0; j < sumFields.length; j++) {
            const fieldName = sumFields[j];
            const sumValue = item[fieldName];

            if (sumValue) {
                group[fieldName] += sumValue;
            }
        }

        group.items.push(item);
    }

    return Object.keys(groupsMap).map(key => groupsMap[key]);
}

module.exports = groupBy;
