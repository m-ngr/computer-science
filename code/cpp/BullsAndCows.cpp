#include<iostream>
#include<stdexcept>
#include<ctime>
#include<cstdlib>
#define MAX 4
using namespace std;

char random_char(){
    return ('a' + rand()%26);
}
string random_word(){
    string str = "";
    for (int i = 0; i < MAX; i++){
        str += random_char();
    }
    return str;
}
pair<int, int> play(const string &word,string user_input){
    int cows = 0, bulls = 0;
    for (int i = 0; i < word.size();i++){
        int ans = (int)user_input.find(word[i]);
        if (ans == i){bulls++;}
        else if(ans > -1){cows++;}
    }
    return make_pair(bulls, cows);
}
bool play_again(){
    char ans;
    cout<<"Play Again(y/n)? ";
    cin>>ans;
    if (ans == 'y' || ans =='Y'){return true;}
    return false;
}
int main(){
    srand(time(NULL));
    string word = random_word();
    string input;
    while(cin>>input){
        pair<int, int> ans = play(word, input);
        cout<<ans.first<<" Bulls , "<<ans.second<<" Cows\n";
        if (ans.first >= MAX){
            cout<<"Winner\n";
            if (play_again()){
                word = random_word();
            }else{
                break;
            }
        }
    }
}
