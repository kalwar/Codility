// you can write to stdout for debugging purposes, e.g.
// console.log('this is a debug message');
// fails on Performance test with timeout error: big tests where number of unique letter pairs exceeds 50.

function solution(p, q) {
  let distinct_strings = [{}];
  let seen = {};

  p.split('').forEach((left, index) => {
    let right = q[index];
    if (seen[[left, right]]) return;
    seen[[left, right]] = true;
    distinct_strings.forEach((key) => {
      let left_match = key[left];
      let right_match = key[right];
      if (!(left_match || right_match)) {
        if (right !== left) {
          distinct_strings.push(Object.assign({}, key, {[right]: true}));
        }
        key[left] = true;
      }
    });
    distinct_strings = [...new Set(distinct_strings)];
  });
  return Math.min(...distinct_strings.map((key) => Object.keys(key).length));
}
