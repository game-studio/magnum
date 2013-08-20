#ifndef Magnum_Shapes_shapeImplementation_h
#define Magnum_Shapes_shapeImplementation_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <utility>
#include <Utility/Assert.h>

#include "DimensionTraits.h"
#include "Magnum.h"
#include "Shapes/Shapes.h"
#include "Shapes/magnumShapesVisibility.h"

namespace Magnum { namespace Shapes { namespace Implementation {

/* Shape type for given dimension count */

template<UnsignedInt> struct ShapeDimensionTraits;

template<> struct ShapeDimensionTraits<2> {
    enum class Type: UnsignedByte {
        Point = 1,
        Line = 2,
        LineSegment = 3,
        Sphere = 5,
        Cylinder = 7,
        Capsule = 11,
        AxisAlignedBox = 13,
        Box = 17,
        Composition = 19
    };
};

template<> struct ShapeDimensionTraits<3> {
    enum class Type: UnsignedByte {
        Point = 1,
        Line = 2,
        LineSegment = 3,
        Sphere = 5,
        Cylinder = 7,
        Capsule = 11,
        AxisAlignedBox = 13,
        Box = 17,
        Plane = 19,
        Composition = 23
    };
};

Debug MAGNUM_SHAPES_EXPORT operator<<(Debug debug, ShapeDimensionTraits<2>::Type value);
Debug MAGNUM_SHAPES_EXPORT operator<<(Debug debug, ShapeDimensionTraits<3>::Type value);

/* Enum value corresponding to given type */

template<class> struct TypeOf;

template<UnsignedInt dimensions> struct TypeOf<Shapes::Point<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::Point;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::Line<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::Line;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::LineSegment<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::LineSegment;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::Sphere<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::Sphere;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::Cylinder<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::Cylinder;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::Capsule<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::Capsule;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::AxisAlignedBox<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::AxisAlignedBox;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::Box<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::Box;
    }
};
template<> struct TypeOf<Shapes::Plane> {
    constexpr static ShapeDimensionTraits<3>::Type type() {
        return ShapeDimensionTraits<3>::Type::Plane;
    }
};
template<UnsignedInt dimensions> struct TypeOf<Shapes::Composition<dimensions>> {
    constexpr static typename ShapeDimensionTraits<dimensions>::Type type() {
        return ShapeDimensionTraits<dimensions>::Type::Composition;
    }
};

/* Polymorphic shape wrappers */

template<UnsignedInt dimensions> struct MAGNUM_SHAPES_EXPORT AbstractShape {
    explicit AbstractShape();
    virtual ~AbstractShape();

    virtual typename ShapeDimensionTraits<dimensions>::Type MAGNUM_SHAPES_LOCAL type() const = 0;
    virtual AbstractShape<dimensions> MAGNUM_SHAPES_LOCAL * clone() const = 0;
    virtual void MAGNUM_SHAPES_LOCAL transform(const typename DimensionTraits<dimensions, Float>::MatrixType& matrix, AbstractShape<dimensions>* result) const = 0;
};

template<class T> struct Shape: AbstractShape<T::Dimensions> {
    T shape;

    explicit Shape() = default;
    explicit Shape(const T& shape): shape(shape) {}
    explicit Shape(T&& shape): shape(std::move(shape)) {}

    typename ShapeDimensionTraits<T::Dimensions>::Type type() const override {
        return TypeOf<T>::type();
    }

    AbstractShape<T::Dimensions>* clone() const override {
        return new Shape<T>(shape);
    }

    void transform(const typename DimensionTraits<T::Dimensions, Float>::MatrixType& matrix, AbstractShape<T::Dimensions>* result) const override {
        CORRADE_INTERNAL_ASSERT(result->type() == type());
        static_cast<Shape<T>*>(result)->shape = shape.transformed(matrix);
    }
};

}}}

#endif
