#include <type_traits>
#include <ratio>
#include <limits>

#define _CONSTEXPR_ constexpr

namespace si
{

// Forward decl
template <typename UNITS, typename STORAGE, typename RATIO = std::ratio<1> > class quantity;

//------------------------------------------------------------------------------
/// @param _Tp a type to be tested
/// @return value true if _Tp is quantity, false otherwise
template <typename _Tp>
struct is_quantity : std::false_type {};

template <typename UNITS, typename STORAGE, typename RATIO>
struct is_quantity<quantity<UNITS, STORAGE, RATIO> > : std::true_type {};

template <typename UNITS, typename STORAGE, typename RATIO>
struct is_quantity<const quantity<UNITS, STORAGE, RATIO> > : std::true_type {};

template <typename UNITS, typename STORAGE, typename RATIO>
struct is_quantity<volatile quantity<UNITS, STORAGE, RATIO> > : std::true_type {};

template <typename UNITS, typename STORAGE, typename RATIO>
struct is_quantity<const volatile quantity<UNITS, STORAGE, RATIO> > : std::true_type {};

//------------------------------------------------------------------------------
/// @param _Xp an integer value
/// @param _Yp an integer value
/// @return value the greatest common divisor (GCD) of _Xp and _Yp
template <intmax_t _Xp, intmax_t _Yp>
struct gcd
{
    static const intmax_t value = gcd<_Yp, _Xp % _Yp>::value;
};

template <intmax_t _Xp>
struct gcd<_Xp, 0>
{
    static const intmax_t value = _Xp;
};

template <>
struct gcd<0, 0>
{
    static const intmax_t value = 1;
};

//------------------------------------------------------------------------------
/// @param _Xp an integer value
/// @param _Yp an integer value
/// @member value the least common multiple (LCM) of _Xp and _Yp
template <intmax_t _Xp, intmax_t _Yp>
struct lcm
{
    static const intmax_t value = _Xp / gcd<_Xp, _Yp>::value * _Yp;
};

//------------------------------------------------------------------------------
/// @param _R1 a std::ratio
/// @param _R2 a std::ratio
/// @return the std::ratio that is the greatest common divisor (GCD) of _R1 and _R2
template <typename _R1, typename _R2>
using ratio_gcd_t = std::ratio
<
    gcd
    <
        _R1::num,
        _R2::num
    >::value,
    lcm
    <
        _R1::den,
        _R2::den
    >::value
>;

} // end of namespace si

//------------------------------------------------------------------------------
/// Specialization of std::common_type for quantity.
template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO1,
    typename STORAGE2,
    typename RATIO2
>
struct std::common_type
<
    si::quantity<UNITS, STORAGE1, RATIO1>,
    si::quantity<UNITS, STORAGE2, RATIO2>
>
{
    using type = si::quantity
    <
        UNITS,
        typename std::common_type<STORAGE1, STORAGE2>::type,
        si::ratio_gcd_t<RATIO1, RATIO2>
    >;
};

namespace si
{

// quantity_cast

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO = typename std::ratio_divide
    <
        typename FromQ::ratio_t,
        typename ToQ::ratio_t
    >::type,
    bool = RATIO::num == 1,
    bool = RATIO::den == 1>
struct quantity_cast_t;

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_t<FromQ, ToQ, RATIO, true, true>
{
    _CONSTEXPR_
    ToQ operator()(const FromQ& __fd) const
    {
        return ToQ
        (
            static_cast<typename ToQ::storage_t>(__fd.count())
        );
    }
};

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_t<FromQ, ToQ, RATIO, true, false>
{
    _CONSTEXPR_
    ToQ operator()(const FromQ& __fd) const
    {
        using _Ct = typename std::common_type
        <
            typename ToQ::storage_t,
            typename FromQ::storage_t,
            intmax_t
        >::type;
        return ToQ
        (
            static_cast<typename ToQ::storage_t>
            (
                static_cast<_Ct>(__fd.count()) /
                static_cast<_Ct>(RATIO::den)
            )
        );
    }
};

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_t<FromQ, ToQ, RATIO, false, true>
{
    _CONSTEXPR_
    ToQ operator()(const FromQ& __fd) const
    {
        using _Ct = typename std::common_type
        <
            typename ToQ::storage_t,
            typename FromQ::storage_t,
            intmax_t
        >::type;
        return ToQ
        (
            static_cast<typename ToQ::storage_t>
            (
                static_cast<_Ct>(__fd.count()) *
                static_cast<_Ct>(RATIO::num)
            )
        );
    }
};

template
<
    typename FromQ,
    typename ToQ,
    typename RATIO
>
struct quantity_cast_t<FromQ, ToQ, RATIO, false, false>
{
    _CONSTEXPR_
    ToQ operator()(const FromQ& __fd) const
    {
        using _Ct = typename std::common_type
        <
            typename ToQ::storage_t,
            typename FromQ::storage_t,
            intmax_t
        >::type;
        return ToQ
        (
            static_cast<typename ToQ::storage_t>
            (
                static_cast<_Ct>(__fd.count()) *
                static_cast<_Ct>(RATIO::num) /
                static_cast<_Ct>(RATIO::den)
            )
        );
    }
};

template <typename ToQ, typename UNITS, typename STORAGE, typename RATIO>
inline
_CONSTEXPR_
typename std::enable_if
<
    is_quantity<ToQ>::value,
    ToQ
>::type
quantity_cast(const quantity<UNITS, STORAGE, RATIO>& __fd)
{
    return quantity_cast_t
    <
        quantity<UNITS, STORAGE, RATIO>,
        ToQ
    >()(__fd);
}

// some special quantity values
template <typename STORAGE>
struct quantity_values
{
public:
    static _CONSTEXPR_ STORAGE zero() {return STORAGE(0);}
    static _CONSTEXPR_ STORAGE max()  {return std::numeric_limits<STORAGE>::max();}
    static _CONSTEXPR_ STORAGE min()  {return std::numeric_limits<STORAGE>::lowest();}
};

// is_ratio
template <typename _Tp>
struct is_ratio : std::false_type{};

template <std::intmax_t _Num, std::intmax_t _Den>
struct is_ratio<std::ratio<_Num, _Den>> : std::true_type{};

// quantity
template <typename UNITS, typename STORAGE, typename RATIO>
class quantity
{
    static_assert(!is_quantity<STORAGE>::value, "A quantity representation can not be a quantity");
    static_assert(is_ratio<RATIO>::value, "Second template parameter of quantity must be a std::ratio");
    static_assert(RATIO::num > 0, "quantity exponents must be positive");

    template <typename _R1, typename _R2>
    struct __no_overflow
    {
    private:
        static const intmax_t __gcd_n1_n2 = gcd<_R1::num, _R2::num>::value;
        static const intmax_t __gcd_d1_d2 = gcd<_R1::den, _R2::den>::value;
        static const intmax_t __n1 = _R1::num / __gcd_n1_n2;
        static const intmax_t __d1 = _R1::den / __gcd_d1_d2;
        static const intmax_t __n2 = _R2::num / __gcd_n1_n2;
        static const intmax_t __d2 = _R2::den / __gcd_d1_d2;
        static const intmax_t max = -((intmax_t(1) << (sizeof(intmax_t) * CHAR_BIT - 1)) + 1);

        template <intmax_t _Xp, intmax_t _Yp, bool __overflow>
        struct __mul    // __overflow == false
        {
            static const intmax_t value = _Xp * _Yp;
        };

        template <intmax_t _Xp, intmax_t _Yp>
        struct __mul<_Xp, _Yp, true>
        {
            static const intmax_t value = 1;
        };

    public:
        static const bool value = (__n1 <= max / __d2) && (__n2 <= max / __d1);
        using type = std::ratio<__mul<__n1, __d2, !value>::value,
                      __mul<__n2, __d1, !value>::value>;
    };

public:
    using units_t = UNITS;
    using storage_t = STORAGE;
    using ratio_t = RATIO;
private:
    storage_t mStorage;
public:

    _CONSTEXPR_
    quantity() = default;

    template <typename STORAGE2>
    _CONSTEXPR_
    explicit
    quantity
    (
        const STORAGE2& __r,
        typename std::enable_if
        <
            std::is_convertible<STORAGE2, storage_t>::value &&
            (
                std::is_floating_point<storage_t>::value ||
                !std::is_floating_point<STORAGE2>::value
            )
        >::type* = 0
    )
    :
    mStorage(__r) {}

    // conversions
    template <typename STORAGE2, typename RATIO2>
    _CONSTEXPR_
    quantity
    (
        const quantity<UNITS, STORAGE2, RATIO2>& __d,
        typename std::enable_if
        <
            __no_overflow<RATIO2, ratio_t>::value &&
            (
                std::is_floating_point<storage_t>::value ||
                (
                    __no_overflow<RATIO2, ratio_t>::type::den == 1 &&
                    !std::is_floating_point<STORAGE2>::value
                )
            )
        >::type* = 0
    )
    :
    mStorage(quantity_cast<quantity>(__d).count()) {}

    // observer

    _CONSTEXPR_ storage_t count() const {return mStorage;}
    _CONSTEXPR_ ratio_t ratio() const { return ratio_t{}; }

    // arithmetic

    _CONSTEXPR_ quantity  operator+() const {return *this;}
    _CONSTEXPR_ quantity  operator-() const {return quantity(-mStorage);}
    quantity& operator++()      {++mStorage; return *this;}
    quantity  operator++(int)   {return quantity(mStorage++);}
    quantity& operator--()      {--mStorage; return *this;}
    quantity  operator--(int)   {return quantity(mStorage--);}

    quantity& operator+=(const quantity& __d) {mStorage += __d.count(); return *this;}
    quantity& operator-=(const quantity& __d) {mStorage -= __d.count(); return *this;}

    quantity& operator*=(const storage_t& rhs) {mStorage *= rhs; return *this;}
    quantity& operator/=(const storage_t& rhs) {mStorage /= rhs; return *this;}
    quantity& operator%=(const storage_t& rhs) {mStorage %= rhs; return *this;}
    quantity& operator%=(const quantity& rhs) {mStorage %= rhs.count(); return *this;}

    // special values

    static _CONSTEXPR_ quantity zero() {return quantity(quantity_values<storage_t>::zero());}
    static _CONSTEXPR_ quantity min()  {return quantity(quantity_values<storage_t>::min());}
    static _CONSTEXPR_ quantity max()  {return quantity(quantity_values<storage_t>::max());}
};

// BaseUnit ==

template <typename _LhsBaseUnit, typename _RhsBaseUnit>
struct __quantity_eq
{
    _CONSTEXPR_
    bool operator()(const _LhsBaseUnit& __lhs, const _RhsBaseUnit& __rhs) const
        {
            using _Ct = typename std::common_type<_LhsBaseUnit, _RhsBaseUnit>::type;
            return _Ct(__lhs).count() == _Ct(__rhs).count();
        }
};

template <typename _LhsBaseUnit>
struct __quantity_eq<_LhsBaseUnit, _LhsBaseUnit>
{
    _CONSTEXPR_
    bool operator()(const _LhsBaseUnit& __lhs, const _LhsBaseUnit& __rhs) const
        {return __lhs.count() == __rhs.count();}
};

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
bool
operator==
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    return __quantity_eq
    <
        quantity<UNITS, STORAGE1, RATIO1>,
        quantity<UNITS, STORAGE2, RATIO2>
    >()(__lhs, __rhs);
}

// BaseUnit !=

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
bool
operator!=
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    return !(__lhs == __rhs);
}

// BaseUnit <

template <typename _LhsBaseUnit, typename _RhsBaseUnit>
struct __quantity_lt
{
    _CONSTEXPR_
    bool operator()(const _LhsBaseUnit& __lhs, const _RhsBaseUnit& __rhs) const
    {
        using _Ct = typename std::common_type<_LhsBaseUnit, _RhsBaseUnit>::type;
        return _Ct(__lhs).count() < _Ct(__rhs).count();
    }
};

template <typename _LhsBaseUnit>
struct __quantity_lt<_LhsBaseUnit, _LhsBaseUnit>
{
    _CONSTEXPR_
    bool operator()
    (
        const _LhsBaseUnit& __lhs,
        const _LhsBaseUnit& __rhs
    ) const
    {
        return __lhs.count() < __rhs.count();
    }
};

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
bool
operator<
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs)
{
    return __quantity_lt
    <
        quantity<UNITS, STORAGE1, RATIO1>,
        quantity<UNITS, STORAGE2, RATIO2>
    >()(__lhs, __rhs);
}

// BaseUnit >

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
bool
operator>
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    return __rhs < __lhs;
}

// BaseUnit <=

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
bool
operator<=
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    return !(__rhs < __lhs);
}

// BaseUnit >=

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
bool
operator>=
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    return !(__lhs < __rhs);
}

// BaseUnit +

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
typename std::common_type
<
    quantity<UNITS, STORAGE1, RATIO1>,
    quantity<UNITS, STORAGE2, RATIO2>
>::type
operator+
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    using _Cd = typename std::common_type
    <
        quantity<UNITS, STORAGE1, RATIO1>,
        quantity<UNITS, STORAGE2, RATIO2>
    >::type;
    return _Cd(_Cd(__lhs).count() + _Cd(__rhs).count());
}

// BaseUnit -

template <typename UNITS, typename STORAGE1, typename RATIO1, typename STORAGE2, typename RATIO2>
inline
_CONSTEXPR_
typename std::common_type
<
    quantity<UNITS, STORAGE1, RATIO1>,
    quantity<UNITS, STORAGE2, RATIO2>
>::type
operator-
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    using _Cd = typename std::common_type
    <
        quantity<UNITS, STORAGE1, RATIO1>,
        quantity<UNITS, STORAGE2, RATIO2>
    >::type;
    return _Cd(_Cd(__lhs).count() - _Cd(__rhs).count());
}

// BaseUnit *

template <typename UNITS, typename STORAGE1, typename RATIO, typename STORAGE2>
inline
_CONSTEXPR_
typename std::enable_if
<
    std::is_convertible
    <
        STORAGE2,
        typename std::common_type<STORAGE1, STORAGE2>::type
    >::value,
    quantity
    <
        UNITS,
        typename std::common_type<STORAGE1, STORAGE2>::type,
        RATIO
    >
>::type
operator*
(
    const quantity<UNITS, STORAGE1, RATIO>& __d,
    const STORAGE2& __s
)
{
    using _Cr = typename std::common_type<STORAGE1, STORAGE2>::type;
    using _Cd = quantity<UNITS, _Cr, RATIO>;
    return _Cd(_Cd(__d).count() * static_cast<_Cr>(__s));
}

template <typename UNITS, typename STORAGE1, typename RATIO, typename STORAGE2>
inline
_CONSTEXPR_
typename std::enable_if
<
    std::is_convertible
    <
        STORAGE1,
        typename std::common_type<STORAGE1, STORAGE2>::type
    >::value,
    quantity
    <
        UNITS,
        typename std::common_type<STORAGE1, STORAGE2>::type,
        RATIO
    >
>::type
operator*
(
    const STORAGE1& __s,
    const quantity<UNITS, STORAGE2, RATIO>& __d
)
{
    return __d * __s;
}

// BaseUnit /

template
<
    typename _BaseUnit,
    typename STORAGE,
    bool = is_quantity<STORAGE>::value
>
struct __quantity_divide_result
{
};

template
<
    typename _BaseUnit,
    typename STORAGE2,
    bool = std::is_convertible
    <
        STORAGE2,
        typename std::common_type<typename _BaseUnit::storage_t, STORAGE2>::type
    >::value
>
struct __quantity_divide_imp
{
};

template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO,
    typename STORAGE2
>
struct __quantity_divide_imp
<
    quantity<UNITS, STORAGE1, RATIO>,
    STORAGE2,
    true
>
{
    using type = quantity
    <
        UNITS,
        typename std::common_type<STORAGE1, STORAGE2>::type,
        RATIO
    >;
};

template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO,
    typename STORAGE2
>
struct __quantity_divide_result
<
    quantity<UNITS, STORAGE1, RATIO>,
    STORAGE2,
    false
>
: __quantity_divide_imp<quantity<UNITS, STORAGE1, RATIO>, STORAGE2>
{
};

template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO,
    typename STORAGE2
>
inline
_CONSTEXPR_
typename __quantity_divide_result
<
    quantity<UNITS, STORAGE1, RATIO>,
    STORAGE2
>::type
operator/
(
    const quantity<UNITS, STORAGE1, RATIO>& __d,
    const STORAGE2& __s
)
{
    using _Cr = typename std::common_type<STORAGE1, STORAGE2>::type;
    using _Cd = quantity<UNITS, _Cr, RATIO>;
    return _Cd(_Cd(__d).count() / static_cast<_Cr>(__s));
}

template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO1,
    typename STORAGE2,
    typename RATIO2
>
inline
_CONSTEXPR_
typename std::common_type<STORAGE1, STORAGE2>::type
operator/
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    using _Ct = typename std::common_type
    <
        quantity<UNITS, STORAGE1, RATIO1>,
        quantity<UNITS, STORAGE2, RATIO2>
    >::type;
    return _Ct(__lhs).count() / _Ct(__rhs).count();
}

// BaseUnit %

template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO,
    typename STORAGE2
>
inline
_CONSTEXPR_
typename __quantity_divide_result<quantity<UNITS, STORAGE1, RATIO>, STORAGE2>::type
operator%
(
    const quantity<STORAGE1, RATIO>& __d,
    const STORAGE2& __s
)
{
    using _Cr = typename std::common_type<STORAGE1, STORAGE2>::type;
    using _Cd = quantity<UNITS, _Cr, RATIO>;
    return _Cd(_Cd(__d).count() % static_cast<_Cr>(__s));
}

template
<
    typename UNITS,
    typename STORAGE1,
    typename RATIO1,
    typename STORAGE2,
    typename RATIO2
>
inline
_CONSTEXPR_
typename std::common_type<quantity<UNITS, STORAGE1, RATIO1>, quantity<UNITS, STORAGE2, RATIO2> >::type
operator%
(
    const quantity<UNITS, STORAGE1, RATIO1>& __lhs,
    const quantity<UNITS, STORAGE2, RATIO2>& __rhs
)
{
    using _Cr = typename std::common_type<STORAGE1, STORAGE2>::type;
    using _Cd = typename std::common_type<quantity<UNITS, STORAGE1, RATIO1>, quantity<UNITS, STORAGE2, RATIO2> >::type;
    return _Cd(static_cast<_Cr>(_Cd(__lhs).count()) % static_cast<_Cr>(_Cd(__rhs).count()));
}

// multiply to derived type
template
<
    typename UNITS,
    typename STORAGE1,
    typename STORAGE2,
    typename RATIO1,
    typename RATIO2
>
using multiply_result = quantity
<
    UNITS,
    typename std::common_type<STORAGE1, STORAGE2>::type,
    typename std::ratio_multiply<RATIO1, RATIO2>
>;

template
<
    typename UNITS,
    typename UNITS1,
    typename STORAGE1,
    typename RATIO1,
    typename UNITS2,
    typename STORAGE2,
    typename RATIO2
>
inline
_CONSTEXPR_
multiply_result<UNITS, STORAGE1, STORAGE2, RATIO1, RATIO2>
multiply
(
    const quantity<UNITS1, STORAGE1, RATIO1>& lhs,
    const quantity<UNITS2, STORAGE2, RATIO2>& rhs
)
{
    using RT = multiply_result<UNITS, STORAGE1, STORAGE2, RATIO1, RATIO2>;

    using CS = typename RT::storage_t;

    return RT{ CS(CS( lhs.count() ) * CS( rhs.count() )) };
}

// divide to derived type
template
<
    typename UNITS,
    typename STORAGE1,
    typename STORAGE2,
    typename RATIO1,
    typename RATIO2
>
using divide_result = quantity
<
    UNITS,
    typename std::common_type<STORAGE1, STORAGE2>::type,
    typename std::ratio_divide<RATIO1, RATIO2>
>;

template
<
    typename UNITS,
    typename UNITS1,
    typename STORAGE1,
    typename RATIO1,
    typename UNITS2,
    typename STORAGE2,
    typename RATIO2
>
inline
_CONSTEXPR_
divide_result<UNITS, STORAGE1, STORAGE2, RATIO1, RATIO2>
divide
(
    const quantity<UNITS1, STORAGE1, RATIO1>& lhs,
    const quantity<UNITS2, STORAGE2, RATIO2>& rhs
)
{
    using RT = divide_result<UNITS, STORAGE1, STORAGE2, RATIO1, RATIO2>;

    using CS = typename RT::storage_t;

    return RT{ CS(CS( lhs.count() ) / CS( rhs.count() )) };
}

// reciprocal to derived type
template
<
    typename UNITS,
    typename STORAGE,
    typename RATIO
>
using reciprocal_result = quantity
<
    UNITS,
    STORAGE,
    std::ratio_divide<std::ratio<1>, RATIO>
>;

template
<
    typename UNITS,
    typename UNITS1,
    typename STORAGE,
    typename RATIO
>
inline
_CONSTEXPR_
reciprocal_result<UNITS, STORAGE, RATIO>
reciprocal
(
    const quantity<UNITS1, STORAGE, RATIO>& input
)
{
    using RT = reciprocal_result<UNITS, STORAGE, RATIO>;

    using CS = typename RT::storage_t;

    return RT{ CS(1 / CS( input.count() )) };
}

} // end of namespace si
