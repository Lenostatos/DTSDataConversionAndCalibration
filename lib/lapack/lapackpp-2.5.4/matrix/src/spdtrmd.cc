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

#include "arch.h"
#include "lafnames.h"
#include LA_EXCEPTION_H
#include LA_SPD_TRIDIAG_MAT_DOUBLE_H


DLLIMPORT double LaSpdTridiagMatDouble::outofbounds_ = 0; // set outofbounds_.

DLLIMPORT int LaSpdTridiagMatDouble::debug_ = 0; // set debug to 0 initially.

DLLIMPORT int* LaSpdTridiagMatDouble::info_ = new int; // turn off info print flag.

double& LaSpdTridiagMatDouble::operator()(int i, int j)
{
    // really should assert that abs(i-j) < 2
    int tmp = i - j;

    switch (tmp)
    {
    case 0:   // main
        if (i > d_.size() - 1)
            return outofbounds_;
        else
            return d_(i);
    case 1:   // lower
    case -1:  // upper
        if (i > dl_.size() - 1)
            return outofbounds_;
        else
            return dl_(i);
    default:
        return outofbounds_;
    }
}

double LaSpdTridiagMatDouble::operator()(int i, int j) const
{
    int tmp = i - j;
    switch (tmp)
    {
    case 0:   // main
        if (i > d_.size() - 1)
            return 0.0;
        else
            return d_(i);
    case 1:   // lower
    case -1:  // upper
        if (i > dl_.size() - 1)
            return 0.0;
        else
            return dl_(i);
    default:
        return 0.0;
    }
}

LaVectorDouble LaSpdTridiagMatDouble::diag(int k)
{

    LaVectorDouble tmp;

    switch (k)
    {
    case 0:   // main
        tmp.ref(d_);
        break;
    case -1:  // lower
    case 1:   // upper
        tmp.ref(dl_);
        break;
    default:
        std::cerr << "Unrecognized integer representation of diagonal\n";
        assert(0);
    }

    return tmp;
}

#define OUTPUT_MATRIX
#ifdef OUTPUT_MATRIX

std::ostream& operator<<(std::ostream& s, const LaSpdTridiagMatDouble& td)

{
    if (*(td.info_))     // print out only matrix info, not actual values
    {
        *(td.info_) = 0; // reset the flag
        s << "maindiag: (" << td.d_.size() << ") " ;
        s << " #ref: " << td.d_.ref_count() << std::endl;
        s << "subdiag: (" << td.dl_.size() << ") " ;
        s << " #ref: " << td.dl_.ref_count() << std::endl;
    }
    else
    {
        int tmp, N = td.size_, n = N - 1;

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                tmp = i - j;
                switch (tmp)
                {
                case 0:   // main
                    s << td.d_(i) << ' ';
                    break;
                case 1:  // lower
                case -1:   // upper
                    if (i < n)
                        s << td.dl_(i) << ' ';
                    break;
                default:
                    s << "  ";
                }
            }
            s << std::endl;
        }
        s << std::endl;
    }
    return s;
}

#else // output individual vectors

std::ostream& operator<<(std::ostream& s, const LaSpdTridiagMatDouble& td)

{
    if (*(td.info_))     // print out only matrix info, not actual values
    {
        *(td.info_) = 0; // reset the flag
        s << "maindiag: (" << td.d_.size() << ") " ;
        s << " #ref: " << td.d_.ref_count() << std::endl;
        s << "subdiag: (" << td.dl_.size() << ") " ;
        s << " #ref: " << td.dl_.ref_count() << std::endl;
    }
    else
    {
        s << td.dl_;
        s << td.d_ ;
        s << td.dl_;
    }

    return s;
}

#endif // OUTPUT_MATRIX
