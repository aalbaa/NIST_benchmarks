#include <iostream>
#include <map>
#include <string>

using namespace std;

int main(){
    map<std::string, int> map_test;
    map_test["Hi"] = 99;
    map_test["No"] = 12;

    cout << map_test["no"] << endl;

    int** p = new int*[10];
    for(int i = 0; i < 10; i++){
        p[i] = new int[2];
    }
    for(int i=0; i<10; i++){
        for(int j=0; j<2; j++){
            p[i][j] = i+j;
        }
    }

    // Deleting
    for(int i=0; i<10; i++){
        delete p[i];
    }   
    delete p;
    cout << p[9][1] << endl;

}