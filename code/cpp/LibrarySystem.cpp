#include <iostream>
#include <algorithm>
#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_BORROW 10

using namespace std;

struct book{
    string name;
    int id;
    int quantity;
    int borrows;
    //constructor
    book(){name = ""; id = -1;quantity = 0;borrows = 0;}
    book(string b_name, int b_id, int b_quantity){
        name = b_name;
        id = b_id;
        quantity = b_quantity;
        borrows = 0;
    }
    //Functions
    bool have_prefix(string prefix){
        if (name.size()< prefix.size()){return false;}
        for (int i = 0; i < prefix.size();i++){
            if (prefix[i] != name[i]){return false;}
        }
        return true;
    }
    bool can_borrow(){
        if (quantity - borrows > 0){return true;}else{return false;}
    }
    bool can_return(){
        if (borrows > 0){return true;}else{return false;}
    }
    void print(){
        cout<<"#id = "<<id<<"\tBook Name = "<<name
            <<"\tQuantity = "<<quantity<<"\tborrows = "<<borrows<<endl;
    }
};

struct user{
    string name;
    int id;
    int borrows[MAX_BORROW];
    int borrow_len = 0;
    //constructor
    user(){name = ""; id = -1;borrow_len = 0;}
    user(string myName, int myID){
        name = myName;
        id = myID;
        borrow_len = 0;
    }
    //Functions
    void print(){
        cout<<"#id = "<<id<<"\tUser Name = "<<name;
        if (borrow_len > 0){
            cout<<"\t"<<borrow_len<<" Books Borrowed, ids:";
            for (int i = 0; i < borrow_len; i++){cout<<" "<<borrows[i];}
            cout<<endl;
        }else{
            cout<<"\t No Books Borrowed\n";
        }
    }
    bool can_borrow(){
        if (borrow_len < MAX_BORROW){return true;}else{return false;}
    }
    bool is_borrowed(int b_id){
        for(int i = 0; i < borrow_len; i++){
            if (borrows[i] == b_id){return true;}
        }
        return false;
    }
    bool borrow_book(int b_id){
        if (is_borrowed(b_id)){return false;}
        borrows[borrow_len++] = b_id;
        return true;
    }
    bool return_book(int b_id){
        for (int i = 0; i < borrow_len; i++){
            if (borrows[i] == b_id){
                borrows[i] = INT_MAX;
                sort(borrows,borrows + borrow_len);
                borrow_len -= 1;
                return true;
            }
        }
        return false;
    }
};
//functions used for sorting
bool by_id(book &a, book &b){
    return a.id < b.id;
}
bool user_by_id(user &a, user &b){
    return a.id < b.id;
}
bool by_name(book &a, book &b){
    return a.name < b.name;
}

struct library{
    book mybooks[MAX_BOOKS];
    user myusers[MAX_USERS];
    int book_len = 0;
    int user_len = 0;
    //constructor
    library(){book_len = 0;user_len = 0;}
    //system Functions
    int menu(){
        cout<<"***********************************\n";
        cout<<"Library System:\n";
        cout<<"1.add book\n"
            <<"2.search books by prefix\n"
            <<"3.print who borrowed books by name\n"
            <<"4.print library by id\n"
            <<"5.print library by name\n"
            <<"6.add user\n"
            <<"7.user borrow book\n"
            <<"8.user return book\n"
            <<"9.print users\n"
            <<"10.Exit\n";
        cout<<"***********************************\n";
        cout<<"Enter your choice: ";
        int opt; cin>>opt;
        while(opt>10 || opt < 1){
            cout<<"Enter your choice: ";
            cin>>opt;
        }
        return opt;
    }
    void run(){
        while(true){
            switch(menu()){
            case 1:
                add_book();
                break;
            case 2:
                search_prefix();
                break;
            case 3:
                who_borrow();
                break;
            case 4:
                print_books_by_id();
                break;
            case 5:
                print_books_by_name();
                break;
            case 6:
                add_user();
                break;
            case 7:
                user_borrow();
                break;
            case 8:
                user_return();
                break;
            case 9:
                print_users();
                break;
            case 10:
                return;
            }
        }
    }
    //Book Functions
    void add_book(){
        if (book_len >= MAX_BOOKS){cout<<"Cannot add more books : Book Full Capacity\n";}

        string b_name; int b_id, b_quantity;
        cout<<"Enter book name, id , quantity: ";
        cin>>b_name>>b_id>>b_quantity;

        for(int i = 0; i < book_len; i++){
            if (mybooks[i].id == b_id){
                cout<<"Book id already exist; book not added\n";
                return;
            }
        }
        mybooks[book_len++] = book(b_name,b_id,b_quantity);
    }

    void print_books(){
        for(int i = 0; i < book_len; i++){mybooks[i].print();}
    }
    void print_books_by_id(){
        sort(mybooks,mybooks+book_len,by_id);
        print_books();
    }
    void print_books_by_name(){
        sort(mybooks,mybooks+book_len,by_name);
        print_books();
    }
    void search_prefix(){
        string prefix;
        cout<<"Enter Book Prefix: "; cin>>prefix;
        sort(mybooks,mybooks+book_len,by_name);
        for(int i = 0; i < book_len; i++){
            if (mybooks[i].have_prefix(prefix)){mybooks[i].print();}
        }
    }
    void who_borrow(){
        int b_id;
        cout<<"Enter book id: "; cin>>b_id;
        for(int i = 0; i < user_len; i++){
            if (myusers[i].is_borrowed(b_id)){myusers[i].print();}
        }
    }
    //User Functions
    void user_borrow(){
        int u_id, b_id,u_idx,b_idx;
        bool valid_user = false, valid_book = false;
        cout<<"Enter user id, book id: ";
        cin>>u_id>>b_id;
        //is user id exist on the system and can he borrow?
        for(int i = 0; i < user_len; i++){
            if (myusers[i].id == u_id){
                if (myusers[i].can_borrow()){
                    valid_user = true;
                    u_idx = i;
                    break;
                }else{
                    cout<<"Borrow Failed: user reached maximum number of borrowings\n";
                    return;
                }
            }
        }
        if(!valid_user){cout<<"Borrow Failed: user id not found\n";return;}
        //is book id exist on the system and can we borrow it?
        for(int i = 0; i < book_len; i++){
            if (mybooks[i].id == b_id){
                if (mybooks[i].can_borrow()){
                    valid_book = true;
                    b_idx = i;
                    break;
                }else{
                    cout<<"Borrow Failed: all books already borrowed\n";
                    return;
                }
            }
        }
        if(!valid_book){cout<<"Borrow Failed: book id not found\n";return;}
        //let the user borrow if he didn't borrow before.
        if (myusers[u_idx].borrow_book(b_id)){
            mybooks[b_idx].borrows += 1;
        }else{
            cout<<"Borrow Failed: user already borrowed this book\n";
        }
    }

    void user_return(){
        int u_id, b_id,u_idx,b_idx;
        bool valid_user = false, valid_book = false;
        cout<<"Enter user id, book id: ";
        cin>>u_id>>b_id;
        //is user id exist on the system?
        for(int i = 0; i < user_len; i++){
            if (myusers[i].id == u_id){
                valid_user = true;
                u_idx = i;
                break;
            }
        }
        if(!valid_user){cout<<"Return Failed: user id not found\n";return;}
        //is book id exist on the system and did we give this book before?
        for(int i = 0; i < book_len; i++){
            if (mybooks[i].id == b_id){
                if (mybooks[i].can_return()){
                    valid_book = true;
                    b_idx = i;
                    break;
                }else{
                    cout<<"Return Failed: we did not give you this book\n";
                    return;
                }
            }
        }
        if(!valid_book){cout<<"Return Failed: book id not found\n";return;}
        //let the user return the book if he borrowed it
        if (myusers[u_idx].return_book(b_id)){
            mybooks[b_idx].borrows -= 1;
        }else{
            cout<<"Return Failed: this user did not borrow this book\n";
        }
    }

    void add_user(){
        if (user_len >= MAX_USERS){cout<<"Cannot add more users : User Full Capacity\n";}
        string u_name; int u_id;
        cout<<"Enter user name, id: ";
        cin>>u_name>>u_id;
        for(int i = 0; i < user_len; i++){
            if (myusers[i].id == u_id){
                cout<<"User id already exist; user not added\n";
                return;
            }
        }
        myusers[user_len++] = user(u_name,u_id);
        sort(myusers,myusers+user_len,user_by_id);
    }

    void print_users(){
        for (int i = 0; i < user_len; i++){
            myusers[i].print();
        }
    }

};

int main(){
    library libsys;
    libsys.run();
    return 0;
}
