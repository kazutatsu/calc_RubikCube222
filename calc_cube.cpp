#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// これは2×2×2のルービックキューブを群論で計算するためのものです．
// 正直に行列の掛け算で実装しているため，改良の余地は大いにあります．

// キューブの状態
using CubeMatrix =vector<vector<complex<double>>>; // 8×8の行列

// 単位元を返す関数
CubeMatrix identity() {
  CubeMatrix id(8, vector<complex<double>>(8,0.0));
  for (int i = 0; i < 8; i++) id[i][i] = 1.0;
  return id;
}

// 2つの元の合成 (AにBをかける)
CubeMatrix multiply(const CubeMatrix& A, const CubeMatrix& B) {
  CubeMatrix C(8, vector<complex<double>>(8,0.0));
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  return C;
}

// sigmaとvより元を生成する関数
CubeMatrix make_matrix(const vector<int>& sigma, const vector<int>& v) {
  CubeMatrix M(8, vector<complex<double>>(8,0.0));
  complex<double> omega(-0.5, sqrt(3.0) / 2.0);

  for (int i = 0; i < 8; i++) {
    int j = sigma[i];
    int twist = v[j];
    M[j][i] = pow(omega, twist);
  }
  return M;
}

// 現状の状態を表示する関数
void print_state(const CubeMatrix& state, const vector<string>& operations) {
  vector<int> sigma(8, 0);
  vector<int> v(8, 0);
  complex<double> omega(-0.5, sqrt(3.0) / 2.0);
  complex<double> omega_inv = pow(omega, 2); // ωの逆数

  const double espsilon = 1e-9; // 数値誤差の許容範囲

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (abs(state[i][j]) < espsilon) {
        continue;
      } else if (abs(state[i][j] - 1.0) < espsilon) {
        sigma[j] = i;
        v[i] = 0;
      } else if (abs(state[i][j] - omega) < espsilon) {
        sigma[j] = i;
        v[i] = 1;
      } else if (abs(state[i][j] - omega_inv) < espsilon) {
        sigma[j] = i;
        v[i] = 2;
      }
    }
  }
  cout << "行列の状態: " << endl;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (abs(state[i][j]) < espsilon) {
        cout << "0  ";
      } else if (abs(state[i][j] - 1.0) < espsilon) {
        cout << "1  ";
      } else if (abs(state[i][j] - omega) < espsilon) {
        cout << "ω  ";
      } else if (abs(state[i][j] - omega_inv) < espsilon) {
        cout << "ω' ";
      }
    }
    cout << endl;
  }
  cout << "sigma: ";
  for (int i = 0; i < 8; i++) {
    cout << sigma[i] + 1 << " "; // 1-8で表示
  }
  cout << endl;
  cout << "v: ";
  for (int i = 0; i < 8; i++) {
    cout << v[i] << " "; // 0-2で表示
  }
  cout << endl;
  cout << "適用した操作: ";
  for (const string& op : operations) {
    cout << op << " ";
  }
  cout << endl;
}

int main() {
  // UDRLFBの定義
  // sigmaは講義では1-8で定義しているが、ここでは0-7で定義する
  vector<int> sigma_U = {1,2,3,0,4,5,6,7};
  vector<int> v_U = {0,0,0,0,0,0,0,0};

  vector<int> sigma_D = {0,1,2,3,5,6,7,4};
  vector<int> v_D = {0,0,0,0,0,0,0,0};
  
    vector<int> sigma_L = {0,6,1,3,4,2,5,7};
    vector<int> v_L = {0,2,1,0,0,2,1,0};

  vector<int> sigma_R = {3,1,2,4,7,5,6,0};
  vector<int> v_R = {1,0,0,2,1,0,0,2};

  vector<int> sigma_F = {7,0,2,3,4,5,1,6};
  vector<int> v_F = {2,1,0,0,0,0,2,1};

  vector<int> sigma_B = {0,1,5,2,3,4,6,7};
  vector<int> v_B = {0,0,2,1,2,1,0,0};

  // 操作のマップ
  map<string, CubeMatrix> env;

  // 各操作の行列を生成(マップ)
  env["U"] = make_matrix(sigma_U, v_U);
  env["D"] = make_matrix(sigma_D, v_D);
  env["L"] = make_matrix(sigma_L, v_L);
  env["R"] = make_matrix(sigma_R, v_R);
  env["F"] = make_matrix(sigma_F, v_F);
  env["B"] = make_matrix(sigma_B, v_B);
  
  // 現在のキューブ状態
  CubeMatrix current = identity();

  cout << "2x2x2 ルービックキューブ群 計算電卓" << endl;
  cout << "【基本操作】 U, D, R, L, F, B" << endl;
  cout << "【コマンド】 save [名前], load [名前], AC (クリア)" << endl;
  cout << "※ 複数操作をスペース区切りで入力すると、左から順に計算します (例: F R B)" << endl;
  cout << "※ 群は右から順に計算するが，行列は左から順に掛けることに注意してください．" << endl << endl;

  vector<string> multipled; // 操作した順に保存するためのベクター

  string line;
  while (cout << "コマンドを入力: ", getline(cin, line)) {
    if (line.empty()) continue;

    stringstream ss(line);
    string cmd;
    ss >> cmd;

    if (cmd == "save") {
      string name;
      if (ss >> name) {
        env[name] = current;
        cout << "現状の状態を操作 '" << name << "' として保存しました。" << endl;
      } else {
        cout << "保存する名前を指定してください。" << endl;
      }
    } else if (cmd == "load") {
      string name;
      if (ss >> name) {
        CubeMatrix loaded = identity();
        cout << "操作 '" << name << "' の入力を開始します。" << endl;
        cout << "sigma (置換を8個、スペース区切りで入力): ";
        vector<int> sigma(8, 0);
        vector<int> v(8, 0); // twistは0で初期化
        for (int i = 0; i < 8; i++) {
          int x;
          cin >> x;
          if (x < 1 || x > 8) {
            cout << "置換は1から8の整数で指定してください。" << endl;
            break;
          }
          sigma[i] = x - 1; // 0-7に変換
        }
        cout << "v (8個のねじりをスペース区切りで入力): ";
        for (int i = 0; i < 8; i++) {
          int twist;
          cin >> twist;
          if (twist < 0 || twist > 2) {
            cout << "ねじりは0から2の整数で指定してください。" << endl;
            break;
          }
          v[i] = twist;
        }
        loaded = make_matrix(sigma, v);
        env[name] = loaded;
        cout << "操作 '" << name << "' を保存しました。" << endl;
      }
    } else if (cmd == "AC") {
      current = identity();
      multipled.clear();
      cout << "状態を初期化しました。" << endl;
    } else {
      // 操作の合成
      bool valid = true;
      CubeMatrix result = current;
      vector<string> ops;
      ops.push_back(cmd); // 最初のコマンドも操作として追加
      string op;
      while (ss >> op) {
        ops.push_back(op);
      }
      for (const string& op_name : ops) {
        if (env.count(op_name)) {
          result = multiply(result, env[op_name]);
          multipled.push_back(op_name);
          print_state(result, multipled);

        } else {
          cout << "操作 '" << op_name << "' は定義されていません。" << endl;
          valid = false;
          break;
        }
      }
      if (valid) {
        current = result;
        cout << "操作を適用しました。" << endl;
      }
    }
  }
  return 0;
}