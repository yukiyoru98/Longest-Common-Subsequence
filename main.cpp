#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

//global
vector< vector<int> > lcsLength;//to find Length of LCS
vector< vector<int> > common;//save common order:1~LCS length
set< vector<char> > allLCS;//save all LCS

int length_LCS(vector<char> s1, vector<char> s2, int s1_size, int s2_size){
    //create matrix:lcsLength,common
    //size of lcsLength is bigger than 2 sequence by 1
    vector<int> column(s2_size+1);
    for(int i=0; i<s1_size+1; i++){
        lcsLength.push_back(column);
        common.push_back(column);
    }
    //initialize lcsLength first column to 0;
    for(int i=0; i<s1_size+1; i++){
        lcsLength[i][0] = 0;
    }
    //initialize lcsLength first row to 0;
    for(int i=0; i<s2_size+1; i++){
        lcsLength[0][i] = 0;
    }
    
    //find LCS length and save order of common characters
    for(int i=1; i<=s1_size; i++){
        for(int j=1; j<=s2_size; j++){
            if(s1[i]==s2[j]){
                lcsLength[i][j] = lcsLength[i-1][j-1]+1;
                common[i][j] = lcsLength[i][j];//save order
            }
            else if(lcsLength[i-1][j] > lcsLength[i][j-1]){
                lcsLength[i][j] = lcsLength[i-1][j];
            }
            else{
                lcsLength[i][j] = lcsLength[i][j-1];
            }
        }
    }
    return lcsLength[s1_size][s2_size];//the last element is length of LCS
}

void printLCS(int i, int j,vector<char> s1, int length, vector<char> LCS){
    int order = common[i][j];
    if(order==1){
        LCS.at(1) = s1[i];//put the first common character in LCS
        allLCS.insert(LCS);//use set to avoid repeated LCS
        LCS.clear();//clear for next search
    }
    else{
        for(int x=i-1; x>0; x--){
            for(int y=j-1; y>0; y--){
                if(common[x][y]==order-1){//found the common character before the current one
                    LCS.at(common[x][y]) = s1[x];//put the common character in LCS
                    printLCS(x, y, s1, length, LCS);//find the previous one
                }
            }
        }
    }
}

void printSet(){
    std::set<vector<char>>::iterator it;
    for(it=allLCS.begin(); it!=allLCS.end(); it++){
        for(int i=1; i<(*it).size(); i++){//[0] is blank
            cout << (*it)[i];
        }
        cout << ',';
    }
}


int main(){
    //prompt for filename input
    string filename;
    ifstream read;
    do{
        cout << "Please input filename:";
        getline(cin, filename);
        read.open(filename.c_str(), ios::in);
    }while(!read.is_open());
    
    //read file
    int num=0;//num of data
    int count=1;//datas done
    read >> num;//get num of datas
    
    string s;
    getline(read, s);//read '\n'
    s.clear();
    while(count <= num){
        cout << "\nAnswer " << count << endl;
        
        //initialize strings
        vector<char> s1;
        vector<char> s2;

        //read 2 sequences for each data
        getline(read, s);//first sequence
        std::copy(s.begin(), s.end(), std::back_inserter(s1));
        s.clear();
        
        getline(read, s);//second sequence
        std::copy(s.begin(), s.end(), std::back_inserter(s2));
        
        //put blank at first position
        s1.insert(s1.begin(),' ');
        s2.insert(s2.begin(),' ');
        
        //get length of LCS
        int x=length_LCS(s1, s2, s1.size()-1, s2.size()-1);
        
        //print LCS
        vector<char> LCS(x+1);//to save LCS, first position if blank
        for(int m=s1.size()-1; m>0; m--){
            for(int n=s2.size()-1; n>0; n--){
                if(common[m][n]==x){
                    LCS.at(x) = s1[m];
                    printLCS(m, n, s1, x, LCS);
                }
            }
        }
        cout << "Length of LCS:" << x << endl;
        cout << "LCS:";
        printSet();
        cout << endl;
        
        allLCS.clear();
        common.clear();
        lcsLength.clear();
        
        count++;
    }
    system("pause");
    return 0;
}



