#include <iostream>
#include <vector>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <fstream>
using namespace std;
#define buf_size 10
typedef struct medicine{
	int id;
	int shortT;
    	int longT;
	int tb;
	int te;
	int realEat;

}med;

bool struct_cmp_by_te(const med a,const med b){
    return (a.realEat + a.te) < (b.realEat + b.te);
}
void te_sort(vector<med> &t){
    //sort the vector t by te from small to big
    sort(t.begin(), t.end(), struct_cmp_by_te);
    return;
}
bool struct_cmp_by_realEat(const med a,const med b){
    return (a.realEat) < (b.realEat);	
}
void realEat_sort(vector<med> &t){
    //sort the vector t by real eat time from small to big
    sort(t.begin(), t.end(), struct_cmp_by_realEat);
    return;
}
void print_task(vector<med> &t){
	int i;
	for(i=0;i<t.size();++i)
		printf("id=%d,shortT=%d,longT=%d,realEat=%d\n",t[i].id,t[i].shortT,t[i].longT,t[i].realEat);

	return ;
}
void read_file(vector<med> &med_d){
	fstream fin;
	const char* input = "input.txt";
	fin.open(input,ios::in);
	char buf[buf_size];
	memset(buf,'\0',buf_size);
	fin.getline(buf,sizeof(buf),'\n');
	int m;
	sscanf(buf, "%d", &m);
	for(int i=0;i<m;++i){
		// read input file
		med tmp;
		fin.getline(buf,sizeof(buf),' ');
		sscanf(buf, "%d", &tmp.id);
		fin.getline(buf,sizeof(buf),' ');
		sscanf(buf, "%d", &tmp.shortT);
		fin.getline(buf,sizeof(buf),'\n');
		sscanf(buf, "%d", &tmp.longT);
		tmp.realEat = 0;
		tmp.shortT = tmp.shortT*60;
		tmp.longT = tmp.longT*60;
		if(tmp.longT - tmp.shortT >= 180){
			tmp.tb = tmp.shortT + 60;
			tmp.te = tmp.longT - 60;
		}
		else{
			tmp.tb = tmp.shortT + int((tmp.longT-tmp.shortT) / 3);
			tmp.te = tmp.longT - int((tmp.longT-tmp.shortT) / 3);
		}
		med_d.push_back(tmp);
	}
}
void find_interval(int *min, int *max, vector<med> med_data){
	*min = -1;
	*max = -1;
	for(int i = 0; i < med_data.size() ; ++i){
		if( ((*min) == -1) || ((*min) >= (med_data[i].realEat + med_data[i].tb)) ){
			*min = (med_data[i].realEat + med_data[i].tb);}
		if( ((*max) == -1) || ((*max) <= (med_data[i].realEat + med_data[i].te)) )
			*max = (med_data[i].realEat + med_data[i].te);
	}
	return ;
}
int eat_time(vector<med> med_data){
	int max,min,max_med_num,med_num,eat_s,eat_e;
	max = -1;
	min = -1;
	max_med_num = 0;
	med_num = 0;
	eat_s = -1;
	eat_e = -1;
	find_interval(&min,&max,med_data);
	for(int time = min; time <= max ; ++time){
		med_num = 0;
		for(int i = 0; i<med_data.size(); ++i){
			if( (time <= med_data[i].realEat + med_data[i].te) && (time >= med_data[i].realEat + med_data[i].tb))
				++med_num;
		}
		if(med_num > max_med_num){
			max_med_num = med_num;
			eat_s = time;
		}
	}
	for(int time = eat_s; time <= max ; ++time){
		med_num = 0;
		for(int i = 0; i<med_data.size(); ++i){
			if( (time <= med_data[i].realEat + med_data[i].te) && (time >= med_data[i].realEat + med_data[i].tb))
				++med_num;
		}
		if(med_num < max_med_num){
			eat_e = time-1;
			break;
		}
	}
	return int((eat_s + eat_e)/2);
}
void schedule(int *hit, int *miss, int *take_med_times, int *hyperperiod, int *Maximum_idle_time, vector<med> med_data){
	fstream fin;
	const char* input = "RandomTable.txt";
	fin.open(input,ios::in);
	char buf[buf_size];
	int m = med_data.size();
	int flag=0;
	*hit = 0;
	*miss = 0;
	*take_med_times = 0;
	*hyperperiod = 0;
	*Maximum_idle_time = 0;
	int last_eat_time = 0;
	while(flag==0){ //use flag to check whether it is hyper period 
		int finish=0;
		int eat = eat_time(med_data);
		te_sort(med_data);
		while(finish!=m){
			finish=0;
			int delay=0;
			int count=0; // hyper period
			te_sort(med_data);
			memset(buf,'\0',buf_size);
			fin.getline(buf,sizeof(buf),' ');
			sscanf(buf, "%d", &delay);
			printf("delay=%d\n",delay);
			for(int i=0; i < med_data.size();++i){
				if( (med_data[i].realEat + med_data[i].longT) < eat ){
					if((delay>=0) && ((med_data[i].realEat + med_data[i].longT) >= (med_data[i].realEat + int((med_data[i].te + med_data[i].tb)/2)+delay))){ //hit
						med_data[i].realEat+=delay;
					}
					else if((delay<0) && ((med_data[i].realEat + med_data[i].shortT) <= (med_data[i].realEat + int((med_data[i].te + med_data[i].tb)/2)+delay))){
						med_data[i].realEat+=delay;
					}	
					med_data[i].realEat += int((med_data[i].te + med_data[i].tb)/2);
					if(*Maximum_idle_time < (med_data[i].realEat - last_eat_time))
						*Maximum_idle_time = med_data[i].realEat - last_eat_time;
					last_eat_time = med_data[i].realEat;
					cout<<"Take Medicine "<<med_data[i].id<<" at "<<med_data[i].realEat<<endl;
					++ *hit;
					++ *take_med_times;
					break;

				}
				else if( ((med_data[i].realEat + med_data[i].longT) >= eat) && ((med_data[i].realEat + med_data[i].shortT) <= eat)){
					if((delay>=0) && ((med_data[i].realEat + med_data[i].longT) >= (eat+delay))){
						med_data[i].realEat=delay+eat;
						cout<<"Take Medicine "<<med_data[i].id<<" at "<<med_data[i].realEat<<endl;
						++count;
						++ *hit;
						if(*Maximum_idle_time < (med_data[i].realEat - last_eat_time))
							*Maximum_idle_time = med_data[i].realEat - last_eat_time;
						last_eat_time = med_data[i].realEat;
					}
					else if((delay<0) && ((med_data[i].realEat + med_data[i].shortT) <= (eat+delay))){
						med_data[i].realEat=delay+eat;
						cout<<"Take Medicine "<<med_data[i].id<<" at "<<med_data[i].realEat<<endl;
						++count;
						++ *hit;
						if(*Maximum_idle_time < (med_data[i].realEat - last_eat_time))
							*Maximum_idle_time = med_data[i].realEat - last_eat_time;
						last_eat_time = med_data[i].realEat;
					}
					else{ //miss
						printf("id=%d,miss\n",med_data[i].id);	
						med_data[i].realEat=eat;
						cout<<"Take Medicine "<<med_data[i].id<<" at "<<med_data[i].realEat<<endl;
						++ *miss;
						++ *take_med_times;
						if(*Maximum_idle_time < (med_data[i].realEat - last_eat_time))
							*Maximum_idle_time = med_data[i].realEat - last_eat_time;
						last_eat_time = med_data[i].realEat;
					}
					++finish;
				}
				else{
					++finish;
				}
			}
			if(count != 0)
				++ *take_med_times;
			if(count==m){
				flag=1;
				*hyperperiod = med_data[0].realEat;
			}
		}
	}
	return ;

}
int main(void){
	vector<med> med_data;
	int hit,miss,take_med_times,hyperperiod,Maximum_idle_time;
	read_file(med_data);
	print_task(med_data);
	schedule(&hit,&miss,&take_med_times,&hyperperiod,&Maximum_idle_time,med_data);
	float hitRate =  hit / float(hit + miss);
	cout<< "hit rate = " << hitRate << endl;
	cout<< "take_med_times= " << take_med_times << " hyperperiod= " << hyperperiod << endl;
	float Grouping = take_med_times / (hyperperiod/float(1440));
	cout<< "Grouping= " << Grouping << endl;
	cout<< "Maximum idle time = " << Maximum_idle_time << endl;
	return 0;
}
