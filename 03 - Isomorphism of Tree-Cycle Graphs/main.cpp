// HW3.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stack>

//#include <pair>

using namespace std;

class Graph {
    bool toPrint;
    int root;


public:
    unsigned int V;
    list <int> * adj;
    list <int> * adjIn;
    vector <vector <int>> cycles;
    vector <string> cyclesCert;
    priority_queue<int> q;
    vector <int> largest;
    Graph(int V, bool toPrint);
    string cert;
    map <int, string> ind;
    map <int, int> level;
    //priority_queue<int> q;
    //priority_queue<int> qSCC;
    //priority_queue<int> qU;
    stack <int> walk;

    void addEdge(int a, int b);
    void makeCert(bool toPrint);
    void prepare(bool toPrint);
    void decycle(int a);
    void certing(bool toPrint);
    void printCert(int a);
    void cycleCerting(bool toPrint);
    void makeQ(void);
    bool compCerts(Graph gr2);
    bool compNL(Graph gr2);
    string lexOrd(vector <string> vs);
};

Graph::Graph(int V, bool toPrint) {
    this->toPrint = toPrint;
    this->V = V;
    adj = new list <int>[V];
    adjIn = new list <int>[V];


}

string Graph::lexOrd(vector <string> vs) {

    string toRet;

    while (vs.size()) {
        //string tmp = vs.front();
        auto tmp = vs.begin();
        for (auto it = vs.begin() + 1; it != vs.end(); it++) {
            //printf("\n%s < %s?", it->c_str(), tmp->c_str());
            if (lexicographical_compare(it->begin(), it->end(), tmp->begin(), tmp->end()  )) {
                tmp = it;
                //printf("\n%s < %s", it->c_str(), tmp->c_str());
            }
        }
        toRet += *tmp;
        //printf("\ns now: %s", toRet.c_str());
        vs.erase(tmp);
    }
    return toRet;
}



void Graph::addEdge(int a, int b) {
    this->adj[a].push_back(b);
    this->adjIn[b].push_back(a);
    if (false) printf("\n%d, %d added\n", a + 1, b + 1);
}

void Graph::makeCert(bool toPrint) {
    for (int i = 0; i < V; i++) {
        if (adjIn[i].size() == 0) {
            root = i;
            break;
        }
    }
    for (int i = 0; i < V; i++) {
        ind[i] = "01";
    }

    if (toPrint)  {
        printf("\n%d R\n", root + 1);
        for (int i = 0; i < V; i++) {
            printf("\nInitially node %d goes to ", i + 1);
            for (auto it = adj[i].begin(); it != adj[i].end(); it++) {
                printf("%d ", *it + 1);
            }
        }
    }


    if (toPrint){
        for (int i = 0; i < V; i++) {
            printf("\nand now node %d goes to ", i + 1);
            for (auto it = adj[i].begin(); it != adj[i].end(); it++) {
                printf("%d ", *it + 1);
            }
        }
    }

    for (int i = 0; i < V; i++) {
        if (adjIn[i].size() == 2) decycle(i);
    }

    for (int i = 0; i < V; i++) {
        if (adjIn[i].size() >= 2) printf("\nHere's size %d", adjIn[i].size());
    }
    prepare(toPrint);
    //printf("\nWe are done :)");
    certing(toPrint);
}

void Graph::prepare(bool toPrint) {
    walk.push(root);
    queue <int> tmpQ;
    tmpQ.push(root);
    level[root] = 0;
    int lvl = 1;
    int toGo = 1;
    int toGoNext = 0;
    int toGoNow = 0;
    //lvl++;
    //bool first = true;

    while (tmpQ.size()) {

        int tmp = tmpQ.front();
        tmpQ.pop();
        if (toGo == 0) {
            toGo = toGoNext;
            toGoNext = 0;
            lvl++;
        }

        if (adjIn[tmp].size() >= 2) printf("\n2+Par"); //decycle(tmp);

        for (auto it = adj[tmp].begin(); it != adj[tmp].end(); it++) {
            tmpQ.push(*it);
            walk.push(*it);
            toGoNext++;
            //toGoNext += adj[*it].size();
            level[*it] = lvl;
        }

        toGo--;
    }
}

void Graph::makeQ(void) {
    int temp = 0;
    for (int i = 0; i < V; i++) {
        if (adj[i].size()) {
            q.push(adj[i].size());

            if (adj[i].size() > temp) {
                largest.clear();
                temp = adj[i].size();
                largest.push_back(i);
            }

            else if (adj[i].size() == temp) {
                largest.push_back(i);
            }
        }
    }

    if (false) {
        printf("\n");
        while (!(q.empty())) {
            printf("%d, ", q.top());
            q.pop();
        }
        printf("\n");
    }
}

bool Graph::compNL(Graph gr2) {
    if (q.size() != gr2.q.size()) return false;
    priority_queue<int> q1 = q;
    priority_queue<int> q2 = gr2.q;

    while (!(q1.empty())) {
        if (q1.top() == q2.top()) {
            q1.pop();
            q2.pop();
        }
        else return false;
    }


    return true;
}

void Graph::decycle(int a) {
    queue <vector <int>> cycP;
    vector <int> tmp;
    tmp.push_back(a);
    cycP.push(tmp);
    vector <int> tmpCyc;
    map <int, int> visited;

    while (cycP.size()) {
        tmp = cycP.front();
        cycP.pop();
        //int last = tmp.back();
        if (tmp.size() > 1 && (tmp.back() == a)) {
            auto abc = tmp.begin();
            char c[5];
            sprintf(c, "0%d", tmp.size() - 1);
            for (int i = 0; i < tmp.size() - 1; i++) {
                auto tmpI = abc + 1;
                adj[*abc].remove(*tmpI);
                adjIn[*tmpI].remove(*abc);
                ind[*tmpI] = c;
                abc = tmpI;
                tmpCyc.push_back(*abc);
            }
            cycles.push_back(tmpCyc);
            return;
        }
        for (auto it = adj[tmp.back()].begin(); it != adj[tmp.back()].end(); it++) {
            if ((visited[*it] == 0) && ( adjIn[*it].size() == 2)) {
                tmp.push_back(*it);
                cycP.push(tmp);
                tmp.pop_back();
                visited[*it] = 1;
            }

        }
    }

    printf("\nError of decycling");
}

void Graph::certing(bool toPrint) {
    vector <int> tmp;
    //vector <int> tmp2;
    vector <string> tmpS;
    while (walk.size() > 1) {
        tmp.clear();
        tmpS.clear();
        int p;
        tmp.push_back(walk.top());
        walk.pop();
        if (tmp.front() != root) {
            if (adjIn[tmp.front()].size() > 1) printf("\nParrent err");
            p = adjIn[tmp.front()].front();
            if (toPrint) printf("\nparrent size of %d is %d", tmp.front() + 1, adjIn[tmp.front()].size());

            while (adjIn[walk.top()].size()) {
                if (adjIn[walk.top()].front() == p && (walk.top() != root)) {
                    tmp.push_back(walk.top());
                    walk.pop();
                }
                else break;
            }

            if (adjIn[tmp.back()].size() == 0) printf("\n0P - %d, R:%d", walk.top()+1, root+1);
            //tmp2 = tmp;
            if (toPrint) {
                for (auto it = tmp.begin(); it != tmp.end(); it++) {
                    if (adj[*it].size()) printf("\nERROR 0 with %d!", *it + 1);
                    else if (adjIn[tmp.front()] != adjIn[*it]) printf("\nERROR 1 with %d!", *it + 1);
                    else if (adjIn[tmp.front()].size() > 1) printf("\nERROR 2with %d!", *it + 1);

                }



                if (adj[p].size() != tmp.size()) printf("\nPARRENT ERROR (%d)", p + 1);
                printf("\nnow cons:");
                for (auto it = tmp.begin(); it != tmp.end(); it++) {
                    printf(" %d", *it + 1);
                }
            }


            while (tmp.size()) {
                tmpS.push_back(ind[tmp.back()]);
                tmp.pop_back();
            }

            //string tmpStr;
            string toStr;
            //string tmpPS;

            //char *stra = new char[ind[p].length() + 1];
            vector<char> stra(ind[p].c_str(), ind[p].c_str() + ind[p].size() + 1);
            //if (ind[p].size() > 2) {
            //	printf("\n2+ind");
            //	tmpPS = ind[p];
            //	tmpPS.erase(tmpPS.begin());
            //	tmpPS.erase(tmpPS.end()-1);
            //	tmpS.push(tmpPS);
            //}
            //stack <string> ss;
            //while (tmpS.size()) {
            //	ss.push(tmpS.top());
            //	tmpS.pop();
            //}

            //while (ss.size()) {
            //	tmpStr += ss.top();
            //	ss.pop();
            //}
            toStr = stra[0] + lexOrd(tmpS) + stra[1];
            ind[p] = toStr;
            //printf("\nP%d Cert %s\n", p+1, ind[p].c_str());
            //adj[p].clear();
        }
        else break;
    }
    cert = ind[root];
    //printCert(root);
    cycleCerting(toPrint);
}

void Graph::printCert(int a) {
    printf("\nCert %s\n", ind[a].c_str());
}

void Graph::cycleCerting(bool toPrint) {
    for (auto it = cycles.begin(); it != cycles.end(); it++) {
        //printf("\nTree %d, Cycle #%d", i, a);
        int a=0;
        vector <string> tmp;
        priority_queue <int> tmp1;
        string str;
        for (auto it2 = it->begin(); it2 != it->end(); it2++) {

            char tmpS1[10];
            char tmpS2[10];
            char tmpS3[10];
            char tmpS4[10];
            sprintf(tmpS1, "%dx", level[*it2]);
            sprintf(tmpS2, "%dy", adj[*it2].size());
            sprintf(tmpS3, "%dz", adjIn[*it2].size());
            //sprintf_s(tmpS4, "%d"
            str += ind[adjIn[*it2].front()];
            str += tmpS1;
            str += tmpS2;
            str += tmpS3;
            str += ind[*it2];
            tmp.push_back(str);
            //tmp1.push(level[*it2]);
            //tmp1.push(adj[*it2].size());
            //printCert(*it2);
            //printf("L: %d", graphs[i]->level[*it2]);
            a += level[*it2];
            str.clear();
        }
        str.clear();
        char tmpS4[10];
        sprintf(tmpS4, "%dE", a);

        int id = 0;
        int ai = 1;
        auto tmp123 = tmp.begin();
        for (auto it34 = tmp.begin() + 1; it34 != tmp.end(); it34++) {
            //printf("\n%s < %s?", it->c_str(), tmp->c_str());
            if (lexicographical_compare(it34->begin(), it34->end(), tmp123->begin(), tmp123->end())) {
                tmp123 = it34;
                id = ai;
                //printf("\n%s < %s", it->c_str(), tmp->c_str());
            }
            ai++;
        }
        //str += tmp[id];
        for (int i = id; i < tmp.size() ; i++) {
            str += tmp[i];
        }

        for (int i = 0; i != id; i++) {
            str += tmp[i];
        }



        //str = lexOrd(tmp) + tmpS4;
        //while (tmp.size()) {
        //	char tmpS5[10];
        //	sprintf_s(tmpS5, "%d", tmp.top());
        //	str += tmp.top();
        //	tmp.pop();
        //	}

        //str += tmpS4;
        //printf("\nCycle Cert %s\n", str.c_str());
        cyclesCert.push_back(str);

    }
}

bool Graph::compCerts(Graph gr2) {
    if (cyclesCert.size() != gr2.cyclesCert.size()) return false;
    //return true;
    vector <string> tmp1 = cyclesCert;
    vector <string> tmp2 = gr2.cyclesCert;

    for (auto it2 = tmp1.begin(); it2 != tmp1.end(); it2++){
        for (auto it = tmp2.begin(); it != tmp2.end(); it++) {
            //printf("\n%s %s ", it2->c_str(), it->c_str());
            if (*it == *it2) {
                //tmp1.pop_back();
                //printf("y");
                tmp2.erase(it);
                //printf("\none popped!");
                break;
            }
            //else printf("n");//return false;
        }
        //if (tmp2.size()) return false;
    }

    if (tmp2.empty()) return true;
    else {
        //printf("\nS:%d, I:%d", tmp2.size(), tmp1.size());
        //printf("\n%s \n%s ", tmp2.begin()->c_str(), tmp1.begin()->c_str());
        return false;
    }
}

int main() {

    priority_queue <int> answ;
    int *isoGraph;
    int *isoF;

    int T, N, M;
    cin >> T;
    cin >> N;
    cin >> M;



    Graph** graphs = new Graph*[T];
    for (int i = 0; i < T; i++)
    {
        graphs[i] = new Graph(N, true);
    }

    for (int i = 0; i < T; i++) {
        int a = 0;
        int b = 0;
        for (int c = 0; c < M; c++) {
            cin >> a;
            cin >> b;
            graphs[i]->addEdge(a - 1, b - 1);
        }

        //printf("\n%d", i);
        //graphs[i]->makeQ();
        //graphs[i]->SCC(false);
        //graphs[i]->Map(false);
    }

    for (int i = 0; i < T; i++) {
        graphs[i]->makeCert(false);
        int a = 0;




    }

    isoGraph = new int[T];
    isoF = new int[T];
    for (int i = 0; i < T; i++) {
        isoGraph[i] = 1;
        isoF[i] = 0;
    }

    for (int x = 0; x < T; x++) {
        //printf("\n%d", graphs[x]->unvisited.size());
        if (isoF[x] == 0) {
            for (int y = x + 1; y < T; y++) {
                //printf("\nG2 is %d", y);
                if (isoF[y] == 0) {
                    if (graphs[x]->cert == graphs[y]->cert) {
                        if (graphs[x]->compNL(*graphs[y])){
                            if (graphs[x]->compCerts(*graphs[y])) {
                                //if (graphs[x]->deepCheck(*graphs[y])) {
                                isoGraph[x] += 1;
                                isoGraph[y] = 0;
                                isoF[y] = 1;
                            }
                        }
                    }
                }
            }
        }

    }

    for (int i = 0; i < T; i++) {
        if (isoGraph[i]) answ.push(isoGraph[i]);

    }


    list <int> tmp;
    while (!(answ.empty())) {
        tmp.push_front(answ.top());
        answ.pop();
    }
    auto it = tmp.begin();
    for (int a = 0; a < tmp.size() - 1; a++) {
        printf("%d ", *it);
        it++;
    }
    printf("%d", tmp.back());
    delete[] graphs;
    //printf("\nHi!");
   // while (1);
    return 0;
}
