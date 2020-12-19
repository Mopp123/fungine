#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include <string>

namespace fungine
{
	namespace components
	{
		class Material;
	}

	namespace graphics
	{

		enum class UniformDataType
		{
			None,

			Float,
			Float2,
			Float3,
			Float4,

			Int,
			Int2,
			Int3,
			Int4,

			Mat4
		};

		template<typename T>
		class ShaderUniform
		{
		private:
			int _location = -1;
			std::string _name;

			T* _data;

			friend class components::Material;

		public:

			ShaderUniform(const ShaderUniform<T>& uniform) :
				_location(uniform._location), _name(uniform._name), _data(uniform._data)
			{}

			ShaderUniform(const std::string& name, T* ptrToValue) :
				_name(name), _data(ptrToValue)
			{}

			inline void setLocation(int location) { _location = location; }
			inline const int& getLocation() const { return _location; }
			inline const std::string& getName() const { return _name; }

			inline const T* const getData() const { return _data; }
		};
	}
}