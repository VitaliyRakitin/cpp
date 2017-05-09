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
		}
		
		~Sort(){
			del();
		};
		
		void del(){
			system("rm -rf tmp");
			cout << "tmp folder removed.\n";
		}

		void init(const char * INPUT_FILE){
			system("mkdir tmp");

			ifstream FileIn(INPUT_FILE, ifstream::ate | ios::binary);
			size_t len = FileIn.tellg(); 
			size_t num = 0; 
			size_t position = 0;

			while(len > position){
				cout << "Init process: " << float(position)/float(len) * 100 << " \%;\n";
				FileIn.seekg(position);
				size_t l = buf_len;
				if(buf_len > len - position)
					l = len - position;

				vector<int> buf(l / sizeof(int));
				FileIn.read(reinterpret_cast<char*>(buf.data()), buf.size()*sizeof(int)); 
				int j = 0;
				sort(buf.begin(), buf.end());

				stringstream  adr;
				adr << "tmp/" << num;
				std::ofstream tmp(adr.str(),std::ios::binary);
				tmp.write(reinterpret_cast<const char*>(&buf[0]), buf.size()*sizeof(int));
				position += buf_len;
				num++;
				FILES.push(adr.str());
			}
		}

		void file_sort(){
			while (FILES.size()!= 0){
				vector<string> files;
				files.resize(std::min(sort_number, int(FILES.size())));  
				for (int i=0; i<files.size(); ++i){	
					files[i] = FILES.front();
					FILES.pop();
				}
				merge(files);
			}
		}

		void merge(vector<string> files){
			size_t len = 0;

			vector<vector<int> > buf;
			buf.resize(files.size());  
			for (int i=0; i<buf.size(); ++i){
				ifstream FileIn(files.front(), ifstream::ate | ios::binary);
				len = FileIn.tellg(); 

				buf[i].resize(len/sizeof(int));
				FileIn.read(reinterpret_cast<char*>(buf.data()), buf.size()*sizeof(int)); 
			}
			cout << ">>> size = " << buf.size() << '\n';
			for (int i=0; i<buf.size(); ++i)	
				cout << ">>>>>> sub_size = " << buf[i].size() << '\n';

		}
	private:
		int buf_len;
		int sort_number;
		queue<string> FILES;
};



int main(int argc, char** argv)
{
	size_t MB = 1024 * 1024;
	size_t buf_size = (size_t) atoll(argv[3]) * MB; 
	Sort s(buf_size, 5);
	s.init(argv[1]);
	s.file_sort();
/*
	ifstream FileIn(INPUT_FILE, ifstream::ate | ios::binary);

	size_t len = FileIn.tellg(); 

	cout << "Input file size " << len / MB << "MB\n";

	size_t position = 0;
	int num = 0; 

    queue<string> myQueue;     // создаем пустую очередь типа string
 
    // добавили в очередь несколько элементов типа string
    myQueue.push("No pain ");
    myQueue.push("- no gain");
 
    cout << "Количество элементов в очереди: " << myQueue.size() << endl;
    cout << "\nВот они: " << myQueue.front() << myQueue.back();
 
    myQueue.pop(); // удаляем один элемент в очереди
    cout << "\nТеперь в очереди остался один элемент: " << myQueue.front();
    */
    return 0;
}