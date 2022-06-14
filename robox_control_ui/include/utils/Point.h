// ======================================
// Filename:    Point.h
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_POINT_H
#define ROBOX_CONTROL_UI_POINT_H

#include <array>

#define STATIC_ASSERT(e) static_assert(e, "!(" #e ")")

namespace Utils
{
    template<typename T, int nDimensions>
    class Point
    {
    private:
        std::array<T, nDimensions> elements_;

    public:
        typedef T ValueType;

        T& operator[](const int i)
        {
            return elements_[i];
        }
        T const& operator[](const int i) const
        {
            return elements_[i];
        }
        void operator+=(const Point& other)
        {
            for(std::size_t i =0; i < nDimensions; ++i)
            {
                elements_[i] += other.elements_[i];
            }
        }
        void operator-=(const Point& other)
        {
            for(std::size_t i =0; i < nDimensions; ++i)
            {
                elements_[i] -= other.elements_[i];
            }
        }
        friend Point operator+(const Point& a, const Point& b)
        {
            Point ret(a);
            ret += b;
            return ret;
        }
        friend Point operator-(const Point& a, const Point& b)
        {
            Point ret(a);
            ret -= b;
            return ret;
        }

        Point() : elements_()
        {
        }
        Point(T x, T y)
        {
            STATIC_ASSERT(nDimensions == 2);
            elements_[0] = x;
            elements_[1] = y;
        }
        Point(T x, T y, T z)
        {
            STATIC_ASSERT(nDimensions == 3);
            elements_[0] = x;
            elements_[1] = y;
            elements_[2] = z;
        }
    };
}// namespace Utils

#endif//TEMPLATE_POINT_H

//    https://stackoverflow.com/questions/11891192/c-templates-simple-point-class