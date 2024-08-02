#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std::filesystem;

const path my_directory = "enter the path to your folder here";


std::vector<std::string> checker() {
  std::vector<std::string> regular;

  if (exists(my_directory) && is_directory(my_directory)) {
    for (const auto& entry : directory_iterator(my_directory)) {
      file_status fstatus = status(entry);

      std::string full_path = entry.path().string();

      if (fstatus.type() == file_type::regular) {
        regular.push_back(full_path);
      }
    }
  } else {
      std::cerr << "Directory not found." << std::endl;
  }

  return regular;
}

std::string get_extension(const std::string& full_path) {

  path p(full_path);
  std::string extension = p.extension().string(); 

  if (!extension.empty() && extension[0] == '.') {
    extension = extension.substr(1);
  }

  return extension;
}

int checking_dir(const std::string& formation) { 
  path dir_path = my_directory / formation;
  int index_num;

  if (!exists(dir_path)) {
    create_directories(dir_path);
  }
  for (const auto& entry : directory_iterator(dir_path)){
    index_num = std::stoi(entry.path().stem().string());
  }

  return index_num;
}

void grant_permissions(const path& file_path) {

  try {
    permissions(file_path, perms::all);
  } catch (const filesystem_error& e) {
    std::cerr << "error setting permissions for file " << file_path << ": " << e.what() << std::endl;
  }
}

int main() {
  std::vector<std::string> files = checker();
  int index;

  for (const auto& file : files) {
    std::string ext = get_extension(file);
    index = checking_dir(ext) + 1;
    std::string result_name = std::to_string(index) + "." + ext;

    grant_permissions(file);
    
    try {
      rename(file, my_directory / result_name);
    } catch (const filesystem_error& e) {
      std::cerr << "error renaming file " << file << ": " << e.what() << std::endl;
    }
  }
  return 0;
}

