#include "math/vec.h"
#include <cstdlib>
#include <cassert>
#include "vec.h"

radiance::math::Vec2::Vec2(float x, float y) :
	mX{ x }, mY{ y }
{

}

float radiance::math::Vec2::x() const
{
	return mX;
}

float radiance::math::Vec2::y() const
{
	return mY;
}

float radiance::math::Vec2::r() const
{
	return mX;
}

float radiance::math::Vec2::g() const
{
	return mY;
}


float radiance::math::Vec2::operator[](size_t index) const
{
	assert(index <= 2);
	return data[index];
}

float& radiance::math::Vec2::operator[](size_t index)
{
	assert(index <= 2);
	return data[index];
}

/*Vector space operations*/
radiance::math::Vec2 radiance::math::Vec2::operator+(const Vec2& rhs) const {
	return Vec2{ mX + rhs.x(),mY + rhs.y()};
}

radiance::math::Vec2 radiance::math::Vec2::operator+=(const Vec2& rhs)
{
	mX += rhs.x();
	mY += rhs.y();
	return *this;
}

radiance::math::Vec2 radiance::math::Vec2::operator-(const Vec2& rhs) const {
	return Vec2{ mX - rhs.x(),mY - rhs.y() };
}

radiance::math::Vec2 radiance::math::Vec2::operator-=(const Vec2& rhs)
{
	mX -= rhs.x();
	mY -= rhs.y();
	return *this;
}

radiance::math::Vec2 radiance::math::Vec2::operator*(const math::Vec2& rhs) const
{
	return Vec2{mX*rhs.x(),mY*rhs.y()};
}

radiance::math::Vec2 radiance::math::Vec2::operator*=(const math::Vec2& rhs)
{
	mX *= rhs.x();
	mY *= rhs.y();
	return *this;
}

radiance::math::Vec2 radiance::math::Vec2::operator*(float rhs) const {
	return Vec2{ mX * rhs,mY * rhs };

}

radiance::math::Vec2 radiance::math::Vec2::operator*=(float rhs)
{
	mX *= rhs;
	mY *= rhs;
	return *this;
}
radiance::math::Vec2 radiance::math::Vec2::operator/(float rhs) const {
	return Vec2{ mX / rhs,mY / rhs };
}

radiance::math::Vec2 radiance::math::Vec2::operator/=(float rhs)
{
	mX /= rhs;
	mY /= rhs;
	return *this;
}
bool radiance::math::Vec2::operator==(const Vec2& rhs) const
{
	return (mX == rhs.x()) && (mY == rhs.y());
}

radiance::math::Vec2 radiance::math::Vec2::operator-() const {
	return Vec2{ -mX,-mY };
}

float* radiance::math::Vec2::getRawData()
{
	return (float*)data;
}

/*Inner product and cross product*/
float radiance::math::Vec2::dot(const Vec2& rhs) const {
	return mX * rhs.x() + mY * rhs.y();
}

float radiance::math::Vec2::norm2() const {
	return this->dot(*this);
}

float radiance::math::Vec2::norm() const {
	return sqrt(norm2());
}

radiance::math::Vec2 radiance::math::Vec2::normalize() const {
	return *this / norm();
}

radiance::math::Vec2 radiance::math::Vec2::min(const Vec2 &rhs) const
{
    return Vec2(fmin(mX,rhs.x()),fmin(mY,rhs.y()));
}

radiance::math::Vec2 radiance::math::Vec2::max(const Vec2 &rhs) const
{
    return Vec2(fmax(mX,rhs.x()),fmax(mY,rhs.y()));
}

radiance::math::Vec2 radiance::math::Vec2::ZERO()
{
    return Vec2{0,0};
}

std::ostream& radiance::math::operator<<(std::ostream& out, const Vec2& v)
{
	return out << "[" << v.x() << "," << v.y() << "]\n";
}

radiance::math::Vec3::Vec3(float x, float y, float z):
	mX{x},mY{y},mZ{z}
{

}

float radiance::math::Vec3::x() const
{
	return mX;
}

float radiance::math::Vec3::y() const
{
	return mY;
}

float radiance::math::Vec3::z() const
{
	return mZ;
}

float radiance::math::Vec3::r() const
{
	return mX;
}

float radiance::math::Vec3::g() const
{
	return mY;
}

float radiance::math::Vec3::b() const
{
	return mZ;
}

float radiance::math::Vec3::operator[](size_t index) const
{
	assert(index <= 3);
	return data[index];
}

float& radiance::math::Vec3::operator[](size_t index)
{
	assert(index <= 3);
	return data[index];
}

radiance::math::Vec2 radiance::math::Vec3::xy() const
{
    return math::Vec2{mX,mY};
}

radiance::math::Vec2 radiance::math::Vec3::xz() const
{
    return math::Vec2{mX,mZ};
}

radiance::math::Vec2 radiance::math::Vec3::yz() const
{
	return math::Vec2{mY,mZ};
}

/*Vector space operations*/
radiance::math::Vec3 radiance::math::Vec3::operator+(const Vec3& rhs) const {
	return Vec3{ mX + rhs.x(),mY + rhs.y() ,mZ + rhs.z() };
}

radiance::math::Vec3 radiance::math::Vec3::operator+=(const Vec3& rhs)
{
	mX += rhs.x();
	mY += rhs.y();
	mZ += rhs.z();
	return *this;
}

radiance::math::Vec3 radiance::math::Vec3::operator-(const Vec3& rhs) const {
	return Vec3{ mX - rhs.x(),mY - rhs.y() ,mZ - rhs.z() };
}

radiance::math::Vec3 radiance::math::Vec3::operator-=(const Vec3& rhs)
{
	mX -= rhs.x();
	mY -= rhs.y();
	mZ -= rhs.z();
	return *this;
}

radiance::math::Vec3 radiance::math::Vec3::operator*(const math::Vec3& rhs) const
{
	return Vec3{mX*rhs.x(),mY*rhs.y(),mZ*rhs.z()};
}

radiance::math::Vec3 radiance::math::Vec3::operator*=(const math::Vec3& rhs)
{
	mX *= rhs.x();
	mY *= rhs.y();
	mZ *= rhs.z();
	return *this;
}

radiance::math::Vec3 radiance::math::Vec3::operator*(float rhs) const {
	return Vec3{ mX*rhs,mY*rhs ,mZ*rhs};

}

radiance::math::Vec3 radiance::math::Vec3::operator*=(float rhs)
{
	mX *= rhs;
	mY *= rhs;
	mZ *= rhs;
	return *this;
}

radiance::math::Vec3 radiance::math::Vec3::operator/(float rhs) const {
	return Vec3{ mX / rhs,mY/ rhs ,mZ/ rhs };
}

radiance::math::Vec3 radiance::math::Vec3::operator/=(float rhs)
{
	mX /= rhs;
	mY /= rhs;
	mZ /= rhs;
	return *this;
}

bool radiance::math::Vec3::operator==(const Vec3& rhs) const
{
	return (mX == rhs.x()) && (mY == rhs.y()) && (mZ == rhs.z());
}

radiance::math::Vec3 radiance::math::Vec3::operator-() const {
	return Vec3{ -mX,-mY,-mZ };
}

float* radiance::math::Vec3::getRawData()
{
	return (float*)data;
}

/*Inner product and cross product*/
float radiance::math::Vec3::dot(const Vec3& rhs) const {
	return mX * rhs.x() + mY * rhs.y() + mZ * rhs.z();
}

radiance::math::Vec3 radiance::math::Vec3::cross(const Vec3& rhs) const {
	return Vec3{
		mY*rhs.z() - mZ*rhs.y(),
		mZ*rhs.x() - mX*rhs.z(),
		mX*rhs.y() - mY*rhs.x()
	};
}

float radiance::math::Vec3::norm2() const {
	return this->dot(*this);
}

float radiance::math::Vec3::norm() const {
	return sqrt(norm2());
}

radiance::math::Vec3 radiance::math::Vec3::normalize() const {
	return *this / norm();
}

radiance::math::Vec3 radiance::math::Vec3::min(const Vec3 &rhs) const
{
	return Vec3(fmin(mX,rhs.x()),fmin(mY,rhs.y()),fmin(mZ,rhs.z()));
}

radiance::math::Vec3 radiance::math::Vec3::max(const Vec3 &rhs) const
{
    return Vec3(fmax(mX,rhs.x()),fmax(mY,rhs.y()),fmax(mZ,rhs.z()));
}

radiance::math::Vec3 radiance::math::Vec3::clamp(float min, float max) const
{
    return math::Vec3{std::clamp(mX,min,max),std::clamp(mY,min,max),std::clamp(mZ,min,max)};
}

radiance::math::Vec3 radiance::math::Vec3::ZERO()
{
    return Vec3{0,0,0};
}

std::istream &radiance::math::operator>>(std::istream &in, Vec3 &v)
{
	char c;
	//in >> c;
	in >> v.data[0];
	in >> c;
	in >> v.data[1];
	in >> c;
	in >> v.data[2];
	
	return in;
}

std::ostream &radiance::math::operator<<(std::ostream &out, const Vec3 &v)
{
	return out << "[" << v.x() << "," << v.y() << "," << v.z() << "]\n";
}

radiance::math::Vec4::Vec4(float x, float y, float z, float w) : mX{x}, mY{y}, mZ{z}, mW{w}
{

}

float radiance::math::Vec4::x() const
{
	return mX;
}

float radiance::math::Vec4::y() const
{
	return mY;
}

float radiance::math::Vec4::z() const
{
	return mZ;
}

float radiance::math::Vec4::w() const
{
	return mW;
}

float radiance::math::Vec4::r() const
{
	return mX;
}

float radiance::math::Vec4::g() const
{
	return mY;
}

float radiance::math::Vec4::b() const
{
	return mZ;
}

float radiance::math::Vec4::a() const
{
	return mW;
}

float radiance::math::Vec4::operator[](size_t index) const{
	assert(index <= 3);
	return data[index];
}

float& radiance::math::Vec4::operator[](size_t index) {
	assert(index <= 3);
	return data[index];
}

radiance::math::Vec3 radiance::math::Vec4::xyz() const
{
    return math::Vec3{mX,mY,mZ};
}

/*Vector space operations*/
radiance::math::Vec4 radiance::math::Vec4::operator+(const Vec4& rhs) const {
	return Vec4{ mX + rhs.x(),mY + rhs.y() ,mZ + rhs.z(), mW + rhs.w() };
}

void radiance::math::Vec4::operator+=(const Vec4& rhs)
{
	mX += rhs.x();
	mY += rhs.y();
	mZ += rhs.z();
	mW += rhs.w();
}

radiance::math::Vec4 radiance::math::Vec4::operator-(const Vec4& rhs) const {
	return Vec4{ mX - rhs.x(),mY - rhs.y() ,mZ - rhs.z(), mW - rhs.w() };
}

void radiance::math::Vec4::operator-=(const Vec4& rhs)
{
	mX -= rhs.x();
	mY -= rhs.y();
	mZ -= rhs.z();
	mW -= rhs.w();
}

radiance::math::Vec4 radiance::math::Vec4::operator*(const Vec4& rhs) const
{
	return Vec4{mX*rhs.x(),mY*rhs.y(),mZ*rhs.z(),mW*rhs.w()};
}

radiance::math::Vec4 radiance::math::Vec4::operator*=(const Vec4& rhs)
{
	mX *= rhs.x();
	mY *= rhs.y();
	mZ *= rhs.z();
	mW *= rhs.w();
	return *this;
}

radiance::math::Vec4 radiance::math::Vec4::operator*(float rhs) const {
	return Vec4{ mX * rhs,mY * rhs ,mZ * rhs, mW*rhs};

}
void radiance::math::Vec4::operator*=(float rhs)
{
	mX *= rhs;
	mY *= rhs;
	mZ *= rhs;
	mW *= rhs;
}

radiance::math::Vec4 radiance::math::Vec4::operator/(float rhs) const {
	return Vec4{ mX / rhs,mY / rhs ,mZ / rhs,mW / rhs };
}
void radiance::math::Vec4::operator/=(float rhs)
{
	mX /= rhs;
	mY /= rhs;
	mZ /= rhs;
	mW /= rhs;
}

bool radiance::math::Vec4::operator==(const Vec4& rhs) const
{
	return (mX == rhs.x()) && (mY == rhs.y()) && (mZ == rhs.z()) && (mW == rhs.w());
}

radiance::math::Vec4 radiance::math::Vec4::operator-() const {
	return Vec4{ -mX,-mY,-mZ, -mW};
}

float* radiance::math::Vec4::getRawData()
{
	return (float*)data;
}

/*Inner product and cross product*/
float radiance::math::Vec4::dot(const Vec4& rhs) const {
	return mX * rhs.x() + mY * rhs.y() + mZ * rhs.z() + mW*rhs.w();
}

float radiance::math::Vec4::norm2() const {
	return this->dot(*this);
}

float radiance::math::Vec4::norm() const {
	return sqrt(norm2());
}

radiance::math::Vec4 radiance::math::Vec4::normalize() const {
	return *this / norm();
}

radiance::math::Vec4 radiance::math::Vec4::min(const Vec4 &rhs) const
{
	return Vec4(fmin(mX,rhs.x()),fmin(mY,rhs.y()),fmin(mZ,rhs.z()),fmin(mW,rhs.w()));
}

radiance::math::Vec4 radiance::math::Vec4::max(const Vec4 &rhs) const
{
    return Vec4(fmax(mX,rhs.x()),fmax(mY,rhs.y()),fmax(mZ,rhs.z()),fmax(mW,rhs.w()));
}

radiance::math::Vec4 radiance::math::Vec4::ZERO()
{
    return Vec4{0,0,0,0};
}

std::ostream& radiance::math::operator<<(std::ostream& out, const Vec4& v)
{
	return out << "[" << v.x() << "," << v.y() << "," << v.z() << "," << v.w() <<  "]\n";
}
