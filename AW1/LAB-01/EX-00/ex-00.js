"use strict";

let strings = ['a', 'ab', 'abc', 'abcd', 'abcde', 'abcdef'];

strings.forEach((str, idx, arr) => {
    arr[idx] = str.slice(2).slice(0, -2);
})

console.log(strings);