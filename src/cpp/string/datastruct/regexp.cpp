// %title
// 正規表現[TODO]
// 
// %overview
// 簡単に書いてみた．TODOリストとして追加．
// エスケープすら無い
//
// %usage
// "+*?." を特殊文字扱いする．
// 
//
// %verified
// 
// 
// %references
// 


struct RegExp {

    struct Node {
        char chr;
        bool wild = false;
        uint8_t type = 0;
        Node(char _chr, bool _wild = false, uint8_t _type = 0)
            :chr(_chr), wild(_wild), type(_type) {
        }
    };

    vector<Node> pattern_;

    RegExp(const string& rawPattern) {
        for (char c : rawPattern) {
            if (c == '+') {
                if (pattern_.empty()) abort();
                pattern_.back().type = 1; // TODO: 
            }
            else if (c == '*') {
                if (pattern_.empty()) abort();
                pattern_.back().type = 2;
            }
            else if (c == '?') {
                if (pattern_.empty()) abort();
                pattern_.back().type = 3;
            }
            else if (c == '.') {
                pattern_.emplace_back(0, true);
            }
            else {
                pattern_.emplace_back(c, false);
            }
        }
    }

    bool match(const string& text) {

        vector<bool> dp1(pattern_.size()+1);
        vector<bool> dp2(pattern_.size()+1);
        dp1[0] = true;

        for (char c : text) {
            for (size_t pi = 0; pi < pattern_.size(); ++pi) {
                if (!dp1[pi]) continue;

                if (pattern_[pi].wild || pattern_[pi].chr == c) {
                    dp2[pi + 1ull] = true;
                    if (pattern_[pi].type == 1 ||
                        pattern_[pi].type == 2)
                        dp2[pi] = true;

                    if (pattern_[pi].type == 2 ||
                        pattern_[pi].type == 3)
                        dp1[pi + 1ull] = true;
                }
                else {
                    if (pattern_[pi].type == 2 ||
                        pattern_[pi].type == 3)
                        dp1[pi + 1ull] = true;
                }
            }
            dp1.swap(dp2);
            fill(dp2.begin(), dp2.end(), false);
        }

        for (size_t pi = 0; pi < pattern_.size(); ++pi) {
            if (!dp1[pi]) continue;
            if (pattern_[pi].type == 2 ||
                pattern_[pi].type == 3)
                dp1[pi + 1] = true;
        }

        return dp1[pattern_.size()];
    }
};


void test() {
    {
        RegExp r("a.c");
        assert(r.match("abc") == true);
        assert(r.match("acc") == true);
        assert(r.match("abb") == false);
        assert(r.match("acd") == false);
        assert(r.match("") == false);
    }
    {
        RegExp r("a+c");
        assert(r.match("aac") == true);
        assert(r.match("ac") == true);
        assert(r.match("c") == false);
        assert(r.match("acc") == false);
        assert(r.match("a") == false);
    }
    {
        RegExp r("a*c");
        assert(r.match("aac") == true);
        assert(r.match("ac") == true);
        assert(r.match("c") == true);
        assert(r.match("acc") == false);
        assert(r.match("a") == false);
    }
    {
        RegExp r("ab?c");
        assert(r.match("abc") == true);
        assert(r.match("ac") == true);
        assert(r.match("adc") == false);
        assert(r.match("acc") == false);
    }
    {
        RegExp r(".*foo.*");
        assert(r.match("barfoobaz") == true);
        assert(r.match("foobaz") == true);
        assert(r.match("foo") == true);
        assert(r.match("fo") == false);
        assert(r.match("") == false);
    }
    {
        RegExp r(".+a.*b.?");
        assert(r.match("xaybz") == true);
        assert(r.match("xxayybz") == true);
        assert(r.match("xwxab") == true);
        assert(r.match("aaab") == true);
        assert(r.match("aab") == true);
        assert(r.match("aabbbbb") == true);
        assert(r.match("abababa") == true);
        assert(r.match("abababaa") == false);
        assert(r.match("ab") == false);
        assert(r.match("abb") == false);
        assert(r.match("a") == false);
        assert(r.match("b") == false);
    }

    cout << "ok" << endl;
}