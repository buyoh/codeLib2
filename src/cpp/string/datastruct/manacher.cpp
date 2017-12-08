// %title
// manacher(一括回文判定)
// 
// %overview
// manacher
// count_num() はO(|S|)で動作するので注意 [TODO]
//
// %usage
// bool operator()(int left, int right)
// left  : 文字列のインデックスを指定する．
// right : 
// @ret : 部分文字列str[left..right]が回文かどうか．
//
// %verified
// https://yukicoder.me/submissions/139334
// 
// %references
// http://snuke.hatenablog.com/entry/2014/12/02/235837
// https://yukicoder.me/problems/no/464/editorial
// 


class Manacher{
public:
    vector<int> radius_;
    
    Manacher(){}
    Manacher(const string& str){build(str);}
    
    void build(const string& str){
        int length = str.size()*2-1;
        radius_.resize(length);
        
        int c = 0;
        for (int i = 0; i < length; i++) {
            int l = c - (i-c);
            if (i+radius_[l] < c+radius_[c]) {
                radius_[i] = radius_[l];
            } else {
                int j = c+radius_[c] - i;
                for (;i-j >= 0 && i+j < length ; j++){
                    if (((i^j)&1)==1) continue;
                    if (str[(i-j)/2] != str[(i+j)/2]) break;
                }
                radius_[i] = j;
                c = i;
            }
        }
    }
    
    inline int operator[](int idx) const{
        return radius_[idx];
    }
    inline size_t size() const{
        return radius_.size();
    }

    // 部分文字列str[left..right]が回文かどうか．計算量O(1)
    // left,rightに指定するインデックスは，基の文字列のインデックスを指定する．
    inline bool operator()(int left,int right){
        return radius_[left+right] > right-left;
    }

    // 回文の個数を求める．
    ll count_num() {
        ll r = 0;
        for (auto it = radius_.begin(); it < radius_.end(); ++it, ++it) {
            r += (*it + 1ll) / 2ll;
        }
        for (auto it = radius_.begin() + 1; it < radius_.end(); ++it, ++it) {
            r += (*it) / 2ll;
        }
        return r;
    }
    
    void print(const string& str){
        for (int i=0;i<str.size();i++){
            cout << str[i] << " ";
        }
        cout << endl;
        for (int i=0;i<radius_.size();i++){
            cout << radius_[i];
        }
        cout << endl;
    }
};