//
//              LAPACK++ 1.1 Linear Algebra Package 1.1
//               University of Tennessee, Knoxvilee, TN.
//            Oak Ridge National Laboratory, Oak Ridge, TN.
//        Authors: J. J. Dongarra, E. Greaser, R. Pozo, D. Walker
//                 (C) 1992-1996 All Rights Reserved
//
//                             NOTICE
//
// Permission to use, copy, modify, and distribute this software and
// its documentation for any purpose and without fee is hereby granted
// provided that the above copyright notice appear in all copies and
// that both the copyright notice and this permission notice appear in
// supporting documentation.
//
// Neither the Institutions (University of Tennessee, and Oak Ridge National
// Laboratory) nor the Authors make any representations about the suitability
// of this software for any purpose.  This software is provided ``as is''
// without express or implied warranty.
//
// LAPACK++ was funded in part by the U.S. Department of Energy, the
// National Science Foundation and the State of Tennessee.


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "lafnames.h"
#include LA_EXCEPTION_H
#include VECTOR_DOUBLE_H
#include "vtmpl.h"

VectorDouble::VectorDouble(unsigned n)
    : p(new vref_type(n))
    , data(p->data)
{
    if (!data) throw LaException("VectorDouble(unsigned)", "out of memory");
}


VectorDouble::VectorDouble(const VectorDouble& m)
{
    // shallow assignment semantics
    ref_vref(m.p);
}

VectorDouble::VectorDouble(value_type *d, unsigned n)
    : p(new vref_type(d, n))
    , data(p->data)
{
    if (!d) throw LaException("VectorDouble(unsigned)", "data is NULL");
}

VectorDouble::VectorDouble(value_type *d, unsigned m, unsigned n,
                           bool row_ordering)
    : p(row_ordering ? new vref_type(m*n) : new vref_type(d, m*n))
    , data(p->data)
{
    if (!d)
        throw LaException("VectorDouble", "data is NULL");

    if (!row_ordering)
    {
        // nothing else to do
    }
    else // row ordering
    {
        if (!data)
            throw LaException("VectorDouble", "out of memory");
        for (unsigned i = 0; i < m * n; i++)
        {
            data[m*(i%n)+(i/n)] = d[i]; // reorder the data to column-major
        }
    }
}

VectorDouble::VectorDouble(unsigned n, value_type scalar)
    : p(new vref_type(n))
    , data(p->data)
{
    if (!data)
        throw LaException("VectorDouble(int,double)", "out of memory");
    vtmpl::assign(*this, scalar);
}

VectorDouble::~VectorDouble()
{
    unref_vref();
}

int VectorDouble::resize(unsigned d)
{
    return vtmpl::resize(*this, d);
}

VectorDouble& VectorDouble::inject(const VectorDouble& m)
{
    if (m.size() != size())
        throw LaException("VectorDouble::inject(VectorDouble)", "vector sizes do not match");

    return vtmpl::inject(*this, m);
}

VectorDouble& VectorDouble::copy(const VectorDouble &m)
{
    return vtmpl::copy(*this, m);
}

std::ostream& operator<<(std::ostream& s, const VectorDouble& m)
{
    return vtmpl::print(s, m);
}

VectorDouble& VectorDouble::operator=(value_type scalar)
{
    return vtmpl::assign(*this, scalar);
}
