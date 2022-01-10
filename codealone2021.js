/* Calculate the minimum required number of swaps of neighbouring letters in a string built from the letters 'a' and 'b', 
after which the string would contain both "aaa" and "bbb" as substrings. */

'use strict';

function solution(S) {
    const leftA = [];
    const rightA = [];
    const leftB = [];
    const rightB = [];

    for (let index = 0; index < S.length; ++index) {
        if (S[index] === 'a') {
            leftA[index] = 0;
            leftB[index] = index > 0 ? leftB[index - 1] + 1 : Infinity;
        } else {
            leftB[index] = 0;
            leftA[index] = index > 0 ? leftA[index - 1] + 1 : Infinity;
        }
    }
    for (let index = S.length - 1; index >= 0; --index) {
        if (S[index] === 'a') {
            rightA[index] = 0;
            rightB[index] = index < S.length - 1 ? rightB[index + 1] + 1 : Infinity;
        } else {
            rightB[index] = 0;
            rightA[index] = index < S.length - 1 ? rightA[index + 1] + 1 : Infinity;
        }
    }

    let hasValidA = false;
    let hasValidB = false;
    let hasThreeA = false;
    let hasThreeB = false;
    for (let index = 1; index < S.length - 1; ++index) {
        if (S[index] === 'a' && isFinite(leftA[index - 1] + rightA[index + 1])) hasValidA = true;
        if (S[index] === 'b' && isFinite(leftB[index - 1] + rightB[index + 1])) hasValidB = true;
        if (S[index - 1] === S[index] && S[index + 1] === S[index]) {
            if (S[index] === 'a') hasThreeA = true;
            else hasThreeB = true;
        }
    }

    if (!hasValidA || !hasValidB) return -1;
    if (hasThreeA && hasThreeB) return 0;

    let min = Infinity;

    if (hasThreeA) {
        for (let index = 1; index < S.length - 1; ++index) {
            if (S[index] === 'b') {
                min = Math.min(min, leftB[index - 1] + rightB[index + 1]);
            }
        }
        return min;
    }
    if (hasThreeB) {
        for (let index = 1; index < S.length - 1; ++index) {
            if (S[index] === 'a') {
                min = Math.min(min, leftA[index - 1] + rightA[index + 1]);
            }
        }
        return min;
    }

    for (let repeat = 0; repeat < 2; ++repeat) {
        for (let index = 0; index < S.length; ++index) {
            let countA = 0;
            let countB = 0;
            let nestedIndex = index;
            for (; nestedIndex < S.length && (countA < 3 || countB < 3); ++nestedIndex) {
                if (S[nestedIndex] === 'a') ++countA;
                else ++countB;
            }
            if (countA < 3 || countB < 3) continue;
            const string = S.slice(index, nestedIndex);
            if (string in mins) {
                min = Math.min(min, mins[string]);
            }
        }
        S = [...S].reverse().join('');
    }

    if (!isFinite(min)) {
        return S;
    }

    return min;
}

const mins = {
   
    aababb: 1,
    aabbab: 2,
    abaabb: 2,
    ababab: 3,
    ababba: 4,
    abbaab: 5,
    abbaba: 4,
    baabab: 4,
    baabba: 5,
    babaab: 4,
    bababa: 3,
    babbaa: 2,
    bbaaba: 2,
    bbabaa: 1,

    aababab: 2,
    aabbaab: 3,
    abaabab: 3,
    ababaab: 4,
    abababb: 2,
    ababbab: 3,
    abbaabb: 3,
    abbabab: 4,
    abbabba: 5,
    baabaab: 5,
    baababa: 4,
    baabbaa: 3,
    babaaba: 3,
    bababaa: 2,
    bababba: 4,
    babbaba: 3,
    bbaabba: 3,
    bbababa: 2,

    aababaab: 3,
    abaabaab: 4,
    abbababb: 3,
    abbabbab: 4,
    baabaaba: 4,
    baababaa: 3,
    babbabba: 4,
    bbababba: 3,
};
