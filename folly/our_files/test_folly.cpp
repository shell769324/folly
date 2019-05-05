#include <folly/ConcurrentSkipList.h>
#include<iostream>
#include "CycleTimer.h"
#include <fstream>
#include<string>
#include <thread>
//#include "pthread.c"
#include "pthread.h"
using  std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

static const int kInitHeadHeight = 10;
pthread_barrier_t mybarrier;

namespace {
using namespace folly;

typedef int ValueType;
typedef ConcurrentSkipList<ValueType> SkipListType;
typedef SkipListType::Accessor skipListAccessor;

struct threadArgs {
string filename;
int thread_id;
skipListAccessor* skipList;
};
typedef struct threadArgs threadArgs;

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
    void printSkipList(skipListAccessor &skipList){
     auto iter = skipList.begin();

        for(;iter!=skipList.end();iter++)
            cout<<*iter<<" ";
    }

    void processTraceChunk(vector<int> &ops,vector<int> &values,skipListAccessor &skipList){
        int n = ops.size();
        
        for(int i = 0;i<n;i++){
            
            int op= ops[i];
            int value =values[i];

            if (op==0){
                auto node = skipList.find(value);
                /*if(DEBUG){
                    if(node==skipList.end()){
                        cout<<"not found"<<endl;
                    }
                    else{
                        cout<<"Found"<<endl;
                    }
                }*/
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
        skipListAccessor skipList = *(args.skipList);
        cout<<"Doing thread "<<thread_id<<endl;
        cout<<filename<<endl;
        fin.open(filename);
        
        string line;
        vector<int> ops;
        vector<int> values;
        int a, b;
        while(fin>>a>>b){
            //cout<<a<<b<<endl;
            ops.push_back(a);
            values.push_back(b);
            //For debugging insert only 1
            /*if (a==1)
                break;*/
        }
        cout<<"Finished while loop : "<<thread_id<<endl;
        pthread_barrier_wait(&mybarrier);
        double startTime = CycleTimer::currentSeconds();
            processTraceChunk(ops,values,skipList);
        double endTime = CycleTimer::currentSeconds();
        double* time_taken = (double*)malloc(sizeof(double));
        *time_taken = endTime-startTime;
        cout<<"Time taken by thread"<<thread_id<<" is "<<*time_taken<<endl;

        cout<<"Completed Thread"<<thread_id<<endl;
        fin.close();
        //cout<<"All done"<<endl;
        return (void*)time_taken;
    }
    void processTrace(string filename,int num_threads,int work){
        pthread_t thread[num_threads];
        threadArgs args[num_threads];
        //call function from each thread
        //double times[num_threads];
        void* status[num_threads];
        auto skipList = SkipListType::create(kInitHeadHeight);
        pthread_barrier_init(&mybarrier, NULL, num_threads);

        for (int i=1; i<num_threads; i++){
            args[i].filename = "./files/"+filename+"_"+std::to_string(work)+"_"+std::to_string(i)+".txt";           
            args[i].thread_id = i;
            args[i].skipList = &skipList; 
            cout<<"Starting thread "<<i<<endl;
            pthread_create(&thread[i], NULL, readFileAndProcess,(void*)&args[i]);
        }
        //Parent thread setup Args and call
        args[0].filename = "./files/"+filename+"_"+std::to_string(work)+"_"+ std::to_string(0)+".txt";
        args[0].thread_id = 0;
        args[0].skipList = &skipList;
        status[0] = readFileAndProcess((void*)&args[0]);
        // wait for worker threads to complete
        for (int i=1; i<num_threads; i++)
            pthread_join(thread[i], &status[i]);
        double max_time = 0;
        cout<<"Threads joined"<<endl;
        for(int i =0;i<num_threads;i++)
            max_time = std::max(max_time,*(double*)status[i]);   
        cout<<"total time taken=  "<<max_time<<endl;
        //Verify if inserted correctly
        //printSkipList(skipList);
    }
}
int main(int argc, char *argv[]){
    checkAdd();
    int num_threads = atoi(argv[1]);
    int work = atoi(argv[2]);

    string filename = "random";

    string scriptname = "./create_tests.py "+ std::to_string(num_threads)+" "+ std::to_string(work);
    string command = "python ";
    command += scriptname;
    //cout<<command<<endl;
    system(command.c_str());
    

    //string filename(argv[1]);
    std::ifstream fin;
    fin.open("./files/"+filename+"_"+std::to_string(work)+"_0.txt");
    //cout<<"./files/"+filename+"_"+std::to_string(work)+"_0.txt"<<endl;
    int a,b;
    int count=0,limit=1;
    while(fin>>a>>b){
        cout<<a<<" "<<b<<endl;
        if(count++>=limit)
            break;
    }
    fin.close();
    double startTime = CycleTimer::currentSeconds();
    processTrace(filename,num_threads,work);
    double endTime = CycleTimer::currentSeconds();
    cout<<"Time taken with "<<num_threads<<" threads is "<<endTime-startTime<<endl;
    
}
