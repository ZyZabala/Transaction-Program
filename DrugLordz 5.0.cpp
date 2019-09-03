#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>

using namespace std;
int userType(0), tmpNum, *ptrNum, hold[6][100], locate[6], total[6];
bool found(0);
char buff[1024], yesno, tmpCha, choice, *ptrCha;
time_t now=time(0); //time function
struct tm tstruct=*localtime(&now);	//library structure for time components
string cell, temp, line;
fstream fs;	//stream-type file handle
FILE *fp;	//pointer-type file handle
HANDLE hand=GetStdHandle(STD_OUTPUT_HANDLE); //for console operations
	
	enum Colors {																								//color enumeration
			black,          //  0
			dark_blue,      //  1
			dark_green,     //  2
			dark_cyan,      //  3
			dark_red,       //  4
			dark_magenta,   //  5
			dark_yellow,    //  6
			light_gray,     //  7
			dark_gray,      //  8
			light_blue,     //  9
			light_green,    // 10
			light_cyan,     // 11
			light_red,      // 12
			light_magenta,  // 13
			light_yellow,   // 14
			white           // 15
		};
	enum Users {
		dealer, //0
		common, //1
		applicant //2																							//userType enumeration
	};
	enum Arrays {
		drug_data, 	//0
		drug_order,	//1
		drug_temp,	//2
		user_data,	//3
		user_apply,	//4
		user_temp,	//5
	};

	struct effects {																							//substructure for drug effects
		char fxn[100];
		float fns;
	};
	struct tdate {																								//substructure for agent transactions
		int mm,dd,yy,tt;
	};
	struct deal {																								//data type deal
		char name[50];
		string id;
		float price, cost;
		int qty;
		effects fx[100];
	}*dr, drugdata[100], drugorder[100], drugtemp[100];
	struct user {																								//data type user
		char name[50], address[100];
		string key;
		long long number;
		float salary;
		tdate td[100];//object vector for trasactions and dates
	}*us, userdata[100], userapply[100], usertemp[100];
	
	class Drug {																								//base class Drug
		public:
			deal read();//read from file
			void write();//write to file
			int find();//find particular keyword
			void show();//display on console
			void del();//delete from file //update array //write to file
			deal change();//update array //write to file
			deal add();//update array //append to file
			deal get();//get new data for array
			deal pay();//dynamic function for current drug
	}transaction;
	class Agent {																								//base class Agent
		public:
			user read();//read from file
			void write();//write to file
			int find(int tmpNum, string temp);//find particular keyword
			void show();//display on console
			void del();//delete from file //update array //write to file
			user change();//update array //write to file
			user add();//update array //append to file
			user get();//get new data for array
			class Apply {//subclass for application process
				public:
					user form();
					user read();
			}apply;
	}person;
	class Instance {																							//base class Instance
		public:
			int login();//get dynamic data for current user
			void logout();//delete dynamic data for current user
			void load(int fore1, int back1, int fore2, int back2);//loading screen
			void chooser();//main screen with options
			int keygen(int max, int min);//generates random 4-digits for user keys
			string intstr(int tmpNum);//converts int to string
			void text(int fore, int back, string temp);//displays colored texts
			void headerDesign(int userType);//main design
			string month(int tmpNum);//get month name from number
			string trnsct(int tmpNum);//get transaction type from number
			string date();//get date from time function
			void validator();//renew inputs if improper
			void bar(int fore, int back, int length, char tmpCha, int dur);//design
	}program;
	
	deal Drug::read() {
		fs.open("Deal.csv",ios::in);
		total[drug_data]=-1;
		getline(fs,line,'\n');
		while(!fs.eof()) {
			getline(fs,cell,',');
			if(strlen(cell.c_str())==0) {
				getline(fs,cell,',');
				getline(fs,cell,',');
			}
			if(strlen(cell.c_str())>0) {
				total[drug_data]++;
				hold[drug_data][total[drug_data]]=-1;
					strcpy(drugdata[total[drug_data]].name, cell.c_str());
					getline(fs,drugdata[total[drug_data]].id,',');
				getline(fs,cell,',');
					drugdata[total[drug_data]].price = atof(cell.c_str());
			}
			if(!fs.eof()) {
				hold[drug_data][total[drug_data]]++;
				getline(fs,cell,',');
					strcpy(drugdata[total[drug_data]].fx[hold[drug_data][total[drug_data]]].fxn, cell.c_str());
				getline(fs,cell,'\n');
					drugdata[total[drug_data]].fx[hold[drug_data][total[drug_data]]].fns = atof(cell.c_str());
			}
		}
		fs.close();
		return *drugdata;
	}
	void Drug::write() {
		remove("Deal.csv");
		fs.open("Deal.csv",ios::out);
		fs<<"Drug Name,Drug ID,Price,Effect,Effectiveness\n";
		for(int i=0; i<=total[drug_data]; i++) {
			fs<<drugdata[i].name<<","<<drugdata[i].id<<","<<drugdata[i].price<<","<<drugdata[i].fx[0].fxn<<","<<drugdata[i].fx[0].fns<<"\n";
			for(int j=1; j<=hold[drug_data][i]; j++) {
				fs<<",,,"<<drugdata[i].fx[j].fxn<<","<<drugdata[i].fx[j].fns<<"\n";
			}
		}
		fs.close();
	}
//	int Drug::find() {
//	}
	void Drug::show() {
		program.headerDesign(userType);
		program.text(black,dark_gray,"DRUG NAME   ID      PRICE         EFFECT                        EFFECTIVENESS\n");
		program.bar(15,0,76,223,0);cout<<endl;
		for(int j=0; j<=total[drug_data]; j++) {
			cout<<left<<setw(12)<<drugdata[j].name//drug name from array from database
				<<setw(8)<<drugdata[j].id //drug id from array from database
				<<"$ "<<fixed<<showpoint<<setprecision(2)<<drugdata[j].price<<"\t  "//drug price from array from database
				<<setw(30)<<left<<drugdata[j].fx[0].fxn //drug effect name
				<<fixed<<showpoint<<setprecision(2)<<drugdata[j].fx[0].fns<<" %"<<endl;//drug effectiveness
			for(int k=1; k<=hold[drug_data][j]; k++) {//drug effect for drug name from array from database
				cout<<"                                  "
					<<setw(30)<<drugdata[j].fx[k].fxn //drug effect name
					<<fixed<<showpoint<<setprecision(2)<<drugdata[j].fx[k].fns<<" %"<<endl;//drug effectiveness
			}
			program.bar(15,0,76,196,0);cout<<endl;
		}
		program.bar(15,0,76,223,0);cout<<endl;
		system("pause");
	}
//	void Drug::del() {
//	}
//	deal Drug::change() {
//	}
	deal Drug::add() {
		program.headerDesign(userType);
		total[drug_data]++;
		hold[drug_data][total[drug_data]]=0;
		cout<<"Enter Drug Details:\n"
			<<"              Name: ";
		cin.getline(drugdata[total[drug_data]].name,50);
		drugdata[total[drug_data]].id=program.intstr(program.keygen(9001,999));
		cout<<"                ID: "<<drugdata[total[drug_data]].id<<endl;
		cout<<"             Price: $ ";
		cin>>drugdata[total[drug_data]].price;
		cout<<" Number of Effects: ";
		cin>>hold[drug_data][total[drug_data]];
		for(int i=0; i<hold[drug_data][total[drug_data]]; i++) {
			program.bar(dark_gray,black,76,196,0);cout<<endl;
			cout<<"         Effect #"<<i+1<<": ";
			cin.ignore();
			cin.getline(drugdata[total[drug_data]].fx[i].fxn,100);
			cout<<"     Effectiveness: ";
			cin>>drugdata[total[drug_data]].fx[i].fns;
		}
		hold[drug_data][total[drug_data]]=hold[drug_data][total[drug_data]]-1;
		transaction.write();
		transaction.read();
		program.bar(light_cyan,black,76,196,1);cout<<endl;
		transaction.show();
		return *drugdata;
	}
//	deal Drug::get() {
//	}
//	deal Drug::pay() {
//	}

	user Agent::read() {
		fs.open("User.csv",ios::in);
		total[user_data]=-1;
		getline(fs,line,'\n');
		while(!fs.eof()) {
			getline(fs,cell,',');
			if(strlen(cell.c_str())==0) {
				getline(fs,cell,',');
				getline(fs,cell,',');
				getline(fs,cell,',');
				getline(fs,cell,',');
			}
			if(strlen(cell.c_str())>0) {
				total[user_data]++;
				hold[user_data][total[user_data]]=-1;
					strcpy(userdata[total[user_data]].name, cell.c_str());
					getline(fs,userdata[total[user_data]].key,',');
				getline(fs,cell,',');
					strcpy(userdata[total[user_data]].address, cell.c_str());
				getline(fs,cell,',');
					userdata[total[user_data]].number = atoll(cell.c_str());
				getline(fs,cell,',');
					userdata[total[user_data]].salary = atof(cell.c_str());
			}
			if(!fs.eof()) {
				hold[user_data][total[user_data]]++;
				getline(fs,cell,',');
					userdata[total[user_data]].td[hold[user_data][total[user_data]]].dd = atoi(cell.c_str());
				getline(fs,cell,',');
					userdata[total[user_data]].td[hold[user_data][total[user_data]]].mm = atoi(cell.c_str());
				getline(fs,cell,',');
					userdata[total[user_data]].td[hold[user_data][total[user_data]]].yy = atoi(cell.c_str());
				getline(fs,cell,'\n');
					userdata[total[user_data]].td[hold[user_data][total[user_data]]].tt = atoi(cell.c_str());
			}
		}
		fs.close();
		return *userdata;
	}
	void Agent::write() {
		fs.open("User.csv",ios::out);
		fs<<"Username,Key,Address,Contact Number,Salary,Day,Month,Year,Transaction\n";
		for(int i=0; i<total[user_data]; i++) {
			fs<<userdata[i].name<<","<<userdata[i].key<<","<<userdata[i].address<<","<<userdata[i].number<<","<<userdata[i].salary<<",";
			for(int j=0; j<hold[user_data][total[user_data]]; j++) {
				if(j==0) fs<<userdata[i].td[j].dd<<","<<userdata[i].td[j].mm<<","<<userdata[i].td[j].yy<<","<<userdata[i].td[j].tt<<"\n";
				if(j>0) fs<<",,,,,"<<userdata[i].td[j].dd<<","<<userdata[i].td[j].mm<<","<<userdata[i].td[j].yy<<","<<userdata[i].td[j].tt<<"\n";
			}
		}
		fs.close();
	}
	int Agent::find(int tmpNum, string temp) {
		found=0;
		if(tmpNum==user_data) {
			for(int i=0; i<total[user_data]; i++) {
				if((string)userdata[i].name==temp) {
					locate[user_data]=i;
					i=total[user_data];
					found=1;
				}
			}
		}
		if(tmpNum==user_apply) {
			for(int i=0; i<total[user_apply]; i++) {
				if((string)userapply[i].name==temp) {
					locate[user_apply]=i;
					i=total[user_apply];
				found=1;
				}
			}
		}
		return found;
	}
	void Agent::show() {
		program.headerDesign(userType);
		program.text(black,dark_gray,"USERNAME          CONTACT NUM    TRANSACTIONS MADE\t\t\t     \n");
		program.bar(15,0,76,223,0);cout<<endl;
		for(int i=0; i<=total[user_data]; i++) {
			cout<<setw(18)<<left<<userdata[i].name
				<<0<<setw(14)<<userdata[i].number
				<<userdata[i].td[0].dd<<" "
				<<program.month(userdata[i].td[0].mm)<<" "
				<<userdata[i].td[0].yy<<" "
				<<program.trnsct(userdata[i].td[0].tt)<<"\n";
			for(int j=1; j<=hold[user_data][i]; j++) {
				cout<<"\t\t\t\t "<<userdata[i].td[j].dd<<" "<<program.month(userdata[i].td[j].mm)<<" "<<userdata[i].td[j].yy<<" "<<program.trnsct(userdata[i].td[j].tt)<<"\n";
			}
			program.bar(15,0,76,196,0);cout<<endl;
		}
		program.bar(15,0,76,223,0);cout<<endl;
		system("pause");
	}
//	void Agent::del() {
//	}
//	user Agent::change() {
//	}
	user Agent::add() {
		program.headerDesign(userType);
		program.text(black,dark_gray,"USERNAME             ADDRESS                      CONTACT NUM    DATE APPLIED\n");
		program.bar(15,0,76,223,0);cout<<endl;
		
		for(int i=0; i<total[user_apply]; i++) {
			cout<<setw(21)<<left<<userapply[i].name
				<<setw(29)<<userapply[i].address
				<<0<<setw(15)<<userapply[i].number
				<<userapply[i].td[hold[user_apply][i]].dd<<" "
				<<program.month(userapply[i].td[hold[user_apply][i]].mm)<<" "
				<<userapply[i].td[hold[user_apply][i]].yy<<endl;
		}
		program.bar(15,0,76,223,0);
		if(strlen(userapply[total[user_apply]].name)==0) {
			cout<<"\nWho do you want to approve? ";
			cin.ignore();
			cin.getline(usertemp[0].name,30);
			person.find(user_apply,(string)usertemp[0].name);
			if(found==1) {
				total[user_data]++;
				userdata[total[user_data]]=userapply[locate[user_apply]];
				program.text(light_green,black,"\t\t\t\tApplication Approved.\n");
				program.bar(white,black,76,223,0);cout<<endl;
				person.write();
				person.read();
				Sleep(1000);
			}
			else {
				program.text(light_red,black,"Username not found!!!\n");
				person.add();
			}
		}
		if(strlen(userapply[total[user_apply]].name)>0) {
			cout<<"\nThere are no applicants right now.\n";
			program.bar(white,black,76,223,0);cout<<endl;
		}
		system("pause");
		return *userdata;
	}
//	user Agent::get() {
//	}
	user Agent::Apply::form() {
		program.headerDesign(userType);
			total[user_apply]++;
			strcpy(userapply[total[user_apply]].name, usertemp[0].name);
			cout<<"Please Input the Necessary Information. \n";
			cout<<"                               Address: ";
			cin.getline(userapply[total[user_apply]].address,100);
			if(strlen(userapply[total[user_apply]].address)<=0) {
				program.validator();
				person.apply.form();
			}
			cout<<"                        Contact Number: ";
			cin>>userapply[total[user_apply]].number;
			if(userapply[total[user_apply]].number<9000000000 || cin.fail() || userapply[total[user_apply]].number>9999999999) {
				program.validator();
				person.apply.form();
			}
			
			program.headerDesign(userType);
			cout<<"Once your application has been approved,\n";Sleep(1000);
			cout<<"You can use this ";
			program.text(light_green,black,"key");
			cout<<" to login successfully.\n";Sleep(1000);
			userapply[total[user_apply]].key=program.intstr(program.keygen(9001,999));
			
			char ulc=201, urc=187, llc=200, lrc=188, col=186;
			program.text(light_green,black,string(1,ulc));
			program.bar(light_green,black,5,205,0);
			program.text(light_green,black,string(1,urc));
			cout<<"\n";
			program.text(light_green,black,string(1,col));
			cout<<" ";
			program.text(black,light_green,userapply[total[user_apply]].key);
			cout<<" ";
			program.text(light_green,black,string(1,col));
			cout<<"\n";
			program.text(light_green,black,string(1,llc));
			program.bar(light_green,black,5,205,0);
			program.text(light_green,black,string(1,lrc));cout<<endl;
			cout<<"However, you will be logged out involuntarily for program updates.\n";
			program.bar(white,black,76,223,0);cout<<endl;
			system("pause");
			
			fs.open("Applicant.csv",ios::app);
			fs<<userapply[total[user_apply]].name<<","
			  <<userapply[total[user_apply]].key<<","
			  <<userapply[total[user_apply]].address<<","
			  <<userapply[total[user_apply]].number<<","
			  <<0<<",";
			strftime(buff,sizeof(buff),"%d",&tstruct);
			fs<<buff<<",";
			strftime(buff,sizeof(buff),"%m",&tstruct);
			fs<<buff<<",";
			strftime(buff,sizeof(buff),"%Y",&tstruct);
			fs<<buff<<","
			  <<-2<<"\n";
			fs.close();
			person.apply.read();
			program.logout();
		return *userapply;
	}
	user Agent::Apply::read() {
		fs.open("Applicant.csv",ios::in);
		getline(fs,line,'\n');
		total[user_apply]=-1;
		while(!fs.eof()) {
			total[user_apply]++;
			getline(fs,cell,',');
				strcpy(userapply[total[user_apply]].name,cell.c_str());
				getline(fs,userapply[total[user_apply]].key,',');
			getline(fs,cell,',');
				strcpy(userapply[total[user_apply]].address,cell.c_str());
			getline(fs,cell,',');
				userapply[total[user_apply]].number = atoll(cell.c_str());
			getline(fs,cell,',');
				userapply[total[user_apply]].salary = atof(cell.c_str());
			hold[user_apply][total[user_apply]]=0;
			getline(fs,cell,',');
				userapply[total[user_apply]].td[hold[user_apply][total[user_apply]]].dd = atoi(cell.c_str());
			getline(fs,cell,',');
				userapply[total[user_apply]].td[hold[user_apply][total[user_apply]]].mm = atoi(cell.c_str());
			getline(fs,cell,',');
				userapply[total[user_apply]].td[hold[user_apply][total[user_apply]]].yy = atoi(cell.c_str());
			getline(fs,cell,'\n');
				userapply[total[user_apply]].td[hold[user_apply][total[user_apply]]].tt = atoi(cell.c_str());
		}
		fs.close();
		return *userapply;
	}
	
	int Instance::login() {
		program.headerDesign(userType);
		cout<<"Username: ";
		cin.getline(usertemp[0].name,50);
		if(strlen(usertemp[0].name)<=0) {
			program.validator();
			program.login();
		}
		if(strlen(usertemp[0].name)>0) {
			person.find(user_data,(string)usertemp[0].name);
			if(found==1) {
				cout<<"     Key: ";
				tmpCha=_getch();
				while(tmpCha!=13) {
					usertemp[0].key.push_back(tmpCha);
					cout<<'X';
					tmpCha=_getch();
				}
				program.headerDesign(userType);
				cout<<"\r\t\t\t  Authenticating User Login...\n";
				program.load(dark_cyan,dark_gray,light_cyan,black);
				if(usertemp[0].key==userdata[locate[user_data]].key) userType=dealer;
				else {
					program.validator();
					program.login();
				}
				program.headerDesign(userType);
			}
			else userType=common;
		}
		return userType;
	}
	void Instance::logout() {
		userType=3;
		program.headerDesign(userType);
		strcpy(usertemp[0].name,"");
		usertemp[0].key.clear();
		strcpy(usertemp[0].address,"");
		usertemp[0].number=0;
		usertemp[0].salary=0;
		usertemp[0].td[0].dd=0;
		usertemp[0].td[0].mm=0;
		usertemp[0].td[0].yy=0;
		usertemp[0].td[0].tt=0;
		cout<<"Logout Successful.\n";
		Sleep(1200);
	}
	void Instance::load(int fore1, int back1, int fore2, int back2) {
	 	cout<<"\t\t\t  ";
	 	program.bar(fore1,back1,27,177,0);
	 	cout<<"\r\t\t\t  ";
	 	program.bar(fore2,back2,27,219,program.keygen(100,0));
	}
	void Instance::chooser() {
		program.headerDesign(userType);
		if(userType==common) {
			cout<<"Hello, ";
			program.text(light_cyan,black,usertemp[0].name);
			cout<<". What would you like to do?\n";
			program.bar(15,0,76,196,0);cout<<endl;
			cout<<"[O] Order drugs\n\n[P] Apply as dealer\n\n[D] Show drugs\n\n[V] View dealers\n\n[L] Logout\n\n[X] Logout and Exit\n";
		}
		if(userType==dealer) {
			cout<<"Good day, ";
			program.text(white,black,usertemp[0].name);
			cout<<". What can I do for you\n";
			program.bar(15,0,76,196,0);cout<<endl;
			cout<<"[O] Get drugs\n[A] Add drugs\n[D] Show drugs\n\n[S] Withdraw salary\n[V] Review dealers\n[C] Review Applicants\n\n[G] Delete drugs\n[R] Report user\n[L] Logout\n[X] Logout and Exit\n";
			
		}
		if(userType==applicant) {
			cout<<"Hello again, ";
			program.text(dark_cyan,black,usertemp[0].name);
			cout<<". What are you doing here?\n";
			program.bar(15,0,76,196,0);cout<<endl;
			cout<<"[O] Order drugs\n\n[D] Show drugs\n\n[V] View dealers\n\n[L] Logout\n\n[X] Logout and Exit\n";
		}
		program.bar(15,0,76,223,0);cout<<endl;
		choice=_getch();
		if(choice=='L') program.logout();
//		if(choice=='O') transaction.get();
		if(choice=='A') transaction.add();
		if(choice=='C') person.add();
		if(choice=='D') transaction.show();
		if(choice=='V') person.show();
		if(choice=='P') person.apply.form();
//		if(choice=='S') person.chage();
		if(choice=='X') {
			program.logout();
			exit(1);
		}
	}
	int Instance::keygen(int max, int min) {
		srand(time(NULL));
		tmpNum=(rand()%max+min);
		return tmpNum;
	}
	string Instance::intstr(int tmpNum) {
		char *ptr=itoa(tmpNum,buff,10);
		temp=string(ptr);
		return temp;
	}
	void Instance::text(int fore, int back, string temp) {
		int color=(fore+(back*16));
		SetConsoleTextAttribute(hand,color);
		cout<<temp;
		SetConsoleTextAttribute(hand,7);
	}
	void Instance::headerDesign(int userType){
		system("cls");
		program.bar(15,0,76,219,0);cout<<endl;
		program.text(light_cyan,black,"\t      ______                  _                   _     \n\t      |  _  \\                | |                 | |    \n\t      | | | |_ __ _   _  __ _| |     ___  _ __ __| |____\n\t      | | | | '__| | | |/ _` | |    / _ \\| '__/ _` |_  /\n\t      | |/ /| |  | |_| | (_| | |___| (_) | | | (_| |/ / \n\t      |___/ |_|   \\__,_|\\__, \\_____/\\___/|_|  \\__,_/___|\n\t                        __/ |                          \n\t                       |___/  ");
		program.text(black,dark_gray,program.date());cout<<endl;
	  	program.bar(15,0,76,220,0);cout<<endl;
		if(userType==common) {
			program.bar(15,0,76,240,0);cout<<endl;
			program.bar(15,0,76,223,0);cout<<endl;
		}
		if(userType==dealer) {
			program.bar(light_cyan,0,76,254,0);cout<<endl;
			program.bar(15,0,76,223,0);cout<<endl;
		}
		if(userType==applicant) {
			program.bar(dark_cyan,0,76,250,0);cout<<endl;
			program.bar(15,0,76,223,0);cout<<endl;
		}
	}
	string Instance::month(int tmpNum) {
		if(tmpNum==1) return temp="Jan";
		if(tmpNum==2) return temp="Feb";
		if(tmpNum==3) return temp="Mar";
		if(tmpNum==4) return temp="Apr";
		if(tmpNum==5) return temp="May";
		if(tmpNum==6) return temp="Jun";
		if(tmpNum==7) return temp="Jul";
		if(tmpNum==8) return temp="Aug";
		if(tmpNum==9) return temp="Sep";
		if(tmpNum==10) return temp="Oct";
		if(tmpNum==11) return temp="Nov";
		if(tmpNum==12) return temp="Dec";
		else return temp="Unk";
	}
	string Instance::trnsct(int tmpNum) {
		if(tmpNum==-2) return temp="Applied as Dealer";
		if(tmpNum==-1) return temp="Logged Out";
		if(tmpNum==0) return temp="Modified the Program.";
		if(tmpNum==1) return temp="Logged In.";
		if(tmpNum==2) return temp="Added new Drugs.";
		if(tmpNum==3) return temp="Reviewed Applications.";
		if(tmpNum==4) return temp="Application Approved.";
		if(tmpNum==5) return temp="User Concerns";
		if(tmpNum==6) return temp="Request for Resignation.";
		if(tmpNum==7) return temp="Removed old Drugs.";
		if(tmpNum==8) return temp="Salary Checkout.";
		if(tmpNum==9) return temp="Relieved of Service.";
		else return temp="Transaction Redacted.";
	}
	string Instance::date() {
		strftime(buff,sizeof(buff),"%d-%m-%Y",&tstruct);
    	return buff;
	}
	void Instance::validator() {
		cin.clear();
		cin.ignore();
		program.headerDesign(userType);
		program.text(light_red,black,"Invalid Input!!!\n");
		Sleep(1200);
		program.text(light_red,black,"Try Again.");
		Sleep(1000);
		system("cls");
	}
	void Instance::bar(int fore, int back, int length, char tmpCha, int dur) {
		for (int i=0;i<=length;i++) {
	 		int color=(fore+(back*16));
			SetConsoleTextAttribute(hand,color);
			cout<<tmpCha;
	 		Sleep(dur);
		}
		SetConsoleTextAttribute(hand,7);
	}
	
	main() {																									//main function for creating files
		fp=fopen("User.csv","r");//check if User database exist
		if(fp) fclose(fp);//if file is found, file is closed
		else if(!fp) {
			fs.open("User.csv",ios::out);
			//create default user database if not yet existing
			fs<<"Username,Key,Address,Contact Number,Salary,Day,Month,Year,Transaction\n"
			  <<"K,1234,Pulilan Bulacan,9066144048,5000,1,11,1998,0\n"
			  <<",,,,,2,6,2016,2\n"
			  <<"Y,5678,Baliuag Bulacan,9776543808,2000,3,4,1997,0\n";
			fs.close();
		}
		fp=fopen("Applicant.csv","r");//check if User database exist
		if(fp) fclose(fp);//if file is found, file is closed
		else if(!fp) {
			fs.open("Applicant.csv",ios::out);
			//create default user database if not yet existing
			fs<<"Username,Key,Address,Contact Number,Salary,Day,Month,Year,Transaction\n";
			fs.close();
		}
		fp=fopen("Deal.csv","r");//check if Deal database exist
		if(fp) fclose(fp);//if file is found, file is closed
		else if(!fp) {
			fs.open("Deal.csv",ios::out);
			//create default deal database if not yet existing
			fs<<"Drug Name,Drug ID,Price,Effect,Effectiveness\n"
			  <<"Biogesic,1000,100,cures headache,98.00\n"
			  <<"Antibiotic,2000,300,cures fever,73.67\n"
			  <<",,,bacterial resistance,1.9\n"
			  <<"Morphine,7777,5000,sleepiness,67.42\n"
			  <<",,,gets you high,87.6\n"
			  <<",,,painkiller,99.97\n"
			  <<",,,WHOOOO!!!,999.99\n";
			fs.close();
		}
		cout<<"\n\n\t\t\t  ";
		program.text(black,light_green,"...Loading... Please Wait...");cout<<endl;
		program.load(dark_green,black,light_green,black);
		person.apply.read();
		transaction.read();
		person.read();
		userType=3;
		while(1) {
			program.login();
			program.chooser();
		}
		return 0;
	}
