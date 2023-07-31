#pragma once
#include "Model.h"
#include <string>
#include <map>
#include <memory>

namespace meow {
	
	class ModelManager {
	public:
		std::shared_ptr<Model> Get(const std::string& filename);



	private:

		std::map<std::string, std::shared_ptr<Model>> m_models;


	};
	extern ModelManager g_ModelManager;
}