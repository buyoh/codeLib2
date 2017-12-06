// %title
// trie tree
// 
// %overview
// 文字列(あるいは配列)の集合を管理する．
// map<string,int> みたいなイメージ
//
// %usage
// TVAL& operator()(IBEGIN begin, IEND end) 
// 配列[begin,end)に対応するTVALの参照を返す
// 
// %verified
// (旧) : http://yukicoder.me/submissions/123054
// %references
// プログラミングチャレンジブック

template<typename TKEY=char, typename TVAL=int>
struct trietree{
    TVAL value;
    map<TKEY,trietree> trie;
    trietree(TVAL v):value(v){}
    trietree():value(0){}
    
    template<typename IBEGIN,typename IEND>
    TVAL& operator()(IBEGIN begin, IEND end) {
        return (begin==end) ? value
                            : trie[*begin](++begin, end);
    }
};
