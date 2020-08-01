#include <iostream>
#include <map>
#include <string>

using namespace std;

int main(){
    map<std::string, int> map_test;
    map_test["Hi"] = 99;
    map_test["No"] = 12;

    cout << map_test["no"] << endl;
}