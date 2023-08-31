#include "SpriteAnimComponent.h"
#include "Renderer/Renderer.h"
#include "Framework/Actor.h"
#include "ResourceManager.h"
#include "Core/Logger.h"

namespace meow {

	CLASS_DEFINITION(SpriteAnimComponent)

	bool SpriteAnimComponent::Initialize()
	{
		SpriteComponent::Initialize();

		SetSequence(defaultSequenceName);
		UpdateSource();

		return true;
	}
	void SpriteAnimComponent::Update(float dt)
	{
		frameTimer -= dt;
		if (frameTimer <= 0)
		{
			frameTimer = 1.0f / sequence->fps;
			frame++;
			if (frame > sequence->endFrame)
			{
				frame = (sequence->loop) ? sequence->startFrame : sequence->endFrame;
			}
		}
		UpdateSource();
	}

	void SpriteAnimComponent::SetSequence(const std::string& name)
	{
		
		if (sequence && sequence->name == name) return;

		if(sequences.find(name) != sequences.end())
		{
			//set sequence
			sequence = &sequences[name];
			//set texture
			if (sequence->texture) m_texture = sequence->texture;
			//reset frame info
			frame = sequence->startFrame;
			frameTimer = 1.0f / sequence->fps;
		}
	}

	void SpriteAnimComponent::UpdateSource()
	{
		vec2 cellSize = m_texture->GetSize() / vec2{ sequence->numColumns, sequence->numRows };
		int column = (frame - 1) % sequence->numColumns;
		int row = (frame - 1) / sequence->numColumns;

		source.x = (int)(column * cellSize.x);
		source.y = (int)(row * cellSize.y);
		source.w = (int)(cellSize.x);
		source.h = (int)(cellSize.y);

	}

	void SpriteAnimComponent::Read(const json_t& value) 
	{
		SpriteComponent::Read(value);

		// read in animation sequences
		if (HAS_DATA(value, sequences) && GET_DATA(value, sequences).IsArray())
		{
			for (auto& sequenceValue : GET_DATA(value, sequences).GetArray())
			{
				AnimSequence sequence;
				READ_NAME_DATA(sequenceValue, "name", sequence.name);
				READ_NAME_DATA(sequenceValue, "fps", sequence.fps);
				READ_NAME_DATA(sequenceValue, "numColumns", sequence.numColumns);
				READ_NAME_DATA(sequenceValue, "numRows", sequence.numRows);
				READ_NAME_DATA(sequenceValue, "startFrame", sequence.startFrame);
				READ_NAME_DATA(sequenceValue, "endFrame", sequence.endFrame);
				READ_NAME_DATA(sequenceValue, "loop", sequence.loop);

				// read texture
				std::string textureName;
				READ_DATA(sequenceValue, textureName);
				sequence.texture = GET_RESOURCE(Texture, textureName, g_renderer);

				sequences[sequence.name] = sequence;
			}
		}
		READ_DATA(value, defaultSequenceName);
		if (defaultSequenceName.empty())
		{
			// if default sequence not specified, use the first sequence in the sequences map
			defaultSequenceName = sequences.begin()->first;
		}

	}


}

