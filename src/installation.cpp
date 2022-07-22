#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

class InstallStatus {
public:
	std::string path_name;
	bool status;

	InstallStatus(std::string path_name, bool status)
		:path_name(path_name), status(status) {}

};

void install() {
	std::vector<InstallStatus> status_list;

	if (!std::filesystem::exists("download")) {
		status_list.push_back({"download", std::filesystem::create_directories("download")});
	} 

	for (int i = 0; i < status_list.size(); i++) {
		std::cout << "Install Status : " << status_list[i].path_name << " - " << ((status_list[i].status) ? "Success" : "Failed") << std::endl;
	}
}