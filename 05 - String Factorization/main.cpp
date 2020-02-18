#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <queue>
using namespace std;

int main() {
    string word;
    cin >> word;
    int num = 0;
    cin >> num;
    string *w;
    vector <map <string*, int>> poss;
    vector <map <int, int>> answ;
    map <char, vector<string*>> fLet;
    map <string, int> fPos;
    w = new string[num];

    for (int i = 0; i < num; i++){
        cin >> w[i];
    }

    //Creating dictionary of words
    for (int i = 0; i < num; i++){
        fLet[w[i].at(0)].push_back(&w[i]);
    }

    //printf("\n%d", fLet.size());

    //search routing
    vector <int> res2;
    for (int i = 0; i < word.length(); i++){
        //printf("\ni:%d, poss:%d", i, poss.size());
        bool promoted = false;

        //check if added words still correspond to line
        for (auto it = poss.begin(); it != poss.end(); it++){
            for (auto& x: *it) {
                //printf("\n%s %c = %c?", x.first->c_str(), x.first->at(x.second), word[i]);
                if (x.first->at(x.second) == word[i]){
                    if (x.first->size() == x.second + 1){
                        if (res2.empty()){
                            promoted = true;
                            res2.push_back(i);
                        }
                        else if (res2.back() < (i - (x.first->size()-1))){
                            promoted = true;
                            res2.push_back(i);
                        }
                        goto her1;
                    }
                    else x.second += 1;
                }
                else {
                    it = poss.erase(it);
                    --it;
                    //printf("\n%s removed", x.first->c_str());
                    break;
                }
            }
        }


        //adding new words
        if (fLet[word[i]].size()){
            for(auto it = fLet[word[i]].begin(); it != fLet[word[i]].end(); it++){
                //if (fPos[(*it)->c_str()] <= i) {
                if ((*(&(*it)->at(1)) == word[i+1])){
                    map<string *, int> tmp;
                    tmp[*it] = 1;
                    poss.push_back(tmp);
                }
            }
        }

        her1: //delete rest when promoted
        if (promoted){
            poss.clear();
        }
    }

    printf("%d", res2.size());
    return 0;
}


