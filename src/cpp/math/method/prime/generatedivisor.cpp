// %title
// 約数列挙
// 
// %overview
// 
// %usage
//
// %words
// divisor
// %verified
// 



vector<int> generateDivisor(int number) {
    vector<int> divs;
    // divs.reserve(2000);
    {
        int d = 1;
        while (d <= M / d) {
            if (M / d * d == M) {
                divs.push_back(d);
                divs.push_back(M / d);
            }
            ++d;
        }
    }
    return divs;
}
