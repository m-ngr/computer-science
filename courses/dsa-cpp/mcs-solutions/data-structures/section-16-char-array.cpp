/**
* Mahmoud Elnagar
*/
#include <iostream>
#include <map>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Is Prefix?
bool is_prefix(const string& str, const string& pref){
  if (pref.size() > str.size()) return false;

  for(int i = 0; i < pref.size(); ++i){
    if (str[i] != pref[i]) return false;
  }

  return true;
}

//Problem #2: Is Suffix?
bool is_suffix(const string& str, const string& suff){
  if (suff.size() > str.size()) return false;
  int si = str.size() - suff.size();
  for(int i = 0; i < suff.size(); ++i){
    if (str[si + i] != suff[i]) return false;
  }

  return true;
}

//Problem #3: Is Substring?
bool is_substr(const string& str, const string& sub){
  if (sub.size() > str.size()) return false;

  int si = 0;
  for(int i = 0; i < str.size(); ++i){
    if (sub[si] != str[i]) si = 0;
    if (sub[si] == str[i]) ++si;
    if (si == sub.size()) return true;
  }

  return false;
}

//Problem #4: Is Subsequence?
bool is_subseq(const string& str, const string& seq){
  if (seq.size() > str.size()) return false;
  int si = 0;
  for(int i = 0; i < str.size(); ++i){
    if(seq[si] == str[i]) ++si;
    if (si == seq.size()) return true;
  }
  return false;
}

//Problem #5: Convert to number
int to_int(const string& str){
  int num = 0;
  for(int i = 0; i < str.size(); ++i){
    if (('0' > str[i]) || (str[i] > '9')) break;
    num = num * 10 + (str[i] - '0');
  }

  return num;
}

//Problem #6: Grouping
void grouping(const string& str){
  char prev = str[0];
  for(auto& ch : str){
    if(ch != prev){
      cout<<" ";
      prev = ch;
    }
    cout<<ch;
  }
}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: Compressing
string compress(const string& str){
  string output;
  int cnt = 0;
  char prev = str[0];
  for(auto& ch: str){
    if(prev != ch){
      output += prev + to_string(cnt) + "_";
      prev = ch;
      cnt = 0;
    }
    ++cnt;
  }
  output += prev + to_string(cnt);
  return output;
}

//Problem #2: Compare strings
bool str_le(const string& s1, const string& s2){
  int sz = min(s1.size(), s2.size());
  for(int i = 0; i < sz; ++i){
    if (s1[i] > s2[i]) return false;
    else if (s1[i] < s2[i]) return true;
  }
  return (s1.size() <= s2.size());
}

//Problem #3: Add two strings
string addstr(const string& n1, const string& n2){
  string res;
  int carry = 0;
  int i1 = n1.size() - 1;
  int i2 = n2.size() - 1;

  for(; (i1 >= 0) && (i2 >= 0); --i1, --i2){
    int num = n1[i1] - '0' + n2[i2] - '0' + carry;
    res = (char)((num%10) + '0') + res;
    carry = num/10;
  }

  for(; i1 >= 0; --i1){
    int num = n1[i1] - '0' + carry;
    res = (char)((num%10) + '0') + res;
    carry = num/10;
  }

  for(; i2 >= 0; --i2){
    int num = n2[i2] - '0' + carry;
    res = (char)((num%10) + '0') + res;
    carry = num/10;
  }

  if (carry) res = (char)(carry + '0') + res;
  return res;
}

#pragma endregion
