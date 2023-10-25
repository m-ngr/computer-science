#include <iostream>
#define MAX 1000
using namespace std;

int display();
void printAll(string name[], int age[], double salary[], char gender[], int added);
void addNew(string name[], int age[], double salary[], char gender[], int &added);
void updateSalary(string name[], double salary[], int added);
void deleteAge(string name[], int age[], double salary[], char gender[], int &added);

int main(void){
    string name[MAX];
    int age[MAX] = {0};
    double salary[MAX] = {0.0};
    char gender[MAX];
    int added = 0;

    while(true){
        switch(display()){
        case 1:
            addNew(name, age, salary,gender,added);
            break;
        case 2:
            printAll(name, age, salary,gender,added);
            break;
        case 3:
            deleteAge(name, age, salary,gender,added);
            break;
        case 4:
            updateSalary(name,salary,added);
            break;
        }
    }
}

void deleteAge(string name[], int age[], double salary[], char gender[], int &added){
    int lowAge, highAge;
    int sub = 0;
    cout<<"Enter Low Age , High Age: "; cin>>lowAge>>highAge;
    for(int i = 0; i < added ; i++){
        if (lowAge <= age[i] && age[i] <= highAge){
            age[i] = -1;
            sub++;
        }
    }
    for(int i = 0; i < added ; i++){
        if (age[i] == -1){
            for(int j = i+1; j < added; j++){
                if (age[j]!= -1){
                    name[i] = name[j];
                    age [i] = age [j];
                    salary[i] = salary [j];
                    gender [i] = gender [j];
                    age[j] = -1;

                    break;
                }
            }
        }
    }
    added = added - sub;
}

void updateSalary(string name[], double salary[], int added){
    string empName;
    cout<<"Enter Name: "; cin>>empName;
    for (int i = 0; i < added ; i++){
        if (name[i] == empName){
            cout<<"Current Salary : "<< salary[i]<< endl;
            cout<<"Enter new salary: "; cin>>salary[i];
            return;
        }
    }
    cout<<empName<<" Not Found\n";
}

void addNew(string name[], int age[], double salary[], char gender[], int &added){
    cout<<"Enter Name: ";   cin>> name[added];
    cout<<"Enter Age: ";   cin>> age[added];
    cout<<"Enter Salary: ";   cin>> salary[added];
    cout<<"Enter Gender: ";   cin>> gender[added];
    added++;
}

void printAll(string name[], int age[], double salary[], char gender[], int added){
    if (added == 0){cout<<"EMPTY\n"; return;}
    for (int i = 0; i < added ; i++){
        cout<<"#ID: "<< i+1<<"\t"
            <<"#NAME: "<< name[i]<< "\t"
            <<"#AGE: "<< age[i]<< "\t"
            <<"#SALARY: "<< salary[i]<< "\t"
            <<"#GENDER: "<< gender[i]<< "\n";
    }
}

int display(){
    cout<<"Enter your Choice:\n"
        <<"1. Add new employee \t2.Print all employees\n"
        <<"3. Delete by age\t4.Update salary by name\n";
    int opt = -1;
    cin>>opt;
    while (opt < 1 || opt > 4){
        cout<<"Please enter number between 1 and 4 : ";
        cin>>opt;
    }
    return opt;
}
