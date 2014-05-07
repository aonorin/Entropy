#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

namespace geometry {

template <class T>
struct Vec2 {

    T x, y;

    Vec2(T x = 0, T y = 0) : x(x), y(y) { }

    inline const Vec2<T> operator *(T mul) const
    {
        return Vec2<T>(x * mul, y * mul);
    }

    inline const Vec2<T> operator *(const Vec2<T>& mul) const
    {
        return Vec2<T>(x * mul.x, y * mul.y);
    }

    inline const Vec2<T> operator /(T div) const
    {
        return Vec2<T>(x / div, y / div);
    }

    inline const Vec2<T> operator /(const Vec2<T>& div) const
    {
        return Vec2<T>(x / div.x, y / div.y);
    }

    inline const Vec2<T> operator +(T add) const
    {
        return Vec2<T>(x + add, y + add);
    }

    inline const Vec2<T> operator +(const Vec2<T>& add) const
    {
        return Vec2<T>(x + add.x, y + add.y);
    }

    inline const Vec2<T> operator -(T subtract) const
    {
        return Vec2<T>(x - subtract, y - subtract);
    }

    inline const Vec2<T> operator -(const Vec2<T>& subtract) const
    {
        return Vec2<T>(x - subtract.x, y - subtract.y);
    }

    inline Vec2<T> operator *=(T mul)
    {
        x *= mul;
        y *= mul;
        return *this;
    }

    inline Vec2<T> operator *=(const Vec2<T>& mul)
    {
        x *= mul.x;
        y *= mul.y;
        return *this;
    }

    inline Vec2<T> operator /=(T div)
    {
        x /= div;
        y /= div;
        return *this;
    }

    inline Vec2<T> operator /=(const Vec2<T>& div)
    {
        x /= div.x;
        y /= div.y;
        return *this;
    }

    inline Vec2<T> operator +=(T add)
    {
        x += add;
        y += add;
        return *this;
    }

    inline Vec2<T> operator +=(const Vec2<T>& add)
    {
        x += add.x;
        y += add.y;
        return *this;
    }

    inline Vec2<T> operator -=(T subtract)
    {
        x -= subtract;
        y -= subtract;
        return *this;
    }

    inline Vec2<T> operator -=(const Vec2<T>& subtract)
    {
        x -= subtract.x;
        y -= subtract.y;
        return *this;
    }

    inline bool operator ==(const Vec2<T>& other)
    {
        return x == other.x && y == other.y;
    }

    inline T length() const
    {
        return sqrt((x * x) + (y * y));
    }

    inline void limit(T limit)
    {
        T lengthSquared = x * x + y * y;

        if((lengthSquared > 0) && (lengthSquared > limit * limit))
        {
            double ratio = limit / sqrt(lengthSquared);
            x *= ratio;
            y *= ratio;
        }
    }

    inline Vec2<T> limited(const Vec2<T>& limit) const
    {
        T lengthSquared = x * x + y * y;

        if((lengthSquared > 0) && (lengthSquared > limit * limit))
        {
            double ratio = limit / sqrt(lengthSquared);
            return Vec2<T>(x * ratio, y * ratio);
        }

        else
        {
            return *this;
        }
    }

    inline void normalize()
    {
        double ratio = 1 / length();
        x *= ratio;
        y *= ratio;
    }

    inline Vec2<T> normalized() const
    {
        double ratio = 1 / length();
        return Vec2<T>(x * ratio, y * ratio);
    }

    inline T distance(const Vec2<T>& other) const
    {
        return sqrt(((other.x - x) * (other.x - x)) + ((other.y - y) * (other.y - y)));
    }

    void rotate( double radians )
    {
        T cosa = std::cos(radians);
        T sina = std::sin(radians);
        T rx = x * cosa - y * sina;
        y = x * sina + y * cosa;
        x = rx;
    }
};

template <class T>
struct Vec3 {

    T x, y, z;

    Vec3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) { }

    inline const Vec3<T> operator *(T mul) const
    {
        return Vec3<T>(x * mul, y * mul, z * mul);
    }

    inline const Vec3<T> operator *(const Vec3<T>& mul) const
    {
        return Vec3<T>(x * mul.x, y * mul.y, z * mul.z);
    }

    inline const Vec3<T> operator /(T div) const
    {
        return Vec3<T>(x / div, y / div, z / div);
    }

    inline const Vec3<T> operator /(const Vec3<T>& div) const
    {
        return Vec3<T>(x / div.x, y / div.y, z / div.z);
    }

    inline const Vec3<T> operator +(T add) const
    {
        return Vec3<T>(x + add, y + add, z + add);
    }

    inline const Vec3<T> operator +(const Vec3<T>& add) const
    {
        return Vec3(x + add.x, y + add.y, z + add.z);
    }

    inline const Vec3<T> operator -(T subtract) const
    {
        return Vec3(x - subtract, y - subtract, z - subtract);
    }

    inline const Vec3<T> operator -(const Vec3<T>& subtract) const
    {
        return Vec3<T>(x - subtract.x, y - subtract.y, z - subtract.z);
    }

    inline Vec3<T> operator *=(T mul)
    {
        x *= mul;
        y *= mul;
        z *= mul;
        return *this;
    }

    inline Vec3<T> operator *=(const Vec3<T> mul)
    {
        x *= mul.x;
        y *= mul.y;
        z *= mul.z;
        return *this;
    }

    inline Vec3<T> operator /=(T div)
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    inline Vec3<T> operator /=(const Vec3<T> div)
    {
        x /= div.x;
        y /= div.y;
        z /= div.z;
        return *this;
    }

    inline Vec3<T> operator +=(T add)
    {
        x += add;
        y += add;
        z += add;
        return *this;
    }

    inline Vec3<T> operator +=(const Vec3<T> add)
    {
        x += add.x;
        y += add.y;
        z += add.z;
        return *this;
    }

    inline Vec3<T> operator -=(T subtract)
    {
        x -= subtract;
        y -= subtract;
        z -= subtract;
        return *this;
    }

    inline Vec3<T> operator -=(const Vec3<T> subtract)
    {
        x -= subtract.x;
        y -= subtract.y;
        z -= subtract.z;
        return *this;
    }

    inline bool operator ==(Vec3<T> other)
    {
        return x == other.x && y == other.y && z == other.z;
    }

    inline bool operator<(const Vec3<T>& b ) const
    {
        if(x < b.x) {
            return true;
        } else if(x > b.x) {
            return false;
        } else {
            if(y<b.y) {
                return true;
            } else if(y > b.y) {
                return false;
            } else {
                if(z < b.z) {
                    return true;
                } else if(z > b.z) {
                    return false;
                } else {
                    return false;
                }
            }
        }
    }

    inline T length()
    {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    inline void limit(T limit)
    {
        T lengthSquared = x * x + y * y + z * z;

        if((lengthSquared > 0) && (lengthSquared > limit * limit))
        {
            double ratio = limit / sqrt(lengthSquared);
            x *= ratio;
            y *= ratio;
            z *= ratio;
        }
    }

    inline Vec2<T> limited(const Vec2<T>& limit) const
    {
        T lengthSquared = x * x + y * y;

        if((lengthSquared > 0) && (lengthSquared > limit * limit))
        {
            double ratio = limit / std::sqrt(lengthSquared);
            return Vec3<T>(x * ratio, y * ratio, z * ratio);
        }

        else
        {
            return *this;
        }
    }

    inline void normalize()
    {
        double ratio = 1 / length();
        x *= ratio;
        y *= ratio;
        z *= ratio;
    }

    inline Vec3<T> normalized() const
    {
        double ratio = 1 / length();
        return Vec3<T>(x * ratio, y * ratio, z * ratio);
    }

    inline T distance(const Vec3<T>& other) const
    {
        return sqrt(((other.x - x) * (other.x - x)) + ((other.y - y) * (other.y - y)) + ((other.z - z) * (other.z - z)));
    }

    inline void rotateX(T angle)
    {
        T sina = std::sin(angle);
        T cosa = std::cos(angle);
        T ry = y * cosa - z * sina;
        T rz = y * sina + z * cosa;
        y = ry;
        z = rz;
    }

    void rotateY( T angle )
    {
        T sina = std::sin(angle);
        T cosa = std::cos(angle);
        T rx = x * cosa - z * sina;
        T rz = x * sina + z * cosa;
        x = rx;
        z = rz;
    }

    void rotateZ( T angle )
    {
        T sina = std::sin(angle);
        T cosa = std::cos(angle);
        T rx = x * cosa - y * sina;
        T ry = x * sina + y * cosa;
        x = rx;
        y = ry;
    }

    void rotate( Vec3<T> axis, T angle )
    {
        T cosa = std::cos(angle);
        T sina = std::sin(angle);

        T rx = (cosa + (1 - cosa) * axis.x * axis.x) * x;
        rx += ((1 - cosa) * axis.x * axis.y - axis.z * sina) * y;
        rx += ((1 - cosa) * axis.x * axis.z + axis.y * sina) * z;

        T ry = ((1 - cosa) * axis.x * axis.y + axis.z * sina) * x;
        ry += (cosa + (1 - cosa) * axis.y * axis.y) * y;
        ry += ((1 - cosa) * axis.y * axis.z - axis.x * sina) * z;

        T rz = ((1 - cosa) * axis.x * axis.z - axis.y * sina) * x;
        rz += ((1 - cosa) * axis.y * axis.z + axis.x * sina) * y;
        rz += (cosa + (1 - cosa) * axis.z * axis.z) * z;

        x = rx;
        y = ry;
        z = rz;
    }
};


typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;

typedef Vec3<unsigned short> Vec3ui16;

} // geometry namespace

#endif // VECTOR_H
