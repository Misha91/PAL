#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;


class LD{
    int dist;
    string w1;
    string w2;
    string wl;
    string iw;
    map <string, int> abcabc;
    int lvl;
    int wd;
    map <int, map<int, int>> ld1;
    map <int, map<int, int>> ld2;
    map <int, int> tmpM;
    map <string, int> results;

public:
    LD(string alph, int dist, string w1, string w2);
    void HW4(void);
    int nextPerm(void);
    int calcDist(string wx, string iw,map <int, map<int, int>> &ldx, int lvl);
    void nextPerm2(string arr, int len, int startPosition, string result);

};

LD::LD(string alph1, int dist1, string w11, string w22){
    this->dist = dist1;


    this->w1 = w11;
    this->w2 = w22;

    if (w1.length() >= w2.length()) wl = w1;
    else wl = w2;
    this->wd = wl.length() - dist;

    for (int i = 0; i <= w1.length(); i++) {
        ld1[i][0] = i;
    }
    for (int i = 0; i <= w2.length(); i++) {
        ld2[i][0] = i;
    }

    for (int j = 0; j <= wd; j++) {
        ld1[0][j] = j;
        ld2[0][j] = j;
    }

    lvl = 0;
}

//gathering all permutations
void LD::nextPerm2(string arr, int len, int startPosition, string result) {
    if (len == 0){
        abcabc[result] = 1;
        return;
    }
    for (int i = startPosition; i <= arr.length()-len; i++){
        result[result.length() - len] = arr[i];
        nextPerm2(arr, len-1, i+1, result);
    }
}

//bad next perm funct
int LD::nextPerm() {
    bool doingTill = true;
    int tmpTR = 1;

    while(doingTill){
        int tmp = tmpM.size()-1;

        if (tmpM[tmp] == 0){
            tmpM[tmp] = 1;
            lvl++;
        }
        else{
            tmpM[tmp] = 0;
            bool carry = true;
            tmp--;
            lvl--;
            while(carry && (tmp >= 0)){
                if (tmpM[tmp] != 1){
                    tmpM[tmp] = 1;
                    lvl++;
                    carry = false;
                }
                else{
                    if (tmp != 0){
                        tmpM[tmp] = 0;
                        tmp--;
                        lvl--;
                    }
                    else{
                        goto ret1;
                    }
                }
            }
        }
        //printf("\n");
        //for (int )

        if (lvl == wd){
            doingTill = false;
            //tmpTR = tmp;
        }

        //if (tmpTR > tmp) tmpTR = tmp;
    }


    iw.clear();
    for (int i = 0; i < tmpM.size(); i++){
       if (tmpM[i]){
           iw.push_back(wl[i]);
       }
    }
    return tmpTR;

    ret1:
        return -1;
}


//hw4 routine
void LD::HW4() {

   priority_queue<string> abc;


    //while (nextPerm() != (-1)){
    //   abcabc[iw]=1;
    //}

    string str123;
    str123.resize(wd);
    nextPerm2(wl, wd, 0, str123);
    for (auto it = abcabc.begin(); it != abcabc.end(); it++){
        abc.push(it->first);
    }

    vector <string> abc2;
    while (abc.size()){
        abc2.push_back(abc.top());
        abc.pop();
    }

    //lvl = nextPerm();
    iw = abc2.back();
    int a = calcDist(w1, iw, ld1, 0);
    int b =  calcDist(w2, iw, ld2, 0);
    results[iw]=1;
    abc2.pop_back();
    while((a > dist || b > dist) || abc2.empty()){
        iw = abc2.back();
        abc2.pop_back();
        if (results[iw] != 1){
            a = calcDist(w1, iw, ld1, 0);
            if (a <= dist) b =  calcDist(w2, iw, ld2, 0);
            results[iw] = 1;
        }

   }
    printf("%s", iw.c_str());
}

//Lev distance
int LD::calcDist(string wx, string iwx,map <int, map<int, int>> &ldx, int lvl) {
    int n = wx.length() ;
    int m = iwx.length() ;

    //printf("\n%s : %s", wx.c_str(), iwx.c_str());
    if (lvl == 0){
        for(int i = 1; i <= n; i++ ){
            //printf("\n%d: ", i);
            for(int j = 1; j <= m; j++ ){
                if( wx[i-1] == iwx[j-1] ) ldx[i][j] = ldx[i-1][j-1];
                else ldx[i][j] = 1+ min(min(ldx[i-1][j-1], ldx[i-1][j]), min(ldx[i][j-1],ldx[i-1][j-1] ));
                //printf("%d ", ldx[aInd(i,j)]);
            }
        }
    }

    else {
        for(int i = 1; i <= n; i++ ){
            //printf("\n%d: ", i);
            for(int j = lvl+1; j <= m; j++ ){
                if( wx[i-1] == iwx[j-1] ) ldx[i][j] = ldx[i-1][j-1];
                else ldx[i][j] = 1+ min(min(ldx[i-1][j-1], ldx[i-1][j]), min(ldx[i][j-1],ldx[i-1][j-1] ));
                //printf("%d ", ldx[aInd(i,j)]);
            }
        }
    }
    return ldx[n][m];
}


int main() {
    string alph;
    int dist;
    string w1;
    string w2;
    cin >> alph;
    cin >> dist;
    cin >> w1;
    cin >> w2;
    LD l(alph, dist, w1,w2);
    l.HW4();
    return 0;
}