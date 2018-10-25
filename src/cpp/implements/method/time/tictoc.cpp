// %title
// 時間計測用マクロ．
// 
// %overview
// 
//
// %usage
// void tic()
// ;計測開始
// void toc()
// ;ticからtocまでに掛かった時間を表示する
// TIME
// ;chrono::system_clock::now()
// MILLISEC
// ;TIMEの差分をミリ秒に変換
//
// %words
// time,tic,toc
// %verified
// 
// %references

#define TIME chrono::system_clock::now()
#define MILLISEC(t) (chrono::duration_cast<chrono::milliseconds>(t).count())
namespace {
    std::chrono::system_clock::time_point ttt;
    inline void tic() { ttt = TIME; }
    inline void toc() { clog << "TIME : " << MILLISEC(TIME - ttt) << '\n'; }
}