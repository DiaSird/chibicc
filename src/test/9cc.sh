#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    # gcc (cc) "input" >  tmp.s
    ./9cc "$input" >tmp.s # Execute (9cc = compiler)
    cc -o tmp tmp.s       # Assemble & Link
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

# test statemet
# assert(expected, input)
assert 0 0 # expected 0, actual 127
assert 42 42
assert 0 "1+2+3+4+5+6+7+8+9+10-55"
assert 0 "1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 - 55"

echo OK
