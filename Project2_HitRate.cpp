#include<iostream>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<cmath>
#define INPUT "Testcase0.txt"
#define DELAY "RandomTable.txt"
using namespace std;
typedef struct medicine 
{
    int id;
    int shortT;
    int longT;
    int Tb;
    int Te;
    int realEat;
    int prevRealEat;
}med;
typedef struct time_record 
{
    int timeT;
    vector<int> id_v;
}tR;
typedef struct result
{
    int timeT;
    int id;
}res;
vector <res> result_list;
vector <tR> tR_list;
vector <med> med_list;
 vector<int> id_r; // 紀錄吃最多的時間(最早)當下的id
int m;
int time_prevv;
int delay;
float hit;
float miss;
float hitRate;
float maxIdleT;
float group;
float end_time;
float group_time=0;

void readfile(){
    FILE *input_f = freopen(INPUT,"r",stdin);
    cin>>m;
    med tempm;
    for(int i=0;i<m;++i){
        cin>>tempm.id>>tempm.shortT>>tempm.longT;
        if(tempm.longT-tempm.shortT >=3 ){
            tempm.Tb = (tempm.shortT+1)*60;
            tempm.Te = (tempm.longT-1)*60;
        }else{
            int temp_inter = (tempm.longT-tempm.shortT)*60/3;
            tempm.Tb = tempm.shortT*60 + temp_inter;
            tempm.Te = tempm.longT*60 - temp_inter;
        }
        tempm.realEat = 0;
        tempm.prevRealEat = 0;
        med_list.push_back(tempm);
        
    }
    fclose(input_f);
//    cout<<m<<endl;
    // for(int i=0;i<m;++i){
    //     cout<<med_list[i].Tb<<" "<<med_list[i].Te<<endl;
    // }
}

void schedule(){
    time_prevv = 0;
    maxIdleT = 0;
    cout<<"time: 0, Eat Mecdicine ID: ";
    for(int i=0;i<m;++i){
        cout<<i<<" ";
    }
    cout<<endl;
    FILE *rand_f = freopen(DELAY,"r",stdin);
    //while(1){ //loop until hyper period
// int flag = 0;
    while(1){
        // for(int i=0; i<m; i++){
        //     cout << med_list[i].realEat << " ";
        // }
        // // break;
        // cout <<endl;
        /****** 找最開始吃藥的時間 ******/
        int minT=1000000; 
        for(int i=0;i<m;++i){
            if(med_list[i].Tb + med_list[i].realEat < minT){
                minT = med_list[i].Tb + med_list[i].realEat;
            }
        }
        // cout << "minT: " <<minT << endl;
        int maxNum=0; //最多task一起做的數量
        tR_list.clear();
        //紀錄tR_list
        int i = minT;
        while(1){ //做到可以排為止
            tR tempTR;
            tempTR.timeT = i;
            /****** 找出時間j可以吃的medicine存在tempTR.idv ******/
            for(int j=0;j<m;++j){
                if(med_list[j].Tb + med_list[j].realEat <= i && med_list[j].Te + med_list[j].realEat >= i){
                    tempTR.id_v.push_back(med_list[j].id);
                }
            }
            //cout<<tempTR.timeT<<": "<<tempTR.id_v.size()<<endl;
            /****** 找可以排成的斷點 ******/
            if(tempTR.id_v.size() == 0){
                break;
            }else if(tempTR.id_v.size() == 1 && i != minT){
                if(tR_list[i-minT-1].id_v.size()==1 && tR_list[i-minT-1].id_v[0]!=tempTR.id_v[0]){
                    break;
                }
            }else{

            }
            //紀錄目前最多可以同時做的case數量
            if(tempTR.id_v.size() >= maxNum){
                maxNum = tempTR.id_v.size();
            }
            tR_list.push_back(tempTR);
            i++;
        }
        // cout <<"DUAN_DIAN: " << tR_list[tR_list.size()-1].timeT <<endl;

        // /****** 排程preprocess ******/
      
        int minTime, maxTime;
// 我離不開這個while
        while(1){
            maxNum = 0;
            for(int i=0; i<tR_list.size(); ++i){
                if(tR_list[i].id_v.size()>maxNum){
                    maxNum = tR_list[i].id_v.size();
                }
            }
            // cout << "hi";
            minTime=tR_list[tR_list.size()-1].timeT;
            maxTime=tR_list[0].timeT;
            id_r.clear();
            /****** 紀錄吃最多的"開始"時間點、"結束"時間點，還有當下的id ******/     
            for(int i=tR_list.size()-1; i>=0;--i){
                // cout << tR_list[i].id_v.size() << " " << maxNum <<endl;
                if(tR_list[i].id_v.size() == maxNum){
                    maxTime = tR_list[i].timeT; // 吃最多的"結束"時間點
                    for(int j=0;j<tR_list[i].id_v.size();++j){
                        id_r.push_back(tR_list[i].id_v[j]); // 當下id
                    }
// 沒有印第二次

                    // for(int t=0; t<id_r.size(); ++t){
                    //     cout << id_r[t] << " ";
                    // }
                    // cout <<endl;

                    while(tR_list[i].id_v.size() == maxNum){ // loop到最後吃最多的時間
                        int j=0;
                        for(j=0;j<id_r.size();++j){
                            if(id_r[j] != tR_list[i].id_v[j]){
                                break;
                            }
                        }
                        if(j != id_r.size()){
                            break;
                        }
                        i--;
                    }
                    minTime = tR_list[i+1].timeT; // 吃最多的"開始"時間點
                    break;
                }
            }
            // cout<<minTime<<" "<<maxTime<<endl;
            /****** 如果第一個不是我想要的，把前面不符合的移除 ******/
        
            for(int i=0; i<tR_list.size();++i){
                for(int j=0;j<tR_list[i].id_v.size();++j){
                    for(int l=0;l<id_r.size();++l){
                        if(tR_list[i].id_v[j] == id_r[l]){
                            tR_list[i].id_v.erase(tR_list[i].id_v.begin()+j);
                        }
                        
                    }
                }
            }
            // for(int i=0; i<tR_list.size(); i++){
            //     cout << tR_list[i].id_v.size();
            // }
            // cout << endl;
            int i;
            for(i=0;i < tR_list.size();++i){
                if(tR_list[i].id_v.size()!=0){
                    // cout << tR_list[i].id_v[0];
                    break;
                }
            }
            if(i == tR_list.size()){
                break;
            }
        }

        /****** 計算吃藥時間，更新realEat ******/
        int time_middle = (minTime + maxTime)/2; //取middle
        // cout << "Time middle: " << time_middle << endl;
        for(int i=0;i<id_r.size();i++){
            for(int j=0;j<m;++j){
                if(med_list[j].id == id_r[i]){
                    med_list[j].prevRealEat = med_list[j].realEat;
                    med_list[j].realEat = time_middle; //移動realEat
                    // group_flag[id_r[i]] = true;
                }
            }
        }

        cin>>delay;
        //cout<<delay<<endl;
        // cout <<id_r.size() <<endl;
        for(i=0;i<id_r.size();++i){
            for(int j=0;j<m;++j){
                if(med_list[j].id == id_r[i]){
                    if(med_list[j].realEat + delay <= med_list[j].longT*60 + med_list[j].prevRealEat && med_list[j].realEat + delay >= med_list[j].shortT*60 + med_list[j].prevRealEat){
                        // med_list[j].realEat += delay;
                        hit++;
                    }else{
                        miss++;
                    }
                }
            }
        }

        end_time =med_list[id_r[0]].realEat;
        if(end_time - time_prevv > maxIdleT){
            maxIdleT = end_time - time_prevv;
        }
        time_prevv = end_time;
        group_time++;
        cout<<"time: "; 
        cout<<med_list[id_r[0]].realEat<<", Eat Mecdicine ID: ";
        for(int i=0;i<id_r.size();++i){
            for(int j=0;j<m;++j){
                if(med_list[j].id == id_r[i]){
                cout<<med_list[j].id<<" ";
                }
            }
        }
    cout<<endl;
        // for(int i=0; i<m; i++){
        //     cout << med_list[i].realEat << " ";
        // }
        // // break;
        // cout <<endl;
        // 終止條件(HyperPeriod)
        if(maxNum==m)
        {
            break;
        }

    }
}

void analysis(){
    cout<<endl<<"Hit Rate: "<<hit/(hit+miss)<<endl;
    cout<<"Grouping: "<<(group_time-1)/(end_time/60/24)<<endl;
    cout<<"Max Idle Time: "<<maxIdleT/60.0<<endl;
}

int main(){
    readfile();
    schedule();
    analysis();
    return 0;
}