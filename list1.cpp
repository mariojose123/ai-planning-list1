#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <unordered_set>
#include <cstdio>
#include <ctime>
#include <algorithm>
using namespace std;

class Node8
{ 
    public:
    int lenght;
    vector<vector<char>> solution;
};
deque<Node8> prevId;
vector<vector<vector<char>>> setDFS;

class ResultBts{
    public:
    map<string,Node8> prev; 
    Node8 solution;
};
const int cinTerminator = -1;

bool is_goal(Node8 n){
    bool isgoal = true;
    char z='0';
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(n.solution[i][j]!=z){
                isgoal= false;
            }
            z=z+1;
        }
    }
    return isgoal;
}

Node8 InitFirstNode(vector<string> inputArray){ 
   Node8 initialNode;
   char number;
    for(int i=0;i<3;i+=1){
        vector<char> line;
        for(int j=1+3*i;j<4+3*i;j++){
            line.push_back(inputArray[j][0]);
        }
        initialNode.solution.push_back(line);
    }
    initialNode.lenght = 0;
    return initialNode;
}

Node8 InitFirstNodeTest(vector<string> inputArray){ 
   Node8 initialNode;
   char number;
    for(int i=0;i<3;i+=1){
        vector<char> line;
        for(int j=0+3*i;j<3+3*i;j++){
            line.push_back(inputArray[j][0]);
        }
        initialNode.solution.push_back(line);
    }
    initialNode.lenght = 0;
    return initialNode;
}

void print8puzzle(Node8 n){
    for(int i = 0;i<3;i++){
        cout<<"\n";
        for(int j = 0;j<3;j++){
            char a = n.solution[i][j];
            cout<<a;
        }
    }
}

vector<Node8> succ8puzzle(Node8 initial){
    vector<Node8> successors8;
    bool up=false;
    bool down=false;
    bool left=false;
    bool right=false;
    for(int i=0;i<initial.solution.size();i++){
        for(int j=0;j<initial.solution[i].size();j++){
            if(initial.solution[i][j]=='0'){
            if(i==0){
                down = true;
            }
            if(i==1){
                down = true;
                up = true;
            }
            if(i==2){
                up = true;
            }
            if(j==0){
                right = true;
            }
            if(j==1){
                left  = true;
                right = true;
            }
            if(j==2){
                left = true;
            }
            if(left==true){
                vector<vector<char>> newsuccessor = initial.solution;
                newsuccessor[i][j] = initial.solution[i][j-1];
                newsuccessor[i][j-1] = '0';
                Node8 n;
                n.solution = newsuccessor;
                n.lenght = initial.lenght+1;
                successors8.push_back(n);
            }
            if(right==true){
                vector<vector<char>> newsuccessor = initial.solution;
                newsuccessor[i][j] = initial.solution[i][j+1];
                newsuccessor[i][j+1] = '0';
                Node8 n;
                n.solution = newsuccessor;
                n.lenght = initial.lenght+1;
                successors8.push_back(n);
            }
            if(up==true){
                vector<vector<char>> newsuccessor = initial.solution;
                newsuccessor[i][j] = initial.solution[i-1][j];
                newsuccessor[i-1][j] = '0';
                Node8 n;
                n.solution = newsuccessor;
                n.lenght = initial.lenght+1;
                successors8.push_back(n);
            }
            if(down==true){
                vector<vector<char>> newsuccessor = initial.solution;
                newsuccessor[i][j] = initial.solution[i+1][j];
                newsuccessor[i+1][j] = '0';
                Node8 n;
                n.solution = newsuccessor;
                n.lenght = initial.lenght+1;
                successors8.push_back(n);
            }
            }
        }
    }
    return successors8;
}
string matrixToString(vector<vector<char>> matrix){
    string s;
    for(int i = 0; i < 3; i++)
    {
    for(int j = 0; j < 3; j++)
    {
        s += " ";
        s += matrix[i][j];
        s += " ";
    }
}
    return s;
}

ResultBts bfsGraph(Node8 initial){
    deque<Node8> open;
    unordered_set<string> closed;
    ResultBts Result;
    open.push_back(initial);
    closed.insert(matrixToString(initial.solution));
    while(!open.empty()){  
        Node8 n = open.front();
        open.pop_front();
        vector<Node8> succ  = succ8puzzle(n);
    
        for(int i=0;i<succ.size();i++){
          
            if(is_goal(succ[i])){
                Result.solution = succ[i];
                cout<<"\n"<<open.size();
                cout<<"\n"<<closed.size();
                return Result; 
            }
            if(closed.find(matrixToString(succ[i].solution))==closed.end()){
                
                closed.insert(matrixToString(succ[i].solution));
                open.push_back(succ[i]);
            }
        }
    }
    return Result;
}

void printSolution(ResultBts nodeResult){
    cout<<matrixToString(nodeResult.solution.solution);
    Node8 n =  nodeResult.solution;
    map<string,Node8>::iterator it;
    do{
    it = nodeResult.prev.find(matrixToString(n.solution));
    if(it!= nodeResult.prev.end()){
        cout<<matrixToString(it->second.solution);
        n = it->second;
    }
    }while(it!= nodeResult.prev.end());

}

int  depth_limited_search(Node8 prev,Node8 initial,int depth_limit){
    Node8 nodeDFS;
    nodeDFS.solution = setDFS[0];
    if(is_goal(initial)){
        return 0;
    }
    if(depth_limit>0){
        vector<Node8> succ  = succ8puzzle(nodeDFS);
        for(int i=0;i<succ.size();i++){
                bool findonset = false;
                for(int j=0;j<setDFS.size();j++){
                    if(setDFS[j]==succ[i].solution){
                        findonset = true;
                    }
                }
                if(!findonset){
                setDFS.insert(setDFS.begin(),succ[i].solution);
                int solution = depth_limited_search(initial,succ[i],depth_limit-1);
                if(solution>=0){
                    prevId.push_front(succ[i]);
                    return solution+1;
                }
                setDFS.erase(setDFS.begin());
                }
        }
    }
    return -1;
}

int Id(Node8 initial){
    for(int i=1;i<100;i++){
        setDFS.insert(setDFS.begin(),initial.solution);
        prevId.clear();
        int solution = depth_limited_search(initial,initial,i);
        cout<<solution;
        if(solution>=0){
            setDFS.clear();
            return solution;
        }
    }
}

int main() {
   vector<string> inputArray;
   /*string input;
   string alg; 
   int num;
   list<int> l;
   getline (cin,input);
   istringstream ss(input);
   string token;*/
    string x;
    int i=0;



    std::clock_t start;
    double duration;
    start = std::clock();

    while(cin>>x){
        if(i%9==0&&i!=0){
        Node8 initialNode = InitFirstNodeTest(inputArray);
        cout<<matrixToString(initialNode.solution);
        cout<<"\n"<<Id(initialNode);
        inputArray.clear();
        }
        inputArray.push_back(x);
        i++;
    }
    Node8 initialNode = InitFirstNodeTest(inputArray);
    cout<<matrixToString(initialNode.solution);
    cout<<"\n";
    cout<<"\n"<<Id(initialNode);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<'\n'<<"printf: "<< duration <<'\n';
    return 1;
}