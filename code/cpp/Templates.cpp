#include<iostream>
#include<stack>
#include<string>
#include<math.h>

//#include<bits/stdc++.h>
using namespace std;
void print(auto x){
    cout<<x<<" ";
}
template<typename Type, typename FuncType>
Type sum(FuncType func, auto...lst){
    Type def{}; //default value depending on Type
    cout<<"No of Arguments: "<< sizeof...(lst)<<endl;
    cout<<"Arguments: ";
    (print(lst),...); //evaluate pre-defined function
    cout<<endl<<"External Function: ";
    (func(lst),...); //evaluate user-defined function
    cout<<endl<<"Result = ";
    return (lst + ... + def); //right fold : return def if empty list
    //return (def + ... + lst); //left fold : return def if empty list
}
void external_print(auto x){
    cout<<x<<", ";
}
int main() {
    cout<<sum<int>(external_print<int>,1,2,3,4,5);
}
