#include "ModelManager.h"

namespace meow {
	ModelManager g_ModelManager;

	std::shared_ptr<Model> ModelManager::Get(const std::string& filename)
	{
		if (m_models.find(filename) == m_models.end()) 
		{
			std::shared_ptr<Model> model = std::make_shared<Model>();
			model->load(filename);
			m_models[filename] = model;
		}

		return m_models[filename];
	}


}
