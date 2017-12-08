// %title
// 回文判定
// 
// %overview
// コピペと手打ちどっちが速いかな
//
// %usage
//
// %verified
// 
// %references
// 

bool iskaibun(const string& str){
    for (int left = 0, right = (int)str.size() - 1; left < right; ++left, --right)
        if (str[left] != str[right])
            return false;
    return true;
}
