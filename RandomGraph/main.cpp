#include <windows.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

const int NodesNum = 10;     // total nodes number
const int ArcNum = 10;       // total arcs number
const int pSize = 3;         // total partition number
const int tSize = 3;         // terminal number in each partition
const int GraphNum = 10000;  // graph number generate

int get_random_num() {
    srand((int)time(0));
    return (int)rand();
}

int get_random_num(int a, int b) { return ((int)rand() % (b - a + 1)) + a; }

string _transform(int num) {
    stringstream ss;
    ss << num;
    string s;
    ss >> s;
    s = "_" + s;
    return s;
}

string _transform(double num) {
    int temp = num * 100;
    stringstream ss;
    ss << temp;
    string s;
    ss >> s;
    s = "_" + s + "%";
    return s;
}

int main() {
    srand((int)time(NULL));
    for (int GraphId = 0; GraphId < GraphNum; GraphId++) {
    }
    cout << "hello world" << endl;
    return 0;
}