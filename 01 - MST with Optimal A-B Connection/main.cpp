//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <string>
#include <map>
#define edge pair <int, int>
using namespace std;
//double duration;
//double krusk = 0;
int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;

class Graph {
    vector<pair<int, edge>> G; // graph
    vector<pair<int, edge>> T; // mst
    map <string, int> wTable;
    //int *parent;
    int *boss;
    int *rank;
    int *visited;
    int V;
    int M;
    list <int> * adj;

public:
    Graph(int V, int M);
    void addEdge();
    void BFS(int start, int end, bool toPrint);
    void print();
    int find_set(int i);
    void union_set(int u, int v);
    int kruskal(vector<int> bestPath);
    int pathWeight(vector <int> path);
    vector<int> bestPath;
    vector<int> shortestPath;

};

Graph::Graph(int V, int M) {
    this->V = V;
    this->M = M;
    adj = new list<int>[V];
    //parent = new int[V];
    boss = new int[V];
    rank = new int[V];
    visited = new int[V];
    G.clear();
    //wTable.resize(V);
    for (int i = 0; i<V; i++) {
        //wTable[i].resize(V);
        visited[i] = 0;
    }

}

void Graph::addEdge() {
    int a(0), b(0), w(0);
    for (int i = 0; i < M; i++) {
        std::cin >> a;
        std::cin >> b;
        std::cin >> w;
        --a;
        --b;
        this->adj[a].push_back(b);
        this->adj[b].push_back(a);
        G.push_back(make_pair(w, edge(a, b)));
        char abc[20];
        sprintf(abc, "%d,%d", a, b);
        wTable[abc] = w;
        sprintf(abc, "%d,%d", b, a);
        wTable[abc] = w;
    }
    sort(G.begin(), G.end());
}

void Graph::print() {

    for (int i = 0; i < V; i++) {
        printf("\ni=%d:", i + 1);
        for (auto it = adj[i].begin(); it != adj[i].end(); it++) {
            printf(" %d", *it + 1);

        }
    }
}
int Graph::pathWeight(vector <int> path) {
    int wMST = 0;
    int prev = 0;
    bool firstIt = false;
    for (auto it = path.begin(); it != path.end(); it++) {
        if (firstIt) {
            char abc[20];
            sprintf(abc, "%d,%d", prev, *it);
            wMST += wTable[abc];

        }
        prev = *it;
        firstIt = true;
    }

    return wMST;
}
void Graph::BFS(int start, int end, bool toPrint) {
    //clock_t start2;
    //start2 = clock();

    vector<vector<int>> pathChecker;
    pathChecker.resize(V);
    vector<int> initPath;
    vector<vector<int>> pathQueue;
    vector<vector<int>> pathQueue2;
    initPath.push_back(start);
    pathQueue.push_back(initPath);
    int currentL = 0;
    visited[start] = 1;
    vector <int> tempVisited;
    unsigned int shortest = 0;
    int lowest = 0;
    bool isTheFirst = true;
    int tempC = 0;

    while (true) {
        //Get and remove oldest element in pathQueue


        //Add visited nodes to the visited[] at new lenght level
        if (pathQueue.size() == 0) {
            //currentL = tmpPath.size();
            if (toPrint) {
                printf("L is %d\n", currentL);
                printf("%d\n", tempVisited.size());
            }
            while (pathQueue2.size()) {
                pathQueue.push_back(pathQueue2.back());
                pathQueue2.pop_back();
            }
            while (tempVisited.size()) {

                if (tempVisited.back() != end) {
                    if (toPrint) printf("%d visited!\n", tempVisited.back() + 1);
                    visited[tempVisited.back()] = 1;

                    if (!(pathChecker[tempVisited.back()].empty())) {
                        pathQueue.push_back(pathChecker[tempVisited.back()]);
                        pathChecker[tempVisited.back()].clear();
                        //printf("one added\n");
                    }
                    tempVisited.pop_back();
                }
                else tempVisited.pop_back();
                //c1++;
            }



        }

        if (pathQueue.size() == 0) break;

        vector<int> tmpPath;
        tmpPath = pathQueue.front();
        pathQueue.erase(pathQueue.begin());
        int tmpMST = 0;


        if (toPrint) {
            printf("Lenght %d\n", tmpPath.size());
            printf("The path is: ");
            for (auto it = tmpPath.begin(); it != tmpPath.end(); it++) {
                printf("%d, ", *it + 1);
            }
            printf("\n");
            printf("Visited are: ");
            for (int i = 0; i < V; i++) {
                printf("%d, ", visited[i]);
            }
            printf("\n");
        }



        //checking if we are at end


        if (tmpPath.back() == end) {
            //if (toPrint) printf("The path has been found!\n");

            if (isTheFirst) {
                //if (toPrint) printf("The first tested\n");
                shortest = tmpPath.size();

                tmpMST = kruskal(tmpPath);
                //if (toPrint) printf("Result is %d\n", tmpMST);
                lowest = tmpMST;
                isTheFirst = false;
            }
            else {
                tmpMST = kruskal(tmpPath);
                //if (toPrint) printf("Result is %d\n", tmpMST);
                if (lowest > tmpMST) {
                    lowest = tmpMST;
                }
            }



        }
        //adding new paths
        if (isTheFirst) {
            for (auto it = adj[tmpPath.back()].begin(); it != adj[tmpPath.back()].end(); it++) {
                //c2++;
                //int a = *it;

                if (!(visited[*it])) {
                    if (toPrint) printf("Considering: %d\n", *it + 1);
                    tmpPath.push_back(*it);

                    if (*it != end) {
                        bool testBool = true;
                        if (pathChecker[*it].empty()) {
                            //printf("pushed new\n");
                            pathChecker[*it] = tmpPath;
                            tempVisited.push_back(*it);
                            //c3++;
                        }


                        else {
                            int ww1, ww2;
                            ww1 = pathWeight(tmpPath);
                            ww2 = pathWeight(pathChecker[*it]);
                            //printf("W1 %d, w2 %d\n", ww1, ww2);
                            if (ww1 < ww2) {
                                if (ww1 > ww2*0.95) pathQueue2.push_back(tmpPath);
                                pathChecker[*it] = tmpPath;
                                //printf("erased\n");

                            }
                            if (0.95*ww1 < ww2) {
                                pathQueue2.push_back(tmpPath);
                                //printf("equal added!\n");

                            }
                            //c4++;
                        }


                    }



                    if (*it == end) {
                        pathQueue.push_back(tmpPath);
                    }
                    tmpPath.pop_back();
                }



            }

        }

    }

    //print results
    printf("%d ", shortest - 1);
    printf("%d ", lowest);
    //printf("%d ", tempC);
    //duration = (clock() - start) / (double)CLOCKS_PER_SEC;
}

int Graph::find_set(int i) {
    int parent = boss[i];
    if (parent != i)
        boss[i] = find_set(parent); // path compression
    return boss[i];

}

void Graph::union_set(int u, int v) {
    if (rank[v] > rank[u])
        boss[u] = v;
    else {
        boss[v] = u;
        if (rank[v] == rank[u])   // change rank?
            rank[u]++;
    }
}

int Graph::kruskal(vector<int> bestPath) {
    //double tempTime = duration;
    //clock_t start;
    //start = clock();
    //duration = 0;
    T.clear();
    for (int i = 0; i < V; i++) {
        boss[i] = i;   // everybody's their own boss
        rank[i] = 0;   // necessary? yes
    }
    int uRep, vRep;
    int prev = 0;
    bool firstIt = false;
    int wMST = 0;
    int counter = 0;

    //add to MST our shortest path
    for (auto it = bestPath.begin(); it != bestPath.end(); it++) {
        if (firstIt) {
            char abc[20];
            sprintf(abc, "%d,%d", prev, *it);
            wMST += wTable[abc];
            counter++;
            union_set(prev, *it);
        }
        prev = *it;
        firstIt = true;
    }
    // increasing weight
    for (unsigned int i = 0; i < G.size(); i++) {
        uRep = find_set(G[i].second.first);
        vRep = find_set(G[i].second.second);
        if (uRep != vRep) {
            wMST += G[i].first;
            counter++;
            union_set(uRep, vRep);
        }
        if (counter == V - 1) break;
    }

    //duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    //krusk += duration;
    //duration = tempTime;
    return wMST;
}


int main()
{
    //clock_t start;
    //start = clock();
    int n, m, a, b;
    cin >> n;        // Read in # of verts
    cin >> m;        // Read in # of edges
    cin >> a;        // Read in start a
    cin >> b;        // Read in end b

    Graph g(n, m);
    g.addEdge();
    //duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    //printf("%f init\n", duration);
    //duration = 0;


    g.BFS(a - 1, b - 1, false);
    //printf("%d, %d, %d, %d", c1, c2, c3, c4);
    //printf("%f BFS, %f krus\n", duration, krusk);

    //while (1);
    return 0;
}