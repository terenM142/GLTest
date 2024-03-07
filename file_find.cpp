#include "file_find.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>


using namespace std;
using namespace filesystem;

bool get_files(const string& newpath, const string& searchfile)
{

	//make try catch because recursive_directory_iterator by default skip
	//folder where access permission is needed, but dont skip syslink which causes abort
	try
	{
		for (const auto& file : recursive_directory_iterator(newpath)) {
			if (file.path().filename() == searchfile) {
				cout << file.path().string() << endl;
				return true;
			}
		}


	}
	catch (const filesystem::filesystem_error& e) {};
	return false;

}
void get_files_in_dir(const string& file)
{

	const string root_path = "/";
	//get_files(root_path, file);
	const unsigned int max_threads = 8;
	atomic_bool flag = false;
	vector <thread> threads;
	queue <string> directory;
	mutex m;

	try
	{
		for (const auto& path_to_dir : directory_iterator(root_path))
		{
			if (path_to_dir.is_directory())
			{
				directory.push(path_to_dir.path().string());
			}
		}
	}
	catch (const filesystem::filesystem_error& e) {};



	threads.reserve(max_threads);
	for (unsigned int i = 0; i < max_threads; ++i) {
		threads.push_back(thread([&]()
			{
				while (true) {
					if (flag)
						return;
					string newpath;
					m.lock();
					if (!directory.empty()) {
						newpath = directory.front();
						directory.pop();
					}
					else {
						m.unlock();
						return;
					}
					m.unlock();
					if (get_files(newpath, file))
						flag = true;
				}
			}));

	}
	for (auto& t : threads)
	{
		t.join();
	}
}

