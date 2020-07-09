#include <windows.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
typedef int NODE;
const int MAXN = 1e6;
string _filename =
    "D:/aMain/git/test/data/random_graph/general_random/group_1/"
    "n2000_t50_p3_b50/"
    "animal";

const int NodesNum = 2000;          // total nodes number
const int pSize = 3;               // total partition number
const int tSize = 50;              // terminal number in each partition
double subGnodeRatio = 0.7;  // V_k/V
double subGarcRatio = 0.1;   // arc/maximum
const int GraphNum = 20;          // graph number generate
const int TreeSpanNodesNum = 10;
bool random_span = 1;

/* G container */
map<NODE, int> NodesValue;
set<NODE> NodesPool;             // G Nodes Pool
set<NODE> _NodesPool;            // copy of G nodes
set<pair<NODE, NODE>> ArcsPool;  // Arcs Pool

/* subG container */
int subGnodenum = 0, subGarcnum = 0;
set<pair<NODE, NODE>> subGArcPool[pSize + 1];  // subG arcs pool
set<NODE> subGNodesPool[pSize + 1];            // subG Nodes pool
set<NODE> subGTerminalPool[pSize + 1];         // subG terminal pool

void CLEAR() {
	NodesValue.clear();
	NodesPool.clear();
	_NodesPool.clear();
	ArcsPool.clear();
	for (int i = 0; i < pSize + 1; i++) {
		subGArcPool[i].clear();
		subGNodesPool[i].clear();
		subGTerminalPool[i].clear();
	}
	return;
}

int get_random_num() {
    srand((int)time(0));
    return (int)rand();
}

int get_random_num(int a, int b) { return ((int)rand() % (b - a + 1)) + a; }

double get_random_num(double a, double b) {
    double r = (double)rand() / (double)RAND_MAX;
    return a + r * (b - a);
}

int ReturnSetRandom(set<int> &S) {
    auto r = rand() % S.size();  // not _really_ random
    auto it = std::begin(S);
    std::advance(it, r);
    int ans = *it;
    S.erase(it);
    return ans;
}

int ReturnSetRandom_WithoutDelete(set<int> &S) {
    auto r = rand() % S.size();  // not _really_ random
    auto it = std::begin(S);
    std::advance(it, r);
    int ans = *it;
    return ans;
}

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

// build subgraph tree
int vis[MAXN];
void bfs(int root, int k) {
    memset(vis, 0, sizeof(vis));
    queue<NODE> q;
    q.push(root);
	int span_num = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (vis[u]) continue;

		if (random_span) {
			span_num = get_random_num(1, TreeSpanNodesNum);
		} else {
			span_num = TreeSpanNodesNum;
		}

        for (int i = 1; i <= span_num; i++) {
            int v = ReturnSetRandom(_NodesPool);
            int l = min(u, v), r = max(u, v);
            subGNodesPool[k].insert(v);
            subGArcPool[k].insert(make_pair(l, r));
            if (subGNodesPool[k].size() > subGnodenum - 1) return;
            q.push(v);
        }
        vis[u] = 1;
    }
    return;
}

// Add arc to subgraph
void AddArcToSubgraph(int k) {
    // Select Terminal
    while (subGTerminalPool[k].size() < tSize) {
        int u = ReturnSetRandom_WithoutDelete(subGNodesPool[k]);
        if (subGTerminalPool[k].find(u) != subGTerminalPool[k].end()) {
            continue;
        } else {
            subGTerminalPool[k].insert(u);
        }
    }

	if (subGArcPool[k].size() > subGarcnum) {
		cout << "No extra arc added" << endl;
	}

    while (subGArcPool[k].size() < subGarcnum) {
        int u = ReturnSetRandom_WithoutDelete(subGNodesPool[k]);
        int v = ReturnSetRandom_WithoutDelete(subGNodesPool[k]);
        if (u > v) swap(u, v);

        if (u == v ||
            subGArcPool[k].find(make_pair(u, v)) != subGArcPool[k].end()) {
            continue;
        } else if (subGTerminalPool[k].count(u) &&
                   subGTerminalPool[k].count(v)) {
            continue;
        } else {
            subGArcPool[k].insert(make_pair(u, v));
        }
		cout << subGArcPool[k].size() << endl;
    }
    return;
}

// merge subgraph
void MergeSubG() {
    // Merge
    for (int k = 1; k <= pSize; k++) {
        for (auto arc : subGArcPool[k]) {
            ArcsPool.insert(arc);
        }
    }
}

// Print graph
void PrintGraph() {
    cout << NodesPool.size() << endl;
    for (auto i : NodesValue) {
        cout << i.first << " " << i.second << endl;
    }

    cout << ArcsPool.size() << endl;
    for (auto arc : ArcsPool) {
        cout << arc.first << " " << arc.second << endl;
    }

    cout << pSize << endl;
    for (int k = 1; k <= pSize; k++) {
        cout << k << endl;

        cout << subGNodesPool[k].size() << endl;
        bool flag = true;
        for (auto i : subGNodesPool[k]) {
            if (flag) {
                cout << i;
                flag = !flag;
            } else {
                cout << " " << i;
            }
        }
        cout << endl;

        cout << subGTerminalPool[k].size() << endl;
        flag = true;
        for (auto i : subGTerminalPool[k]) {
            if (flag) {
                cout << i;
                flag = !flag;
            } else {
                cout << " " << i;
            }
        }
        cout << endl;
    }
    return;
}

void output(string filename) {
    ofstream flow(filename);

    flow << NodesPool.size() << endl;
    for (auto i : NodesValue) {
        flow << i.first << " " << i.second << endl;
    }

    flow << ArcsPool.size() << endl;
    for (auto arc : ArcsPool) {
        flow << arc.first << " " << arc.second << endl;
    }

    flow << pSize << endl;
    for (int k = 1; k <= pSize; k++) {
        flow << k << endl;

        flow << subGNodesPool[k].size() << endl;
        bool flag = true;
        for (auto i : subGNodesPool[k]) {
            if (flag) {
                flow << i;
                flag = !flag;
            } else {
                flow << " " << i;
            }
        }
        flow << endl;

        flow << subGTerminalPool[k].size() << endl;
        flag = true;
        for (auto i : subGTerminalPool[k]) {
            if (flag) {
                flow << i;
                flag = !flag;
            } else {
                flow << " " << i;
            }
        }
        flow << endl;
    }
    return;
}

int main() {
    srand((int)time(NULL));
    for (int GraphId = 1; GraphId <= GraphNum; GraphId++) {

		CLEAR();

        string filename = _filename + _transform(GraphId) + ".txt";

        // Assign Nodes value
        for (int i = 1; i <= NodesNum; i++) {
            NodesValue[i] = get_random_num(50, 1000);
            NodesPool.insert(i);
        }

        // Build Subgraph
        for (int k = 1; k <= pSize; k++) {
			subGnodenum = (int)(NodesNum * get_random_num(0.3, 0.7));
			subGarcnum = (int)(subGnodenum * (subGnodenum - 1) / 2 * subGarcRatio);
			//cout << subGarcnum << endl;
            _NodesPool = NodesPool;
            NODE root = ReturnSetRandom(_NodesPool);
            bfs(root, k);
            AddArcToSubgraph(k);
        }

        // Merge
        MergeSubG();

        // Print
        // PrintGraph();
        output(filename);
        cout << "graph " << GraphId << " finish" << endl;
    }
    cout << "hello world" << endl;
    return 0;
}