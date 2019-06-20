#pragma once

#include <string>
#include <fstream>
#include <deque>
#include <algorithm>

#include "Angel.h"
#include "Object.h"
#include "Material.h"
#include "Group.h"
#include "Model.h"
#include "Color.h"
#include "Light.h"
#include "Face.h"

class FileReader {
	public:
		static Object read_object(std::string path);
		static void read_materials(std::string obj_dir, std::string mtl_file_name, std::map<std::string, Material>& materials_map);
		static Model read_model(std::string path);
		static void split_string(std::string s, std::deque<std::string>& d);
		static std::string up_directory(std::string path);
};