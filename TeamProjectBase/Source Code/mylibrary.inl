//*************************************************************************************************************
//
// マイライブラリ[Mylibrary.inl]
// Contents : Mylibrary inline functions
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CBMPFILE_INL
#define _CBMPFILE_INL

/*
// struct inline func
// 構造体のインライン関数
*/

//----------------------------------------------------------------------------------------------------
// ビットフィールド
//----------------------------------------------------------------------------------------------------
inline _BITS4&
_BITS4::operator = (const int &rhs)
{
	this->data = rhs;
	return *this;
}

//----------------------------------------------------------------------------------------------------
// 範囲
//----------------------------------------------------------------------------------------------------
/* * コンストラクタ */
inline _RANGE::_RANGE(const int nMax, const int nMin)
{
	this->nMax = nMax;
	this->nMin = nMin;
}
inline _RANGE::_RANGE(CONST INTEGER2SOURCE& rhs)
{
	this->nMax = rhs.nX;
	this->nMin = rhs.nY;
}
inline _RANGE::_RANGE(const int rhs)
{
	this->nMax = rhs;
	this->nMin = rhs;
}
/* * 単項演算子 + */
inline _RANGE
_RANGE::operator + (void) const
{
	return *this;
}
/* * 単項演算子 - */
inline _RANGE
_RANGE::operator - (void) const
{
	return _RANGE(-this->nMax, -this->nMin);
}

/* * 二項演算子 + */
inline _RANGE
_RANGE::operator + (CONST _RANGE& rhs) const
{
	return _RANGE(this->nMax + rhs.nMax, this->nMin + rhs.nMin);
}
inline _RANGE
_RANGE::operator + (CONST INTEGER2SOURCE& rhs) const
{
	return _RANGE(this->nMax + rhs.nX, this->nMin + rhs.nY);
}
inline _RANGE
_RANGE::operator + (CONST int rhs) const
{
	return _RANGE(this->nMax + rhs, this->nMin + rhs);
}

/* * 二項演算子 - */
inline _RANGE
_RANGE::operator - (CONST _RANGE& rhs) const
{
	return _RANGE(this->nMax - rhs.nMax, this->nMin - rhs.nMin);
}
inline _RANGE
_RANGE::operator - (CONST INTEGER2SOURCE& rhs) const
{
	return _RANGE(this->nMax - rhs.nX, this->nMin - rhs.nY);
}
inline _RANGE
_RANGE::operator - (CONST int rhs) const
{
	return _RANGE(this->nMax - rhs, this->nMin - rhs);
}

/* * 二項演算子 * */
inline _RANGE
_RANGE::operator * (CONST _RANGE& rhs) const
{
	_RANGE(this->nMax * rhs.nMax, this->nMin * rhs.nMin);
}
inline _RANGE
_RANGE::operator * (CONST INTEGER2SOURCE& rhs) const
{
	return _RANGE(this->nMax * rhs.nX, this->nMin * rhs.nY);
}
inline _RANGE
_RANGE::operator * (CONST int rhs) const
{
	return _RANGE(this->nMax * rhs, this->nMin * rhs);
}

/* * 二項演算子 / */
inline _RANGE
_RANGE::operator / (CONST _RANGE& rhs) const
{
	return _RANGE(this->nMax / rhs.nMax, this->nMin / rhs.nMin);
}
inline _RANGE
_RANGE::operator / (CONST INTEGER2SOURCE& rhs) const
{
	return _RANGE(this->nMax / rhs.nX, this->nMin / rhs.nY);
}
inline _RANGE
_RANGE::operator / (CONST int rhs) const
{
	return _RANGE(this->nMax / rhs, this->nMin / rhs);
}

/* * 比較演算子 == */
inline bool
_RANGE::operator == (CONST _RANGE& rhs) const
{
	return this->nMax == rhs.nMax && this->nMin == rhs.nMin;
}
inline bool
_RANGE::operator == (CONST INTEGER2SOURCE& rhs) const
{
	return this->nMax == rhs.nX && this->nMin == rhs.nY;
}
inline bool
_RANGE::operator == (CONST int rhs) const
{
	return this->nMax == rhs && this->nMin == rhs;
}

/* * 比較演算子 != */
inline bool
_RANGE::operator != (CONST _RANGE& rhs) const
{
	return this->nMax != rhs.nMax || this->nMin != rhs.nMin;
}
inline bool
_RANGE::operator != (CONST INTEGER2SOURCE& rhs) const
{
	return this->nMax != rhs.nX || this->nMin != rhs.nY;
}
inline bool
_RANGE::operator != (CONST int rhs) const
{
	return this->nMax != rhs || this->nMin != rhs;
}
/* * 代入演算子 += */
inline _RANGE&
_RANGE::operator += (CONST _RANGE& rhs)
{
	this->nMax += rhs.nMax;
	this->nMin += rhs.nMin;
	return *this;
}
inline _RANGE&
_RANGE::operator += (CONST INTEGER2SOURCE& rhs)
{
	this->nMax += rhs.nX;
	this->nMin += rhs.nY;
	return *this;
}
inline _RANGE&
_RANGE::operator += (CONST int rhs)
{
	this->nMax += rhs;
	this->nMin += rhs;
	return *this;
}

//* * 代入演算子 -= */
inline _RANGE&
_RANGE::operator -= (CONST _RANGE& rhs)
{
	this->nMax -= rhs.nMax;
	this->nMin -= rhs.nMin;
	return *this;
}
inline _RANGE&
_RANGE::operator -= (CONST INTEGER2SOURCE& rhs)
{
	this->nMax -= rhs.nX;
	this->nMin -= rhs.nY;
	return *this;
}
inline _RANGE&
_RANGE::operator -= (CONST int rhs)
{
	this->nMax -= rhs;
	this->nMin -= rhs;
	return *this;
}

//* * 代入演算子 *= */
inline _RANGE&
_RANGE::operator *= (CONST _RANGE& rhs)
{
	this->nMax *= rhs.nMax;
	this->nMin *= rhs.nMin;
	return *this;
}
inline _RANGE&
_RANGE::operator *= (CONST INTEGER2SOURCE& rhs)
{
	this->nMax *= rhs.nX;
	this->nMin *= rhs.nY;
	return *this;
}
inline _RANGE&
_RANGE::operator *= (CONST int rhs)
{
	this->nMax *= rhs;
	this->nMin *= rhs;
	return *this;
}

//* * 代入演算子 /= */
inline _RANGE&
_RANGE::operator /= (CONST _RANGE& rhs)
{
	this->nMax /= rhs.nMax;
	this->nMin /= rhs.nMin;
	return *this;
}
inline _RANGE&
_RANGE::operator /= (CONST INTEGER2SOURCE& rhs)
{
	this->nMax /= rhs.nX;
	this->nMin /= rhs.nY;
	return *this;
}
inline _RANGE&
_RANGE::operator /= (CONST int rhs)
{
	this->nMax /= rhs;
	this->nMin /= rhs;
	return *this;
}

/* * メンバアクセス演算子[] */
inline int
_RANGE::operator[] (unsigned int nIdx) const
{
	return (&this->nMax)[nIdx];
}
inline int&
_RANGE::operator[] (unsigned int nIdx)
{
	return (&this->nMax)[nIdx];
}

/* * 乱数の取得 */
inline int
_RANGE::GetRand(void)
{
	return (rand() % nMax) + nMin;
}

/* * 取得できるか？ */
inline bool
_RANGE::IsItGet(void)
{
	return (nMax != 0);
}

//----------------------------------------------------------------------------------------------------
// INTEGER2
//----------------------------------------------------------------------------------------------------
/* * コンストラクタ */
inline
INTEGER2::INTEGER2(const int nMax, const int nMin)
{
	this->nX = nMax;
	this->nY = nMin;
}
inline
INTEGER2::INTEGER2(CONST _RANGE& rhs)
{
	this->nX = rhs.nMax;
	this->nY = rhs.nMin;
}
inline
INTEGER2::INTEGER2(const int rhs)
{
	this->nX = rhs;
	this->nY = rhs;
}

/* * 単項演算子 + */
inline INTEGER2
INTEGER2::operator + (void) const
{
	return *this;
}
/* * 単項演算子 - */
inline INTEGER2
INTEGER2::operator - (void) const
{
	return INTEGER2(-this->nX, -this->nY);
}

/* * 二項演算子 + */
inline INTEGER2
INTEGER2::operator + (CONST INTEGER2& rhs) const
{
	return INTEGER2(this->nX + rhs.nX, this->nY + rhs.nY);
}
inline INTEGER2
INTEGER2::operator + (CONST _RANGE& rhs) const
{
	return INTEGER2(this->nX + rhs.nMax, this->nY + rhs.nMin);
}
inline INTEGER2
INTEGER2::operator + (CONST int rhs) const
{
	return INTEGER2(this->nX + rhs, this->nY + rhs);
}

/* * 二項演算子 - */
inline INTEGER2
INTEGER2::operator - (CONST INTEGER2& rhs) const
{
	return INTEGER2(this->nX - rhs.nX, this->nY - rhs.nY);
}
inline INTEGER2
INTEGER2::operator - (CONST _RANGE& rhs) const
{
	return INTEGER2(this->nX - rhs.nMax, this->nY - rhs.nMin);
}
inline INTEGER2
INTEGER2::operator - (CONST int rhs) const
{
	return INTEGER2(this->nX - rhs, this->nY - rhs);
}

/* * 二項演算子 * */
inline INTEGER2
INTEGER2::operator * (CONST INTEGER2& rhs) const
{
	return INTEGER2(this->nX * rhs.nX, this->nY * rhs.nY);
}
inline INTEGER2
INTEGER2::operator * (CONST _RANGE& rhs) const
{
	return INTEGER2(this->nX * rhs.nMax, this->nY * rhs.nMin);
}
inline INTEGER2
INTEGER2::operator * (CONST int rhs) const
{
	return INTEGER2(this->nX * rhs, this->nY * rhs);
}

/* * 二項演算子 / */
inline INTEGER2
INTEGER2::operator / (CONST INTEGER2& rhs) const
{
	return INTEGER2(this->nX / rhs.nX, this->nY / rhs.nY);
}
inline INTEGER2
INTEGER2::operator / (CONST _RANGE& rhs) const
{
	return INTEGER2(this->nX / rhs.nMax, this->nY / rhs.nMin);
}
inline INTEGER2
INTEGER2::operator / (CONST int rhs) const
{
	return INTEGER2(this->nX / rhs, this->nY / rhs);
}

/* * 比較演算子 == */
inline bool
INTEGER2::operator == (CONST INTEGER2& rhs) const
{
	return this->nX == rhs.nX && this->nY == rhs.nY;
}
inline bool
INTEGER2::operator == (CONST _RANGE& rhs) const
{
	return this->nX == rhs.nMax && this->nY == rhs.nMin;
}
inline bool
INTEGER2::operator == (CONST int rhs) const
{
	return this->nX == rhs && this->nY == rhs;
}

/* * 比較演算子 != */
inline bool
INTEGER2::operator != (CONST INTEGER2& rhs) const
{
	return this->nX != rhs.nX || this->nY != rhs.nY;
}
inline bool
INTEGER2::operator != (CONST _RANGE& rhs) const
{
	return this->nX != rhs.nMax || this->nY != rhs.nMin;
}
inline bool
INTEGER2::operator != (CONST int rhs) const
{
	return this->nX != rhs || this->nY != rhs;
}

/* * 代入演算子 += */
inline INTEGER2&
INTEGER2::operator += (CONST INTEGER2& rhs)
{
	this->nX += rhs.nX;
	this->nY += rhs.nY;
	return *this;
}
inline INTEGER2&
INTEGER2::operator += (CONST _RANGE& rhs)
{
	this->nX += rhs.nMax;
	this->nY += rhs.nMin;
	return *this;
}
inline INTEGER2&
INTEGER2::operator += (CONST int rhs)
{
	this->nX += rhs;
	this->nY += rhs;
	return *this;
}

//* * 代入演算子 -= */
inline INTEGER2&
INTEGER2::operator -= (CONST INTEGER2& rhs)
{
	this->nX -= rhs.nX;
	this->nY -= rhs.nY;
	return *this;
}
inline INTEGER2&
INTEGER2::operator -= (CONST _RANGE& rhs)
{
	this->nX -= rhs.nMax;
	this->nY -= rhs.nMin;
	return *this;
}
inline INTEGER2&
INTEGER2::operator -= (CONST int rhs)
{
	this->nX -= rhs;
	this->nY -= rhs;
	return *this;
}

//* * 代入演算子 *= */
inline INTEGER2&
INTEGER2::operator *= (CONST INTEGER2& rhs)
{
	this->nX *= rhs.nX;
	this->nY *= rhs.nY;
	return *this;
}
inline INTEGER2&
INTEGER2::operator *= (CONST _RANGE& rhs)
{
	this->nX *= rhs.nMax;
	this->nY *= rhs.nMin;
	return *this;
}
inline INTEGER2&
INTEGER2::operator *= (CONST int rhs)
{
	this->nX *= rhs;
	this->nY *= rhs;
	return *this;
}

//* * 代入演算子 /= */
inline INTEGER2&
INTEGER2::operator /= (CONST INTEGER2& rhs)
{
	this->nX /= rhs.nX;
	this->nY /= rhs.nY;
	return *this;
}
inline INTEGER2&
INTEGER2::operator /= (CONST _RANGE& rhs)
{
	this->nX /= rhs.nMax;
	this->nY /= rhs.nMin;
	return *this;
}
inline INTEGER2&
INTEGER2::operator /= (CONST int rhs)
{
	this->nX /= rhs;
	this->nY /= rhs;
	return *this;
}

/* * メンバアクセス演算子[] */
inline int
INTEGER2::operator[] (unsigned int nIdx) const
{
	return (&this->nX)[nIdx];
}
inline int&
INTEGER2::operator[] (unsigned int nIdx)
{
	return (&this->nX)[nIdx];
}

//----------------------------------------------------------------------------------------------------
// INTEGER3
//----------------------------------------------------------------------------------------------------
inline
INTEGER3::INTEGER3(int nX, int nY, int nZ)
{
	this->x = nX;
	this->y = nY;
	this->z = nZ;
}
inline
INTEGER3::INTEGER3(int nRhs)
{
	this->x = nRhs;
	this->y = nRhs;
	this->z = nRhs;
}

/* * 単項演算子 + */
inline INTEGER3
INTEGER3::operator + (void) const
{
	return *this;
}

/* * 単項演算子 - */
inline INTEGER3
INTEGER3::operator - (void) const
{
	return INTEGER3(-this->x, -this->y, -this->z);
}

/* * 二項演算子 + */
inline INTEGER3
INTEGER3::operator + (CONST INTEGER3& rhs) const
{
	return INTEGER3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}
inline INTEGER3
INTEGER3::operator + (CONST int rhs) const
{
	return INTEGER3(this->x + rhs, this->y + rhs, this->z + rhs);
}

/* * 二項演算子 - */
inline INTEGER3
INTEGER3::operator - (CONST INTEGER3& rhs) const
{
	return INTEGER3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}
inline INTEGER3
INTEGER3::operator - (CONST int rhs) const
{
	return INTEGER3(this->x - rhs, this->y - rhs, this->z - rhs);
}

/* * 二項演算子 * */
inline INTEGER3
INTEGER3::operator * (CONST INTEGER3& rhs) const
{
	return INTEGER3(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
}
inline INTEGER3
INTEGER3::operator * (CONST int rhs) const
{
	return INTEGER3(this->x * rhs, this->y * rhs, this->z * rhs);
}

/* * 二項演算子 / */
inline INTEGER3
INTEGER3::operator / (CONST INTEGER3& rhs) const
{
	return INTEGER3(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
}
inline INTEGER3
INTEGER3::operator / (CONST int rhs) const
{
	return INTEGER3(this->x / rhs, this->y / rhs, this->z / rhs);
}

/* * 比較演算子 == */
inline bool
INTEGER3::operator == (CONST INTEGER3& rhs) const
{
	return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
}
inline bool
INTEGER3::operator == (CONST int rhs) const
{
	return this->x == rhs && this->y == rhs && this->z == rhs;
}

/* * 比較演算子 != */
inline bool
INTEGER3::operator != (CONST INTEGER3& rhs) const
{
	return this->x != rhs.x || this->y != rhs.y || this->z != rhs.z;
}
inline bool
INTEGER3::operator != (CONST int rhs) const
{
	return this->x != rhs || this->y != rhs || this->z != rhs;
}

/* * 代入演算子 += */
inline INTEGER3&
INTEGER3::operator += (CONST INTEGER3& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return *this;
}
inline INTEGER3&
INTEGER3::operator += (CONST int rhs)
{
	this->x += rhs;
	this->y += rhs;
	this->z += rhs;
	return *this;
}

/* * 代入演算子 -= */
inline INTEGER3&
INTEGER3::operator -= (CONST INTEGER3& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	return *this;
}
inline INTEGER3&
INTEGER3::operator -= (CONST int rhs)
{
	this->x -= rhs;
	this->y -= rhs;
	this->z -= rhs;
	return *this;
}

/* * 代入演算子 *= */
inline INTEGER3&
INTEGER3::operator *= (CONST INTEGER3& rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
	this->z *= rhs.z;
	return *this;
}
inline INTEGER3&
INTEGER3::operator *= (CONST int rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	return *this;
}

/* * 代入演算子 /= */
inline INTEGER3&
INTEGER3::operator /= (CONST INTEGER3& rhs)
{
	this->x /= rhs.x;
	this->y /= rhs.y;
	this->z /= rhs.z;
	return *this;
}
inline INTEGER3&
INTEGER3::operator /= (CONST int rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	return *this;
}

/* * メンバアクセス演算子[] */
inline int
INTEGER3::operator[] (unsigned int nIdx) const
{
	return (&this->x)[nIdx];
}
inline int&
INTEGER3::operator[] (unsigned int nIdx)
{
	return (&this->x)[nIdx];
}

//----------------------------------------------------------------------------------------------------
// INTEGER4
//----------------------------------------------------------------------------------------------------
/* * コンストラクタ */
inline
INTEGER4::INTEGER4(int x, int y, int z, int w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
inline
INTEGER4::INTEGER4(int nRhs)
{
	this->x = nRhs;
	this->y = nRhs;
	this->z = nRhs;
	this->w = nRhs;
}

/* * 単項演算子 + */
inline INTEGER4
INTEGER4::operator + (void) const
{
	return *this;
}

/* * 単項演算子 - */
inline INTEGER4
INTEGER4::operator - (void) const
{
	return INTEGER4(-this->x, -this->y, -this->z, -this->w);
}

/* * 二項演算子 + */
inline INTEGER4
INTEGER4::operator + (CONST INTEGER4& rhs) const
{
	return INTEGER4(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w);
}
inline INTEGER4
INTEGER4::operator + (CONST int rhs) const
{
	return INTEGER4(this->x + rhs, this->y + rhs, this->z + rhs, this->w + rhs);
}

/* * 二項演算子 - */
inline INTEGER4
INTEGER4::operator - (CONST INTEGER4& rhs) const
{
	return INTEGER4(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w);
}
inline INTEGER4
INTEGER4::operator - (CONST int rhs) const
{
	return INTEGER4(this->x - rhs, this->y - rhs, this->z - rhs, this->w - rhs);
}

/* * 二項演算子 * */
inline INTEGER4
INTEGER4::operator * (CONST INTEGER4& rhs) const
{
	return INTEGER4(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w);
}
inline INTEGER4
INTEGER4::operator * (CONST int rhs) const
{
	return INTEGER4(this->x * rhs, this->y * rhs, this->z * rhs, this->w * rhs);
}

/* * 二項演算子 / */
inline INTEGER4
INTEGER4::operator / (CONST INTEGER4& rhs) const
{
	return INTEGER4(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z, this->w / rhs.w);
}
inline INTEGER4
INTEGER4::operator / (CONST int rhs) const
{
	return INTEGER4(this->x / rhs, this->y / rhs, this->z / rhs, this->w / rhs);
}

/* * 比較演算子 == */
inline bool
INTEGER4::operator == (CONST INTEGER4& rhs) const
{
	return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w;
}
inline bool
INTEGER4::operator == (CONST int rhs) const
{
	return this->x == rhs && this->y == rhs && this->z == rhs && this->w == rhs;
}

/* * 比較演算子 != */
inline bool
INTEGER4::operator != (CONST INTEGER4& rhs) const
{
	return this->x != rhs.x || this->y != rhs.y || this->z != rhs.z || this->w != rhs.w;
}
inline bool
INTEGER4::operator != (CONST int rhs) const
{
	return this->x != rhs || this->y != rhs || this->z != rhs || this->w != rhs;
}

/* * 代入演算子 += */
inline INTEGER4&
INTEGER4::operator += (CONST INTEGER4& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	this->w += rhs.w;
	return *this;
}
inline INTEGER4&
INTEGER4::operator += (CONST int rhs)
{
	this->x += rhs;
	this->y += rhs;
	this->z += rhs;
	this->w += rhs;
	return *this;
}

/* * 代入演算子 -= */
inline INTEGER4&
INTEGER4::operator -= (CONST INTEGER4& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	this->w -= rhs.w;
	return *this;
}
inline INTEGER4&
INTEGER4::operator -= (CONST int rhs)
{
	this->x -= rhs;
	this->y -= rhs;
	this->z -= rhs;
	this->w -= rhs;
	return *this;
}

/* * 代入演算子 *= */
inline INTEGER4&
INTEGER4::operator *= (CONST INTEGER4& rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
	this->z *= rhs.z;
	this->w *= rhs.w;
	return *this;
}
inline INTEGER4&
INTEGER4::operator *= (CONST int rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	this->w *= rhs;
	return *this;
}

/* * 代入演算子 /= */
inline INTEGER4&
INTEGER4::operator /= (CONST INTEGER4& rhs)
{
	this->x /= rhs.x;
	this->y /= rhs.y;
	this->z /= rhs.z;
	this->w /= rhs.w;
	return *this;
}
inline INTEGER4&
INTEGER4::operator /= (CONST int rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	this->w /= rhs;
	return *this;
}

/* * メンバアクセス演算子[] */
inline int
INTEGER4::operator[] (unsigned int nIdx) const
{
	return (&this->x)[nIdx];
}
inline int&
INTEGER4::operator[] (unsigned int nIdx)
{
	return (&this->x)[nIdx];
}


//----------------------------------------------------------------------------------------------------
// 2成分float
//----------------------------------------------------------------------------------------------------
inline FLOAT2
FLOAT2::operator +(const FLOAT2 &rhs) const
{
	return FLOAT2(x + rhs.x, y + rhs.y);
}

inline FLOAT2
FLOAT2::operator -(const FLOAT2 &rhs) const
{
	return FLOAT2(x - rhs.x, y - rhs.y);
}

inline FLOAT2
FLOAT2::operator -(void) const
{
	return FLOAT2(x * -1.0f, y * -1.0f);
}

inline FLOAT2
FLOAT2::operator *(const FLOAT2 &rhs) const
{
	return FLOAT2(x * rhs.x, y * rhs.y);
}

inline FLOAT2
FLOAT2::operator /(const FLOAT2 &rhs) const
{
	return FLOAT2(x / rhs.x, y / rhs.y);
}

inline FLOAT2
FLOAT2::operator *(float rhs) const
{
	return FLOAT2(x * rhs, y * rhs);
}

inline FLOAT2
FLOAT2::operator /(float rhs) const
{
	return FLOAT2(x / rhs, y / rhs);
}

inline FLOAT2
FLOAT2::operator +(const INTEGER2 &rhs) const
{
	return FLOAT2(x + rhs.nX, y + rhs.nY);
}

inline FLOAT2
FLOAT2::operator -(const INTEGER2 &rhs) const
{
	return FLOAT2(x - rhs.nX, y - rhs.nY);
}

inline FLOAT2
FLOAT2::operator *(const INTEGER2 &rhs) const
{
	return FLOAT2(x * rhs.nX, y * rhs.nY);
}

inline FLOAT2
FLOAT2::operator /(const INTEGER2 &rhs) const
{
	return FLOAT2(x / rhs.nX, y / rhs.nY);
}

inline float
FLOAT2::Dot(const FLOAT2 &rhs) const
{
	return x * rhs.x + y * rhs.y;
}

inline float
FLOAT2::Cross(const FLOAT2 &rhs) const
{
	return x * rhs.y - y * rhs.x;
}

inline float
FLOAT2::Length(void) const
{
	return sqrtf(LengthSq());
}

inline float
FLOAT2::LengthSq(void) const
{
	return x * x + y * y;
}

inline void
FLOAT2::Norm(void)
{
	const float fLength = Length();
	if (fLength > 0.0f)
	{
		x /= fLength;
		y /= fLength;
	}
}

inline FLOAT2
FLOAT2::GetNorm(void) const
{
	const float fLength = Length();
	if (fLength > 0.0f) {
		return FLOAT2(x / fLength, y / fLength);
	}
	return FLOAT2(0.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------
// 2Dベクトル
//----------------------------------------------------------------------------------------------------
inline VEC2
& VEC2::operator=(const FLOAT2 & rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

inline bool
VEC2::IsVertical(const VEC2 &rhs) const
{
	float DotValue = Dot(rhs);
	return (-MYLIB_OX_EPSILON < DotValue && DotValue < MYLIB_OX_EPSILON);	// 誤差範囲内なら垂直と判定
}

inline bool
VEC2::IsParallel(const VEC2 &rhs) const
{
	float CrossValue = Cross(rhs);
	return (-MYLIB_OX_EPSILON < CrossValue && CrossValue < MYLIB_OX_EPSILON);	// 誤差範囲内なら平行と判定
}

inline bool
VEC2::IsSharpAngle(const VEC2 &rhs) const
{
	return (Dot(rhs) >= 0.0f);
}

//----------------------------------------------------------------------------------------------------
// 2D直線
//----------------------------------------------------------------------------------------------------
inline FLOAT2
LINE_2D::GetPoint(float fCoffi) const
{
	return pos + fCoffi * vec;
}

//----------------------------------------------------------------------------------------------------
// 2D線分
//----------------------------------------------------------------------------------------------------
inline FLOAT2 SEGMENT_2D::GetEndPoint(void) const
{
	return pos + vec;
}

//----------------------------------------------------------------------------------------------------
// 2Dカプセル
//----------------------------------------------------------------------------------------------------
inline void
CAPSULE_2D::set(SEGMENT_2D &seg, float &fRadius)
{
	this->fRadius = fRadius;
	this->Segment = seg;
}

inline void
CAPSULE_2D::set(CONST FLOAT2 &pos, CONST VEC2 &vec, float &fRadius)
{
	this->fRadius = fRadius;
	this->Segment.pos = pos;
	this->Segment.vec = vec;
}

//----------------------------------------------------------------------------------------------------
// 3成分float
//----------------------------------------------------------------------------------------------------
inline FLOAT3
FLOAT3::operator +(const FLOAT3 &rhs) const
{
	return FLOAT3(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline FLOAT3
FLOAT3::operator -(const FLOAT3 &rhs) const
{
	return FLOAT3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline FLOAT3
FLOAT3::operator -(void) const
{
	return FLOAT3(x * -1.0f, y * -1.0f, z * -1.0f);
}

inline FLOAT3
FLOAT3::operator *(const FLOAT3 &rhs) const
{
	return FLOAT3(x * rhs.x, y * rhs.y, z * rhs.z);
}

inline FLOAT3
FLOAT3::operator /(const FLOAT3 &rhs) const
{
	return FLOAT3(x / rhs.x, y / rhs.y, z / rhs.z);
}

inline FLOAT3
FLOAT3::operator *(float rhs) const
{
	return FLOAT3(x * rhs, y * rhs, z * rhs);
}

inline FLOAT3
FLOAT3::operator /(float rhs) const
{
	return FLOAT3(x / rhs, y / rhs, z / rhs);
}

inline float
FLOAT3::Dot(const FLOAT3 &rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

inline FLOAT3
FLOAT3::Cross(const FLOAT3 &rhs) const
{
	return FLOAT3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

inline float
FLOAT3::Length() const
{
	return sqrtf(LengthSq());
}

inline float
FLOAT3::LengthSq() const
{
	return x * x + y * y + z * z;
}

inline void
FLOAT3::Norm()
{
	const float fLength = Length();
	if (fLength > 0.0f)
	{
		x /= fLength;
		y /= fLength;
		z /= fLength;
	}
}

inline FLOAT3
FLOAT3::GetNorm() const
{
	const float fLength = Length();
	if (fLength > 0.0f) {
		return FLOAT3(x / fLength, y / fLength, z / fLength);
	}
	return FLOAT3(0.0f, 0.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------
// 3Dベクトル
//----------------------------------------------------------------------------------------------------
inline VEC3& VEC3::operator =(const FLOAT3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

inline bool
VEC3::IsVertical(const VEC3 &rhs) const
{
	float DotValue = Dot(rhs);
	return (-MYLIB_OX_EPSILON < DotValue && DotValue < MYLIB_OX_EPSILON);	// 誤差範囲内なら垂直と判定
}

inline bool
VEC3::IsParallel(const VEC3 &rhs) const
{
	float CrossValue = Cross(rhs).LengthSq();
	return (-MYLIB_OX_EPSILON < CrossValue && CrossValue < MYLIB_OX_EPSILON);	// 誤差範囲内なら平行と判定
}

inline bool
VEC3::IsSharpAngle(const VEC3 &rhs) const
{
	return (Dot(rhs) >= 0.0f);
}

//----------------------------------------------------------------------------------------------------
// 直線
//----------------------------------------------------------------------------------------------------
inline FLOAT3
LINE::GetPoint(float fCoffi) const
{
	return pos + fCoffi * vec;
}

//----------------------------------------------------------------------------------------------------
// 線分
//----------------------------------------------------------------------------------------------------
inline FLOAT3
SEGMENT::GetEndPoint() const
{
	return pos + vec;
}

//----------------------------------------------------------------------------------------------------
// マトリックス
//----------------------------------------------------------------------------------------------------
inline MATRIX
MATRIX::operator +(CONST MATRIX &rhs) const
{
	return MATRIX(_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
		_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
		_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
		_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
}
inline MATRIX
MATRIX::operator -(CONST MATRIX &rhs) const
{
	return MATRIX(_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
		_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
		_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
		_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
}
inline MATRIX
MATRIX::operator -(void) const
{
	return MATRIX(
		-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44);
}
inline MATRIX
MATRIX::operator *(CONST MATRIX &rhs) const
{
	D3DXMATRIX matT;
	D3DXMatrixMultiply(&matT, this, &rhs);
	return matT;
}

inline MATRIX
MATRIX::operator * (float rhs) const
{
	return MATRIX(
		_11 * rhs, _12 * rhs, _13 * rhs, _14 * rhs,
		_21 * rhs, _22 * rhs, _23 * rhs, _24 * rhs,
		_31 * rhs, _32 * rhs, _33 * rhs, _34 * rhs,
		_41 * rhs, _42 * rhs, _43 * rhs, _44 * rhs);
}

inline MATRIX
MATRIX::operator / (float rhs) const
{
	float fInv = 1.0f / rhs;
	return MATRIX(
		_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
		_21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
		_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
		_41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
}

inline void
MATRIX::Identity(void)
{
	D3DXMatrixIdentity(this);
}

inline void
MATRIX::Inverse(MATRIX *pMat, float *pDeterminant)
{
	D3DXMatrixInverse(this, pDeterminant, pMat);
}

//----------------------------------------------------------------------------------------------------
// タイマー情報
//----------------------------------------------------------------------------------------------------
/* * コンストラクタ */
inline _TIMER_INFO::_TIMER_INFO(int nStart, int nEnd)
{
	this->nStart = nStart;
	this->nEnd = nEnd;
}


//----------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報
//----------------------------------------------------------------------------------------------------
inline _SETING_UV::_SETING_UV(int nCntU, int nNumU, int nCntV, int nNumV)
{
	this->nCntU = nCntU;
	this->nNumU = nNumU;
	this->nCntV = nCntV;
	this->nNumV = nNumV;
}

//----------------------------------------------------------------------------------------------------
// テクスチャアニメーション情報
//----------------------------------------------------------------------------------------------------
inline _TEXTUREANIMEINFO::_TEXTUREANIMEINFO(int nFrame, int nFrameMax, int nIteration)
{
	this->nFrame = nFrame;
	this->nFrameMax = nFrameMax;
	this->nIteration = nIteration;
}


//----------------------------------------------------------------------------------------------------
// トランスフォーム情報
//----------------------------------------------------------------------------------------------------
///* * コンストラクタ */
//inline
//TRANSFORM::TRANSFORM(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &scal, D3DXMATRIX &mtxWorld)
//{
//	this->pos = pos;
//	this->rot = rot;
//	this->scal = scal;
//	this->mtxWorld = mtxWorld;
//}
//inline
//TRANSFORM::TRANSFORM(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &scal)
//{
//	this->pos = pos;
//	this->rot = rot;
//	this->scal = scal;
//}

// マトリックスの初期化
inline void
TRANSFORM::Identity(void)
{
	D3DXMatrixIdentity(&mtxWorld);
}

//----------------------------------------------------------------------------------------------------
// ゲージ情報
//----------------------------------------------------------------------------------------------------
/* * コンストラクタ */
inline
GAGE_INFO::GAGE_INFO(RANGE SourceRange, int nSourceValue, float fSourceCoefficient)
{
	range = SourceRange;
	nValue = nSourceValue;
	fCoefficient = fSourceCoefficient;
}

/*
// class inline func
// クラスのインライン関数
*/

//----------------------------------------------------------------------------------------------------
// class CMeasurement　計測クラス
//----------------------------------------------------------------------------------------------------
/* * 計測開始 */
inline void
CMeasurement::Start(void)
{
#ifdef MYLIB_MEASURE
	// 開始時の時間の取得
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_start);
#endif
}

/* * 計測終了 */
inline void
CMeasurement::End(void)
{
#ifdef MYLIB_MEASURE
	// 終了時の時間の取得
	QueryPerformanceCounter(&m_end);
#endif
}

/* * 描画 */
inline void
CMeasurement::Draw(void)
{
#ifdef MYLIB_MEASURE
	std::cout << "処理速度 == " << (m_end.QuadPart - m_start.QuadPart) * 1000.0 / m_freq.QuadPart  << "[ms.]\n";
#endif
}

inline double
CMeasurement::GetTime(void)
{
#ifdef MYLIB_MEASURE
	return (m_end.QuadPart - m_start.QuadPart) * 1000.0 / m_freq.QuadPart;
#endif
}

//----------------------------------------------------------------------------------------------------
// template<class T>class MyVector マイベクタークラス
//----------------------------------------------------------------------------------------------------
/* * コンストラクタ */
template<class T>
inline MyVector<T>::MyVector()
{
	this->m_nSize = MYLIB_INT_UNSET;
	this->m_nCapacity = MYLIB_INT_UNSET;
	this->m_pData = NULL;
}

/* * コンストラクタ */
template<class T>
inline MyVector<T>::MyVector(const MyVector<T>& source)
{
	this->m_nSize = MYLIB_INT_UNSET;
	this->m_nCapacity = MYLIB_INT_UNSET;
	this->m_pData = NULL;
	operator=(source);
}

// デストラクタ
template<class T>
inline MyVector<T>::~MyVector()
{
	if (this->m_pData != NULL)
	{
		delete[]this->m_pData;
		this->m_pData = NULL;
	}
}

// = のオーバーロード
template<class T>
inline MyVector<T>& MyVector<T>::operator=(const MyVector<T>& source)
{
	this->clear();
	resize(source.this->m_nSize);
	memcpy(this->m_pData, source.m_pData, (size_t)this->m_nSize * sizeof(T));
	return *this;
}

// コンテナが空かどうかを判定する
template<class T>
inline bool MyVector<T>::empty(void) const
{
	return this->m_nSize == 0;
}
// サイズの取得
template<class T>
inline int& MyVector<T>::size(void)
{
	return this->m_nSize;
}
// 内容量の取得
template<class T>
inline int& MyVector<T>::capacity(void)
{
	return this->m_nCapacity;
}

// []のオーバーロード
template<class T>
inline T & MyVector<T>::operator[](int nArray)
{
	return this->m_pData[nArray];
}
// []のオーバーロード
template<class T>
inline const T & MyVector<T>::operator[](int nArray) const
{
	return this->m_pData[nArray];
}
// クリア
template<class T>
inline void MyVector<T>::clear(void)
{
	if (this->m_pData != NULL)
	{
		this->m_nSize = 0;
		this->m_nCapacity = 0;
		delete[]this->m_pData;
		this->m_pData = NULL;
	}
}

// 先頭要素を指すイテレータを取得する。
template<class T>
inline T * MyVector<T>::begin(void)
{
	return this->m_pData;
}
// 先頭要素を指すイテレータを取得する。
template<class T>
inline const T * MyVector<T>::begin(void) const
{
	return this->m_pData;
}
// 末尾の次を指すイテレータを取得する
template<class T>
inline T * MyVector<T>::end(void)
{
	return this->m_pData + this->m_nSize;
}
// 末尾の次を指すイテレータを取得する
template<class T>
inline const T * MyVector<T>::end(void) const
{
	return this->m_pData + this->m_nSize;
}
// 先頭要素への参照を取得する。
template<class T>
inline T & MyVector<T>::front(void)
{
	return this->m_pData[0];
}
// 先頭要素への参照を取得する。
template<class T>
inline const T & MyVector<T>::front(void) const
{
	return this->m_pData[0];
}
// 末尾要素への参照を取得する
template<class T>
inline T & MyVector<T>::back(void)
{
	return this->m_pData[this->m_nSize - 1];
}
// 末尾要素への参照を取得する
template<class T>
inline const T & MyVector<T>::back(void) const
{
	return this->m_pData[this->m_nSize - 1];
}

// 他のvectorオブジェクトとデータを入れ替える。
template<class T>
inline void MyVector<T>::swap(MyVector<T>& rhs)
{
	// 変数宣言
	int       nRhs_size;		// 右辺のサイズ
	int       nRhs_cap;			// 右辺の内容量
	T*        pRhs_data;		// 右辺のデータ

	// サイズの交換
	Mybfunc_swap(this->m_nSize, rhs.m_nSize, nRhs_size);
	// 内容量の交換
	Mybfunc_swap(this->m_nCapacity, rhs.m_nCapacity, nRhs_cap);
	// データの交換
	Mybfunc_swap(this->m_pData, rhs.m_pData, pRhs_data);
}

// 要素数を変更する
template<class T>
inline void MyVector<T>::resize(int nNewSize)
{
	// 新しいサイズが大きいとき
	if (nNewSize > this->m_nCapacity)
	{
		// 余裕を作る
		reserve(AddCapacity(nNewSize));
		// サイズを設定
		this->m_nSize = nNewSize;
	}
}
// 要素数を変更する
template<class T>
inline void MyVector<T>::resize(int nNewSize, const T & tValue)
{
	// 内容量より新しいサイズが大きいとき
	if (nNewSize > this->m_nCapacity)
	{	// 余裕を作る
		reserve(AddCapacity(nNewSize));
	}
	// サイズより新しいサイズが大きいとき
	if (nNewSize > this->m_nSize)
	{
		// 新しい分をコピーする
		for (int nCntData = this->m_nSize; nCntData < nNewSize; nCntData++)
		{
			memcpy(&Data[nCntData], &tValue, sizeof(tValue));
		}
	}
	// サイズを設定
	this->m_nSize = nNewSize;
}
// 内容量を変更する
template<class T>
inline void MyVector<T>::reserve(int nNewCapacity)
{
	// 新しい容量が今の容量以下の時処理しない
	if (nNewCapacity <= this->m_nCapacity)
	{
		return;
	}
	// 変数宣言
	T* pNewData;	// 新しいデータ

	// 領域の確保
	pNewData = new T[nNewCapacity];
	// データがあった時
	if (this->m_pData != NULL)
	{// メモリをコピー
		memcpy(pNewData, this->m_pData, this->m_nSize * sizeof(T));
		// データを開放
		delete[]this->m_pData;
	}
	// データに新しいデータを代入
	this->m_pData = pNewData;
	// 内容量を設定
	this->m_nCapacity = nNewCapacity;
}

// 新たな要素を末尾に追加する
template<class T>
inline void MyVector<T>::push_back(const T & tValue)
{
	// サイズと内容量が同じとき
	if (this->m_nSize == this->m_nCapacity)
	{
		// 余裕を作る
		reserve(AddCapacity(this->m_nSize + 1));
	}
	// 値をコピーする
	memcpy(&this->m_pData[this->m_nSize], &tValue, sizeof(tValue));
	// サイズをインクリメント
	this->m_nSize++;
}
// 新たな要素を末尾に追加する
template<class T>
inline void MyVector<T>::push_back(void)
{
	// サイズと内容量が同じとき
	if (this->m_nSize == this->m_nCapacity)
	{
		// 余裕を作る
		reserve(AddCapacity(this->m_nSize + 1));
	}
	// サイズをインクリメント
	this->m_nSize++;
}

// 指定した要素を削除する。
template<class T>
inline T * MyVector<T>::erase(const T * pIt)
{
	// 変数宣言
	const int nOff; //  削除する先頭
	nOff = pIt - this->m_pData;
	// メモリを移動する
	memmove(
		this->m_pData + nOff,
		this->m_pData + nOff + 1,
		((size_t)this->m_nSize - (size_t)nOff - 1) * sizeof(T)
	);
	// サイズをデクリメント
	this->m_nSize--;
	// 削除した次のイテレーターを返す
	return this->m_pData + nOff;
}
// 指定した範囲の要素を削除
template<class T>
inline T * MyVector<T>::erase(const T * pIt, const T * pIt_last)
{
	// 変数宣言
	const int nCount = pIt_last - pIt;	// 削除するカウント
	const int nOff = pIt - Data;		// 削除する先頭
	// メモリを移動する
	memmove(
		this->m_pData + nOff,
		this->m_pData + nOff + nCount,
		((size_t)this->m_nSize - (size_t)nOff - nCount) * sizeof(T));
	// サイズを減らす
	this->m_nSize -= (int)nCount;
	// 削除した次のポインタを渡す
	return this->m_pData + nOff;
}

// 任意の位置に新たな要素を挿入する
template<class T>
inline T * MyVector<T>::insert(const T * pIt, const T & tValue)
{
	// 変数宣言
	const int nOff = pIt - this->m_pData;	// 変更する先頭
	// サイズと内容量が同じとき
	if (this->m_nSize == this->m_nCapacity)
	{// 内容量を変更する
		reserve(AddCapacity(this->m_nSize + 1));
	}
	// 削除する先頭よりサイズの方が大きいとき
	if (nOff < (int)this->m_nSize)
	{// メモリを移動する
		memmove(
			this->m_pData + nOff + 1,
			this->m_pData + nOff,
			((size_t)this->m_nSize - (size_t)nOff) * sizeof(T)
		);
	}
	// 変更するメモリにコピー
	memcpy(&this->m_pData[nOff], &tValue, sizeof(tValue));
	// サイズをインクリメント
	this->m_nSize++;
	// 挿入したポインタを返す
	return this->m_pData + nOff;
}

// 任意の値が要素内に含まれているか
template<class T>
inline bool MyVector<T>::contains(const T & tValue) const
{
	// 変数宣言
	const T* pData = this->m_pData;						// データポインタ
	const T* pData_end = this->m_pData + this->m_nSize;	// データの終わり
	while (pData < pData_end)
	{
		if (*pData++ == tValue)
		{
			return true;
		}
	}
	return false;
}
// 指定された値を検索する
template<class T>
inline T * MyVector<T>::find(const T & tValue)
{
	// 変数宣言
	T* pData = this->m_pData;							// データポインタ
	const T* pData_end = this->m_pData + this->m_nSize;	// データの終わり
	while (pData < pData_end)
	{
		if (*pData == tValue)
		{
			break;
		}
		else
		{
			++pData;
		}
	}
	return pData;
}
// 指定された値を検索する
template<class T>
inline const T * MyVector<T>::find(const T & tValue) const
{
	// 変数宣言
	const T* pData = this->m_pData;						// データポインタ
	const T* pData_end = this->m_pData + this->m_nSize;	// データの終わり
	while (pData < pData_end)
	{
		if (*data == tValue)
		{
			break;
		}
		else
		{
			++pData;
		}
	}
	return pData;
}
// 指定された値を検索し削除する
template<class T>
inline bool MyVector<T>::find_erase(const T & tValue)
{
	// 変数宣言
	const T* pIt = find(tValue);
	if (pIt < this->m_pData + this->m_nSize)
	{
		erase(pIt);
		return true;
	}
	return false;
}
// イテレーターからインデックスにする
template<class T>
inline int MyVector<T>::index_from_ptr(const T * pIt) const
{
	// 変数宣言
	const int nOff = pIt - this->m_pData;
	return nOff;
}

// 末尾要素を削除する
template<class T>
inline void MyVector<T>::pop_back(void)
{
	// 大きさをデクリメント
	this->m_nSize--;
}

/* * 内容量を増やす */
template<class T>
inline int MyVector<T>::AddCapacity(int nSize) const
{
	// 変数宣言
	int nNewCapacity;	// 新しい容量
	// 新しい容量を設定
	nNewCapacity = Mybfunc_iifEx(
		(this->m_nCapacity != 0),
		(this->m_nCapacity + this->m_nCapacity / 2),
		8);
	// 大きいほうを返す
	return Mybfunc_iifEx(
		(nNewCapacity > this->m_nSize),
		nNewCapacity,
		this->m_nSize);
}

//----------------------------------------------------------------------------------------------------
// class CLoadInitSetingfromFile ファイルから初期設定を読みこむクラス
//----------------------------------------------------------------------------------------------------

// * [contents] ファイルから一行ずつ読みこむ
// * [input] ファイル名 関数のアドレス
// * [return] enum型(結果)
inline CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::ReadLineByLineFromFile(FILE_NAME pFileName, void(*ReadLine)(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData))
{
	// 変数宣言
	CLoadInitSetingfromFile Load;
	LOADRESULT result;	// 結果

	// 読み込みファイルの情報を作成
	if((result = Load.LoadFileIntoString(pFileName)) != LR_SUCCESS)
	{
		return result;
	}

	// 文字列から初期設定を読み込む
	if ((result = Load.SetingfromString(ReadLine)) != LR_SUCCESS)
	{
		return result;
	}

	// ファイルデータの破棄
	Load.DeleteFileData();

	return result;
}

// * [contents] ファイルから一行ずつ読みこむ
// * [input] ファイル名 関数のアドレス
// * [return] enum型(結果)
inline CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::ReadLineByLineFromFile(FILE_NAME pFileName, void(*ReadLine)(CONST_STRING cnpLine))
{
	// 変数宣言
	CLoadInitSetingfromFile Load;
	LOADRESULT result;	// 結果

	// 読み込みファイルの情報を作成
	if ((result = Load.LoadFileIntoString(pFileName)) != LR_SUCCESS)
	{
		return result;
	}

	// 文字列から初期設定を読み込む
	if ((result = Load.SetingfromString(ReadLine)) != LR_SUCCESS)
	{
		return result;
	}

	// ファイルデータの破棄
	Load.DeleteFileData();

	return result;
}

// * [contents] ファイルから一行ずつ読みこむ
// * [input] ファイル名 関数のアドレス
// * [return] enum型(結果)
inline CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::ReadLineByLineFromFile(FILE_NAME pFileName, void(*ReadLine)(CONST_STRING cnpLine, void *pVoidPtr), void * pVoidPtr)
{
	// 変数宣言
	CLoadInitSetingfromFile Load;
	LOADRESULT result;	// 結果

	// 読み込みファイルの情報を作成
	if ((result = Load.LoadFileIntoString(pFileName)) != LR_SUCCESS)
	{
		return result;
	}

	// 文字列から初期設定を読み込む
	if ((result = Load.SetingfromString(ReadLine, pVoidPtr)) != LR_SUCCESS)
	{
		return result;
	}

	// ファイルデータの破棄
	Load.DeleteFileData();

	return result;
}

// * [contents] ファイル情報の作成
// * [output] クラスのポインタ
// * [input] ファイル名
// * [return] enum型(結果)
inline  CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::MakeLoadFile(CLoadInitSetingfromFile * pOutput, FILE_NAME pFileName)
{
	// 生成
	pOutput = new CLoadInitSetingfromFile;
	// 生成に失敗した時
	if (pOutput == NULL)
	{
		return LR_FAILURE;
	}

	// 文字列へファイルをロードする
	LOADRESULT enmResult = pOutput->LoadFileIntoString(pFileName, "rb");

	if (pOutput->m_pFileData == NULL)
	{
		return enmResult;
	}

	return LR_SUCCESS;
}

// * [contents] ファイル情報の作成
// * [input] ファイル名
// * [return] 作成したクラス
inline CLoadInitSetingfromFile *
CLoadInitSetingfromFile::MakeLoadFile(FILE_NAME pFileName)
{
	// 変数宣言
	CLoadInitSetingfromFile*pOut = new CLoadInitSetingfromFile;
	// 生成に失敗した時
	if (pOut == NULL)
	{
		return NULL;
	}

	// 文字列へファイルをロードする
	pOut->LoadFileIntoString(pFileName, "rb");

	if (pOut->m_pFileData == NULL)
	{
		return NULL;
	}

	return pOut;
}

// * [contents] ファイル情報の開放
// * [output] 開放するクラスのポインタ
inline void 
CLoadInitSetingfromFile::Release(CLoadInitSetingfromFile * pClass)
{
	// クラスポインタがNULLの時
	if (pClass == NULL)
	{
		return;
	}

	// ファイルデータの破棄
	pClass->DeleteFileData();

	// クラスポインタを破棄
	delete pClass;
	pClass = NULL;
}

// * [contents] ファイルデータの破棄
inline void
CLoadInitSetingfromFile::DeleteFileData(void)
{
	// ファイルデータがNULLじゃない時
	if (this->m_pFileData != NULL)
	{// ファイデータを破棄
		delete[] this->m_pFileData;
		this->m_pFileData = NULL;
	}
}

// * [contents] ファイルサイズの取得
// * [input] ファイルのポインタ
// * [return] サイズ
inline UVLONG
CLoadInitSetingfromFile::GetFileSize(FILE * pFile)
{
// 変数宣言
	long lTop;	// 先頭位置
	long lSize;	// ファイルサイズ

	// 初期化
	lTop = MYLIB_INT_UNSET;
	lSize = MYLIB_INT_UNSET;

	// 先頭のファイル位置を取得
	lTop = ftell(pFile);

	// (ファイル位置を取得成功 && ファイル位置を尾末に設定に成功)が失敗した時
	if ((lTop != MYLIB_FAILURE &&
		fseek(pFile, 0, SEEK_END) == MYLIB_SUCCESS) != MYLIB_TRUE)
	{
		return MYLIB_FAILURE;
	}

	// 尾末のファイル位置（サイズ）を取得
	lSize = ftell(pFile);

	// (サイズの取得成功 && ファイル位置を先頭に設定に成功)が失敗した時
	if ((lSize != MYLIB_FAILURE &&
		fseek(pFile, lTop, SEEK_SET) == MYLIB_SUCCESS) != MYLIB_TRUE)
	{
		return MYLIB_FAILURE;
	}

	return (UVLONG)lSize;
}

// * [contents]　文字列へファイルをロードする
// * [input]　ファイル名,開くモード
// * [return]　結果
inline CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::LoadFileIntoString(FILE_NAME pFileName, OPEN_MODE Mode)
{
	// 変数宣言
	FILE *pFile;		// ファイルポインタ
	// ファイルを開く
	if ((pFile = fopen(pFileName, Mode)) == NULL)
	{// 失敗した時
		return M_OPEN_FAIL;
	}

	// 変数宣言
	UVLONG uvlSize = GetFileSize(pFile);	// ファイルサイズ
	// ファイルサイズの取得失敗した時
	if (uvlSize == (UVLONG)MYLIB_FAILURE)
	{
		// ファイルを閉じる
		fclose(pFile);

		return M_SIZE_FAIL;
	}

	// 変数宣言
	FILE_DATA pFileData = new char[(size_t)uvlSize];
	// ファイルデータの確保失敗した時
	if (pFileData == NULL)
	{
		// ファイルを閉じる
		fclose(pFile);

		return M_FILEDATA_FAIL;
	}

	// ファイル読み込み失敗した時
	if ((UVLONG)fread(pFileData, 1, (size_t)uvlSize, pFile) != uvlSize)
	{
		// ファイルを閉じる
		fclose(pFile);
		// ファイルデータの開放
		delete[]pFileData;

		return M_READ_FAIL; 
	}

	// ファイルを閉じる
	fclose(pFile);

	// ファイルサイズの設定
	this->m_nuFileSize = (FILE_SIZE)uvlSize;
	// ファイルデータの設定
	this->m_pFileData = pFileData;

	return LR_SUCCESS;
}

// * [contents]　文字列から設定を読み込む
inline CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::SetingfromString(void(*ReadLine)(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData))
{
	// バッファの領域確保
	STRING pBuffe = new char[this->m_nuFileSize + 1];
	// 確保失敗した時
	if (pBuffe == NULL)
	{
		return B_FAIL;
	}
	// バッファ領域の最後に設定
	STRING pBuffeEnd = pBuffe + this->m_nuFileSize;
	// 領域をコピーする
	memcpy(pBuffe, this->m_pFileData, this->m_nuFileSize);
	// バッファの最後を0に設定
	pBuffe[this->m_nuFileSize] = MYLIB_CHAR_UNSET;

	STRING pEntryData = NULL;	// 登録用データ
	STRING pEntryType = NULL;	// 登録用のタイプ
	STRING pLineEnd = NULL;		// 1行の尾末
	STRING pLine = pBuffe;		// 1行分の文字列

	// バッファの終了地点までループする
	for (pLine = pBuffe; pLine < pBuffeEnd; pLine = pLineEnd + 1)
	{
		// 1行分の文字列を取得
		// 行末の時スキップ
		while (*pLine == '\n' || *pLine == '\r')
		{
			pLine++;
		}
		// 1行の尾末の初期化
		pLineEnd =pLine;
		// 行末までスキップ
		while (pLineEnd < pBuffeEnd && *pLineEnd != '\n' && *pLineEnd != '\r')
		{
			pLineEnd++;
		}
		// 行末から初期化
		pLineEnd[0] = MYLIB_CHAR_UNSET;

		// 1行の最初が [ 最後が ] の時
		if (pLine[0] == '[' &&pLineEnd > pLine &&pLineEnd[-1] == ']')
		{
			// 1行の最後を初期化
			pLineEnd[-1] = MYLIB_CHAR_UNSET;

			/* --- 1行から[][]を抜き取る --- */
			// 変数宣言
			CONST_STRING pDataEnd;		// 登録用のデータの最後
			CONST_STRING pTypeStart;	// 登録用のタイプの最初
			STRING       pTypeEnd;		// 登録用のデータの最後
			CONST_STRING pDataStart;	// 登録用のタイプの最初

			// 初期化
			pDataEnd = pLineEnd - 1;	// 行の最後を設定
			pTypeStart = pLine + 1;		// 最初の [ をスキップ
			pTypeEnd = (STRING)(void*)GetRangeStrfromChar(pTypeStart, pDataEnd, ']');			// 最初の ] までスキップ
			pDataStart = pTypeEnd ? GetRangeStrfromChar(pTypeEnd + 1, pDataEnd, '[') : NULL;	// 2つ目の [ までスキップ

			// 登録用のタイプの最後と登録用のデータの最初のどちらかが存在していない時最初から
			if (pTypeEnd == MYLIB_CHAR_UNSET ||
				pDataStart == MYLIB_CHAR_UNSET)
			{
				continue;
			}
			// 一つ目の ] を初期化
			*pTypeEnd = MYLIB_CHAR_UNSET;
			// 二つ目の [ を飛ばす
			pDataStart++;

			/* --- 抜き取ったらそれぞれに代入 --- */
			// 登録用データの設定
			pEntryType = (STRING)(void*)pTypeStart;
			// 登録用のタイプの設定
			pEntryData = (STRING)(void*)pDataStart;
		}

		// 登録データの取得条件式
		if ((pEntryData != NULL && pEntryType != NULL))
		{
			ReadLine(pLine, pEntryType, pEntryData);
		}
	}

	if (pBuffe != NULL)
	{
		delete[] pBuffe;
		pBuffe = NULL;
	}

	return LR_SUCCESS;
}

// * [contents]　文字列から設定を読み込む
inline CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::SetingfromString(void(*ReadLine)(CONST_STRING cnpLine))
{
	// バッファの領域確保
	STRING pBuffe = new char[this->m_nuFileSize + 1];
	// 確保失敗した時
	if (pBuffe == NULL)
	{
		return B_FAIL;
	}
	// バッファ領域の最後に設定
	STRING pBuffeEnd = pBuffe + this->m_nuFileSize;
	// 領域をコピーする
	memcpy(pBuffe, this->m_pFileData, this->m_nuFileSize);
	// バッファの最後を0に設定
	pBuffe[this->m_nuFileSize] = MYLIB_CHAR_UNSET;

	STRING pLineEnd = NULL;		// 1行の尾末
	STRING pLine = pBuffe;		// 1行分の文字列

	// バッファの終了地点までループする
	for (pLine = pBuffe; pLine < pBuffeEnd; pLine = pLineEnd + 1)
	{
		// 1行分の文字列を取得
		// 行末の時スキップ
		while (*pLine == '\n' || *pLine == '\r')
		{
			pLine++;
		}
		// 1行の尾末の初期化
		pLineEnd = pLine;
		// 行末までスキップ
		while (pLineEnd < pBuffeEnd && *pLineEnd != '\n' && *pLineEnd != '\r')
		{
			pLineEnd++;
		}
		// 行末から初期化
		pLineEnd[0] = MYLIB_CHAR_UNSET;

		// 1行読み込む
		ReadLine(pLine);
	}

	if (pBuffe != NULL)
	{
		delete[] pBuffe;
		pBuffe = NULL;
	}

	return LR_SUCCESS;
}

// * [contents]　文字列から設定を読み込む
inline CLoadInitSetingfromFile::LOADRESULT
CLoadInitSetingfromFile::SetingfromString(void(*ReadLine)(CONST_STRING cnpLine, void * pVoidPtr), void * pVoidPtr)
{
	// バッファの領域確保
	STRING pBuffe = new char[this->m_nuFileSize + 1];
	// 確保失敗した時
	if (pBuffe == NULL)
	{
		return B_FAIL;
	}
	// バッファ領域の最後に設定
	STRING pBuffeEnd = pBuffe + this->m_nuFileSize;
	// 領域をコピーする
	memcpy(pBuffe, this->m_pFileData, this->m_nuFileSize);
	// バッファの最後を0に設定
	pBuffe[this->m_nuFileSize] = MYLIB_CHAR_UNSET;

	STRING pLineEnd = NULL;		// 1行の尾末
	STRING pLine = pBuffe;		// 1行分の文字列

	// バッファの終了地点までループする
	for (pLine = pBuffe; pLine < pBuffeEnd; pLine = pLineEnd + 1)
	{
		// 1行分の文字列を取得
		// 行末の時スキップ
		while (*pLine == '\n' || *pLine == '\r')
		{
			pLine++;
		}
		// 1行の尾末の初期化
		pLineEnd = pLine;
		// 行末までスキップ
		while (pLineEnd < pBuffeEnd && *pLineEnd != '\n' && *pLineEnd != '\r')
		{
			pLineEnd++;
		}
		// 行末から初期化
		pLineEnd[0] = MYLIB_CHAR_UNSET;

		// 1行読み込む
		ReadLine(pLine, pVoidPtr);
	}

	if (pBuffe != NULL)
	{
		delete[] pBuffe;
		pBuffe = NULL;
	}

	return LR_SUCCESS;
}

// * [contents] 文字列から任意の文字以降の範囲を取得
// * [input] 文字列、文字列の終わり、任意の文字
// * [return] 取得した文字列
inline CONST_STRING
CLoadInitSetingfromFile::GetRangeStrfromChar(CONST_STRING cnStr, CONST_STRING cnStrEnd, char marker)
{
	// 変数宣言
	CONST_STRING cnRetStr;	// 返す文字列ポインタ
	cnRetStr = (CONST_STRING)memchr(cnStr, (int)marker, cnStrEnd - cnStr);
	return cnRetStr;
}

//----------------------------------------------------------------------------------------------------
// class CInputkey 入力キークラス
//----------------------------------------------------------------------------------------------------

/* * 初期化 */
inline void
CInputkey::Init(void)
{
	this->m_nSize = 0;		// サイズ
	this->m_pKeyTop = NULL;		// キーの先頭ポインタ
}

/* * キーデータの取得 */
inline void
CInputkey::KeyInit(void)
{
	this->m_pKeyTop->nData = 0;		// データ
	this->m_pKeyTop->nKey = 0;		// キー
	this->m_pKeyTop->pNext = NULL;	// キーの先頭ポインタ
}

/* * キーデータの生成 */
inline void
CInputkey::KeyCreate(void)
{
	// 生成
	this->m_pKeyTop = new INPUTKEYCELL;
	// 生成失敗
	if (this->m_pKeyTop == NULL)
	{
		return;
	}
	// キーの初期化
	this->KeyInit();
}

/* * キーデータリストの開放 */
inline void
CInputkey::KeyListRelease(void)
{
	// 変数宣言
	INPUTKEYCELL* pTemp;	// 一時保存用
	INPUTKEYCELL* pSwap;	// 交換用

							// 先頭ポインタ
	pTemp = this->m_pKeyTop;
	// 存在している時
	if (pTemp != NULL)
	{
		// リストの開放
		while (pTemp != NULL)
		{
			// 交換用に設定
			pSwap = pTemp->pNext;
			// 保存用を破棄
			delete pTemp;
			// 保存用に設定
			pTemp = pSwap;
		}
	}
	this->m_pKeyTop = NULL;
}

/* * キーデータの挿入 */
inline void
CInputkey::KeyInsert(int nData, int nKey)
{
	// 変数宣言
	INPUTKEYCELL * pInsert;			// 挿入するポインタ
									// 生成
	pInsert = new INPUTKEYCELL;
	// 生成に失敗
	if (pInsert == NULL)
	{
		return;
	}
	// データの設定
	pInsert->nData = nData;
	// キーの設定
	pInsert->nKey = nKey;
	// 挿入する次のポインタの設定
	pInsert->pNext = this->m_pKeyTop;
	// 先頭ポインタの設定
	this->m_pKeyTop = pInsert;
	// サイズをインクリメント
	m_nSize++;
}

/* * キーデータを抜き取る */
inline void
CInputkey::KeyPullout(int nKey)
{
	// 変数宣言
	INPUTKEYCELL *pTarget = NULL;	// ターゲット用ポインタ
	INPUTKEYCELL *pChain = NULL;	// つなぎのポインタ

									// ターゲットの設定
	pTarget = this->m_pKeyTop;
	// ターゲットがいない時
	if (pTarget == NULL)
	{
		return;
	}
	// つなぎのポインタの設定
	pChain = pTarget->pNext;

	// 先頭を抜き取る場合
	if (pTarget->nKey == nKey)
	{
		// サイズをデクリメント
		m_nSize--;
		// つなぎのポインタを先頭にする
		this->m_pKeyTop = pChain;
		// ターゲットを破棄
		delete pTarget;
		pTarget = NULL;
		return;
	}

	// 先頭以外を抜き取る場合
	while (pTarget != NULL)
	{
		// ターゲットのキーと任意のキーが同じとき
		if (pTarget->nKey == nKey)
		{
			// サイズをデクリメント
			m_nSize--;
			// つなぎの次にターゲットの次のポインタを設定
			pChain->pNext = pTarget->pNext;
			// ターゲットの破棄
			delete pTarget;
			pTarget = NULL;
			return;
		}
		// つなぎにターゲットポインタを設定
		pChain = pTarget;
		// ターゲットにターゲットの次を設定
		pTarget = pTarget->pNext;
	}
}

/* * キーデータの取得 */
inline int
CInputkey::GetKeyData(int nKey)
{
	// 変数宣言
	INPUTKEYCELL *pTarget = NULL;	// ターゲットのポインタ
	INPUTKEYCELL *pChain = NULL;	// つなぎのポインタ

									// ターゲットの設定
	pTarget = this->m_pKeyTop;
	// ターゲットがNULLじゃない時ループ
	while (pTarget != NULL)
	{
		// ターゲットのキーと任意のキーが同じとき
		if (pTarget->nKey == nKey)
		{// データを返す
			return pTarget->nData;
		}
		// ターゲットをつなぎに設定
		pChain = pTarget;
		// ターゲットにターゲットに次を設定
		pTarget = pTarget->pNext;
	}
	return -1;
}

/* * サイズの取得 */
inline int
CInputkey::GetSize(void)
{
	return m_nSize;
}

//----------------------------------------------------------------------------------------------------
// class CString 文字列クラス
//----------------------------------------------------------------------------------------------------

/* * コンストラクタ */
inline
CString::CString()
{
	this->Init();
}

/* * デストラクタ */
inline
CString::~CString()
{
	this->release();
}

/* * 初期化 */
inline void
CString::Init(void)
{
	this->m_string = NULL;						// 文字列ポインタ
	this->m_nStringLength = MYLIB_INT_UNSET;	// 文字列の長さ
}

/* * 開放処理 */
inline void
CString::release(void)
{
	// NULL開放されていない時
	if (this->m_string != NULL)
	{
		// 文字列の破棄
		delete[]this->m_string;
		// 初期化
		this->Init();
	}
}

/* * 文字列の設定（設定していた場合再設定する） */
inline void
CString::Set(STRING string)
{
	// 開放処理
	this->release();
	// 生成
	if (this->m_string == NULL)
	{
		this->m_nStringLength = strlen(string) + 1;
		this->m_string = new char[this->m_nStringLength];
		strcpy(this->m_string, string);
	}
}
inline void
CString::Set(CONST_STRING string)
{
	// 開放処理
	this->release();
	// 生成
	if (this->m_string == NULL)
	{
		this->m_nStringLength = strlen(string) + 1;
		this->m_string = new char[this->m_nStringLength];
		strcpy(this->m_string, string);
	}
}

/* * 文字列の取得 */
inline STRING
CString::Get(void)
{
	return this->m_string;
}

/* * サイズの取得 */
inline int & CString::size(void)
{
	return this->m_nStringLength;
}

#endif