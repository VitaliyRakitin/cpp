#include <iostream>
#include <queue>        // подключаем заголовочный файл очереди
#include <string> // заголовочный файл для работы со строками типа string
using namespace std;
 
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream> 
#include <stdio.h>
#include <cstdlib>
 

class Sort{
	public:
		Sort(int buf_size, int sort_num){
			buf_len = buf_size;
			sort_number = sort_num;		
			batch_size = buf_len/(sort_number + 1);
		}
		
		~Sort(){
			del();
		};
		
		void del(){
			//system("rm -rf tmp");
			cout << "tmp folder removed.\n";
		}

		void init(const char * INPUT_FILE){
			system("mkdir tmp");

			ifstream FileIn(INPUT_FILE, ios:: in | ios::binary);				
			FileIn.seekg(0, FileIn.end);

			size_t len = FileIn.tellg(); 
			size_t num = 0; 
			size_t position = 0;
			//cout  << len <<"\n"; 
			while(len > position){
				//cout << "Init process: " << float(position)/float(len) * 100 << " \%;\n";
				FileIn.seekg(position);
				size_t l = buf_len;
				if(buf_len > len - position)
					l = len - position;

				vector<int> buf(l / sizeof(int));
				FileIn.read(reinterpret_cast<char*>(buf.data()), buf.size()*sizeof(int)); 

				int j = 0;
				sort(buf.begin(), buf.end());
				
				stringstream  adr;
				adr <<  "tmp/init" << num;
				////cout << buf[0] << " !!! "<< buf[1] << " " << buf[2] << "\n";

				ofstream FileOut;
				FileOut.open(adr.str(), ios::out | ofstream::binary);
				FileOut.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(int));
				FileOut.close();
				//cout << "SIZE = "<<  buf.size() << "\n";
				position += buf_len;
				num++;
				FILES.push(adr.str());
/*
				vector<int> buf1(buf.size());
				ifstream FileCheck(adr.str(), ios:: in | ios::binary);
				FileCheck.read(reinterpret_cast<char*>(buf1.data()), buf1.size() * sizeof(int)); 
				
				for (int k=0;k< buf.size();k++){
					//cout <<k <<" " <<buf[k] << " "<< buf1[k] << "\n";
				}
				FileCheck.close();
				//break;
				*/

			}
			FileIn.close();
			////cout << "Init process: " << 100 << " \%;\n";
		}

		void file_sort(){
			//cout << "file sort\n";
			int num = 0;
			////cout << FILES.size() << "\n";
			while (FILES.size() > 1){
				cout << "FILES LEFT: "<< FILES.size() << "\n";
				vector<string> files;
				for (int i=0; (i<sort_number) && (i <= FILES.size()) ; ++i){	
					//cout << i << " " << FILES.size();
					//cout << "CHLEN";
					//cout << "PENiSF"<< FILES.front() << "\n HUi\n";
					files.push_back(FILES.front());
					FILES.pop();
				}
				merge(files, num);
				//cout << "h";
				files.clear();
				////cout << "!";
				stringstream  adr;
				adr <<  "tmp/new" << num;
				FILES.push(adr.str());
				////cout << FILES.front() << "\n";
				num++;
			}
			stringstream  cmd;
			cmd <<  "cp " << FILES.front() << " result";
			const string tmp = cmd.str();
			const char* cstr = tmp.c_str();
			cout << cstr << "\n";
			system(cstr);
			
		}

		void merge(vector<string> files, int k){
			//cout << "merge: " << files.size() << ": \n";
			//for(int i=0;i<files.size();i++){
			//	cout << files[i] << " ";
			//}
			//cout << "\n";
			vector<vector<int> > buf;
			vector<int> len;
			vector<int> position;
			vector<int> sorted;
			vector<bool> check;
			vector<size_t> place;

			buf.resize(files.size());  

			len.resize(files.size());  
			position.resize(files.size()); 
			sorted.resize(buf_len);; 
			check.resize(files.size());
			place.resize(files.size());
			//FileIn.resize(files.size());  
				//FileIn.seekg(position);
			////cout << "merge 1: " << files.size() <<"\n" ;

			for (int i=0; i<buf.size(); ++i){
				ifstream FileIn(files[i], ios::in | ios::binary);
				FileIn.seekg(0, FileIn.end);
				len[i] = FileIn.tellg(); 
				//cout << len[i]/sizeof(int) << " ";
				position[i] = min(batch_size, int(len[i]/sizeof(int)));
				cout << files[i] << " " << len[i] << "\n";
				FileIn.seekg(0);


				len[i] -= position[i] * sizeof(int);
 				buf[i].resize(position[i]);			

				FileIn.read(reinterpret_cast<char*>(buf[i].data()), buf[i].size() * sizeof(int)); 
				//cout << buf[i][0] << " "<< buf[i][1] << " " << buf[i][2] << "\n";
				FileIn.close();

 				place[i] = 0;
				check[i] = true;
			}//cout << "\n";
			//cout << "merge 2\n"  << buf[0].size()<<"\n";
			//cout << "@\n";
			bool full_check = true;				
			size_t num = 0;
			int resize = 0;

			
			while (full_check){

				////cout << "1\n";
				////cout << sorted.size() << " " << batch_size / sizeof(int) <<"\n";
				int minimum = INT_MAX;
				size_t min_pos = -1;
				////cout << buf[0].size() << " " << num<< " "<< place[0] << "\n";
				////cout << "2\n";

				for (int i=0; i<buf.size(); i++){
					////cout << check[i]<<" ";
					if ((check[i] == true) && (minimum >= buf[i][place[i]])){
						//cout << "CHECK: " << check[i] << "\n";
						minimum = buf[i][place[i]];
						min_pos = i;
					}
					//cout << buf[i][place[i]] << " ";
				}
				
				//cout << num << " min: " << min_pos << " "<< buf[min_pos][place[min_pos]] << "\n";

				sorted.push_back(buf[min_pos][place[min_pos]]);
				place[min_pos] += 1;

				if (place[min_pos] >= buf[min_pos].size()){
					if (len[min_pos] < 1) {						
	//					//cout << "I AM HERE\n";
						check[min_pos] = false;
					}
					else{

					place[min_pos] = 0;

					ifstream FileIn(files[min_pos], ios::in | ios::binary);
					FileIn.seekg(position[min_pos] * sizeof(int));

					size_t pos = min(batch_size, int(len[min_pos]/sizeof(int)));
					len[min_pos] -= pos * sizeof(int);
					position[min_pos] += pos;

					buf[min_pos].clear();
					buf[min_pos].resize(pos);
					
					FileIn.read(reinterpret_cast<char*>(buf[min_pos].data()), buf[min_pos].size() * sizeof(int));
					//cout << "START";
					//for (int i=0;i<buf[min_pos].size();i++)
					//	cout << buf[min_pos][i] << "\n";

					resize += 1;
//					//cout << "RESIZE " << resize << " " << min_pos << " " << pos << " " << position[min_pos] <<" "<<len[min_pos] << " " << buf[min_pos].size() <<" "<< check[min_pos] << "\n" ;

					}
				}


				if (sorted.size() >= batch_size / sizeof(int)){
					//cout << "OUT\n";
					stringstream  adr;
					adr <<  "tmp/new" << k;
					
					ofstream FileOut(adr.str(), ios::out | ios::app | ios::binary);
					FileOut.write(reinterpret_cast<const char*>(&sorted[0]), sorted.size() * sizeof(int));

					sorted.clear();
				}

				full_check = false;
				for (int i=0;i<check.size();i++)
					full_check += check[i];
				////cout << "!!!!";
				num++;
			}

			//cout << "FULL OUT\n";
			stringstream  adr;
			adr <<  "tmp/new" << k;
			
			ofstream FileOut(adr.str(), ios::out | ios::app | ios::binary);
			FileOut.write(reinterpret_cast<const char*>(&sorted[0]), sorted.size() * sizeof(int));

			sorted.clear();
/*
			0,
			////cout << ">>> "<< k << " : size = " << buf.size() << " Batch = " <<buf_len <<"/"<<sort_number<< "=" <<  batch_size <<'\n';
			for (int i=0; i<buf.size(); ++i)	
				////cout << ">>>>>> sub_size = " << buf[i].size() << " "<< buf[i][0] << " "<< buf[i][1000] << " "<< buf[i][2000] << " "<< buf[i][300000]<< '\n';
*/
		}
	private:
		int buf_len;
		int sort_number;
		int batch_size;
		queue<string> FILES;
};



int main(int argc, char** argv)
{
	size_t MB = 1024 ;
	size_t buf_size = (size_t) atoll(argv[3]) * MB; 
	Sort s(buf_size, 5);
	s.init(argv[1]);
	s.file_sort();

    return 0;
}