#include <iostream>
#include <stdio.h>
#include<vector>
#include <stdlib.h>
#include <ctime>
#include<utility>
#include <unistd.h>
#include <bits/stdc++.h>

using namespace std;

//initializing global variables
long nodes,prunes;
int player, n, time_left, kay;
vector<pair<int,int> > my_rings;
vector<pair<int,int> > other_rings; //position of opponent's rings

int rings_left_to_place ; //six
int position_on_line[8][42][6]; // six
int max_depth =4;
long stopping_factor=0;
int all_my_sequences[5][200][6]; //six
int all_other_sequences[5][200][6]; //six
clock_t t1;
// int num_of_moves=0;
// int max_moves=100;
vector<vector<int>> threatened_markers;
int threat_count=2;

struct Config{
	vector<vector<int> > board;
	vector<pair<int,int> > my_rings;
	vector<pair<int,int> > other_rings;
	int num_my_markers = 0;
	int num_other_markers = 0;
};
double min_tree(Config& config, int depth, pair<int,int> arr[]);
float min_float(float a, float b)
{
	if (a < b) return a;
	return b;
}
float max_float(float a, float b)
{
	if (a > b) return a;
	return b;
}
bool greater_float(pair<float,vector<int> > a, pair<float,vector<int> > b){
  return a.first > b.first;
}
double min_tree(Config& config, int depth, pair<int,int> arr[], float alpha, float beta);
vector<vector<int> > find_k_in_a_specific_direction(int k, int dir_num ,pair<int,int> arr[], Config& config){
	
	// returns vector containing (type,dir_num, start_point_hex, start_point_ang,end_point_hex, end_point_ang);
	int i,counter,last_type,type,points;
	int row_limit= 2*n+1;
	int eff= dir_num%(2*n+1);
	if(eff%(2*n)==0) points= n-1;
	else if(eff<n) points= eff+n+1;
	else if(eff>n) points= 3*n -eff +1;
	else points= 2*n -1;
	vector<vector<int> > answer; 
	//function that tells us with a few checks if it is not possible 
	last_type=-1;
	type=-1;

	for(i=0;i<points;i++){
		pair<int,int> temp= (arr[dir_num*row_limit+ i ]);
		int hex_num= get<0>(temp);
		int ang_num= get<1>(temp);
		type= config.board[hex_num][ang_num];
		if(type==last_type && (type==2||type==3)){
			counter++;
		}
		else if(type!= last_type && (type==2||type==3)){
			last_type= type;
			counter=1;
		}
		else{
			last_type=type;
			counter=0;
		}
		if(counter>=k){
			vector<int> sub_answer;
			sub_answer.push_back(type);
			sub_answer.push_back(dir_num);
			temp=arr[dir_num*row_limit+ i-k+1];
			sub_answer.push_back(get<0>(temp));
			sub_answer.push_back(get<1>(temp));
			sub_answer.push_back(hex_num);
			sub_answer.push_back(ang_num);
			answer.push_back(sub_answer);
		}
	}
	return answer;
}
vector<vector<int> > find_k_dash_in_a_specific_direction(int k, bool my, int dir_num ,pair<int,int> arr[], Config &config){
	// naive implementation
	//returns vector containing (type,dir_num, start_point_hex, start_point_ang,end_point_hex, end_point_ang);
	
	int i,counter,last_type,type,points;
	int row_limit= 2*n+1;
	int eff= dir_num%(2*n+1);
	if(eff%(2*n)==0) points= n-1;
	else if(eff<n) points= eff+n+1;
	else if(eff>n) points= 3*n -eff +1;
	else points= 2*n -1;
	vector<vector<int> > answer; //khali hai abhi
	// if(points<10 && ){
	// 	//checking ki possible bhi hai
	// }
	//scope to optimise , but i think it would be insignificant 
	//later make a function that tells us with a few checks if it is pakka not possible but cant say if its true
	// last_type=-1;
	// type=-1;
	counter=0;
	int marker,ring;
	int last_ring=0;
	int ring_counter=0;
	if(my){
		marker=2;
		ring=0;
		// last_type=marker;
	}
	else{
		marker= 3;
		ring=1;
	}
	// last_type=marker;
	bool exceed=false;
	bool broken= false;
	// int count_ring=0;
	// int temp_type;
	for(i=0;i<points;i++){
		pair<int,int> temp= (arr[dir_num*row_limit+ i ]);
		int hex_num= get<0>(temp);
		int ang_num= get<1>(temp);
		type= config.board[hex_num][ang_num];

		if(type==marker){
			counter++;
			// if(counter>k) exceed=true;
			// broken=false;
			// temp_type=marker;
		}
		else if(type==ring){
			if(ring_counter==0){
				ring_counter++;
				counter++;
				last_ring=i;
			}
			else{
				counter= i- last_ring;
				last_ring= i;

			}
		}
		else{
			counter=0;
			ring_counter=0;
		}

		if(counter>=k){
			vector<int> sub_answer;
			sub_answer.push_back(marker);
			sub_answer.push_back(dir_num);
			temp=arr[dir_num*row_limit+ i-k+1];
			sub_answer.push_back(get<0>(temp));
			sub_answer.push_back(get<1>(temp));
			sub_answer.push_back(hex_num);
			sub_answer.push_back(ang_num);
			answer.push_back(sub_answer);
		}
		// else if(broken && exceed ){
		// 	vector<int> sub_answer;
		// 	sub_answer.push_back(marker);
		// 	sub_answer.push_back(dir_num);
		// 	temp=arr[dir_num*row_limit+ i-1-k+1];
		// 	sub_answer.push_back(get<0>(temp));
		// 	sub_answer.push_back(get<1>(temp));
		// 	temp=arr[dir_num*row_limit+ i-1];
		// 	sub_answer.push_back(get<0>(temp));
		// 	sub_answer.push_back(get<1>(temp));
		// 	answer.push_back(sub_answer);
		// 	exceed=false;
		// 	broken=false;
		// }
		// last_type=type;


	}
	// cerr<<"nhi hai"<<endl;
	return answer;
}

void find_all_seq_in_a_direction(int dir_num ,pair<int,int> arr[], Config &config){
	//int i,counter,last_type,type,points;
	// cerr<<"enter baby func"<<endl;
	int points;
	int row_limit= 2*n+1;
	int eff= dir_num%(2*n+1);
	if(eff%(2*n)==0) points= n-1;
	else if(eff<n) points= eff+n+1;
	else if(eff>n) points= 3*n -eff +1;
	else points= 2*n -1;
	// vector<vector<int> > answer;
	// vector<vector<vector<int>>> all_my_seq;
	// vector<vector<vector<int>>> all_other_seq;

	// for(int j=2; j<=5; j++){
	// 	vector<vector<int>> temp1;
	// 	all_my_seq.push_back(temp1);
	// 	vector<vector<int>> temp2;
	// 	all_other_seq.push_back(temp2);
	// }

	bool my_start=false;
	bool other_start=false;
	bool my_just_broken= false;
	bool other_just_broken= false;
	int my_counter=0;
	int other_counter=0;
	int my_ring_counter=0;
	int other_ring_counter=0;
	int type;
	int my_last_index;
	int other_last_index;
	int i,j;


	//cerr<<"no fault in initialisation"<<endl;
	for(i=0;i<points;i++){
		pair<int,int> temp= (arr[dir_num*row_limit+ i ]);
		int hex_num= get<0>(temp);
		int ang_num= get<1>(temp);
		type= config.board[hex_num][ang_num];
		// cerr<<"type is "<<type<<endl;
		// cerr<<"hex is "<<hex_num<<"and ang is "<<ang_num<<endl;
		if(type==2 || type==0){
			if(my_counter==0){
				my_start=true;
				my_just_broken=false;
				other_just_broken=true;
				other_ring_counter=0;
			}
			my_counter++;
			if(type==0){
				//ring
				if(my_ring_counter==0){
					my_last_index=i;
					my_ring_counter=1;
				}
				else{
					//my_ring_counter=1;
					my_counter= i - my_last_index;
					my_last_index=i;
				}
			}
		}
		else if(type==3 || type==1){
			if(other_counter==0){
				other_start=true;
				other_just_broken=false;
				my_just_broken=true; // check for opp side seq and then kill
				my_ring_counter=0;
			}
			other_counter++;
			if(type==1){
				if(other_ring_counter==0){
					other_ring_counter=1;
					other_last_index=i;
				}
				else{
					other_counter= i- other_last_index;
					other_last_index=i;
				}
			}
		}
		else{
			//blank space
			my_just_broken=true;
			other_just_broken=true;
			my_ring_counter=0;
			other_ring_counter=0;
		}
		////////
		if(!my_start && !other_start){
			// do nothing
		}
		else if(my_start && !my_just_broken && my_counter>1){
			//add next biggest
			// => i - c +1 to i 
			if(my_counter >= kay){
				// vector<int> t;
				// t.push_back(2);
				// t.push_back(dir_num);
				temp=arr[dir_num*row_limit+ i-(kay-1)];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// t.push_back(hex_num);
				// t.push_back(ang_num);
				// all_my_seq[3].push_back(t);
				int next= all_my_sequences[kay-2][0][0]+1;
				all_my_sequences[kay-2][next][0]=2;
				all_my_sequences[kay-2][next][1]=dir_num;
				all_my_sequences[kay-2][next][2]=temp.first;
				all_my_sequences[kay-2][next][3]=temp.second;
				all_my_sequences[kay-2][next][4]=hex_num;
				all_my_sequences[kay-2][next][5]=ang_num;
				all_my_sequences[kay-2][0][0]=next;

			}
			else{

				// vector<int> t;
				// t.push_back(2);
				// t.push_back(dir_num);
				temp=arr[dir_num*row_limit+ i-my_counter+1];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// t.push_back(hex_num);
				// t.push_back(ang_num);
				// all_my_seq[my_counter-2].push_back(t);
				//my_counter-2
				int next= all_my_sequences[my_counter-2][0][0]+1;
				all_my_sequences[my_counter-2][next][0]=2;
				all_my_sequences[my_counter-2][next][1]=dir_num;
				all_my_sequences[my_counter-2][next][2]=temp.first;
				all_my_sequences[my_counter-2][next][3]=temp.second;
				all_my_sequences[my_counter-2][next][4]=hex_num;
				all_my_sequences[my_counter-2][next][5]=ang_num;
				all_my_sequences[my_counter-2][0][0]=next;			
			}

		}
		else if(other_start && !other_just_broken && other_counter>1){

			if(other_counter >= kay){
				// vector<int> t;
				// t.push_back(3);
				// t.push_back(dir_num);
				temp=arr[dir_num*row_limit+ i-(kay-1)];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// t.push_back(hex_num);
				// t.push_back(ang_num);
				//all_other_seq[3].push_back(t);

				int next= all_other_sequences[kay-2][0][0]+1;
				all_other_sequences[kay-2][next][0]=3;
				all_other_sequences[kay-2][next][1]=dir_num;
				all_other_sequences[kay-2][next][2]=temp.first;
				all_other_sequences[kay-2][next][3]=temp.second;
				all_other_sequences[kay-2][next][4]=hex_num;
				all_other_sequences[kay-2][next][5]=ang_num;
				all_other_sequences[kay-2][0][0]=next;
			}
			else{
				// vector<int> t;
				// t.push_back(3);
				// t.push_back(dir_num);
				temp=arr[dir_num*row_limit+ i-other_counter+1];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// t.push_back(hex_num);
				// t.push_back(ang_num);
				// all_other_seq[other_counter-2].push_back(t);


				int next= all_other_sequences[other_counter-2][0][0]+1;
				all_other_sequences[other_counter-2][next][0]=3;
				all_other_sequences[other_counter-2][next][1]=dir_num;
				all_other_sequences[other_counter-2][next][2]=temp.first;
				all_other_sequences[other_counter-2][next][3]=temp.second;
				all_other_sequences[other_counter-2][next][4]=hex_num;
				all_other_sequences[other_counter-2][next][5]=ang_num;
				all_other_sequences[other_counter-2][0][0]=next;				
			}
		}

		else if(my_start && my_just_broken){
			//add all abckwards
			int counter;
			if(my_counter>kay) counter=kay+1; //POTENTIAL ERROR
			else counter= my_counter; 
			for(j=2;j<counter;j++){
				//add one by one
				// cerr<<"yahan aana chahiye"<<endl;
				// cerr<<"hex "<<hex_num<<endl;
				// cerr<<"ang "<<ang_num<<endl;
				// vector<int> t;
				// t.push_back(2);
				// t.push_back(dir_num);
				// temp=arr[dir_num*row_limit+ i-j];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// temp=arr[dir_num*row_limit+ i-1];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// all_my_seq[j-2].push_back(t);	

				int next= all_my_sequences[j-2][0][0]+1;
				all_my_sequences[j-2][next][0]=2;
				all_my_sequences[j-2][next][1]=dir_num;
				temp=arr[dir_num*row_limit+ i-j];
				all_my_sequences[j-2][next][2]=temp.first;
				all_my_sequences[j-2][next][3]=temp.second;
				temp=arr[dir_num*row_limit+ i-1];
				all_my_sequences[j-2][next][4]=temp.first;
				all_my_sequences[j-2][next][5]=temp.second;
				all_my_sequences[j-2][0][0]=next;
			}
			my_start=false;
			my_counter=0;
			my_just_broken=false;
		}
		else if(other_start && other_just_broken){

			int counter;
			if(other_counter>kay) counter=kay+1;
			else counter= other_counter; 
			for(j=2;j<counter;j++){
				//add one by one
				// vector<int> t;
				// t.push_back(3);
				// t.push_back(dir_num);
				// temp=arr[dir_num*row_limit+ i-j];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// temp=arr[dir_num*row_limit+ i-1];
				// t.push_back(temp.first);
				// t.push_back(temp.second);
				// all_other_seq[j-2].push_back(t);	


				int next= all_other_sequences[j-2][0][0]+1;
				all_other_sequences[j-2][next][0]=3;
				all_other_sequences[j-2][next][1]=dir_num;
				temp=arr[dir_num*row_limit+ i-j];
				all_other_sequences[j-2][next][2]=temp.first;
				all_other_sequences[j-2][next][3]=temp.second;
				temp=arr[dir_num*row_limit+ i-1];
				all_other_sequences[j-2][next][4]=temp.first;
				all_other_sequences[j-2][next][5]=temp.second;
				all_other_sequences[j-2][0][0]=next;
			}			
			other_start=false;
			other_counter=0;
			other_just_broken=false;
		}
		else{
			//do nothing
		}

		if(i==points-1){
			if(my_start){
				int counter;
				if(my_counter>kay) counter=kay+1;
				else counter= my_counter; 
				for(j=2;j<counter;j++){
					//add one by one
					// vector<int> t;
					// t.push_back(2);
					// t.push_back(dir_num);
					// temp=arr[dir_num*row_limit+ i-j+1];
					// t.push_back(temp.first);
					// t.push_back(temp.second);
					// // temp=arr[dir_num*row_limit+ i];
					// t.push_back(hex_num);
					// t.push_back(ang_num);
					// all_my_seq[j-2].push_back(t);

					int next= all_my_sequences[j-2][0][0]+1;
					all_my_sequences[j-2][next][0]=2;
					all_my_sequences[j-2][next][1]=dir_num;
					temp=arr[dir_num*row_limit+ i-j+1];
					all_my_sequences[j-2][next][2]=temp.first;
					all_my_sequences[j-2][next][3]=temp.second;
					// temp=arr[dir_num*row_limit+ i-1];
					all_my_sequences[j-2][next][4]=hex_num;
					all_my_sequences[j-2][next][5]=ang_num;
					all_my_sequences[j-2][0][0]=next;	


				}
				// my_start=false;
				// my_counter=0;				
			}
			else if(other_start){
				int counter;
				if(other_counter>kay) counter=kay+1;
				else counter= other_counter; 
				for(j=2;j<counter;j++){
					//add one by one
					// vector<int> t;
					// t.push_back(3);
					// t.push_back(dir_num);
					// temp=arr[dir_num*row_limit+ i-j+1];
					// t.push_back(temp.first);
					// t.push_back(temp.second);
					// // temp=arr[dir_num*row_limit+ i];
					// t.push_back(hex_num);
					// t.push_back(ang_num);
					// all_other_seq[j-2].push_back(t);

					int next= all_other_sequences[j-2][0][0]+1;
					all_other_sequences[j-2][next][0]=3;
					all_other_sequences[j-2][next][1]=dir_num;
					temp=arr[dir_num*row_limit+ i-j+1];
					all_other_sequences[j-2][next][2]=temp.first;
					all_other_sequences[j-2][next][3]=temp.second;
					// temp=arr[dir_num*row_limit+ i-1];
					all_other_sequences[j-2][next][4]=hex_num;
					all_other_sequences[j-2][next][5]=ang_num;
					all_other_sequences[j-2][0][0]=next;						
				}
			}
			else{
				//kuch bhi dalne ki zaroorat nhi hai
			}
		}

	}
	// cerr<<"exit baby func"<<endl;
	// return make_pair(all_my_seq, all_other_seq);

}

void find_all_seq(pair<int,int> arr[], Config &config){
	//just alters the global array
	int line_num=0;
	int total_lines= 6*n +3;
	
	for(line_num=0;line_num<total_lines;line_num++){

		find_all_seq_in_a_direction(line_num,arr,config);

	}

}

vector<vector<int> > find_k_throughout(int k,pair<int,int> arr[], Config& config){
	//the board(my config) is a global variable
	//the function returns a vector containing vectors of 6 ints i.e. one type 2 coordinates corresponding to sequences of 5 same markers
	//i have to check the (2*n+1)*3 lines available
	int line_num=0;
	int total_lines= 6*n +3;
	vector<vector<int> > final_answer;
	for(line_num=0;line_num<total_lines;line_num++){
		vector<vector<int> > temp = find_k_in_a_specific_direction(k,line_num,arr,config);
		for(int j=0;j<temp.size();j++){
			final_answer.push_back(temp[j]);
		}
	}
	return final_answer;
}
vector<vector<int>> find_k_dash_throughout(int k,bool my,pair<int,int> arr[], Config& config){
	//the board(my config) is a global variable
	//the function returns a vector containing vectors of 6 ints i.e. one type 2 coordinates corresponding to sequences of 5 same markers
	//i have to check the (2*n+1)*3 lines available
	int line_num=0;
	int total_lines= 6*n +3;
	vector<vector<int> > final_answer;
	for(line_num=0;line_num<total_lines;line_num++){
		vector<vector<int> > temp = find_k_dash_in_a_specific_direction(k,my,line_num,arr,config);
		for(int j=0;j<temp.size();j++){
			final_answer.push_back(temp[j]);
		}
	}
	return final_answer;
}

int left_best_jump(int line, int index, pair<int,int> arr[], Config &config){
	//returns best jump for a ring 
	bool start=false;
	bool possible=false;
	// int counter=index;
	int row_limit=2*n+1;
	int type;
	int i;
	for(i=index-1; i>=0;i--){
		type= config.board[arr[line*row_limit+i].first][arr[line*row_limit+i].second];
		if(type==1 || type==0) break;
		else if(start&& type!= -1){
		}
		else if(start && type==-1){
			possible=true;
			break;	
		}
		else if(!start && type==-1 ){
			// counter--;
		}
		else if(!start){
			start=true;
			// counter--;
		}
	}
	if(possible) return i;
	else return index;
}

int right_best_jump(int line, int index, pair<int,int> arr[], Config &config){
	//returns best jump for a ring 
	bool start=false;
	bool possible=false;
	int type;
	// int counter=index;
	int row_limit=2*n+1;
	int i;
	for( i=index+1; i<row_limit;i++){
		if(arr[line*row_limit+i].first==-1) break;
		type= config.board[arr[line*row_limit+i].first][arr[line*row_limit+i].second];
		if(type==1 || type==0) break;
		else if(start&& type!= -1){
		}
		else if(start && type==-1){
			possible=true;
			break;	
		}
		else if(!start && type==-1 ){
		}
		else if(!start){
			start=true;
		}
	}
	if(possible) return i;
	else return index;
}


void threatened(Config &config, bool my_attack,  pair<int,int> arr[]){

	int row_limit= 2*n+1;
	bool start=false;	
	bool possible=false;
	int left_best,right_best;
	int marker;
	vector<pair<int,int> > rings;
	int to_place;
	///my =true => that i am looking at the threat i cause
	if(my_attack){
		rings= config.my_rings;
		marker=3;
		to_place= threat_count+1;
		// the_ring=1;
	}
	else{
		rings= config.other_rings;
		marker=2;
		to_place=threat_count;
		// the_ring=0;

	}
	for(int i=0; i<rings.size(); i++){
		//cerr<<"ho"<<endl;
		pair<int,int> ring= rings[i];
		int hex= ring.first;
		int ang= ring.second;
		// cerr<<endl;
		for(int j=0;j<6;j=j+2){
			//cerr<<"hee"<<endl;
			int line= position_on_line[hex][ang][j];
			int index= position_on_line[hex][ang][j+1];
			// cerr<<"line is "<<line<<endl;
			// cerr<<"ind is "<<index<<endl;

			left_best= left_best_jump(line,index,arr,config);
			right_best= right_best_jump(line,index,arr,config);
			// cerr<<"lb is "<<left_best<<endl;
			// cerr<<"rb is "<<right_best<<endl;
			for(int k=left_best;k<=right_best;k++){
				int type= config.board[arr[line*row_limit+k].first][arr[line*row_limit+k].second];
				//cerr<<"type is "<<type<<endl;
				if(type==marker){
					threatened_markers[arr[line*row_limit+k].first][arr[line*row_limit+k].second]=to_place;
				}
			}

		}
	}
	//after this you have to add 2 to to_pplace
	// now using the ring positions of my opponent calc the threats
	// return ans;
}

//features
int index_given_rest(int hex, int ang, int line){
	for(int i=0;i<6;i=i+2){
		if(position_on_line[hex][ang][i]==line){
			return position_on_line[hex][ang][i+1];
		}
	}
	return -100000;
}
class Evaluation{
	//feature funcs
	public:

		static int my_markers(Config &config, pair<int,int> arr[]){
			int counter =0;
			int marker= 2;
			int i,j,k;
			if(config.board[0][0]==marker) counter++;
			for(i=0;i<=n;i++){
				for(j=0;j<6*i;j++){
					if(config.board[i][j]==marker) counter++;
				}
			}
			return counter;
		}
		static int other_markers(Config &config, pair<int,int> arr[]){
			int counter =0;
			int marker= 3;
			int i,j,k;
			if(config.board[0][0]==marker) counter++;
			for(i=0;i<=n;i++){
				for(j=0;j<6*i;j++){
					if(config.board[i][j]==marker) counter++;
				}
			}
			return counter;
		}

		
		static bool no_threat(Config &config, pair<int,int> arr[], int marker_to_be_checked, int num, int seq[6]){
			//returns true iff its a seq of my_markers which is unthreatened
			//given everything i have to check the threats,
			//placement contains the indices I have to after the start point for each vector in already found and check for threats;
			// bool answer= true;

			int type= seq[0];
			if (type!=marker_to_be_checked) {
				return false;
				cerr<<"you have given me a wrong seq for threat analysis"<<endl;
			} 
			int i;
			// int index;
			int line= seq[1];
			for(i=0;i<num;i++){
				// index= placement[i];
				pair<int,int> temp= arr[(2*n+1)*line+ i];
				if(threatened_markers[temp.first][temp.second] == threat_count + type - 4) return false;
			}
			return true;

		}
		static int secure_general(Config &config, pair<int,int> arr[], int marker_to_be_checked, int num, int already_found[200][6])
		{
			int i,j,k;
			int counter=0;
			for(i=1;i<=already_found[0][0];i++){
				if(no_threat(config, arr,  marker_to_be_checked,num, already_found[i])) counter++; 
			}
			return counter;
		}
		static int find_my_sec_2(Config &config, pair<int,int> arr[],  int already_found[200][6]){
			// vector<int> placement(2) ;
			// placement={0,1};
			return secure_general(config, arr,2,2,already_found);
		}
		static int find_other_sec_2(Config &config, pair<int,int> arr[], int already_found[200][6]){
			return secure_general(config, arr,3,2,already_found);
		}
		static int find_my_sec_3(Config &config, pair<int,int> arr[],  int already_found[200][6]){
			return secure_general(config, arr,2,3,already_found);
		}
		static int find_other_sec_3(Config &config, pair<int,int> arr[],  int already_found[200][6]){
			return secure_general(config, arr,3,3,already_found);
		}

		static int find_my_sec_4(Config &config, pair<int,int> arr[], int already_found[200][6]){
			return secure_general(config, arr,2,4,already_found);
		}
		static int find_other_sec_4(Config &config, pair<int,int> arr[],  int already_found[200][6]){
			return secure_general(config, arr,3,4,already_found);
		}
		static int find_my_sec_5(Config &config, pair<int,int> arr[], int already_found[200][6]){
			return secure_general(config, arr,2,5,already_found);
		}
		static int find_other_sec_5(Config &config, pair<int,int> arr[], int already_found[200][6]){
			return secure_general(config, arr,3,5,already_found);
		}
		static int find_my_sec_6(Config &config, pair<int,int> arr[], int already_found[200][6]){
			return secure_general(config, arr,2,6,already_found);
		}
		static int find_other_sec_6(Config &config, pair<int,int> arr[], int already_found[200][6]){
			return secure_general(config, arr,3,6,already_found);
		}
		static int num_of_my_rings(Config &config){
			return config.my_rings.size();
		}
		static int num_of_other_rings(Config &config){
			return config.other_rings.size();
		}
		static double evaluate(Config &config, pair<int,int> arr[]){

			// vector<vector<vector<int>>> all_my_seq(4);
			// vector<vector<vector<int>>> all_other_seq(4);

			// for(int j=2; j<=5; j++){
			// 	vector<vector<int>> temp1;
			// 	all_my_seq.push_back(temp1);
			// 	vector<vector<int>> temp2;
			// 	all_other_seq.push_back(temp2);
			// }

			for(int i=0; i<n-1;i++){ //six
				for(int j=0;j<6;j++){ //six
					all_my_sequences[i][0][j]=0;
					all_other_sequences[i][0][j]=0;
				}
			}


			find_all_seq(arr,config);
			// pair<vector<vector<vector<int>>>,vector<vector<vector<int>>>> p = find_all_seq(arr,config);



			// vector<vector<int> > found_my_2_store= find_k_dash_throughout(2,true,arr,config); // board time

			// vector<vector<int> > found_other_2_store= find_k_dash_throughout(2,false,arr,config); // board time
			// vector<vector<int> > found_my_3_store= find_k_dash_throughout(3,true,arr,config); // board time
			// vector<vector<int> > found_other_3_store= find_k_dash_throughout(3,false,arr,config); //board time
			// vector<vector<int> > found_my_4_store= find_k_dash_throughout(4,true,arr,config); //board time
			// vector<vector<int> > found_other_4_store= find_k_dash_throughout(4,false,arr,config); //bpard time
			// vector<vector<int> > found_my_5_store= find_k_dash_throughout(5,true,arr,config); //board time
			// vector<vector<int> > found_other_5_store= find_k_dash_throughout(5,false,arr,config);		//board time	

			// int found_my_1= config.num_my_markers;
			// int found_other_1= config.num_other_markers;
			// // int found_my_1= my_markers(config,arr);
			// // int found_other_1= other_markers(config,arr);
			// int found_my_2= found_my_2_store.size();
			// int found_other_2= found_other_2_store.size();
			// int found_my_3= found_my_3_store.size();
			// int found_other_3= found_other_3_store.size();
			// int found_my_4= found_my_4_store.size(); 
			// int found_other_4= found_other_4_store.size();
			// int found_my_5= found_my_5_store.size();
			// int found_other_5= found_other_5_store.size();

			// //debug
			// cerr<<"found my 2 - "<< found_my_2<<endl;
			// found_my_2 = p.first[0].size();
			// cerr<<"found my 2 - "<< found_my_2<<endl;

			// for(int i=0; i< p.first[0].size();i++){
			// 	for(int j=0; j< 6;j++){
			// 		cerr<<p.first[0][i][j]<<"  ";
			// 	}
			// 	cerr<<endl;
			// }
			// cerr<<endl;

			// cerr<<"found other 2 - "<< found_other_2<<endl;
			// found_other_2 = p.second[0].size();
			// cerr<<"found other 2 - "<< found_other_2<<endl;

			// cerr<<"found my 3 - "<< found_my_3<<endl;
			// found_my_3 = p.first[1].size();
			// cerr<<"found my 3 - "<< found_my_3<<endl;

			// cerr<<"found other 3 - "<< found_other_3<<endl;
			// found_other_3 = p.second[1].size();
			// cerr<<"found other 3 - "<< found_other_3<<endl;

			// cerr<<"found my 4 - "<< found_my_4<<endl;
			// found_my_4 = p.first[2].size();
			// cerr<<"found my 4 - "<< found_my_4<<endl;

			// cerr<<"found other 4 - "<< found_other_4<<endl;
			// found_other_4 = p.second[2].size();
			// cerr<<"found other 4 - "<< found_other_4<<endl;
			
			int found_my_1 = config.num_my_markers;

			int found_my_2 = all_my_sequences[0][0][0];
			int found_my_3 = all_my_sequences[1][0][0];
			int found_my_4 = all_my_sequences[2][0][0];
			int found_my_5 = all_my_sequences[3][0][0];
			int found_my_6, found_other_6;
			if(kay==6)  found_my_6 = all_my_sequences[4][0][0];

			int found_other_1 =config.num_other_markers;
			int found_other_2=all_other_sequences[0][0][0];
			int found_other_3=all_other_sequences[1][0][0];
			int found_other_4=all_other_sequences[2][0][0];
			int found_other_5=all_other_sequences[3][0][0];
			if(kay==6) found_other_6= all_other_sequences[4][0][0];

			//six - add the other 4 numbers with app weights

			int i,j,k;

			threatened(config, false, arr); //board + 50 time
			threatened(config, true, arr); // board + 50 time
			threat_count=threat_count+2;

			int found_my_sec_1=0;
			int found_other_sec_1=0;
			int found_my_threatened=0;
			int found_other_threatened=0;


			for(i=1; i<=n;i++){
				for(j=0;j<6*i;j++){
					//cerr<<opponents_threat[i][j]<<"  ";
					if(threatened_markers[i][j]==threat_count-2) found_my_threatened++;//abhi counting threatened vale
					if(threatened_markers[i][j]==threat_count-1)  found_other_threatened++;
				}
				// cerr<<endl;
			}

			//cerr<<"hey3"<<endl;
			//cerr<<endl;
			// for(i=1; i<=n;i++){
			// 	for(j=0;j<6*i;j++){
			// 		// cerr<<my_threat[i][j]<<"  ";
			// 		//if(opponents_threat[i][j]) found_my_sec_1++;//abhi counting threatened vale
			// 		 if(my_threat[i][j])  found_other_sec_1++;
			// 	}
			// 	// cerr<<endl;
			// }
			found_my_sec_1= found_my_1-found_my_threatened;
			found_other_sec_1= found_other_1- found_other_threatened;
			
			//cerr<<"hey"<<endl; //my attack on opponent matlab opponent ke markers ka pe threat hai ya nhi is stored
			int found_my_sec_2= find_my_sec_2(config,arr, all_my_sequences[0] );
			int found_other_sec_2	= find_other_sec_2(config, arr,all_other_sequences[0]);
			int found_my_sec_3= find_my_sec_3(config,arr,  all_my_sequences[1]);
			int found_other_sec_3	= find_other_sec_3(config, arr,  all_other_sequences[1]);
			int found_my_sec_4= find_my_sec_4(config, arr,  all_my_sequences[2] );
			int found_other_sec_4	= find_other_sec_4(config, arr, all_other_sequences[2]);
			int found_my_sec_5= find_my_sec_5(config,arr, all_my_sequences[3]);
			int found_other_sec_5	= find_other_sec_5(config, arr, all_other_sequences[3]);
			
			int found_my_sec_6, found_other_sec_6;

			if(kay==6){
				found_my_sec_6= find_my_sec_6(config,arr, all_my_sequences[4]);
				found_other_sec_6	= find_other_sec_6(config, arr, all_other_sequences[4]);			
			}
	

			//cerr<<"hey4"<<endl;
			// int found_my_sec_2G2= find_my_sec_2G2(config,arr, opponents_threat, found_my_2G2_store );
			// int found_other_sec_2G2	= find_other_sec_2(config, arr, my_threat, found_other_2G2_store);		

			double val=0.0;
			// rajas_doesnt_believe_in_me= rajas_doesnt_believe_in_me+ 3*found_my_sec_2 - 3*found_other_sec_2+ found_my_2 - found_other_2;	
			//cerr<<endl;


			



			///////////////////////////////
			val= val + (1.0)*(found_my_1) ;
			// cerr<<"found_my_1 "<<found_my_1<<endl;

			val= val + (-1.0)*(found_other_1) ;
			// cerr<<"found_other_1 "<<found_other_1<<endl;

			val= val + (4.0)*(found_my_2) ;
			// cerr<<"found_my_2 "<<found_my_2<<endl;
	
			val= val + (-4.0)*(found_other_2) ;
			// cerr<<"found_other_2 "<<found_other_2<<endl;
	
			val= val + (16.0)*(found_my_3) ;
			// cerr<<"found_my_3 "<<found_my_3<<endl;
	
			val= val + (-16.0)*(found_other_3) ;
		    // cerr<<"found_other_3 "<<found_other_3<<endl;
	
			val= val + (64.0)*(found_my_4);
			// cerr<<"found_my_4 "<<found_my_4<<endl;
	
			val= val + (-64.0)*(found_other_4);
			// cerr<<"found_other_4 "<<found_other_4<<endl;

			val= val + (300.0)*(found_my_5);
			// cerr<<"found_my_5 "<<found_my_5<<endl;
	
			val= val + (-300.0)*(found_other_5);
			// cerr<<"found_other_5 "<<found_other_5<<endl;			

			// val= val + (45.0)*(found_my_2G2);
			// // cerr<<"found_my_2G2 "<<found_my_2G2<<endl;

			// val= val + (-45.0)*(found_other_2G2);
			// // cerr<<"found_other_2G2 "<<found_other_2G2<<endl;
			
			val= val + (5.0)*(found_my_sec_1);
			// cerr<<"found_my_sec_1 "<<found_my_sec_1<<endl; 																																																																																																																																																																																																							
			
			val= val + (-5.0)*(found_other_sec_1);
			// cerr<<"found_other_sec_1 "<<found_other_sec_1<<endl;
			
			val= val + (10.0)*(found_my_sec_2);
			// cerr<<"found_my_sec_2 "<<found_my_sec_2<<endl;
			
			val= val + (-10.0)*(found_other_sec_2);
			// cerr<<"found_other_sec_2 "<<found_other_sec_2<<endl;
			
			val= val + (75.0)*(found_my_sec_3);
			// cerr<<"found_my_sec_3 "<<found_my_sec_3<<endl;
			
			val= val + (-75.0)*(found_other_sec_3);
			// cerr<<"found_other_sec_3 "<<found_other_sec_3<<endl;
			
			val= val + (150.0)*(found_my_sec_4);
			// cerr<<"found_my_sec_4 "<<found_my_sec_4<<endl;
			
			val= val + (-150.0)*(found_other_sec_4);
			// cerr<<"found_other_sec_4 "<<found_other_sec_4<<endl;
			
			val= val + (1000.0)*(found_my_sec_5);
			// cerr<<"found_my_sec_5 "<<found_my_sec_5<<endl;
			
			val= val + (-1000.0)*(found_other_sec_5);
			// cerr<<"found_other_sec_5 "<<found_other_sec_5<<endl;
			
			if(kay==6){
				val= val + (12000.0)*(found_my_sec_6);	
				val= val + (-12000.0)*(found_other_sec_6);
			}

			val= val + (100000.)*(n-num_of_my_rings(config))*(n-num_of_my_rings(config));
			// cerr<<"removed_my_rings "<<(5-num_of_my_rings(config))<<endl;
			val= val + (-100000.)*(n-num_of_other_rings(config))*(n-num_of_other_rings(config));
			// cerr<<"removed_other_rings "<<(5-num_of_other_rings(config))<<endl;
			// cerr<<"val is"<<val<<endl;
			// cerr<<"nhi tha error"<<endl;
			return val;

		}

};
double evaluation_function(Config &config, pair<int,int> arr[])
{
	return Evaluation::evaluate(config,arr);
	int other_markers = 0;
	int my_markers = 0;
	for (int i = 0; i < config.board.size(); ++i)
	{
		for (int j = 0; j < config.board[i].size(); ++j)
		{
			if (config.board[i][j] == 2) my_markers++;
			if (config.board[i][j] == 3) other_markers++;
		}
	}
	// if (config.my_rings.size()<5) cerr<<"Rings are decreasing"<<endl;
	// cerr<<"My markers"<<my_markers<<endl;
	// cerr<<"Other markers"<<other_markers<<endl;
	// cerr<<"Diff of rings"<<config.my_rings.size()<<endl;
	// cerr<<"Inter"<<(((5 - config.my_rings.size()) * 100))<<endl;
	// cerr<<((my_markers - other_markers) + (((int) (5 - config.my_rings.size())) * 100))<<endl;
	return my_markers + (((int) (n - config.my_rings.size())) * 100) - other_markers - ((((int) (n - config.other_rings.size())) * 100));
}
vector<int> take_input(int* t, string s){
	// tokenizes the string and puts all integers into a vector without the delimiters like P,X,RS etc.
	vector<int> data;
	int num = 0;
	int left = -1;
	string temp;
	for (int i = 0; i < s.size(); ++i)
	{		
		if (s.at(i) == ' ')
		{
			temp = s.substr(left+1, i - left - 1);
			if (num%3 != 0) data.push_back(stoi(temp));
			left = i;
			num++;
		}
	}
	data.push_back(stoi(s.substr(left+1,s.size()-left-1)));
	int sz = data.size();
	if (sz == 2) *t = 0;
	else if (sz == 4) *t = 1;
	else if (sz == 10)
	{
		if (s.at(0) == 'S')
		{
			*t = 2;
		}
		else *t = 3;
	}
	else if (s.at(0) == 'R') *t = 4;
  	else *t = 5;
	return data;
	// Returns 0 if places ring, 1 if moves ring(S M), and 2 if row is also removed (S M R X), 3 if row is (R X S M) and 4 if (R X S M R X)
}
void update_ring(int x1, int x2, int y1, int y2, Config& config, int who)
{
	if (who == 0) 
		for (int i = 0; i < config.my_rings.size(); ++i)
		{
			if (config.my_rings[i].first == x1 && config.my_rings[i].second == x2)
			{
				config.my_rings[i].first = y1;
				config.my_rings[i].second = y2;
				break;
			}
		}
	else
		for (int i = 0; i < config.other_rings.size(); ++i)
		{
			if (config.other_rings[i].first == x1 && config.other_rings[i].second == x2)
			{
				config.other_rings[i].first = y1;
				config.other_rings[i].second = y2;
				break;
			}
		} 
}

void select_and_move(int x1, int x2, int y1, int y2, pair<int,int> arr[], Config& config, int who){
	// cerr<<endl;
	//cerr<<"Trying a select and move for piece "<<who<<" from ("<<x1<<","<<x2<<") to ("<<y1<<","<<y2<<")."<<endl;
	if (config.board[x1][x2] != 1 && config.board[x1][x2] != 0) cerr<<"There is no ring there "<<x1<<","<<x2<<"  "<<y1<<","<<y2<<" "<<who<<" we found here "<<config.board[x1][x2]<<endl;
	if (config.board[y1][y2] != -1) cerr<<"There is no empty space there"<<endl;
	int index1, index2, linenum;
	for (int i = 0; i < 6; i+=2)
	{
		for (int j = 0; j < 6; j+=2)
		{
			if (position_on_line[x1][x2][i] ==  position_on_line[y1][y2][j])
			{
				index1 = position_on_line[x1][x2][i+1];
				index2 = position_on_line[y1][y2][j+1];
				linenum = position_on_line[x1][x2][i];
				break;
			}

		}
	}
	for (int i = min(index1,index2) + 1; i < max(index1, index2); ++i)
	{
		if (config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second] != -1)
		{
			if (config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second] == 2)
			{
				config.num_other_markers++;
				config.num_my_markers--;
			}
			else if (config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second] == 3)
			{
				config.num_my_markers++;
				config.num_other_markers--;
			}
			else{
				cerr<<"I shouldnt have tried a selcet and move for this"<<endl;
				cerr<<endl;
			}
			//flipping step
			config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second] = 5 - config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second]; 
		}

	}
	if (who == 1)
	{
		config.board[x1][x2] = 3;
		config.board[y1][y2] = 1;
	}
	else {
		config.board[x1][x2] = 2;
		config.board[y1][y2] = 0;
	}
	if (who == 0) config.num_my_markers++;
	else config.num_other_markers++;
	update_ring(x1,x2,y1,y2,config,who);
}
// void reverse_select_and_move(int x1, int x2, int y1, int y2, pair<int,int> arr[], Config& config, int who){
// 	config.board[x1][x2] = -1;
// 	select_and_move(y1,y2,x1,x2,arr,config,who);
// 	config.board[y1][y2] = -1;
// 	if (who == 0) config.num_my_markers -= 2;
// 	else config.num_other_markers -= 2;
// }

void remove_row(int x1, int x2, int y1, int y2, pair<int,int> arr[], Config& config){
	int index1, index2, linenum;
	for (int i = 0; i < 6; i+=2)
	{
		for (int j = 0; j < 6; j+=2)
		{
			if (position_on_line[x1][x2][i] ==  position_on_line[y1][y2][j])
			{
				index1 = position_on_line[x1][x2][i+1];
				index2 = position_on_line[y1][y2][j+1];
				linenum = position_on_line[x1][x2][i];
				break;
			}

		}
	}
	if (config.board[arr[linenum * (2*n+1) + index1].first][arr[linenum * (2*n+1) + index1].second] == 2)
		config.num_my_markers -= kay;
	else config.num_other_markers -= kay;
	for (int i = min(index1,index2); i <= max(index1, index2); ++i)
	{
		config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second] = -1;
	}
}
// void reverse_remove_row(int x1, int x2, int y1, int y2, pair<int,int> arr[], Config& config, int who){
// 	int index1, index2, linenum;
// 	for (int i = 0; i < 6; i+=2)
// 	{
// 		for (int j = 0; j < 6; j+=2)
// 		{
// 			if (position_on_line[x1][x2][i] ==  position_on_line[y1][y2][j])
// 			{
// 				index1 = position_on_line[x1][x2][i+1];
// 				index2 = position_on_line[y1][y2][j+1];
// 				linenum = position_on_line[x1][x2][i];
// 				break;
// 			}

// 		}
// 	}
// 	for (int i = min(index1,index2); i <= max(index1, index2); ++i)
// 	{
// 		if (who == 0)
// 			config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second] = 2;
// 		else if(who==1)
// 			config.board[arr[linenum * (2*n+1) + i].first][arr[linenum * (2*n+1) + i].second] = 3;
// 		else 
// 			cerr<<"problem in WHO value"<<endl;
// 	}
// 	if (who == 0) config.num_my_markers += 5;
// 	else config.num_other_markers += 5;
// }

void remove_ring(int x1, int x2, Config& config, int who){
	if (who == 0)
		for (int i = 0; i < config.my_rings.size(); ++i){
			if (config.my_rings[i].first == x1 && config.my_rings[i].second == x2)
			{
				config.my_rings.erase(config.my_rings.begin()+i);
				break;
			}
		}
	else
		for (int i = 0; i < config.other_rings.size(); ++i){
			if (config.other_rings[i].first == x1 && config.other_rings[i].second == x2)
			{
				config.other_rings.erase(config.other_rings.begin()+i);
				break;
			}
		}
	config.board[x1][x2] = -1;
}
// void reverse_remove_ring(int x1, int x2, Config& config, int who, int index){
// 	if (who == 0)
// 	{
// 		config.my_rings.insert(config.my_rings.begin()+index,make_pair(x1,x2));
// 		config.board[x1][x2] = 0;
// 	}
// 	else 
// 	{
// 		config.other_rings.insert(config.other_rings.begin()+index,make_pair(x1,x2));
// 		config.board[x1][x2] = 1;
// 	}
// }
void execute_move(vector<int> move, int move_type, pair<int,int> arr[], Config& config, int who){ //execute the other player's move
	if (move_type == 0){
		config.board[move[0]][move[1]] = who;
		if (who == 1)
			config.other_rings.push_back(pair<int,int>(move[0],move[1]));
		else
			config.my_rings.push_back(pair<int,int>(move[0],move[1]));
	}
	else if (move_type == 1) {
		select_and_move(move[0],move[1],move[2],move[3],arr,config,who);
	}
	else if (move_type == 2) {
		select_and_move(move[0],move[1],move[2],move[3],arr,config,who);
		remove_row(move[4],move[5],move[6],move[7],arr,config);
		remove_ring(move[8],move[9],config,who);
	}
	else if (move_type == 3) {
		remove_row(move[0],move[1],move[2],move[3],arr,config);
		remove_ring(move[4],move[5],config,who);
		select_and_move(move[6],move[7],move[8],move[9],arr,config,who);
	}
	else if (move_type == 4){
    remove_row(move[0],move[1],move[2],move[3],arr,config);
    remove_ring(move[4],move[5],config,who);
    select_and_move(move[6],move[7],move[8],move[9],arr,config,who);
    remove_row(move[10],move[11],move[12],move[13],arr,config);
    remove_ring(move[14],move[15],config,who);
	}
	else {
	select_and_move(move[0],move[1],move[2],move[3],arr,config,who);
	remove_row(move[4],move[5],move[6],move[7],arr,config);
	remove_ring(move[8],move[9],config,who);
	remove_row(move[10],move[11],move[12],move[13],arr,config);
	remove_ring(move[14],move[15],config,who);
	}
}
void directions_creator(pair<int,int> arr[]){
	//fills the given array with the points on a line in my hexagon
	//n is the side of the hexagon
	int i,j,k;
	int ends=0;
	int num_of_rows= 6*n + 3;
	int row_num=0;
	int row_lim= 2*n+1; //one extra so that we know when to cut off
	//int directions= (2*n+1)*(3);
	int points_in_one_direction=2*n;
	for(i=0;i<num_of_rows*row_lim;i++){
		arr[i]=make_pair(-1,-1);
	}

	//direction1
	//add the short end row(- 30 deg one first (top pe jo hai))
	for(k=1;k< n;k++){
		arr[row_num*row_lim + k -1]= make_pair(n, k);
	}
	row_num++;
	for(ends=1;ends<=n-1;ends++,row_num++){
		for(j=0;j<ends;j++)			arr[row_num*row_lim + j] = make_pair(n-j,(n-j)*6 + j - ends);
		for(j=0;j<=(n- ends);j++) 	arr[row_num*row_lim + ends+ j] = make_pair(n -ends, j);
		for(j=ends-1; j>=0;j--)		arr[row_num*row_lim + n + (ends-j) ] = make_pair(n-j, n-j -j +ends);
	}
	//middle
	for(j=1;j<ends;j++)			arr[row_num*row_lim + j-1] = make_pair(n-j,(n-j)*6 + j - ends);
	for(j=0;j<=(n- ends);j++) 	arr[row_num*row_lim + ends- 1+ j] = make_pair(n -ends, j);
	for(j=ends-1; j>0;j--)		arr[row_num*row_lim + n + ends-j-1 ] = make_pair(n-j, n-j -j +ends);	
	row_num++;
	
	//end
	for(ends= n-1; ends>=1; ends--,row_num++){
		for(j=0;j<ends;j++)			arr[row_num*row_lim + j] = make_pair(n-j,(n-j)*5 - (n-ends));
		for(j=0;j<=(n- ends);j++) 	arr[row_num*row_lim + ends+ j] = make_pair(n -ends, 4*(n-ends)-j);
		for(j=ends-1; j>=0;j--)		arr[row_num*row_lim + n + (ends-j) ] = make_pair(n-j, (n-j)*2 +(n-ends));		
	}
	//last line

	//for(j=1;j<ends;j++)			arr[row_num*row_lim + j] = make_pair(n-j,(n-j)*5 - (n-ends));
	for(j=1;j<(n- ends);j++) 	arr[row_num*row_lim + j-1] = make_pair(n -ends, 4*(n-ends)-j);
	//for(j=ends-1; j>=0;j--)		arr[row_num*row_lim + n + (ends-j) ] = make_pair(n-j, (n-j)*2 +(n-ends));	
	row_num++;


	//direction2(+30deg)
	//pehli chopped off vali	
	for(k=1;k<n;k++)				arr[row_num*row_lim + (k-1)]= make_pair(n,n*5+ k);
	row_num++;
	//baki pehla half
	for(ends=1;ends<n;ends++,row_num++){
		for(j=0;j<ends;j++) 		arr[row_num*row_lim + j]= make_pair(n-j, (n-j)*5 -(ends-j)); //initial
		for(j=0;j<n-ends+1;j++)		arr[row_num*row_lim + ends+ j]= make_pair(n-ends, ((n-ends)*5 + j)%((n-ends)*6));
		for(j=0; j< ends; j++)		arr[row_num*row_lim + j + n+ 1]= make_pair(n-ends+1+j, j+1);
	}
	//middle
	for(j=1;j<ends;j++) 		arr[row_num*row_lim + j-1]= make_pair(n-j, (n-j)*5 -(ends-j)); //initial
	for(j=0;j<n-ends+1;j++)		arr[row_num*row_lim + ends-1+ j]= make_pair(n-ends, ((n-ends)*5 + j));
	for(j=0; j< ends-1; j++)		arr[row_num*row_lim + j + n]= make_pair(n-ends+1+j, j+1);
	row_num++;

	for(ends=n-1; ends>0;ends--,row_num++){
		for(j=0;j<ends;j++)			arr[row_num*row_lim + j]= make_pair(n-j, (n-j)*4 -(n-ends));
		for(j=0;j<n-ends+1;j++)		arr[row_num*row_lim + ends+j]= make_pair(n-ends, (n-ends)*3 - j);
		for(j=0;j<ends;j++)			arr[row_num*row_lim + j + n+ 1]= make_pair(n-ends+ 1 + j, (n - ends)*2 +1 + j );
	}

	//exact last
	for(k=1;k<n;k++) arr[row_num*row_lim + k-1]= make_pair(n , 3*n - k);
	row_num++;

	//ab vertical direction which is supposed to be easy
	for(k=1;k<n;k++)				arr[row_num*row_lim + (k-1)]= make_pair(n,n*4+ k);
	row_num++;
	//baki pehla half
	for(ends=1;ends<n;ends++,row_num++){
		for(j=0;j<ends;j++) 		arr[row_num*row_lim + j]= make_pair(n-j, (n-j)*4 -(ends-j)); //initial
		for(j=0;j<n-ends+1;j++)		arr[row_num*row_lim + ends+ j]= make_pair(n-ends, ((n-ends)*4 + j));
		for(j=0; j< ends; j++)		arr[row_num*row_lim + j + n+ 1]= make_pair(n-ends+1+j, (n-ends+1+j)*5 + j+1);
	}
	//middle
	for(j=1;j<ends;j++) 		arr[row_num*row_lim + j-1]= make_pair(n-j, (n-j)*4 -(ends-j)); //initial
	for(j=0;j<n-ends+1;j++)		arr[row_num*row_lim + ends-1+ j]= make_pair(n-ends, ((n-ends)*4 + j));
	for(j=0; j< ends-1; j++)		arr[row_num*row_lim + j + n]= make_pair(n-ends+1+j, 0);
	row_num++;

	for(ends=n-1; ends>0;ends--,row_num++){
		for(j=0;j<ends;j++)			arr[row_num*row_lim + j]= make_pair(n-j, (n-j)*3 -(n-ends));
		for(j=0;j<n-ends+1;j++)		arr[row_num*row_lim + ends+j]= make_pair(n-ends, (n-ends)*2 - j);
		for(j=0;j<ends;j++)			arr[row_num*row_lim + j + n+ 1]= make_pair(n-ends+ 1 + j, (n - ends) );
	}

	//exact last
	for(k=1;k<n;k++) arr[row_num*row_lim + k-1]= make_pair(n , 2*n - k);
}
void fill_positions(pair<int,int> arr[]){
	// cerr<<i<<endl;
	//testing
	int i,j,k;
	// int n=5;//size of hexagon

	//declare and initialise the rows array 
	int directions= (2*n+1)*(3);
	int points_in_one_direction=2*n;
	int row_limit= points_in_one_direction+1; // the extra one so that we know when to cut off
	directions_creator(arr);

	//initialise the position_on_line matrix
	for(i=0;i<=n;i++){
		for(j=0;j<6*i;j++){
			for(k=0;k<6;k++){
				position_on_line[i][j][k]=-1;
			}
		}
	}
	for(k=0;k<6;k++)	position_on_line[0][0][k]=-1;
	//yeh bahut zaroori hai kyonki upar vale loop meu set nhi uhua


	//now fill the positions on line matrix for each position on the board while traversing the directions matrix
	//cerr<<"position_on_line[0][0][0] is "<<position_on_line[0][0][0]<<endl;
	for(i=0;i<directions;i++){
		for(j=0;j<row_limit;j++){
			pair<int,int> pos= arr[i*row_limit+ j];
			if(get<0>(pos)==-1) break;
			if(position_on_line[get<0>(pos)][get<1>(pos)][0]== -1){
				position_on_line[get<0>(pos)][get<1>(pos)][0]= i; //line number
				position_on_line[get<0>(pos)][get<1>(pos)][1]= j; // usmei index
			}
			else if(position_on_line[get<0>(pos)][get<1>(pos)][2]== -1){
				position_on_line[get<0>(pos)][get<1>(pos)][2]= i;
				position_on_line[get<0>(pos)][get<1>(pos)][3]= j;
			}
			else if(position_on_line[get<0>(pos)][get<1>(pos)][4]== -1){
				position_on_line[get<0>(pos)][get<1>(pos)][4]= i;
				position_on_line[get<0>(pos)][get<1>(pos)][5]= j;
			}
			else{
				cerr<<"ERROR"<<endl;
			}
		}
	}
}


void print_valid_positions(int x1, int x2, pair<int,int> arr[], Config& config)
{
	for (int i = 0; i < 6; i+=2)
	{
		int lineno = position_on_line[x1][x2][i];
		int ind = position_on_line[x1][x2][i+1];
		bool foundMarkers = false;
		for (int j = ind-1; j >= 0; --j)
		{
			pair<int,int> t = arr[lineno * (2 * n + 1) + j];
			if (t.first == -1) break;
			int val = config.board[t.first][t.second];
			if (val == 2 || val == 3) foundMarkers = true;
			else if (val == 0 || val == 1) break;
			else if (val == -1 && foundMarkers) 
			{
				cerr<<t.first<<" "<<t.second<<endl;
				break;
			}
			else cerr<<t.first<<" "<<t.second<<endl;
		}
		foundMarkers = false;
		for (int j = ind+1; j < (2 * n + 1); ++j)
		{
			pair<int,int> t = arr[lineno * (2 * n + 1) + j];
			if (t.first == -1) break;
			int val = config.board[t.first][t.second];
			if (val == 2 || val == 3) foundMarkers = true;
			else if (val == 0 || val == 1) break;
			else if (val == -1 && foundMarkers) 
			{
				cerr<<t.first<<" "<<t.second<<endl;
				break;
			}
			else cerr<<t.first<<" "<<t.second<<endl;
		}
	}
}

vector<vector<int> > board_initialisation(){
	// Set up the board
	// -1 will be empty
	// 0 will be my ring and 2 will be marker
	// 1 will be the other ring and 3 will be the marker
	vector<vector<int> > board;
	vector<int> first_row;
	first_row.push_back(-1);
	board.push_back(first_row);
	for (int i = 1; i <= n; ++i)
	{
		vector<int> temp;
		for (int i = 0; i < 6*n; ++i) //six, POTENTIAL ERROR
		{
			temp.push_back(-1);
		}
		board.push_back(temp);
	}
	return board;
}

void print_pair(pair<int,int> pa){
	cerr<<"("<< get<0>(pa)<<", "<<get<1>(pa)<<")";
}

pair<string,double> minimax(Config& config, int depth, pair<int,int> arr[])
{ 
  nodes=0;
  prunes=0;
  float alpha = -1000000000;
  float beta = 1000000000;
  double utility_to_maximize = -1000000000;
  string maxstr;
  vector<pair<float,vector<int> > > mapped_evaluation;
  mapped_evaluation.clear();
  for (int ring_ind = 0; ring_ind < config.my_rings.size(); ++ring_ind)
  { 
    int x1 = config.my_rings[ring_ind].first;
    int x2 = config.my_rings[ring_ind].second;
    // cerr<<"Ring seen where"<<endl;
    for (int i = 0; i < 6; i+=2)
    {
      int lineno = position_on_line[x1][x2][i];
      int ind = position_on_line[x1][x2][i+1];
      bool foundMarkers = false;
      Config temp_config;
      // cerr<<"Before the loop"<<endl;
      for (int j = ind-1; j >= 0; --j)
      {
        temp_config.board = config.board;
        temp_config.my_rings = config.my_rings;
        temp_config.other_rings = config.other_rings;
        temp_config.num_my_markers = config.num_my_markers;
        temp_config.num_other_markers = config.num_other_markers;
        pair<int,int> t = arr[lineno * (2 * n + 1) + j];
        if (t.first == -1) break;
        int val = temp_config.board[t.first][t.second];
        if (val == 2 || val == 3) foundMarkers = true;
        else if (val == 0 || val == 1) break;
        else 
        {
          select_and_move(x1,x2,t.first,t.second,arr,temp_config,0);
          // cerr<<"Moved "<<x1<<" "<<x2<<" "<<endl;
          vector<vector<int> > rows = find_k_throughout(kay,arr,temp_config);
          Config temp_ring_config;
          for (int ind = 0; ind < rows.size(); ++ind)
          {
            if (rows[ind][0] == 2)
            {
              temp_ring_config.board = temp_config.board;
              temp_ring_config.my_rings = temp_config.my_rings;
              temp_ring_config.other_rings = temp_config.other_rings;
              temp_ring_config.num_my_markers = temp_config.num_my_markers;
              temp_ring_config.num_other_markers = temp_config.num_other_markers;
              remove_row(rows[ind][2],rows[ind][3],rows[ind][4],rows[ind][5],arr,temp_ring_config);
              Config tc;
              for (int k = 0; k < temp_ring_config.my_rings.size(); ++k)
              {   
                tc.board = temp_ring_config.board;
                tc.my_rings = temp_ring_config.my_rings;
                tc.other_rings = temp_ring_config.other_rings;
                tc.num_my_markers = temp_ring_config.num_my_markers;
                tc.num_other_markers = temp_ring_config.num_other_markers;
                remove_ring(temp_ring_config.my_rings[k].first,temp_ring_config.my_rings[k].second,tc,0);
                pair<float,vector<int> > intermediate;
                intermediate.first = evaluation_function(tc,arr);
                intermediate.second.clear();
                intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);intermediate.second.push_back(rows[ind][2]);intermediate.second.push_back(rows[ind][3]);intermediate.second.push_back(rows[ind][4]);intermediate.second.push_back(rows[ind][5]);intermediate.second.push_back(temp_ring_config.my_rings[k].first);intermediate.second.push_back(temp_ring_config.my_rings[k].second);
                mapped_evaluation.push_back(intermediate);
                // double temp_utility = min_tree(tc,depth-1,arr,alpha,beta);
                // // cerr<<"----------"<<endl;
                // if (temp_utility > utility_to_maximize)
                // {
                //  utility_to_maximize = temp_utility;
                //  maxstr = "S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)+" RS "+ to_string(rows[ind][2])+" "+ to_string(rows[ind][3])+" RE "+to_string(rows[ind][4])+" "+ to_string(rows[ind][5])+" X "+to_string(temp_ring_config.my_rings[k].first)+" "+to_string(temp_ring_config.my_rings[k].second);
                // }
                // // cout<<"S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)+" RS "+ to_string(rows[ind][2])+" "+ to_string(rows[ind][3])+" RE "+to_string(rows[ind][4])+" "+ to_string(rows[ind][5])+" X "+to_string(temp_ring_config.my_rings[k].first)+" "+to_string(temp_ring_config.my_rings[k].second)<<endl;
                // // cout<<temp_utility<<endl;
                // // cout<<"-----------"<<endl;
                // alpha = max_float(alpha, temp_utility);
              }
            }
          }
          if (rows.size() == 0)
          {
            pair<float,vector<int> > intermediate;
            intermediate.first = evaluation_function(temp_config,arr);
            intermediate.second.clear();
            intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);
            mapped_evaluation.push_back(intermediate);
            // // cerr<<"Is the problem here"<<endl;
            // double temp_utility = min_tree(temp_config,depth-1,arr,alpha,beta);
            // // cerr<<"----------"<<endl;
            // // cerr<<"Even came out of here"<<endl;
            // if (temp_utility > utility_to_maximize)
            // {
            //  utility_to_maximize = temp_utility;
            //  maxstr = "S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second);
            // }
            // // cout<<"S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)<<endl;
            // // cout<<temp_utility<<endl;
            // // cout<<"--------"<<endl;
            // alpha = max_float(alpha, temp_utility);
          }
          if (foundMarkers) break;
        }
      }
      // cerr<<"The first round is complete"<<endl;
      foundMarkers = false;
      for (int j = ind+1; j < (2 * n + 1); ++j)
      {
        temp_config.board = config.board;
        temp_config.my_rings = config.my_rings;
        temp_config.other_rings = config.other_rings;
        temp_config.num_my_markers = config.num_my_markers;
        temp_config.num_other_markers = config.num_other_markers;
        pair<int,int> t = arr[lineno * (2 * n + 1) + j];
        if (t.first == -1) break;
        int val = temp_config.board[t.first][t.second];
        if (val == 2 || val == 3) foundMarkers = true;
        else if (val == 0 || val == 1) break;
        else 
        {
          select_and_move(x1,x2,t.first,t.second,arr,temp_config,0);
          vector<vector<int> > rows = find_k_throughout(kay,arr,temp_config);
          Config temp_ring_config;
          for (int ind = 0; ind < rows.size(); ++ind)
          {
            if (rows[ind][0] == 2)
            {
              temp_ring_config.board = temp_config.board;
              temp_ring_config.my_rings = temp_config.my_rings;
              temp_ring_config.other_rings = temp_config.other_rings;
              temp_ring_config.num_my_markers = temp_config.num_my_markers;
              temp_ring_config.num_other_markers = temp_config.num_other_markers;
              remove_row(rows[ind][2],rows[ind][3],rows[ind][4],rows[ind][5],arr,temp_ring_config);
              Config tc;
              for (int k = 0; k < temp_ring_config.my_rings.size(); ++k)
              {   
                tc.board = temp_ring_config.board;
                tc.my_rings = temp_ring_config.my_rings;
                tc.other_rings = temp_ring_config.other_rings;
                tc.num_my_markers = temp_ring_config.num_my_markers;
                tc.num_other_markers = temp_ring_config.num_other_markers;
                remove_ring(temp_ring_config.my_rings[k].first,temp_ring_config.my_rings[k].second,tc,0);
                pair<float,vector<int> > intermediate;
                intermediate.first = evaluation_function(tc,arr);
                intermediate.second.clear();
                intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);intermediate.second.push_back(rows[ind][2]);intermediate.second.push_back(rows[ind][3]);intermediate.second.push_back(rows[ind][4]);intermediate.second.push_back(rows[ind][5]);intermediate.second.push_back(temp_ring_config.my_rings[k].first);intermediate.second.push_back(temp_ring_config.my_rings[k].second);
                mapped_evaluation.push_back(intermediate);
                // double temp_utility = min_tree(tc,depth-1,arr,alpha,beta);
                // // cerr<<"----------"<<endl;
                // if (temp_utility > utility_to_maximize)
                // {
                //  utility_to_maximize = temp_utility;
                //  maxstr = "S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)+" RS "+ to_string(rows[ind][2])+" "+ to_string(rows[ind][3])+" RE "+to_string(rows[ind][4])+" "+ to_string(rows[ind][5])+" X "+to_string(temp_ring_config.my_rings[k].first)+" "+to_string(temp_ring_config.my_rings[k].second);
                // }
                // // cout<<"S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)+" RS "+ to_string(rows[ind][2])+" "+ to_string(rows[ind][3])+" RE "+to_string(rows[ind][4])+" "+ to_string(rows[ind][5])+" X "+to_string(temp_ring_config.my_rings[k].first)+" "+to_string(temp_ring_config.my_rings[k].second)<<endl;
                // // cout<<temp_utility<<endl;
                // // cout<<"-----------"<<endl;
                // alpha = max_float(alpha, temp_utility);
              }
            }
          }
          if (rows.size() == 0)
          {
            pair<float,vector<int> > intermediate;
            intermediate.first = evaluation_function(temp_config,arr);
            intermediate.second.clear();
            intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);
            mapped_evaluation.push_back(intermediate);
            // double temp_utility = min_tree(temp_config,depth-1,arr,alpha,beta);
            // // cerr<<"----------"<<endl;
            // if (temp_utility > utility_to_maximize)
            // {
            //  utility_to_maximize = temp_utility;
            //  maxstr = "S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second);
            // }
            // // cout<<"S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)<<endl;
            // // cout<<temp_utility<<endl;
            // // cout<<"--------"<<endl;
            // alpha = max_float(alpha, temp_utility);
          }
          if (foundMarkers) break;
        }
      }
    }
  }
  if (depth>1) sort(mapped_evaluation.begin(), mapped_evaluation.end(), greater_float);
  Config temp_config;
  for (int ind = 0; ind < mapped_evaluation.size(); ++ind)
  {
    temp_config.board = config.board;
    temp_config.my_rings = config.my_rings;
    temp_config.other_rings = config.other_rings;
    temp_config.num_my_markers = config.num_my_markers;
    temp_config.num_other_markers = config.num_other_markers;
    if (mapped_evaluation[ind].second.size() == 10){
      select_and_move(mapped_evaluation[ind].second[0],mapped_evaluation[ind].second[1],mapped_evaluation[ind].second[2],mapped_evaluation[ind].second[3],arr,temp_config,0);
      remove_row(mapped_evaluation[ind].second[4],mapped_evaluation[ind].second[5],mapped_evaluation[ind].second[6],mapped_evaluation[ind].second[7],arr,temp_config);  
      remove_ring(mapped_evaluation[ind].second[8],mapped_evaluation[ind].second[9],temp_config,0);
      double temp_utility = min_tree(temp_config,depth-1,arr,alpha,beta);
      // cerr<<"----------"<<endl;
      if (temp_utility > utility_to_maximize)
      {
        utility_to_maximize = temp_utility;
        maxstr = "S "+to_string(mapped_evaluation[ind].second[0])+" "+to_string(mapped_evaluation[ind].second[1])+" "+"M "+to_string(mapped_evaluation[ind].second[2])+" "+to_string(mapped_evaluation[ind].second[3])+" RS "+ to_string(mapped_evaluation[ind].second[4])+" "+ to_string(mapped_evaluation[ind].second[5])+" RE "+to_string(mapped_evaluation[ind].second[6])+" "+ to_string(mapped_evaluation[ind].second[7])+" X "+to_string(mapped_evaluation[ind].second[8])+" "+to_string(mapped_evaluation[ind].second[9]);
      }
      // cout<<"S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)+" RS "+ to_string(rows[ind][2])+" "+ to_string(rows[ind][3])+" RE "+to_string(rows[ind][4])+" "+ to_string(rows[ind][5])+" X "+to_string(temp_ring_config.my_rings[k].first)+" "+to_string(temp_ring_config.my_rings[k].second)<<endl;
      // cout<<temp_utility<<endl;
      // cout<<"-----------"<<endl;
      alpha = max_float(alpha, temp_utility);
    }
    else if (mapped_evaluation[ind].second.size() == 4){
      select_and_move(mapped_evaluation[ind].second[0],mapped_evaluation[ind].second[1],mapped_evaluation[ind].second[2],mapped_evaluation[ind].second[3],arr,temp_config,0);
      double temp_utility = min_tree(temp_config,depth-1,arr,alpha,beta);
      // cerr<<"----------"<<endl;
      if (temp_utility > utility_to_maximize)
      {
        utility_to_maximize = temp_utility;
        maxstr = "S "+to_string(mapped_evaluation[ind].second[0])+" "+to_string(mapped_evaluation[ind].second[1])+" "+"M "+to_string(mapped_evaluation[ind].second[2])+" "+to_string(mapped_evaluation[ind].second[3]);
      }
      // cout<<"S "+to_string(x1)+" "+to_string(x2)+" "+"M "+to_string(t.first)+" "+to_string(t.second)+" RS "+ to_string(rows[ind][2])+" "+ to_string(rows[ind][3])+" RE "+to_string(rows[ind][4])+" "+ to_string(rows[ind][5])+" X "+to_string(temp_ring_config.my_rings[k].first)+" "+to_string(temp_ring_config.my_rings[k].second)<<endl;
      // cout<<temp_utility<<endl;
      // cout<<"-----------"<<endl;
      alpha = max_float(alpha, temp_utility);
    }
    else {
      cerr<<"WHAT??"<<endl;
    }
  }
  // cerr<<maxstr<<endl;
  // cerr<<"The max string is "<<maxstr<<endl;
  // cerr<<"The utility is "<<utility_to_maximize<<endl;
  // cerr<<"prunes in this minimax call were: "<<prunes<<endl;
  // cerr<<"and the number of nodes was: "<<nodes<<endl;
  return make_pair(maxstr,utility_to_maximize);
}
void print_board(Config& config){
    //print the board
    cerr<<endl;
    cerr<<"centre is "<<config.board[0][0]<<endl;

    int layer;
    int angle;
    for(layer=1; layer<=n; layer++){
        cerr<< "layer num "<< layer<<"\t\t";
        for(angle=0;angle<6*layer;angle++){
            cerr<<config.board[layer][angle]<<"  ";
        }
        cerr<<endl;
    }

    cerr<<"my rings: "<<config.num_my_markers<<endl;
    cerr<<"other rings: "<<config.num_other_markers<<endl;
}

double max_tree(Config& config, int depth, pair<int,int> arr[], float alpha, float beta)
{  
  double best_maximisation= -1000000000;
  nodes++;
  if (depth == 0) 
  {
    return evaluation_function(config,arr);
  }
  if (config.my_rings.size() <= 2 && config.other_rings.size()>=3)
    return 2000000 * (depth + 1);
  else if (config.other_rings.size() <= 2 && config.my_rings.size()>=3)
    return -2000000 * (depth + 1);
  double utility_to_maximize = -100000000;
  vector<pair<float,vector<int> > > mapped_evaluation;
  mapped_evaluation.clear();
  for (int ring_ind = 0; ring_ind < config.my_rings.size(); ++ring_ind)
  { 
    int x1 = config.my_rings[ring_ind].first;
    int x2 = config.my_rings[ring_ind].second;
    for (int i = 0; i < 6; i+=2)
    {
      int lineno = position_on_line[x1][x2][i];
      int ind = position_on_line[x1][x2][i+1];
      bool foundMarkers = false;
      Config temp_config;
      for (int j = ind-1; j >= 0; --j)
      {
        temp_config.board = config.board;
        temp_config.my_rings = config.my_rings;
        temp_config.other_rings = config.other_rings;
        temp_config.num_my_markers = config.num_my_markers;
        temp_config.num_other_markers = config.num_other_markers;
        pair<int,int> t = arr[lineno * (2 * n + 1) + j];
        if (t.first == -1) break;
        int val = temp_config.board[t.first][t.second];
        if (val == 2 || val == 3) foundMarkers = true;
        else if (val == 0 || val == 1) break;
        else 
        {
          select_and_move(x1,x2,t.first,t.second,arr,temp_config,0);
          vector<vector<int> > rows = find_k_throughout(kay,arr,temp_config);
          Config temp_ring_config;
          for (int ind = 0; ind < rows.size(); ++ind)
          {
            if (rows[ind][0] == 2)
            {
              temp_ring_config.board = temp_config.board;
              temp_ring_config.my_rings = temp_config.my_rings;
              temp_ring_config.other_rings = temp_config.other_rings;
              temp_ring_config.num_my_markers = temp_config.num_my_markers;
              temp_ring_config.num_other_markers = temp_config.num_other_markers;
              remove_row(rows[ind][2],rows[ind][3],rows[ind][4],rows[ind][5],arr,temp_ring_config);
              Config tc;
              for (int k = 0; k < temp_ring_config.my_rings.size(); ++k)
              {   
                tc.board = temp_ring_config.board;
                tc.my_rings = temp_ring_config.my_rings;
                tc.other_rings = temp_ring_config.other_rings;
                tc.num_my_markers = temp_ring_config.num_my_markers;
                tc.num_other_markers = temp_ring_config.num_other_markers;
                remove_ring(temp_ring_config.my_rings[k].first,temp_ring_config.my_rings[k].second,tc,0);
                pair<float,vector<int> > intermediate;
                intermediate.first = evaluation_function(tc,arr);
                // if(best_maximisation< intermediate.first) best_maximisation=intermediate.first;

                if(best_maximisation<intermediate.first){
                  best_maximisation=intermediate.first;
                  if(depth==1){
                    alpha = max_float(alpha, best_maximisation);
                    if(alpha>=beta){
                      return best_maximisation;
                      //POTENTIAL ERROR - return beta
                    }
                  } 
                }

                intermediate.second.clear();
                intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);intermediate.second.push_back(rows[ind][2]);intermediate.second.push_back(rows[ind][3]);intermediate.second.push_back(rows[ind][4]);intermediate.second.push_back(rows[ind][5]);intermediate.second.push_back(temp_ring_config.my_rings[k].first);intermediate.second.push_back(temp_ring_config.my_rings[k].second);
                mapped_evaluation.push_back(intermediate);
                // double temp_utility = min_tree(tc,depth-1,arr, alpha, beta);
                // if (temp_utility > utility_to_maximize)
                // {
                //  utility_to_maximize = temp_utility;
                // }
                // alpha = max_float(alpha, temp_utility);
                // if (alpha >= beta) return temp_utility;
              }
            }
          }
          if (rows.size() == 0)
          {
            pair<float,vector<int> > intermediate;
            intermediate.first = evaluation_function(temp_config,arr);
            // if(best_maximisation< intermediate.first) best_maximisation=intermediate.first;
            if(best_maximisation<intermediate.first){
              best_maximisation=intermediate.first;
              if(depth==1){
                alpha = max_float(alpha, best_maximisation);
                if(alpha>=beta){
                  return best_maximisation;
                  
                }
              } 
            }
            intermediate.second.clear();
            intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);
            mapped_evaluation.push_back(intermediate);
            // double temp_utility = min_tree(temp_config,depth-1,arr, alpha, beta);
            // if (temp_utility > utility_to_maximize)
            // {
            //  utility_to_maximize = temp_utility;
            // }
            // alpha = max_float(alpha, temp_utility);
            // if (alpha >= beta) return temp_utility;
          }
          if (foundMarkers) break;
        }
      }
      foundMarkers = false;
      for (int j = ind+1; j < (2 * n + 1); ++j)
      {
        temp_config.board = config.board;
        temp_config.my_rings = config.my_rings;
        temp_config.other_rings = config.other_rings;
        temp_config.num_my_markers = config.num_my_markers;
        temp_config.num_other_markers = config.num_other_markers;
        pair<int,int> t = arr[lineno * (2 * n + 1) + j];
        if (t.first == -1) break;
        int val = temp_config.board[t.first][t.second];
        if (val == 2 || val == 3) foundMarkers = true;
        else if (val == 0 || val == 1) break;
        else 
        {
          select_and_move(x1,x2,t.first,t.second,arr,temp_config,0);
          vector<vector<int> > rows = find_k_throughout(kay,arr,temp_config);
          Config temp_ring_config;
          for (int ind = 0; ind < rows.size(); ++ind)
          {
            if (rows[ind][0] == 2)
            {
              temp_ring_config.board = temp_config.board;
              temp_ring_config.my_rings = temp_config.my_rings;
              temp_ring_config.other_rings = temp_config.other_rings;
              temp_ring_config.num_my_markers = temp_config.num_my_markers;
              temp_ring_config.num_other_markers = temp_config.num_other_markers;
              remove_row(rows[ind][2],rows[ind][3],rows[ind][4],rows[ind][5],arr,temp_ring_config);
              Config tc;
              for (int k = 0; k < temp_ring_config.my_rings.size(); ++k)
              {   
                tc.board = temp_ring_config.board;
                tc.my_rings = temp_ring_config.my_rings;
                tc.other_rings = temp_ring_config.other_rings;
                tc.num_my_markers = temp_ring_config.num_my_markers;
                tc.num_other_markers = temp_ring_config.num_other_markers;
                remove_ring(temp_ring_config.my_rings[k].first,temp_ring_config.my_rings[k].second,tc,0);
                pair<float,vector<int> > intermediate;
                intermediate.first = evaluation_function(tc,arr);
                // if(best_maximisation< intermediate.first) best_maximisation=intermediate.first;
                if(best_maximisation<intermediate.first){
                  best_maximisation=intermediate.first;
                  if(depth==1){
                    alpha = max_float(alpha, best_maximisation);
                    if(alpha>=beta){
                      return best_maximisation;
                      
                    }
                  } 
                }
                intermediate.second.clear();
                intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);intermediate.second.push_back(rows[ind][2]);intermediate.second.push_back(rows[ind][3]);intermediate.second.push_back(rows[ind][4]);intermediate.second.push_back(rows[ind][5]);intermediate.second.push_back(temp_ring_config.my_rings[k].first);intermediate.second.push_back(temp_ring_config.my_rings[k].second);
                mapped_evaluation.push_back(intermediate);
                // double temp_utility = min_tree(tc,depth-1,arr, alpha, beta);
                // if (temp_utility > utility_to_maximize)
                // {
                //  utility_to_maximize = temp_utility;
                // }
                // alpha = max_float(alpha, temp_utility);
                // if (alpha >= beta) return temp_utility;
              }
            }
          }
          if (rows.size() == 0)
          {
            pair<float,vector<int> > intermediate;
            intermediate.first = evaluation_function(temp_config,arr);
            // if(best_maximisation< intermediate.first) best_maximisation=intermediate.first;
            if(best_maximisation<intermediate.first){
              best_maximisation=intermediate.first;
              if(depth==1){
                alpha = max_float(alpha, best_maximisation);
                if(alpha>=beta){
                  return best_maximisation;
                  
                }
              } 
            }

            intermediate.second.clear();
            intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);
            mapped_evaluation.push_back(intermediate);
            // double temp_utility = min_tree(temp_config,depth-1,arr, alpha, beta);
            // if (temp_utility > utility_to_maximize)
            // {
            //  utility_to_maximize = temp_utility;
            // }
            // alpha = max_float(alpha, temp_utility);
            // if (alpha >= beta) return temp_utility;
          }
          if (foundMarkers) break;
        }
      }
    }
  }
  if(depth==1)  return best_maximisation;
  // if(depth==1) {
  //   cerr<<"No pruning at this end"<<endl;
  //   return best_maximisation;
  // }
  if (depth>1) sort(mapped_evaluation.begin(), mapped_evaluation.end(), greater_float);
  Config temp_config;
  for (int ind = 0; ind < mapped_evaluation.size(); ++ind)
  {
    temp_config.board = config.board;
    temp_config.my_rings = config.my_rings;
    temp_config.other_rings = config.other_rings;
    temp_config.num_my_markers = config.num_my_markers;
    temp_config.num_other_markers = config.num_other_markers;
    if (mapped_evaluation[ind].second.size() == 10){
      select_and_move(mapped_evaluation[ind].second[0],mapped_evaluation[ind].second[1],mapped_evaluation[ind].second[2],mapped_evaluation[ind].second[3],arr,temp_config,0);
      remove_row(mapped_evaluation[ind].second[4],mapped_evaluation[ind].second[5],mapped_evaluation[ind].second[6],mapped_evaluation[ind].second[7],arr,temp_config);  
      remove_ring(mapped_evaluation[ind].second[8],mapped_evaluation[ind].second[9],temp_config,0);
      double temp_utility = min_tree(temp_config,depth-1,arr, alpha, beta);
      if (temp_utility > utility_to_maximize)
      {
        utility_to_maximize = temp_utility;
      }
      alpha = max_float(alpha, temp_utility);
      if (alpha >= beta) {
          if(depth>1 ) prunes++;
          return temp_utility;
      }
    }
    else if (mapped_evaluation[ind].second.size() == 4){
      select_and_move(mapped_evaluation[ind].second[0],mapped_evaluation[ind].second[1],mapped_evaluation[ind].second[2],mapped_evaluation[ind].second[3],arr,temp_config,0);
      double temp_utility = min_tree(temp_config,depth-1,arr, alpha, beta);
      if (temp_utility > utility_to_maximize)
      {
        utility_to_maximize = temp_utility;
      }
      alpha = max_float(alpha, temp_utility);
      if (alpha >= beta) {
          if(depth>1 ) prunes++;
          return temp_utility;
      }
    }
    else {
      cerr<<"WHAT??"<<endl;
    }
  }
  return utility_to_maximize;
}
double min_tree(Config& config, int depth, pair<int,int> arr[], float alpha, float beta)
{  

  double best_minimisation = 1000000000;
  nodes++;
  if (depth == 0) 
  {
    return evaluation_function(config,arr);
  }
  if (config.my_rings.size() <= 2 && config.other_rings.size()>=3)
    return 20000000 * (depth + 1);
  else if (config.other_rings.size() <= 2 && config.my_rings.size()>=3)
    return -20000000 * (depth + 1);
  double utility_to_minimize = 1000000000;
  vector<pair<float,vector<int> > > mapped_evaluation;
  mapped_evaluation.clear();
  for (int ring_ind = 0; ring_ind < config.other_rings.size(); ++ring_ind)
  { 
    int x1 = config.other_rings[ring_ind].first;
    int x2 = config.other_rings[ring_ind].second;
    for (int i = 0; i < 6; i+=2)
    {
      int lineno = position_on_line[x1][x2][i];
      int ind = position_on_line[x1][x2][i+1];
      bool foundMarkers = false;
      Config temp_config;
      for (int j = ind-1; j >= 0; --j)
      {
        temp_config.board = config.board;
        temp_config.my_rings = config.my_rings;
        temp_config.other_rings = config.other_rings;
        temp_config.num_my_markers = config.num_my_markers;
        temp_config.num_other_markers = config.num_other_markers;
        pair<int,int> t = arr[lineno * (2 * n + 1) + j];
        if (t.first == -1) break;
        int val = temp_config.board[t.first][t.second];
        if (val == 2 || val == 3) foundMarkers = true;
        else if (val == 0 || val == 1) break;
        else 
        {
          select_and_move(x1,x2,t.first,t.second,arr,temp_config,1);
          vector<vector<int> > rows = find_k_throughout(kay,arr,temp_config);
          Config temp_ring_config;
          for (int ind = 0; ind < rows.size(); ++ind)
          {
            if (rows[ind][0] == 3)
            {
              temp_ring_config.board = temp_config.board;
              temp_ring_config.my_rings = temp_config.my_rings;
              temp_ring_config.other_rings = temp_config.other_rings;
              temp_ring_config.num_my_markers = temp_config.num_my_markers;
              temp_ring_config.num_other_markers = temp_config.num_other_markers;
              remove_row(rows[ind][2],rows[ind][3],rows[ind][4],rows[ind][5],arr,temp_ring_config);
              Config tc;
              for (int k = 0; k < temp_ring_config.other_rings.size(); ++k)
              {   
                tc.board = temp_ring_config.board;
                tc.my_rings = temp_ring_config.my_rings;
                tc.other_rings = temp_ring_config.other_rings;
                tc.num_my_markers = temp_ring_config.num_my_markers;
                tc.num_other_markers = temp_ring_config.num_other_markers;
                remove_ring(temp_ring_config.other_rings[k].first,temp_ring_config.other_rings[k].second,tc,1);
                pair<float,vector<int> > intermediate;

                intermediate.first = evaluation_function(tc,arr);
                if(best_minimisation>intermediate.first){
                  best_minimisation=intermediate.first;
                  //beta cant be edited if its not d1, varna non d1 ke liye edited beta se compare hoga baad mei which is totally wrong 
                  // beta = min_float(beta, best_minimisation);
                  // if(alpha>=beta && depth==1) return best_minimisation;
                  if(depth==1){
                    beta = min_float(beta, best_minimisation);
                    if(alpha>=beta){
                      return best_minimisation;
                      
                    }
                  } 
                }

                intermediate.second.clear();
                intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);intermediate.second.push_back(rows[ind][2]);intermediate.second.push_back(rows[ind][3]);intermediate.second.push_back(rows[ind][4]);intermediate.second.push_back(rows[ind][5]);intermediate.second.push_back(temp_ring_config.other_rings[k].first);intermediate.second.push_back(temp_ring_config.other_rings[k].second);
                mapped_evaluation.push_back(intermediate);
                // double temp_utility = max_tree(tc,depth-1,arr, alpha, beta);
                // // cerr<<"Temp utility "<<temp_utility<<endl;
                // if (temp_utility < utility_to_minimize)
                // {
                //  utility_to_minimize = temp_utility;
                // }
                // beta = min_float(beta, temp_utility);
                // if (alpha >= beta) return temp_utility;
              }
            }
          }
          if (rows.size() == 0)
          {
            pair<float,vector<int> > intermediate;
            intermediate.first = evaluation_function(temp_config,arr);
            // if(best_minimisation>intermediate.first) best_minimisation=intermediate.first;

            if(best_minimisation>intermediate.first){
              best_minimisation=intermediate.first;
              if(depth==1){
                beta = min_float(beta, best_minimisation);
                if(alpha>=beta){
                  return best_minimisation;
                  
                }
              } 
            }




            intermediate.second.clear();
            intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);
            mapped_evaluation.push_back(intermediate);
            // double temp_utility = max_tree(temp_config,depth-1,arr, alpha, beta);
            // // cerr<<"Temp utility "<<temp_utility<<endl;
            // if (temp_utility < utility_to_minimize)
            // {
            //  utility_to_minimize = temp_utility;
            // }
            // beta = min_float(beta, temp_utility);
            // if (alpha >= beta) return temp_utility;
          }
          if (foundMarkers) break;
        }
      }
      foundMarkers = false;
      for (int j = ind+1; j < (2 * n + 1); ++j)
      {
        temp_config.board = config.board;
        temp_config.my_rings = config.my_rings;
        temp_config.other_rings = config.other_rings;
        temp_config.num_my_markers = config.num_my_markers;
        temp_config.num_other_markers = config.num_other_markers;
        pair<int,int> t = arr[lineno * (2 * n + 1) + j];
        if (t.first == -1) break;
        int val = temp_config.board[t.first][t.second];
        if (val == 2 || val == 3) foundMarkers = true;
        else if (val == 0 || val == 1) break;
        else 
        {
          select_and_move(x1,x2,t.first,t.second,arr,temp_config,1);
          vector<vector<int> > rows = find_k_throughout(kay,arr,temp_config);
          Config temp_ring_config;
          for (int ind = 0; ind < rows.size(); ++ind)
          {
            if (rows[ind][0] == 3)
            {
              temp_ring_config.board = temp_config.board;
              temp_ring_config.my_rings = temp_config.my_rings;
              temp_ring_config.other_rings = temp_config.other_rings;
              temp_ring_config.num_my_markers = temp_config.num_my_markers;
              temp_ring_config.num_other_markers = temp_config.num_other_markers;
              remove_row(rows[ind][2],rows[ind][3],rows[ind][4],rows[ind][5],arr,temp_ring_config);
              Config tc;
              for (int k = 0; k < temp_ring_config.other_rings.size(); ++k)
              {   
                tc.board = temp_ring_config.board;
                tc.my_rings = temp_ring_config.my_rings;
                tc.other_rings = temp_ring_config.other_rings;
                tc.num_my_markers = temp_ring_config.num_my_markers;
                tc.num_other_markers = temp_ring_config.num_other_markers;
                remove_ring(temp_ring_config.other_rings[k].first,temp_ring_config.other_rings[k].second,tc,1);
                pair<float,vector<int> > intermediate;
                intermediate.first = evaluation_function(tc,arr);
                // if(best_minimisation>intermediate.first) best_minimisation=intermediate.first;
                if(best_minimisation>intermediate.first){
                  best_minimisation=intermediate.first;
                  if(depth==1){
                    beta = min_float(beta, best_minimisation);
                    if(alpha>=beta){
                      return best_minimisation;
                      
                    }
                  } 
                }

                intermediate.second.clear();
                intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);intermediate.second.push_back(rows[ind][2]);intermediate.second.push_back(rows[ind][3]);intermediate.second.push_back(rows[ind][4]);intermediate.second.push_back(rows[ind][5]);intermediate.second.push_back(temp_ring_config.other_rings[k].first);intermediate.second.push_back(temp_ring_config.other_rings[k].second);
                mapped_evaluation.push_back(intermediate);
                // double temp_utility = max_tree(tc,depth-1,arr, alpha, beta);
                // // cerr<<"Temp utility "<<temp_utility<<endl;
                // if (temp_utility < utility_to_minimize)
                // {
                //  utility_to_minimize = temp_utility;
                // }
                // beta = min_float(beta, temp_utility);
                // if (alpha >= beta) return temp_utility;
              }
            }
          }
          if (rows.size() == 0)
          {
            pair<float,vector<int> > intermediate;
            intermediate.first = evaluation_function(temp_config,arr);
            // if(best_minimisation>intermediate.first) best_minimisation=intermediate.first; 
            if(best_minimisation>intermediate.first){
              best_minimisation=intermediate.first;
              if(depth==1){
                beta = min_float(beta, best_minimisation);
                if(alpha>=beta){
                  return best_minimisation;
              
                }
              } 
            }

            intermediate.second.clear();
            intermediate.second.push_back(x1);intermediate.second.push_back(x2);intermediate.second.push_back(t.first);intermediate.second.push_back(t.second);
            mapped_evaluation.push_back(intermediate);
            // double temp_utility = max_tree(temp_config,depth-1,arr, alpha, beta);
            // // cerr<<"Temp utility "<<temp_utility<<endl;
            // if (temp_utility < utility_to_minimize)
            // {
            //  utility_to_minimize = temp_utility;
            // }
            // beta = min_float(beta, temp_utility);
            // if (alpha >= beta) return temp_utility;
          }
          if (foundMarkers) break;
        }
      }
    }
  }

  if(depth==1) {
    //cerr<<"No pruning at this end"<<endl;
    return best_minimisation;
  } 
  
  if(depth>1) sort(mapped_evaluation.begin(), mapped_evaluation.end());
  Config temp_config;
  for (int ind = 0; ind < mapped_evaluation.size(); ++ind)
  {
    temp_config.board = config.board;
    temp_config.my_rings = config.my_rings;
    temp_config.other_rings = config.other_rings;
    temp_config.num_my_markers = config.num_my_markers;
    temp_config.num_other_markers = config.num_other_markers;
    if (mapped_evaluation[ind].second.size() == 10){
      select_and_move(mapped_evaluation[ind].second[0],mapped_evaluation[ind].second[1],mapped_evaluation[ind].second[2],mapped_evaluation[ind].second[3],arr,temp_config,1);
      remove_row(mapped_evaluation[ind].second[4],mapped_evaluation[ind].second[5],mapped_evaluation[ind].second[6],mapped_evaluation[ind].second[7],arr,temp_config);  
      remove_ring(mapped_evaluation[ind].second[8],mapped_evaluation[ind].second[9],temp_config,1);
      double temp_utility = max_tree(temp_config,depth-1,arr, alpha, beta);
      if (temp_utility < utility_to_minimize)
      {
        utility_to_minimize = temp_utility;
      }
      beta = min_float(beta, temp_utility);
      if (alpha >= beta) {
          if(depth>1 ) prunes++; 
          return temp_utility;
      }
    }
    else if (mapped_evaluation[ind].second.size() == 4){
      select_and_move(mapped_evaluation[ind].second[0],mapped_evaluation[ind].second[1],mapped_evaluation[ind].second[2],mapped_evaluation[ind].second[3],arr,temp_config,1);
      double temp_utility = max_tree(temp_config,depth-1,arr, alpha, beta);
      if (temp_utility < utility_to_minimize)
      {
        utility_to_minimize = temp_utility;
      }
      beta = min_float(beta, temp_utility);
      if (alpha >= beta) {
          if(depth>1 ) prunes++;
          return temp_utility;
      }
    }
    else {
      cerr<<"WHAT??"<<endl;
    }
  }
  return utility_to_minimize;
}
//this is main


void play(){
	double others_time=0;
	srand(time(NULL));
	cin>>player>>n>>time_left>>kay;
	cerr<<"The"<<kay<<endl;
	player--;
	t1=clock();
	rings_left_to_place= n;
	if (n == 5) max_depth = 2;
	else max_depth = 4;
	// Set up the board
	// -1 will be empty
	// 0 will be my ring and 2 will be marker
	// 1 will be the other ring and 3 will be the marker

	//declare and initialise the rows array 
	Config config;
	config.board = board_initialisation();

	int directions= (2*n+1)*(3);
	int points_in_one_direction=2*n;
	int row_limit= points_in_one_direction+1; // the extra one so that we know when to cut off
	pair<int,int> arr[directions*(row_limit)];
	fill_positions(arr);
	// t1 = clock();
	string x;
	getline(cin, x);
	if (player == 1)
	{
		int move_t;
		string s;
		getline(cin,s);
		vector<int> move = take_input(&move_t,s);
		execute_move(move,move_t,arr,config,1);
	}

	//initializing threatened vector
	int i,j;
	vector<int> f_row;
	f_row.push_back(0);
	threatened_markers.push_back(f_row);
	for(i=1;i<=n;i++){
		vector<int> temp;
		for(j=0;j<6*i;j++){
			
			temp.push_back(0);
		}
		threatened_markers.push_back(temp);
	}

	//Game loop starts
	int num_moves = 0;
	int counter = 0;
	while (true)
	{
		if (rings_left_to_place > 0)
		{	//places rings at a random x and y
			// if (rings_left_to_place == 6){
			// 	cout<<"P 0 0"<<endl;
			// 	config.my_rings.push_back(pair<int,int>(0,0));
			// 	config.board[0][0] = 0;
			// }
			// if (rings_left_to_place == 5){
			// 	cout<<"P 1 1"<<endl;
			// 	config.my_rings.push_back(pair<int,int>(1,1));
			// 	config.board[1][1] = 0;
			// }
			// if (rings_left_to_place == 4){
			// 	cout<<"P 4 5"<<endl;
			// 	config.my_rings.push_back(pair<int,int>(4,5));
			// 	config.board[4][5] = 0;
			// }
			// if (rings_left_to_place == 3){
			// 	cout<<"P 1 2"<<endl;
			// 	config.my_rings.push_back(pair<int,int>(1,2));
			// 	config.board[1][2] = 0;
			// }
			// if (rings_left_to_place == 2){
			// 	cout<<"P 2 7"<<endl;
			// 	config.my_rings.push_back(pair<int,int>(2,7));
			// 	config.board[2][7] = 0;
			// }
			// if (rings_left_to_place == 1){
			// 	cout<<"P 4 11"<<endl;
			// 	config.my_rings.push_back(pair<int,int>(4,11));
			// 	config.board[4][11] = 0;
			// }
			// rings_left_to_place--;
			int x = rand() % (n+1);
			int y;
			if (x == 0) y = 0;
			else y = rand() % (6*x);	// % with 0 is not defined
			while (config.board[x][y] != -1 || ((x==n) && (y%n==0))){
				x = rand() % (n+1);
				if (x == 0) y = 0;
				else y = rand() % (6*x);
			}
			config.board[x][y] = 0;
			cout<<"P "<<x<<" "<<y<<endl;	// writing to the output so server sends the data
			rings_left_to_place--;
			config.my_rings.push_back(pair<int,int>(x,y));	//an array of rings
			cout.flush();
		}
		else {
			cerr<<"WHY "<<endl;
			print_board(config);
			// cerr<<"My markers are "<<config.num_my_markers<<endl;
			// cerr<<"Other's markers are "<<config.num_other_markers<<endl;
			num_moves++;
			// if (num_moves < 5) max_depth = 3;
			// else if (num_moves < 12) max_depth = 4;
			// else 
			// {
			// 	max_depth = 5;
			// 	// if (config.my_rings.size() == 3) max_depth = 6;
			// }
			vector<vector<int> > rows = find_k_throughout(kay,arr,config);
			clock_t t2,t4;
			// cerr<<"Found the rows"<<endl;
			if (rows.size()==0)
			{
				// cerr<<"No 5 row pehle se"<<endl;
				string will_play;
				clock_t right_now= clock();
				double total_time_elapsed = (double) (right_now - t1) / CLOCKS_PER_SEC;
				if (total_time_elapsed - others_time > time_left - 10) //six- gen later
				{
					will_play = minimax(config,max_depth - 2,arr).first;
				}
				else if (total_time_elapsed - others_time > time_left - 25)
				{
					will_play = minimax(config,max_depth - 1,arr).first;
				}
				else {
					cerr<<"Came here"<<endl;
					t2 = clock();
					will_play = minimax(config,max_depth,arr).first;
					bool prev = true;
					int till_where = 1;
					while (prev){
						prev = false;
						clock_t t3 = clock();
						if (((double)(t3 - t2)/(CLOCKS_PER_SEC)) <((double) 10) / (10 * max(config.my_rings.size(),config.other_rings.size())))
						{
							will_play = minimax(config,max_depth+till_where,arr).first;
							counter++;
							till_where++;
							prev = true;
						}
						t2 = t3;
					}
					cerr<<max_depth+till_where<<endl;
				}
				//cerr<<"I have found the move I want to play"<<endl;
				//cerr<<"Actual "<<((double)(t4 - t3)/(CLOCKS_PER_SEC))<<endl;
				cerr<<counter<<endl;
				int move_t;
				vector<int> move = take_input(&move_t,will_play);
				execute_move(move,move_t,arr,config,0);

				vector<vector<int> > rows_left = find_k_throughout(kay,arr,config);
				if (rows_left.size()>0 && config.my_rings.size()>kay-3)
				{
					// cerr<<"reached"<<endl;
					for (int index = 0; index < rows_left.size(); ++index)
					{
						if (rows_left[index][0] == 2 && config.my_rings.size()>kay-3)
						{
							will_play = will_play + " RS " + to_string(rows_left[index][2]) + " " + to_string(rows_left[index][3]) + " RE " + to_string(rows_left[index][4]) + " " + to_string(rows_left[index][5]) + " X " + to_string(config.my_rings[0].first) + " " + to_string(config.my_rings[0].second);
							remove_row(rows_left[index][2],rows_left[index][3],rows_left[index][4],rows_left[index][5],arr,config);
							remove_ring(config.my_rings[0].first,config.my_rings[0].second,config,0);
							break;
						}
					}
				}

				// vector<vector<int> > rows_left = find_k_throughout(5,arr,config);
				// if (rows_left.size()>0 && config.my_rings.size()>2)
				// {
				// 	for (int index = 0; index < rows_left.size(); ++index)
				// 	{
				// 		if (rows_left[index][0] == 2 && config.my_rings.size()>2)
				// 		{
				// 			will_play = will_play + " RS " + to_string(rows_left[index][2]) + " " + to_string(rows_left[index][3]) + " RE " + to_string(rows_left[index][4]) + " " + to_string(rows[index][5]) + " X " + to_string(config.my_rings[0].first) + " " + to_string(config.my_rings[0].second);
				// 			remove_row(rows[index][2],rows[index][3],rows[index][4],rows[index][5],arr,config);
				// 			remove_ring(config.my_rings[0].first,config.my_rings[0].second,config,0);
				// 		}
				// 	}
				// }
				cout<<will_play<<endl;
				cout.flush();
			}
			else {
				cerr<<"There was already a row of 5 markers"<<endl;
				bool will_win = false;
				Config temp_config;
				double temp_util = -10000000000;
				string maxsuff;
				for (int ind = 0; ind < rows.size(); ++ind)
				{
					if (rows[ind][0] == 2)
					{
						cerr<<"Present here"<<endl;
						temp_config.board = config.board;
						temp_config.my_rings = config.my_rings;
						temp_config.other_rings = config.other_rings;
						temp_config.num_my_markers = config.num_my_markers;
						temp_config.num_other_markers = config.num_other_markers;
						remove_row(rows[ind][2],rows[ind][3],rows[ind][4],rows[ind][5],arr,temp_config);
						Config tc;
						for (int k = 0; k < temp_config.my_rings.size(); ++k)
						{		
							cerr<<"Also here"<<endl;
							tc.board = temp_config.board;
							tc.my_rings = temp_config.my_rings;
							tc.other_rings = temp_config.other_rings;
							tc.num_my_markers = temp_config.num_my_markers;
							tc.num_other_markers = temp_config.num_other_markers;
							remove_ring(temp_config.my_rings[k].first,temp_config.my_rings[k].second,tc,0);
							pair<string,double> ret = minimax(tc,max_depth,arr);
							if (ret.second > temp_util)
							{
								will_win = (tc.my_rings.size() == n-3);
								maxsuff = "RS "+to_string(rows[ind][2])+" "+to_string(rows[ind][3])+" RE "+to_string(rows[ind][4])+" "+to_string(rows[ind][5])+" X "+to_string(temp_config.my_rings[k].first)+" "+to_string(temp_config.my_rings[k].second);
								if (!will_win)
									maxsuff = maxsuff + " " + ret.first;
								temp_util = ret.second;
							}	
						}
					}
				}
				int move_t;
				vector<int> move = take_input(&move_t,maxsuff);
				cout<<maxsuff<<endl;
				if (!will_win)
					execute_move(move,move_t,arr,config,0);
				cout.flush();
			}
		}
		int move_t;
		string s;
		clock_t wait_start= clock();
		getline(cin,s);
		clock_t wait_end= clock();
		others_time=others_time+ ((double)(wait_end- wait_start))/(CLOCKS_PER_SEC);


		vector<int> move = take_input(&move_t,s);
		execute_move(move,move_t,arr,config,1);
	}
}
int main(){
	play();
	return 0;
}


