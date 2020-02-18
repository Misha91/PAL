
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <list>
#include <string>


using namespace std;

class Graph {
    unsigned int V;
    list <int> * adj;
    list <int> * adjIn;
    list <int> * adjMap;
    int *home;

    int *indexSCC;
    int *lowlink;
    int *pred;
    bool *instack;

    map <string, int> wTable;
    int index;
    int scc;
    int S;
    map <int, vector <int>> SCCs;
    map <int, int> blueMap;

    stack <int> blue;
    set <int> unvisited;
    vector <vector <int>> possPaths;
    int XXL = 0;

public:
    Graph(int V);
    void addEdge(int a, int b);
    void addBlue(int bn);
    void HW2(bool toPrint);
    void SSCCreate(int i, bool toPrint);
    void SCC(bool toPrint);
    void Map(bool toPrint);
    void FindLongest();
    void DFS(vector <int> tmpPath, map <int, int> vis, int a);
    int BFS(int start, int end, map <int, int> vis);
    void BFS2(int start, bool toPrint);
    int BFSUtil(vector <int> tmpPath, map <int, int> vis);

};

Graph::Graph(int V) {
    this->V = V;

    adj = new list <int>[V];
    adjIn = new list <int>[V];
    //visited = new int[V];
    home = new int[V];
    indexSCC = new int[V];
    lowlink = new int[V];
    pred = new int[V];
    instack = new bool[V];
    index = 0;
    scc = 0;
    S = 0;


    for (int i = 0; i < this->V; i++) {
        indexSCC[i] = 0;
        //visited[i] = 0;
    }
}

void Graph::addBlue(int bn) {
    blue.push(bn);
}

void Graph::addEdge(int a, int b) {
    //printf("\n%d, %d", a, b);
    this->adj[a].push_back(b);
    this->adjIn[b].push_back(a);

}

void Graph::HW2(bool toPrint) {

    SCC(toPrint);
    Map(toPrint);
    FindLongest();

}

void Graph::SSCCreate(int i, bool toPrint) {

    if (toPrint) printf("\nConsidering %d", i);
    indexSCC[i] = index + 1;
    lowlink[i] = index + 1;
    pred[i] = S;
    instack[i] = true;
    S = i;
    index++;
    if (toPrint) printf("\nConsidering %d, S is %d", i, S);
    for (auto it = adj[i].begin(); it != adj[i].end(); it++) {
        if (indexSCC[*it] == 0) {
            SSCCreate(*it, toPrint);
            lowlink[i] = min(lowlink[i], lowlink[*it]);
        }

        else if (instack[*it]) {
            lowlink[i] = min(lowlink[i], lowlink[*it]);
        }

    }

    if (lowlink[i] == indexSCC[i]) {
        if (toPrint) printf("\nSCC found! i is %d, Its nodes are:", i);
        vector <int> tmp;
        int x;
        do {
            tmp.push_back(S);
            x = S;
            if (toPrint) printf("%d, ", x);
            S = pred[S];
            pred[x] = -1;
            instack[x] = false;
            home[x] = scc;
        } while (x != i);
        SCCs.insert(pair <int, vector<int>>(scc, tmp));
        scc++;
    }
}

void Graph::SCC(bool toPrint) {
    //printf("\nSCC has started!");
    for (int i = 0; i< V; i++) {
        if (indexSCC[i] == 0) SSCCreate(i, toPrint);
    }
    //printf("\nSCC is over!");
}

void Graph::Map(bool toPrint) {
    //printf("\nMapping!");
    set <int> connections;
    adjMap = new list <int>[SCCs.size()];

    for (int i = 0; i<SCCs.size(); i++) {
        unvisited.insert(i);
        blueMap[home[blue.top()]] = blue.top();
        blue.pop();
    }
    for (auto it1 = SCCs.begin(); it1 != SCCs.end(); it1++) {
        for (auto it = it1->second.begin(); it != it1->second.end(); it++) {
            if (toPrint) printf("\nConsidering SCC %d, node %d", it1->first, *it);
            for (auto it3 = adj[*it].begin(); it3 != adj[*it].end(); it3++) {
                connections.insert(home[*it3]);
            }
        }

        connections.erase(it1->first);
        for (auto it3 = connections.begin(); it3 != connections.end(); it3++) {
            adjMap[it1->first].push_back(*it3);
            unvisited.erase(*it3);
        }
        connections.clear();
    }

    if (toPrint) {
        for (int i = 0; i < SCCs.size(); i++) {
            printf("\nSSC #%d nodes:", i);
            for (auto it3 = adjMap[i].begin(); it3 != adjMap[i].end(); it3++) {
                printf("%d,", *it3);
            }
        }
    }

}

void Graph::FindLongest() {

    vector<int> tmpPath;
    map <int, int> vis;

    for (auto it = unvisited.begin(); it != unvisited.end(); it++) {
        BFS2(*it, false);
    }

    int shortest = 0;
    vis.clear();

    while (possPaths.size()) {
        if (possPaths.back().size() == XXL) {
            tmpPath = possPaths.back();
            if (possPaths.size())  possPaths.pop_back();
            int temp = 0;

            temp = BFSUtil(tmpPath, vis);

            if (!(shortest)) {
                shortest = temp;
            }
            else if (temp < shortest) shortest = temp;
        }
        else possPaths.pop_back();
    }



    printf("%d %d", XXL, shortest);

}

int Graph::BFSUtil(vector <int> tmpPath, map <int, int> vis) {
    int temp = 0;
    for (auto it = tmpPath.begin(); it != tmpPath.end() - 1; it++) {
        char abc[20];
        sprintf(abc, "%d,%d", *it, *(it + 1));
        //std::string abc = abc1;
        if (wTable[abc]) { temp += wTable[abc]; }
        else {
            wTable[abc] = BFS(blueMap[*it], blueMap[*(it + 1)], vis);
            temp += wTable[abc];
        }
    }
    return temp;
}

void Graph::DFS(vector<int> tmpPath, map <int, int> vis, int a) {
    tmpPath.push_back(a);
    vis[a] = 1;
    bool ifAdded = false;


    for (auto it = adjMap[a].begin(); it != adjMap[a].end(); it++) {
        if (vis[*it] != 1) {
            ifAdded = true;
            DFS(tmpPath, vis, *it);
        }
    }

    if (!(ifAdded)) {
        if (!(XXL)) {
            XXL = tmpPath.size();
            possPaths.push_back(tmpPath);
        }
        else if (tmpPath.size() == XXL) {
            bool to = true;
            for (auto it = possPaths.begin(); it != possPaths.end() - 1; it++) {
                if (tmpPath == *it) {
                    to = false;
                    printf("\nCopy!");
                }
            }
            if (to) possPaths.push_back(tmpPath);
            else if (tmpPath.size() > XXL) {
                XXL = tmpPath.size();
                bool to = true;
                for (auto it = possPaths.begin(); it != possPaths.end() - 1; it++) {
                    if (tmpPath == *it) {
                        to = false;
                        printf("\nCopy!");
                    }
                }
                if (to) possPaths.push_back(tmpPath);
            }
            printf("\nDFS has %d path(s), XXL is %d\n", possPaths.size(), XXL);
            if (false) {
            }
        }

    }
}

int Graph::BFS(int start, int end, map <int, int> vis) {
    ;
    vector <int> tmpPath;
    vector <int> tmpPath2;
    int lenght = 1;
    bool found = false;
    vis[start] = 1;
    tmpPath.push_back(start);


    while (!(found)) {

        if (tmpPath.size() == 0) {
            tmpPath = tmpPath2;
            tmpPath2.clear();
            lenght++;
        }

        for (auto it = adj[tmpPath.front()].begin(); it != adj[tmpPath.front()].end(); it++) {
            if (*it == end) return lenght;
            else if (vis[*it] != 1) {
                vis[*it] = 1;
                tmpPath2.push_back(*it);
            }
        }
        tmpPath.erase(tmpPath.begin());
    }
}

void Graph::BFS2(int start, bool toPrint) {
    vector<vector<int>> pathChecker;
    pathChecker.resize(V);
    vector<int> initPath;
    vector<vector<int>> pathQueue;
    initPath.push_back(start);
    pathQueue.push_back(initPath);
    vector <int> tempVisited;
    bool levelUP = false;
    map <int, int> vis;
    XXL = 1;

    while (true) {
        //Get and remove oldest element in pathQueue


        //Add visited nodes to the visited[] at new lenght level
        if (pathQueue.size() == 0) {

            XXL++;
            levelUP = true;

            while (tempVisited.size()) {
                if (!(pathChecker[tempVisited.back()].empty())) {
                    pathQueue.push_back(pathChecker[tempVisited.back()]);
                }
                tempVisited.pop_back();
            }
        }

        if (pathQueue.size() == 0) break;

        vector<int> tmpPath;
        tmpPath = pathQueue.front();
        pathQueue.erase(pathQueue.begin());


        for (auto it = adjMap[tmpPath.back()].begin(); it != adjMap[tmpPath.back()].end(); it++) {
            //printf("From %d, Considering %d\n", tmpPath.back(), *it);
            bool canGo = true;

            for (auto it2 =tmpPath.crbegin(); it2 != tmpPath.crend(); ++it2){
                if (*it2 == *it){
                    canGo = false;
                    break;
                }
            }

            if (canGo) {
                if (toPrint) printf("Considering: %d\n", *it + 1);
                if (levelUP) {
                    pathChecker.clear();
                    pathChecker.resize(V);
                    levelUP = false;
                }

                tmpPath.push_back(*it);

                if (pathChecker[*it].empty()) {
                    pathChecker[*it] = tmpPath;
                    tempVisited.push_back(*it);
                }


                else {
                    int ww1, ww2;
                    ww1 = BFSUtil(tmpPath, vis);
                    ww2 = BFSUtil(pathChecker[*it], vis);
                    // printf("WTMP %d, wpC %d\n", ww1, ww2);
                    if (ww1 < ww2) {
                        //if (ww1 > ww2*0.95) pathQueue2.push_back(tmpPath);
                        //printf("added %d\n", *it);
                        pathChecker[*it] = tmpPath;
                        //printf("erased\n");

                    }
                }
                tmpPath.pop_back();
            }
        }
    }

    while (pathChecker.size()) {

        if (pathChecker.back().size()) {
            if (toPrint) {
                printf("\nAdding path: ");
                for (auto it = pathChecker.back().begin(); it != pathChecker.back().end(); it++) {
                    printf("%d, ", *it + 1);
                }
                printf("\n");
            }
            possPaths.push_back(pathChecker.back());
        }
        pathChecker.pop_back();
    }
    XXL--;

}

int main() {


    int v, b, m;
    cin >> v;
    cin >> b;
    cin >> m;

    Graph g(v);

    for (int i = 0; i < b; i++) {
        cin >> v;
        g.addBlue(v);
    }

    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> b;
        g.addEdge(v, b);
    }

    g.HW2(false);

    return 0;
}