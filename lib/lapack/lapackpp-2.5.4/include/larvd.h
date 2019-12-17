// -*-C++-*-

// Copyright (C) 2004
// Christian Stimming <stimming@tuhh.de>

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2, or (at
// your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

//      LAPACK++ (V. 1.1)
//      (C) 1992-1996 All Rights Reserved.

/** @file
 * @brief Row vector
 */

#ifndef _LA_ROW_VECTOR_DOUBLE_H_
#define _LA_ROW_VECTOR_DOUBLE_H_

#include "lafnames.h"

#ifndef _LA_GEN_MAT_DOUBLE_H_
#include LA_GEN_MAT_DOUBLE_H
#endif


/** @brief Row vector
 *
 * A row vector is simply an nx1 matrix, only that it can be
 * constructed and accessed by a single dimension. (FIXME: It is
 * unclear whether this class is actually well supported inside
 * Lapack++; probably you should only use @ref LaVectorDouble
 * instead.)
 */
class LaRowVectorDouble: public LaGenMatDouble
{
public:

    inline LaRowVectorDouble();
    inline LaRowVectorDouble(int);
    inline LaRowVectorDouble(double*, int);
    inline LaRowVectorDouble(const LaGenMatDouble&);

    inline int size() const;
    inline int inc() const;
    inline LaIndex index() const;
    inline int start() const;
    inline int end() const;


    inline double& operator()(int i);
    inline const double& operator()(int i) const ;
    inline LaRowVectorDouble operator()(const LaIndex&);

    inline LaRowVectorDouble& operator=(const LaGenMatDouble &A);
    inline LaRowVectorDouble& operator=(double d);

};


inline LaRowVectorDouble::LaRowVectorDouble() : LaGenMatDouble(1, 0) {}
inline LaRowVectorDouble::LaRowVectorDouble(int i) : LaGenMatDouble(1, i) {}


inline LaRowVectorDouble::LaRowVectorDouble(double *d, int m) :
    LaGenMatDouble(d, 1, m) {}


inline LaRowVectorDouble::LaRowVectorDouble(const LaGenMatDouble& G) :
    LaGenMatDouble(G)
{
    assert(G.size(0) == 1);

}


inline int LaRowVectorDouble::size() const
{
    return LaGenMatDouble::size(0) * LaGenMatDouble::size(1);
}

inline double& LaRowVectorDouble::operator()(int i)
{
    return LaGenMatDouble::operator()(0, i);
}

inline const double& LaRowVectorDouble::operator()(int i) const
{
    return LaGenMatDouble::operator()(0, i);
}

inline LaRowVectorDouble LaRowVectorDouble::operator()(const LaIndex& I)
{
    return LaGenMatDouble::operator()(LaIndex(0, 0), I);
}

inline LaRowVectorDouble& LaRowVectorDouble::operator=(const LaGenMatDouble &A)
{
    LaGenMatDouble::copy(A);
    return *this;
}

inline LaRowVectorDouble& LaRowVectorDouble::operator=(double d)
{
    LaGenMatDouble::operator=(d);
    return *this;
}

#if 0
inline LaRowVectorDouble& LaRowVectorDouble::copy(const LaGenMatDouble &A)
{
    assert(A.size(0) == 1 || A.size(1) == 1);   //make sure rhs is a
    // a vector.
    LaGenMatDouble::copy(A);
    return *this;
}


inline LaRowVectorDouble& LaRowVectorDouble::ref(const LaGenMatDouble &A)
{
    assert(A.size(0) == 1 || A.size(1) == 1);
    LaGenMatDouble::ref(A);
    return *this;
}


inline LaRowVectorDouble& LaRowVectorDouble::inject(const LaGenMatDouble &A)
{
    assert(A.size(0) == 1 || A.size(1) == 1);
    LaGenMatDouble::inject(A);
    return *this;
}

#endif

inline int LaRowVectorDouble::inc() const
{
    return LaGenMatDouble::inc(1);
}

inline LaIndex LaRowVectorDouble::index() const
{
    return LaGenMatDouble::index(1);
}

inline int LaRowVectorDouble::start() const
{
    return LaGenMatDouble::start(1);
}

inline int LaRowVectorDouble::end() const
{
    return LaGenMatDouble::end(1);
}

#endif
// _LA_ROW_VECTOR_DOUBLE_H_
