//  Copyright (c) 2016 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef TOY_DIM_H
#define TOY_DIM_H

/* toy */
#include <toyobj/String/String.h>
#include <toyobj/Object.h>
#include <toyobj/Util/Colour.h>
#include <toyui/Types.h>

/* std */
#include <array>

#if defined _WIN32
#undef OPAQUE
#endif

namespace toy
{
	enum _refl_ Dimension : unsigned int
	{
		DIM_X = 0,
		DIM_Y = 1,
		DIM_XX = 2,
		DIM_YY = 3,
		DIM_NULL = 4
	};

	enum _refl_ Direction : unsigned int
	{
		DIRECTION_NONE = 4,
		READING = 0,
		PARAGRAPH = 1,
		PARALLEL = 2,
		ORTHOGONAL = 3,
	};

	enum _refl_ Pivot : unsigned int
	{
		FORWARD = 0,
		REVERSE = 1
	};

	enum _refl_ Align : unsigned int
	{
		LEFT = 0,
		CENTER = 1,
		RIGHT = 2,
		OUT_LEFT = 3,
		OUT_RIGHT = 4
	};

	extern float AlignExtent[5];
	extern float AlignSpace[5];

	enum _refl_ FrameType : unsigned int
	{
		FRAME = 0,
		LAYER = 1,
		MASTER_LAYER = 2,
		SPACE_LAYER = 3
	};

	enum _refl_ LayoutSolver : unsigned int
	{
		FRAME_SOLVER = 0,
		ROW_SOLVER = 1,
		GRID_SOLVER = 2,
		TABLE_SOLVER = 3,
	};

	enum _refl_ AutoLayout : unsigned int
	{
		NO_LAYOUT = 0,
		AUTO_SIZE = 1,
		AUTO_LAYOUT = 2
	};

	enum _refl_ Flow : unsigned int
	{
		FLOW = 0,			// AUTO_LAYOUT
		OVERLAY = 1,		// AUTO_SIZE
		ALIGN = 2,			// AUTO_POSITION
		FREE = 3			// NO_LAYOUT
	};

	enum _refl_ Sizing : unsigned int
	{
		FIXED,
		SHRINK,
		WRAP,
		EXPAND
	};

	struct _refl_ TOY_UI_EXPORT Space
	{
		_attr_ Direction direction;
		_attr_ Sizing sizingLength;
		_attr_ Sizing sizingDepth;

		Space(Direction dir, Sizing length, Sizing depth) : direction(dir), sizingLength(length), sizingDepth(depth) {}
		Space(SpacePreset preset) { *this = Space::preset(preset); }

		static Space preset(SpacePreset preset);
	};

	enum _refl_ SpacePreset : unsigned int
	{
		SHEET,               // PARAGRAPH   direction, WRAP   length, WRAP   depth
		FLEX,			     // PARALLEL    direction, WRAP   length, WRAP   depth
		ITEM,                // READING     direction, SHRINK length, SHRINK depth
		UNIT,                // PARAGRAPH   direction, SHRINK length, SHRINK depth
		BLOCK,               // PARAGRAPH   direction, FIXED  length, FIXED  depth
		LINE,	             // READING     direction, WRAP   length, SHRINK depth
		STACK,               // PARAGRAPH   direction, SHRINK length, WRAP   depth 
		DIV,	             // ORTHOGONAL  direction, WRAP   length, SHRINK depth
		SPACER,              // PARALLEL    direction, WRAP   length, SHRINK depth
		BOARD,               // READING     direction, EXPAND length, EXPAND depth
		LAYOUT               // PARAGRAPH   direction, EXPAND length, EXPAND depth
	};

	extern Space SpacePresets[11];

	enum _refl_ Clipping : unsigned int
	{
		NOCLIP = 0,
		CLIP = 1
	};

	enum _refl_ Opacity : unsigned int
	{
		OPAQUE = 0,
		CLEAR = 1,
		HOLLOW = 2
	};

	template <class T>
	class _refl_ Dim
	{
	public:
		_constr_ Dim(T a, T b) : d_values{{ a, b }} {}
		_constr_ Dim() : Dim(T(), T()) {}

		T operator[](size_t i) const { return d_values[i]; }
		T& operator[](size_t i) { return d_values[i]; }

	public:
		union {
			std::array<T, 2> d_values;
			struct { _attr_ T x; _attr_ T y; };
			struct { T w; T h; };
		};
	};

	template class _refl_ _array_ _struct_ TOY_UI_EXPORT Dim<size_t>;
	template class _refl_ _array_ _struct_ TOY_UI_EXPORT Dim<AutoLayout>;
	template class _refl_ _array_ _struct_ TOY_UI_EXPORT Dim<Sizing>;
	template class _refl_ _array_ _struct_ TOY_UI_EXPORT Dim<Align>;
	template class _refl_ _array_ _struct_ TOY_UI_EXPORT Dim<Pivot>;

	class _refl_ _struct_ _array_ DimFloat
	{
	public:
		_constr_ DimFloat(float a, float b) : d_values{ { a, b } } {}
		_constr_ DimFloat(float val) : DimFloat(val, val) {}
		DimFloat() : DimFloat(float(), float()) {}

		bool null() const { return d_values[0] == float() && d_values[1] == float(); }

		bool operator==(const DimFloat& other) const { return (x == other.x) && (y == other.y); }

		float operator[](size_t i) const { return d_values[i]; }
		float& operator[](size_t i) { return d_values[i]; }

		DimFloat operator+(const DimFloat& rhs) const { return { this->x + rhs.x, this->y + rhs.y }; }
		DimFloat operator-(const DimFloat& rhs) const { return { this->x - rhs.x, this->y - rhs.y }; }
		DimFloat operator*(const DimFloat& rhs) const { return { this->x * rhs.x, this->y * rhs.y }; }
		DimFloat operator/(const DimFloat& rhs) const { return { this->x / rhs.x, this->y / rhs.y }; }

	public:
		union {
			std::array<float, 2> d_values;
			struct { _attr_ float x; _attr_ float y; };
			struct { float w, h; };
		};
	};

	class _refl_ _struct_ _array_ BoxFloat : public Struct
	{
	public:
		_constr_ BoxFloat(float a, float b, float c, float d) : d_values{{ a, b, c, d }}, d_null(cnull()) {}
		_constr_ BoxFloat(float uniform) : BoxFloat(uniform, uniform, uniform, uniform) {}
		BoxFloat() : BoxFloat(0.f) { d_null = true; }

		BoxFloat(int a, int b, int c, int d) : BoxFloat(float(a), float(b), float(c), float(d)) {}

		float operator[](size_t i) const { return d_values[i]; }
		float& operator[](size_t i) { d_null = false; return d_values[i]; }

		void clear() { this->assign(0.f); d_null = true; }

		bool null() const { return d_null; }
		bool cnull() const { return (x == 0.f && y == 0.f && w == 0.f && h == 0.f); }

		void assign(float a, float b, float c, float d) { x = a; y = b; w = c; h = d; d_null = cnull(); }
		void assign(float val) { x = val; y = val; w = val; h = val; d_null = cnull(); }

		bool intersects(const BoxFloat& other) const
		{
			return !(other.x > x + w || other.y > y + h || other.x + other.w < x || other.y + other.h < y);
		}

		float* pointer() { return &d_values[0]; }

		DimFloat offset() const { return { this->x0, this->y0 }; }
		DimFloat size() const { return DimFloat{ this->x0, this->y0 } + DimFloat{ this->x1, this->y1 }; }

	public:
		union {
			std::array<float, 4> d_values;
			struct { float x0, y0, x1, y1; };
			struct { _attr_ float x; _attr_ float y; _attr_ float w; _attr_ float h; };
		};
		bool d_null;
	};
}

#endif // TOY_DIM_H
