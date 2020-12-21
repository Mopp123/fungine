#pragma once

#include "Renderer.h"

/*
	Renderer's are completely static components
*/
namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			

			class TerrainRenderer : public Renderer
			{
			public:
				TerrainRenderer();
				~TerrainRenderer();

				// processes all render passes and after that clears all render ques
				virtual void flush();

				virtual const size_t getSize() const override;
			};
		}
	}
}