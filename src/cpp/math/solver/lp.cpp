// %title
// 簡易線形計画問題ソルバー[TODO]
//
// %overview
// 等式形の線形計画問題を単体法で解く．
//
// %usage
// ; 一番下のmain関数を見てください．
//
// %verified
// http://www.fujilab.dnj.ynu.ac.jp/lecture/system2.pdf
// https://yukicoder.me/submissions/213095
// http://dic.nicovideo.jp/a/%E7%B7%9A%E5%BD%A2%E8%A8%88%E7%94%BB%E5%95%8F%E9%A1%8C
// [TODO]
//
// %references
//

class EasyLP {
 public:
  const int dimension;
  // 左辺
  vector<vector<double>> equation;
  // 定数項
  vector<double> offset;
  // 目的関数
  vector<double> objective;
  // 求まった目的関数の値
  double objective_c;

  EasyLP(int n) : dimension(n), objective(n) {}

  // 目的関数を最小化する．
  // 式の情報は書き換えられる．
  double minimize() {
    while (true) {
      // printTableau();
      // 目的関数の値を減少させることができる変数を探す
      int idx = 0;
      {
        double val = objective[0];
        for (int i = 1; i < dimension; ++i) {
          if (val > objective[i]) {
            val = objective[i];
            idx = i;
          }
        }
        // 無いなら最適解が得られた
        if (val >= 0)
          break;
      }

      // どの程度まで増やせば良いだろうか
      int target = -1;
      {
        double val, lim = 0;
        for (int i = 0; i < equation.size(); ++i) {
          if (equation[i][idx] == 0)
            continue;  // 0除算防止
          val = offset[i] / equation[i][idx];
          if (0 < val && (target == -1 || val < lim)) {
            lim = val;
            target = i;
          }
        }
      };
      if (target == -1)
        break;
      {
        // ピボット係数
        double p = equation[target][idx];

        // ピボット係数を p で除算
        for (double& e : equation[target])
          e /= p;
        offset[target] /= p;

        // ピボット列の掃き出し
        for (int i = 0; i < equation.size(); ++i) {
          if (i == target)
            continue;

          double d = equation[i][idx];
          for (int j = 0; j < dimension; j++) {
            if (j != idx)
              equation[i][j] -= d * equation[target][j];
            else
              equation[i][j] = 0;
          }

          offset[i] -= d * offset[target];
        }
        {
          double d = objective[idx];
          for (int j = 0; j < dimension; j++) {
            if (j != idx)
              objective[j] -= d * equation[target][j];
            else
              objective[j] = 0;
          }

          objective_c += d * offset[target];
        }
      }
    }
    return -objective_c;
  }

  void printTableau() {
    for (int i = 0; i < equation.size(); ++i) {
      for (double a : equation[i]) {
        printf(" %7.2f", a);
      }
      printf(" | %7.2f\n", offset[i]);
    }
    for (int i = equation.size(); 0 <= i; --i) {
      printf("---------");
    }
    printf("\n");
    for (double a : objective) {
      printf(" %7.2f", a);
    }
    printf(" %7.2f\n#####\n", objective_c);
  }
};

int main() {
  {
      // EasyLP lp(5);
      // lp.equation.push_back({ 60.0, 40.0,  1.0,  0.0, 0.0 }); lp.offset.push_back(3800.0);
      // lp.equation.push_back({ 20.0, 30.0,  0.0,  1.0, 0.0 }); lp.offset.push_back(2100.0);
      // lp.equation.push_back({ 20.0, 10.0,  0.0,  0.0, 1.0 }); lp.offset.push_back(1200.0);
      // lp.objective = { -400.0, -300.0, 0.0, 0.0, 0.0 };   lp.objective_c = 0.0;
      //
      // printf("%.7f\n", lp.minimize());
  } {
    EasyLP lp(6);
    lp.equation.push_back({5.0, 1.0, 2.0, 1.0, 0.0, 0.0});
    lp.offset.push_back(20.0);
    lp.equation.push_back({2.0, 2.0, 6.0, 0.0, 1.0, 0.0});
    lp.offset.push_back(30.0);
    lp.equation.push_back({2.0, 6.0, 4.0, 0.0, 0.0, 1.0});
    lp.offset.push_back(40.0);
    lp.objective = {-1.0, -1.0, -1.0, 0.0, 0.0};
    lp.objective_c = 0.0;

    printf("%.7f\n", lp.minimize());
  }
  return 0;
}