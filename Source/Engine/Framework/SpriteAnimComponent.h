#pragma once
#include "SpriteComponent.h"
namespace meow {
	class SpriteAnimComponent : public SpriteComponent
	{
	public:
		struct AnimSequence
		{
			std::string name;

			float fps = 0;
			int numColumns = 0;
			int numRows = 0;

			int startFrame = 0;
			int endFrame = 0;

			bool loop = true;

			std::shared_ptr<Texture> texture;
		};
	public:
		CLASS_DECLARATION(SpriteAnimComponent)
		bool Initialize() override;
		void Update(float dt) override;

		void SetSequence(const std::string& name);
		void UpdateSource();

	public:
		int frame = 0;
		float frameTimer = 0;
		AnimSequence* sequence = nullptr;
		std::map<std::string, AnimSequence> sequences;
	private:
		std::string defaultSequenceName;


	};

}