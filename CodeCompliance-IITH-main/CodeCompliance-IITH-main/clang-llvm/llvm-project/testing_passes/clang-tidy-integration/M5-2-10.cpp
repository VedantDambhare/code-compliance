#include <bits/stdc++.h>
int main()
{
    int num, average, sum = 0;
    int i, n=1;
    i = n + sum-- + average++; // Non-compliant
    i = ++sum + n++; // Non-compliant
    i = --n + average++; // Non-compliant
    return 0;
}