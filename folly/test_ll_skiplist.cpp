#include <folly/ConcurrentSkipList.h>
#include<iostream>
//#include<folly/our_utils/CycleTimer.h>
#include <fstream>
#include<string>
#include <thread>


#define NUM_THREADS 12
#define DEBUG 0
#define MAX_LINES 10000
static const int kInitHeadHeight = 10;
//static const int kMaxValue = 0x1000000;

using  std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

namespace {
using namespace folly;

typedef int ValueType;
typedef ConcurrentSkipList<ValueType> SkipListType;
typedef SkipListType::Accessor skipListAccessor;

typedef struct {
string filename;
int thread_id;
}threadArgs;

    void checkAdd(){
        int size = 10;
        auto skipList = SkipListType::create(kInitHeadHeight);
        for (int i = 0; i < size; ++i) {
            skipList.insert(i);
            skipList.insert(3*i);
        }
        auto iter = skipList.begin();

        for(;iter!=skipList.end();iter++)
            cout<<*iter<<" ";
        
        cout<<endl;
        skipList.erase(37);
        iter = skipList.begin();
        for(;iter!=skipList.end();iter++)
            cout<<*iter<<" ";
        cout<<endl;

        auto node =skipList.find(6);
        if(node==skipList.end())
            cout<<"Not found"<<endl;
        else
        {
            cout<<*node;
        }

        node =skipList.find(7);
        if(node==skipList.end())
            cout<<"Not found"<<endl;
        else
        {
            cout<<*node;
        }

    }

    void processTraceChunk(vector<int> &ops,vector<int> &values,skipListAccessor &skipList){
        int n = ops.size();
        
        for(int i = 0;i<n;i++){
            
            int op= ops[i];
            int value =values[i];

            if (op==0){
                auto node = skipList.find(value);
                if(DEBUG){
                    if(node==skipList.end()){
                        cout<<"not found"<<endl;
                    }
                    else{
                        cout<<"Found"<<endl;
                    }
                }
            }
            else if (op==1)
                skipList.insert(value);
                
            else 
                skipList.erase(value);
        }
    }

    void* readFileAndProcess(void* argsptr){
        //Each thread should do the below
        std::ifstream fin;
        threadArgs args = *(threadArgs*) argsptr;
        string filename = args.filename;
        int thread_id = args.thread_id;
        fin.open(filename);
        string line;
        int chunk_size = MAX_LINES/NUM_THREADS;
        int start = thread_id * chunk_size;
        vector<int> ops;
        vector<int> values;
        int a, b;
        while(fin>>a>>b){
            //cout<<a<<b<<endl;
            ops.push_back(a);
            values.push_back(b);        
        }
        auto skipList = SkipListType::create(kInitHeadHeight);
        processTraceChunk(ops,values,skipList);
        cout<<"Thread"<<endl;
        return NULL;
    }
    void processTrace(string filename){
        auto skipList = SkipListType::create(kInitHeadHeight);
        void* filenameptr = &filename;
        pthread_t thread[NUM_THREADS];
        threadArgs args[NUM_THREADS];
        //call function from each thread
        for (int i=1; i<NUM_THREADS; i++){
            args[i].filename = filename+ std::to_string(i)+".txt";
            args[i].thread_id = i;
            pthread_create(&thread[i], NULL, readFileAndProcess,(void*)&args[i]);
        }
        //Parent thread setup Args and call
        args[0].filename = filename+ std::to_string(0)+".txt";
        args[0].thread_id = 0;
        readFileAndProcess((void*)&args[0]);
        // wait for worker threads to complete
        for (int i=1; i<NUM_THREADS; i++)
            pthread_join(thread[i], NULL);
    }

}
void printUsage(){
    //TODO:
}
int main(int argc, char *argv[]){
    //checkAdd();
    /*if(argc!=1){
        printUsage();
        return 0;
    }*/
    int a ,b;
    string filename(argv[1]);
    std::ifstream fin;
    fin.open(filename);
    int count=0,limit =0;
    //processTrace(filename);   
}
