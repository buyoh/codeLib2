// %title
// 正規表現[TODO]
// 
// %overview
// 簡単に書いてみた．TODOリストとして追加．
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

    template<typename T>
    struct Range {
        T begin, tail;
        inline Range(T _begin, T _end) :begin(_begin), tail(_end) { }
        inline explicit Range(T _single) : begin(_single), tail(_single) { }
        inline Range() : begin(0), tail(0) { }

        inline bool cover(T x) const noexcept { return begin <= x && x <= tail; }
    };

    struct CharSet {
        vector<pair<Range<char>, bool>> ranges_;
        inline CharSet() { }
        inline CharSet(const CharSet& cs) :ranges_(cs.ranges_) { }
        inline CharSet(CharSet&& cs) :ranges_(move(cs.ranges_)) { }
        inline explicit CharSet(const Range<char>& r) { push(r); }
        inline explicit CharSet(char c) { push(Range(c)); }

        void push(const Range<char>& r, bool negative = false) {
            ranges_.emplace_back(r, negative);
        }
        inline bool cover(char c) const {
            return any_of(ranges_.begin(), ranges_.end(),
                [c](const pair<Range<char>, bool>& p) {return p.second ^ (p.first.cover(c)); });
        }
        static const CharSet Any;
    };

    enum struct ERep {
        J1, // 1個
        M1, // 1個以上
        M0, // 0個以上
        L1, // 1個以下
    };

    struct Node {
        CharSet chr;
        ERep type;
        inline explicit Node(CharSet&& _chr, ERep _type = ERep::J1)
            :chr(_chr), type(_type) {
        }
        inline explicit Node(const CharSet& _chr, ERep _type = ERep::J1)
            :chr(_chr), type(_type) {
        }
        inline bool match(char c) const {
            return chr.cover(c);
        }
        // 0文字の一致を認めるか
        inline bool isOk0() const noexcept { return type == ERep::L1 || type == ERep::M0; }
        // 2文字以上の一致を認めるか
        inline bool isOkMany() const noexcept { return type == ERep::M1 || type == ERep::M0; }
    };

    vector<Node> pattern_; // TODO: Link NFA


    tuple<size_t, CharSet> parseCharSet(const string& rawPattern, size_t startPtr = 0) {
        int escape = 0;

        CharSet charset;
        bool enable_range = false;
        bool enable_holderLastChr = false;
        char holderLastChr;
        bool negative = false;

        for (size_t i = startPtr; i < rawPattern.size(); ++i) {
            char c = rawPattern[i];
            char cL = rawPattern[i - 1]; // C++11
            char cP = rawPattern[i + 1];

            if (escape == 0 && c == ']') {
                assert(enable_range == false);
                if (enable_holderLastChr) // charが余っているならばcharsetに入れる
                    charset.push(Range(holderLastChr));
                return make_tuple(i, charset);
            }
            else if (escape == 0 && c == '-') {
                assert(enable_holderLastChr == true);
                assert(enable_range == false);
                enable_range = true;
            }
            else if (escape == 0 && c == '^') {
                assert(negative == false);
                negative = true;
            }
            else if (escape == 0 && c == '/') {
                escape = 1;
            }
            else {
                if (enable_range) {
                    assert(enable_holderLastChr == true);
                    if (holderLastChr > c)
                        charset.push(Range(c, holderLastChr));
                    else
                        charset.push(Range(holderLastChr, c));
                    enable_holderLastChr = false;
                    enable_range = false;
                    negative = false;
                }
                else {
                    if (enable_holderLastChr) // charが余っているならばcharsetに入れる
                        charset.push(Range(holderLastChr), negative),
                        negative = false;
                    enable_holderLastChr = true;
                    holderLastChr = c; // holderLastChrに値を入れておく
                }
                escape = 0;
            }
        }
        abort();
    }

    int parse(const string& rawPattern, size_t startPtr = 0) {

        int escape = 0;

        for (size_t ptr = startPtr; ptr < rawPattern.size(); ++ptr) {
            char c = rawPattern[ptr];
            //char cL = rawPattern[ptr - 1]; // C++11
            //char cP = rawPattern[ptr + 1];

            // open
            if (escape == 1) {
                // escaped
                // 特定の文字として解釈
                pattern_.emplace_back(CharSet(c));
                escape = 0;
            }
            else if (c == '+') {
                // 1つ以上
                if (pattern_.empty()) abort();
                pattern_.back().type = ERep::M1; // TODO: 
            }
            else if (c == '*') {
                // 0個以上
                if (pattern_.empty()) abort();
                pattern_.back().type = ERep::M0;
            }
            else if (c == '?') {
                // 0か1つ
                if (pattern_.empty()) abort();
                pattern_.back().type = ERep::L1;
            }
            else if (c == '.') {
                // 任意の文字
                pattern_.emplace_back(CharSet::Any);
            }
            else if (c == '[') {
                // 特定の範囲の文字
                auto ret = parseCharSet(rawPattern, ptr + 1);
                ptr = get<0>(ret);
                pattern_.emplace_back(move(get<1>(ret)));
            }
            else if (c == '/') {
                // escape
                escape = 1;
            }
            else {
                // 特定の文字
                pattern_.emplace_back(CharSet(c));
            }
        }
    }

    RegExp(const string& rawPattern) {
        parse(rawPattern);
    }

    bool match(const string& text) {

        vector<bool> dp1(pattern_.size() + 1); // current dp
        vector<bool> dp2(pattern_.size() + 1); // next dp
        dp1[0] = true;

        for (char c : text) {
            for (size_t pi = 0; pi < pattern_.size(); ++pi) {
                if (!dp1[pi]) continue;

                if (pattern_[pi].match(c)) {
                    dp2[pi + 1ull] = true; // 次のパターンに進む
                    if (pattern_[pi].isOkMany())
                        dp2[pi] = true; // 次のパターンに進まなくても良い
                }
                if (pattern_[pi].isOk0())
                    dp1[pi + 1ull] = true; // 今のパターンをスキップする
            }
            dp1.swap(dp2);
            fill(dp2.begin(), dp2.end(), false);
        }

        for (size_t pi = 0; pi < pattern_.size(); ++pi) {
            if (!dp1[pi]) continue;
            if (pattern_[pi].isOk0())
                dp1[pi + 1ull] = true; // 今のパターンをスキップする
        }

        return dp1[pattern_.size()];
    }
};
const RegExp::CharSet RegExp::CharSet::Any
    = RegExp::CharSet(RegExp::Range(numeric_limits<char>::min(), numeric_limits<char>::max()));


void test() {
    {
        RegExp r("a.c");
        assert(r.match("abc") == true);
        assert(r.match("acc") == true);
        assert(r.match("abb") == false);
        assert(r.match("acd") == false);
        assert(r.match("ac") == false);
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
        RegExp r(".");
        assert(r.match("a") == true);
        assert(r.match("z") == true);
        assert(r.match("aa") == false);
        assert(r.match("zz") == false);
        assert(r.match("") == false);
    }
    {
        RegExp r("..");
        assert(r.match("aa") == true);
        assert(r.match("zz") == true);
        assert(r.match("a") == false);
        assert(r.match("zzz") == false);
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
    {
        RegExp r("b[b-dx]d");
        assert(r.match("bbd") == true);
        assert(r.match("bcd") == true);
        assert(r.match("bdd") == true);
        assert(r.match("bxd") == true);
        assert(r.match("bad") == false);
        assert(r.match("bed") == false);
        assert(r.match("ddd") == false);
        assert(r.match("bbb") == false);
    }
    {
        RegExp r("[a-z][a-z0-9]*");
        assert(r.match("regexp") == true);
        assert(r.match("mt19937") == true);
        assert(r.match("destr0yer") == true);
        assert(r.match("1024") == false);
        assert(r.match("200ok") == false);
        assert(r.match("a") == true);
        assert(r.match("1") == false);
    }
    {
        RegExp r("/[[o/-x]+/]");
        assert(r.match("[o-x]") == true);
        assert(r.match("[oo]") == true);
        assert(r.match("[-]") == true);
        assert(r.match("[]") == false);
        assert(r.match("[xx") == false);
        assert(r.match("[") == false);
        assert(r.match("[[ooxx]") == false);
    }

    cout << "ok" << endl;
}
