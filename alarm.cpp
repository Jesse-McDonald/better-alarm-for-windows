#include <string>
#include <windows.h>
using namespace std;
string getexepath()
{
  char result[ MAX_PATH ];
  string full =string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
  int dir = full.find_last_of('\\');
  return full.substr(0,dir)+'\\';
}
#include <iostream>

#include <fstream>
#include "stdlib.h"

#include <ctime>
#include <conio.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>

typedef int alarmEvent;
int tMod(int time, int base){
	return ((time%base+base)%base);
}
alarmEvent alarms []={//format for times 1DHHMM, where D is days since Sunday, HH is hours since midnight, MM is minutes since hour
		//so 110750 would be an alarm for Monday at 7:50 AM
		110750,//Monday 	7:50
		120850,//Tuesday	8:50
		130750,//Wednesday	7:50
		140850,//Thursday	8:50
		150850,//Friday  	8:50
		141143,
		100950 //Sunday 	9:50

};
//mod by 70000 when taking a time difference

alarmEvent timeComp(){
	int ret =10;
	time_t t = time(0);
	struct tm * now = localtime( & t );
	ret+=now->tm_wday;
	ret*=100;
	ret+=now->tm_hour;
	ret*=100;
	ret+=now->tm_min;
	return ret;
}

string path =getexepath();
/*

SYSTEMTIME lastUpdated;
bool init=false;

bool filechange(ifstream alarms){
	FILETIME creationTime,
	         lpLastAccessTime,
	         lastWriteTime;
	bool err = GetFileTime( (void *)alarms, &creationTime, &lpLastAccessTime, &lastWriteTime );
	SYSTEMTIME fTime;
	bool res = FileTimeToSystemTime( &lastWriteTime, &fTime );
	if (!(err&&res)){}
	if(!init) {
		init=true;
		lastUpdated=fTime;
	}
	if (fTime==lastUpdated){
		lastUpdated=lastWriteTime;
		return true;
	}
	return false;
}*/


int alarmRisky(){

	path+="alarm.bat";
	bool triggered=false;
	while(true){
		time_t t = time(0);   // get time now
		struct tm * now = localtime( & t );
		int hour = now->tm_hour;
		int min = now->tm_min;
		if(hour<7){
			cout<<"sleeping for "<<7-hour<<" hours."<<endl;
			Sleep((7-hour)*60*100*60);

		}else if (hour>8){
			cout<<"sleeping for "<<(23-hour)*60*60*100<<" hours."<<endl;
			Sleep((23-hour)*60*60*100);
		}else if (min<40){
			cout<<"sleeping for "<<40-min<<" minutes."<<endl;
			Sleep((40-min)*60*100);
		}else if(min==50&&hour==7){
			if(!triggered){
				system(path.c_str());
				triggered=true;
				cout<<"sleeping for 2 minutes and alarm locked."<<endl;
				Sleep(120000);

				min = now->tm_min;
			}
		if(triggered&&min!=50){
		  triggered=false;
		  cout<<"Alarm reset.\n";
		  hour = now->tm_hour;
		  cout<<"sleeping for "<<(23-hour)<<" hours."<<endl;
		  Sleep((23-hour)*60*60*100);
		}
		}
	}
	return 0;
}

int alarmHugeCPU(){

	path+="alarm.bat";
	bool triggered=false;
	while(true){
		time_t t = time(0);   // get time now
		struct tm * now = localtime( & t );
		int hour = now->tm_hour;
		int min = now->tm_min;
		if(min==50&&hour==7){
			if(!triggered){
				system(path.c_str());
				triggered=true;
			}
		if(triggered&&min!=50){
		  triggered=false;
		  cout<<"Alarm reset.\n";
		}
		}
	}
	return 0;
}

int alarm(){
	alarmEvent nextAlarm;
	time_t t = time(0);
	struct tm * now = localtime( & t );
	int alarmsSize=sizeof(alarms)/sizeof(alarms[0]);
	int mDist;
	for(int i=0;i<alarmsSize;i++){

	}
	int wake=8;//:40
	path+="alarm.bat";
	bool triggered=false;
	cout<<"wake at: "<<wake<<":40"<<endl;
	while(true){
		t = time(0);   // get time now
		tm * now = localtime( & t );
		int hour = now->tm_hour;
		int min = now->tm_min;
		if(hour<wake-1){
			cout<<"["<<hour<<":"<<min<<"] sleeping for 1 hour."<<endl;
			Sleep(60*1000*60);
		}else if (hour>wake){
			cout<<"["<<hour<<":"<<min<<"] sleeping for 1 hour."<<endl;
			Sleep(60*1000*60);
		}else if (min<40){
			cout<<"["<<hour<<":"<<min<<"] sleeping for "<<40-min<<" minutes."<<endl;
			Sleep((40-min)*60*1000);
		}else if(min==50&&hour==wake){
			if(!triggered){
				system(path.c_str());
				triggered=true;
				cout<<"["<<hour<<":"<<min<<"] sleeping for 2 minutes and alarm locked."<<endl;
				Sleep(120000);

				min = now->tm_min;
			}
		}else if(triggered&&min!=50){
		  triggered=false;
		  cout<<"Alarm reset.\n";
		  hour = now->tm_hour;
		  cout<<"["<<hour<<":"<<min<<"] sleeping for 1 hour."<<endl;
		  Sleep(60*60*1000);
		}

	}
	return 0;
}

int weekAlarm(){
	alarmEvent nextAlarm;
	int alarmsSize=sizeof(alarms)/sizeof(alarms[0]);
	int bTime, tTime,pos;
	path+="alarm.bat";
	bool triggered=false;
	alarmEvent now;
	while(true){
		bTime=70000;
		now=timeComp();
		for(int i=0;i<alarmsSize;i++){
			alarms[i];
			tTime=tMod((alarms[i]-now),70000);
			if(tTime<bTime){
				bTime=tTime;
				pos=i;
			}
		}
		nextAlarm=alarms[pos];
		cout<<"Wake at: "<<(nextAlarm/100)%100<<":"<<nextAlarm%100<<endl;
		now = timeComp();
		cout<<nextAlarm<<endl;
		cout<<nextAlarm-now;
		cout<<tMod((nextAlarm-now),70000)<<endl;
		if(now==nextAlarm){
					if(!triggered){
						system(path.c_str());
						triggered=true;
						cout<<"["<<(now/100)%100<<":"<<now%100<<"] sleeping for 2 minutes and alarm locked."<<endl;
						Sleep(120000);
					}
		}else if(tMod((nextAlarm-now),70000)>200){
			cout<<"["<<(now/100)%100<<":"<<now%100<<"] sleeping for 1 hour."<<endl;
			Sleep(60*1000*60);
		}else if (tMod(((nextAlarm)-now),70000)>10){
			cout<<"["<<(now/100)%100<<":"<<now%100<<"] sleeping for "<<tMod(((nextAlarm-now)%70000),100)<<" minutes."<<endl;
			Sleep(tMod(((nextAlarm-now)%70000),100)*60*1000);
		}else if(triggered&&nextAlarm!=now){
		  triggered=false;
		  cout<<"Alarm reset.\n";
		  cout<<"["<<(now/100)%10000<<":"<<now%100<<"] alarm reset."<<endl;
		}

	}
	return 0;
}

int main() {
	alarm();
	return 0;
}

