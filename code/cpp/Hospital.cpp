#include <iostream>
#include <cstdlib>
using namespace std;
#define SPEC 20
#define MAXQ 5
struct q{
    char name[24];
    bool urgent;
    q* next;
};

void enQ(q** head, string name, bool urgent){
    q* newNode = (q*)malloc(sizeof(q));
    if(newNode == NULL){cout<<"ERROR MALLOC\n";return;}
    //
    for(int i = 0 ; i < 24; i++){
        newNode->name[i] = name[i];
    }
    //
    newNode->urgent = urgent;
    newNode->next = NULL;
    if (*head == NULL){*head = newNode; return;}
    if (urgent){newNode->next = *head; *head = newNode; return;}
    q* currentNode = *head;
    while(currentNode->next != NULL){
        currentNode = currentNode->next;
    }
    currentNode->next = newNode;
}
void deQ(q** head){
    if (*head == NULL){return;}
    q* temp = *head;
    (*head) = (*head)->next;
    cout<<temp->name<<" go with the DR.\n";
    free(temp);
}
void viewQ(q* head){
    while(head != NULL){
        cout<<"Name: "<<head->name<<"\t"<<(head->urgent?"Urgent\n":"Normal\n");
        head = head->next;
    }
}
int countQ(q*head){
    int cnt = 0;
    while(head != NULL){
        cnt++;
        head = head->next;
    }
    return cnt;
}

int menu(){
    cout<<"Enter your choice:\n"
        <<"1.Add new patient\t"<<"2.Print all patients\n"
        <<"3.Get next patient\t"<<"4.Exit\n";
    int opt = 0;    cin>>opt;
    while(opt > 4 || opt < 1){
        cout<<"Enter value from 1 to 4: ";
        cin>>opt;
    }
    return opt;
}

void addNew(q* head[]){
    int sp;
    string name;
    int urgent;
    cout<<"Enter Specialization: ";
    cin>>sp;
    while(sp>SPEC || sp < 1){
        cout<<"Specialization from 1 to "<<SPEC<<endl;
        cout<<"Enter Specialization: ";
        cin>>sp;
    }
    if (countQ(head[sp-1])< MAXQ){
        cout<<"Enter Name , Status(1/0): ";
        cin>>name>>urgent;
        enQ(&head[sp-1], name, urgent?true:false);
    }else{
        cout<<"Specialization "<<sp<<" is FULL\n";
    }
}

void printAll(q* head[]){
    cout<<"***********************************\n";
    bool noPatients = true;
    for (int i = 0 ; i < SPEC; i++){
        int cnt = countQ(head[i]);
        if (cnt){
            noPatients = false;
            cout<<"There are "<<cnt<<" patients in Specialization "<< i+1 <<endl;
            viewQ(head[i]);
            cout<<"***********************************\n";
        }
    }
    if (noPatients){
        cout<<"No Patients in any Specialization\n";
        cout<<"***********************************\n";
    }
}
void nextPatient(q* head[]){
    cout<<"Enter Specialization(1 : "<<SPEC<<"): ";
    int sp; cin>>sp;
    while(sp>SPEC || sp < 1){
        cout<<"Enter Specialization(1 : "<<SPEC<<"): "; cin>>sp;
    }
    if (countQ(head[sp-1])){
        deQ(&head[sp-1]);
    }else{
        cout<<"No patients, you can have rest\n";
    }
}
int main(){
    q* head[SPEC] = {NULL};
    while(true){
        switch(menu()){
        case 1:
            addNew(head);
            break;
        case 2:
            printAll(head);
            break;
        case 3:
            nextPatient(head);
            break;
        case 4:
            return 0;
        }
    }
}
