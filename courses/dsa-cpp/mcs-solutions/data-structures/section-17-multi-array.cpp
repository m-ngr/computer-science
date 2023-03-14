/**
* Mahmoud Elnagar
*/
#include <iostream>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Smaller row?
void smaller_row(){
  int n, m; cin>>n>>m;
  int mat[n][m];
  
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < m; ++j) cin>>mat[i][j];
  }

  int q; cin>>q;

  while(q--){
    int r1, r2; cin>>r1>>r2;
    --r1,--r2;
    bool smaller = true;
    for(int j = 0; j < m; ++j){
      if (mat[r1][j] >= mat[r2][j]){
        smaller = false;
        break;
      }
    }
    cout<<(smaller?"YES":"NO")<<endl;
  }
}

//Problem #2: Triangular matrix
void tri_sum(){
  int n; cin>>n;
  int up_sum = 0, low_sum = 0;

  for(int i = 0; i < n; ++i){
    for(int j = 0; j < n; ++j){
      int num; cin>>num;
      if (i == j){
        up_sum += num;
        low_sum += num;
      }else if (i < j){
        up_sum += num;
      }else{
        low_sum += num;
      }
    }
  }

  cout<<low_sum<<" "<<up_sum<<endl;
}

//Problem #3: Transpose
void transpose(){
  int n, m; cin>>n>>m;
  int mat[m][n];

  for(int i = 0; i < n; ++i){
    for(int j = 0; j < m; ++j){
      cin>>mat[j][i];
    }
  }

  for(int i = 0; i < m; ++i){
    for(int j = 0; j < n; ++j){
      cout<<mat[i][j]<<" ";
    }
    cout<<endl;
  }

}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: How many primes
bool is_prime(int n){
  for(int i = 2; i < n; ++i){
    if(n % i == 0) return false;
  }
  return true;
}

void count_primes(){
  int n, m; cin>>n>>m;
  bool prime[n][m];

  for(int i = 0; i < n; ++i){
    for(int j = 0; j < m; ++j) {
      int num; cin>>num;
      prime[i][j] = is_prime(num);
    }
  }

  int q; cin>>q;
  while (q--) {
    int i,j,r,c; cin>>i>>j>>r>>c;
    int cnt = 0;
    for(int si = i; si < i+r; ++si){
      for(int sj = j; sj < j+c; ++sj){
        if (prime[si][sj]) ++cnt;
      }
    }
    cout<<cnt<<endl;
  }
}

//Problem #2: Find mountains
void print_mountains(){
  int n,m; cin>>n>>m;
  int mat[n][m];
  int di[] = {1,1,1,0,0,-1,-1,-1};
  int dj[] = {1,0,-1,1,-1,1,0,-1};

  for(int i = 0; i < n; ++i){
    for(int j = 0; j < m; ++j) cin>>mat[i][j];
  }

  for(int i = 0; i < n; ++i){
    for(int j = 0; j < m; ++j){
      bool mnt = true;
      for(int k = 0; k < 8; ++k){
        int r = i + di[k];
        int c = j + dj[k];
        if (0 > r || r >= n) continue;
        if (0 > c || c >= m) continue;
        if (mat[i][j] <= mat[r][c]){
          mnt = false;
          break;
        }
      }
      if(mnt) cout<<i<<" "<<j<<endl;
    }
  }
}

//Problem #3: Active Robot
void robot(){
  int n,m; cin>>n>>m;
  int i = 0, j = 0;
  int k; cin>>k;
  int di[] = {-1,0,1,0};
  int dj[] = {0,1,0,-1};

  while(k--){
    int d, s; cin>>d>>s;
    --d;
    i = (i + di[d] * s + n) % n;
    j = (j + dj[d] * s + m) % m;
    cout<<"("<<i<<", "<<j<<")"<<endl;
  }
}

//Problem #4: Flatten 3D Array
void flat3d(){
  int d,r,c; cin>>d>>r>>c;
  int op; cin>>op;
  if (op == 1){
    int k,i,j; cin>>k>>i>>j;
    cout<< (r*c*k + c*i + j)<<endl;
  }else{
    int idx; cin>>idx;
    int k,i,j;
    idx;
    k = idx / (r*c);
    idx %= (r*c);
    i = idx / c;
    idx %= c;
    j = idx;
    cout<<k<<" "<<i<<" "<<j<<endl;
  }
}

#pragma endregion
