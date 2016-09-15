// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
// vi: set et ts=4 sw=4 sts=4:
/*
  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.

  Consult the COPYING file in the top-level source directory of this
  module for the precise wording of the license and the list of
  copyright holders.
*/
/*!
 * \file
 *
 * \brief A number of commonly used algebraic functions for the localized OPM automatic
 *        differentiation (AD) framework.
 *
 * This file provides AD variants of the the most commonly used functions of the <cmath>
 * header file.
 */
#ifndef OPM_LOCAL_AD_MATH_HPP
#define OPM_LOCAL_AD_MATH_HPP

#include "Evaluation.hpp"

#include <opm/material/common/MathToolbox.hpp>

namespace Opm {
namespace DenseAd {
// forward declaration of the Evaluation template class
template <class ValueT, int numVars>
class Evaluation;

// provide some algebraic functions
template <class ValueType, int numVars>
Evaluation<ValueType, numVars> abs(const Evaluation<ValueType, numVars>& x)
{
    Evaluation<ValueType, numVars> result;

    if (x.value < 0.0) {
        result.value = -x.value;
        for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
            result.derivatives[curVarIdx] = -x.derivatives[curVarIdx];
    }
    else {
        result.value = x.value;
        for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
            result.derivatives[curVarIdx] = x.derivatives[curVarIdx];
    }

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> min(const Evaluation<ValueType, numVars>& x1,
                                   const Evaluation<ValueType, numVars>& x2)
{
    Evaluation<ValueType, numVars> result;

    if (x1.value < x2.value) {
        result.value = x1.value;

        std::copy(x1.derivatives.begin(),
                  x1.derivatives.end(),
                  result.derivatives.begin());
    }
    else  {
        result.value = x2.value;

        std::copy(x2.derivatives.begin(),
                  x2.derivatives.end(),
                  result.derivatives.begin());
    }

    return result;
}

template <class Arg1ValueType, class ValueType, int numVars>
Evaluation<ValueType, numVars> min(const Arg1ValueType& x1,
                                   const Evaluation<ValueType, numVars>& x2)
{
    Evaluation<ValueType, numVars> result;

    if (x1 < x2.value) {
        result.value = x1;

        std::fill(result.derivatives.begin(),
                  result.derivatives.end(),
                  0.0);
    }
    else  {
        result.value = x2.value;

        std::copy(x2.derivatives.begin(),
                  x2.derivatives.end(),
                  result.derivatives.begin());
    }

    return result;
}

template <class Arg2ValueType, class ValueType, int numVars>
Evaluation<ValueType, numVars> min(const Evaluation<ValueType, numVars>& x1,
                                   const Arg2ValueType& x2)
{ return min(x2, x1); }

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> max(const Evaluation<ValueType, numVars>& x1,
                                   const Evaluation<ValueType, numVars>& x2)
{
    Evaluation<ValueType, numVars> result;

    if (x1.value > x2.value) {
        result.value = x1.value;

        std::copy(x1.derivatives.begin(),
                  x1.derivatives.end(),
                  result.derivatives.begin());
    }
    else  {
        result.value = x2.value;

        std::copy(x2.derivatives.begin(),
                  x2.derivatives.end(),
                  result.derivatives.begin());
    }

    return result;
}

template <class Arg1ValueType, class ValueType, int numVars>
Evaluation<ValueType, numVars> max(const Arg1ValueType& x1,
                                   const Evaluation<ValueType, numVars>& x2)
{
    Evaluation<ValueType, numVars> result;

    if (x1 > x2.value) {
        result.value = x1;

        std::fill(result.derivatives.begin(),
                  result.derivatives.end(),
                  0.0);
    }
    else  {
        result.value = x2.value;

        std::copy(x2.derivatives.begin(),
                  x2.derivatives.end(),
                  result.derivatives.begin());
    }

    return result;
}

template <class Arg2ValueType, class ValueType, int numVars>
Evaluation<ValueType, numVars> max(const Evaluation<ValueType, numVars>& x1,
                                   const Arg2ValueType& x2)
{ return max(x2, x1); }

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> tan(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    const ValueType& tmp = ValueTypeToolbox::tan(x.value);
    result.value = tmp;

    // derivatives use the chain rule
    const ValueType& df_dx = 1 + tmp*tmp;
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> atan(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    result.value = ValueTypeToolbox::atan(x.value);

    // derivatives use the chain rule
    const ValueType& df_dx = 1/(1 + x.value*x.value);
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> atan2(const Evaluation<ValueType, numVars>& x,
                                     const Evaluation<ValueType, numVars>& y)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    result.value = ValueTypeToolbox::atan2(x.value, y.value);

    // derivatives use the chain rule
    const ValueType& alpha = 1/(1 + (x.value*x.value)/(y.value*y.value));
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx) {
        result.derivatives[curVarIdx] =
            alpha
            /(y.value*y.value)
            *(x.derivatives[curVarIdx]*y.value - x.value*y.derivatives[curVarIdx]);
    }

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> sin(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    result.value = ValueTypeToolbox::sin(x.value);

    // derivatives use the chain rule
    const ValueType& df_dx = ValueTypeToolbox::cos(x.value);
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> asin(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    result.value = ValueTypeToolbox::asin(x.value);

    // derivatives use the chain rule
    const ValueType& df_dx = 1.0/ValueTypeToolbox::sqrt(1 - x.value*x.value);
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> cos(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    result.value = ValueTypeToolbox::cos(x.value);

    // derivatives use the chain rule
    const ValueType& df_dx = -ValueTypeToolbox::sin(x.value);
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> acos(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    result.value = ValueTypeToolbox::acos(x.value);

    // derivatives use the chain rule
    const ValueType& df_dx = - 1.0/ValueTypeToolbox::sqrt(1 - x.value*x.value);
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> sqrt(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    const ValueType& sqrt_x = ValueTypeToolbox::sqrt(x.value);
    result.value = sqrt_x;

    // derivatives use the chain rule
    ValueType df_dx = 0.5/sqrt_x;
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx) {
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];
    }

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> exp(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;
    Evaluation<ValueType, numVars> result;

    const ValueType& exp_x = ValueTypeToolbox::exp(x.value);
    result.value = exp_x;

    // derivatives use the chain rule
    const ValueType& df_dx = exp_x;
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

// exponentiation of arbitrary base with a fixed constant
template <class ValueType, int numVars, class ExpType>
Evaluation<ValueType, numVars> pow(const Evaluation<ValueType, numVars>& base,
                                   const ExpType& exp)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;
    Evaluation<ValueType, numVars> result;

    const ValueType& pow_x = ValueTypeToolbox::pow(base.value, exp);
    result.value = pow_x;

    if (base == 0.0) {
        // we special case the base 0 case because 0.0 is in the valid range of the
        // base but the generic code leads to NaNs.
        result = 0.0;
    }
    else {
        // derivatives use the chain rule
        const ValueType& df_dx = pow_x/base.value*exp;
        for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
            result.derivatives[curVarIdx] = df_dx*base.derivatives[curVarIdx];
    }

    return result;
}

// exponentiation of constant base with an arbitrary exponent
template <class BaseType, class ValueType, int numVars>
Evaluation<ValueType, numVars> pow(const BaseType& base,
                                   const Evaluation<ValueType, numVars>& exp)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    if (base == 0.0) {
        // we special case the base 0 case because 0.0 is in the valid range of the
        // base but the generic code leads to NaNs.
        result = 0.0;
    }
    else {
        const ValueType& lnBase = ValueTypeToolbox::log(base);
        result.value = ValueTypeToolbox::exp(lnBase*exp.value);

        // derivatives use the chain rule
        const ValueType& df_dx = lnBase*result.value;
        for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
            result.derivatives[curVarIdx] = df_dx*exp.derivatives[curVarIdx];
    }

    return result;
}

// this is the most expensive power function. Computationally it is pretty expensive, so
// one of the above two variants above should be preferred if possible.
template <class ValueType, int numVars>
Evaluation<ValueType, numVars> pow(const Evaluation<ValueType, numVars>& base,
                                   const Evaluation<ValueType, numVars>& exp)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    if (base == 0.0) {
        // we special case the base 0 case because 0.0 is in the valid range of the
        // base but the generic code leads to NaNs.
        result = 0.0;
    }
    else {
        ValueType valuePow = ValueTypeToolbox::pow(base.value, exp.value);
        result.value = valuePow;

        // use the chain rule for the derivatives. since both, the base and the exponent can
        // potentially depend on the variable set, calculating these is quite elaborate...
        const ValueType& f = base.value;
        const ValueType& g = exp.value;
        const ValueType& logF = ValueTypeToolbox::log(f);
        for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx) {
            const ValueType& fPrime = base.derivatives[curVarIdx];
            const ValueType& gPrime = exp.derivatives[curVarIdx];
            result.derivatives[curVarIdx] = (g*fPrime/f + logF*gPrime) * valuePow;
        }
    }

    return result;
}

template <class ValueType, int numVars>
Evaluation<ValueType, numVars> log(const Evaluation<ValueType, numVars>& x)
{
    typedef MathToolbox<ValueType> ValueTypeToolbox;

    Evaluation<ValueType, numVars> result;

    result.value = ValueTypeToolbox::log(x.value);

    // derivatives use the chain rule
    const ValueType& df_dx = 1/x.value;
    for (unsigned curVarIdx = 0; curVarIdx < result.derivatives.size(); ++curVarIdx)
        result.derivatives[curVarIdx] = df_dx*x.derivatives[curVarIdx];

    return result;
}

} // namespace DenseAd

// a kind of traits class for the automatic differentiation case. (The toolbox for the
// scalar case is provided by the MathToolbox.hpp header file.)
template <class ValueT, int numVars>
struct MathToolbox<Opm::DenseAd::Evaluation<ValueT, numVars> >
{
private:
public:
    typedef ValueT ValueType;
    typedef Opm::MathToolbox<ValueType> InnerToolbox;
    typedef typename InnerToolbox::Scalar Scalar;
    typedef Opm::DenseAd::Evaluation<ValueType, numVars> Evaluation;

    static ValueType value(const Evaluation& eval)
    { return eval.value; }

    static decltype(InnerToolbox::scalarValue(0.0)) scalarValue(const Evaluation& eval)
    { return InnerToolbox::scalarValue(eval.value); }

    static Evaluation createConstant(ValueType value)
    { return Evaluation::createConstant(value); }

    static Evaluation createVariable(ValueType value, int varIdx)
    { return Evaluation::createVariable(value, varIdx); }

    template <class LhsEval>
    static typename std::enable_if<std::is_same<Evaluation, LhsEval>::value,
                                   LhsEval>::type
    decay(const Evaluation& eval)
    { return eval; }

    template <class LhsEval>
    static typename std::enable_if<std::is_same<Evaluation, LhsEval>::value,
                                   LhsEval>::type
    decay(const Evaluation&& eval)
    { return eval; }

    template <class LhsEval>
    static typename std::enable_if<std::is_floating_point<LhsEval>::value,
                                   LhsEval>::type
    decay(const Evaluation& eval)
    { return eval.value; }

    // comparison
    static bool isSame(const Evaluation& a, const Evaluation& b, Scalar tolerance)
    {
        typedef MathToolbox<ValueType> ValueTypeToolbox;

        // make sure that the value of the evaluation is identical
        if (!ValueTypeToolbox::isSame(a.value, b.value, tolerance))
            return false;

        // make sure that the derivatives are identical
        for (unsigned curVarIdx = 0; curVarIdx < numVars; ++curVarIdx)
            if (!ValueTypeToolbox::isSame(a.derivatives[curVarIdx], b.derivatives[curVarIdx], tolerance))
                return false;

        return true;
    }

    // arithmetic functions
    template <class Arg1Eval, class Arg2Eval>
    static Evaluation max(const Arg1Eval& arg1, const Arg2Eval& arg2)
    { return Opm::DenseAd::max(arg1, arg2); }

    template <class Arg1Eval, class Arg2Eval>
    static Evaluation min(const Arg1Eval& arg1, const Arg2Eval& arg2)
    { return Opm::DenseAd::min(arg1, arg2); }

    static Evaluation abs(const Evaluation& arg)
    { return Opm::DenseAd::abs(arg); }

    static Evaluation tan(const Evaluation& arg)
    { return Opm::DenseAd::tan(arg); }

    static Evaluation atan(const Evaluation& arg)
    { return Opm::DenseAd::atan(arg); }

    static Evaluation atan2(const Evaluation& arg1, const Evaluation& arg2)
    { return Opm::DenseAd::atan2(arg1, arg2); }

    static Evaluation sin(const Evaluation& arg)
    { return Opm::DenseAd::sin(arg); }

    static Evaluation asin(const Evaluation& arg)
    { return Opm::DenseAd::asin(arg); }

    static Evaluation cos(const Evaluation& arg)
    { return Opm::DenseAd::cos(arg); }

    static Evaluation acos(const Evaluation& arg)
    { return Opm::DenseAd::acos(arg); }

    static Evaluation sqrt(const Evaluation& arg)
    { return Opm::DenseAd::sqrt(arg); }

    static Evaluation exp(const Evaluation& arg)
    { return Opm::DenseAd::exp(arg); }

    static Evaluation log(const Evaluation& arg)
    { return Opm::DenseAd::log(arg); }

    template <class RhsValueType>
    static Evaluation pow(const Evaluation& arg1, const RhsValueType& arg2)
    { return Opm::DenseAd::pow(arg1, arg2); }

    template <class RhsValueType>
    static Evaluation pow(const RhsValueType& arg1, const Evaluation& arg2)
    { return Opm::DenseAd::pow(arg1, arg2); }

    static Evaluation pow(const Evaluation& arg1, const Evaluation& arg2)
    { return Opm::DenseAd::pow(arg1, arg2); }
};

}

#endif
