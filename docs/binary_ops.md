# operator+,-,*,/,%([si::units_t](units_t.md))

```c++
template<typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr auto operator+ // (1)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)-> [std::common_type_t](common_type.md)<decltype(aLHS), decltype(aRHS)>;

template<typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr auto operator- // (2)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)-> [std::common_type_t](common_type.md)<decltype(aLHS), decltype(aRHS)>;

template<typename QuantityT1, typename ValueT1, typename IntervalT1, typename QuantityT2, typename ValueT2, typename IntervalT2>
constexpr auto operator* // (3)
(
    units_t<ValueT1, IntervalT1, QuantityT1> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT2> aRHS
)-> units_t<std::common_type_t<ValueT1, ValueT2>, /*see note 3*/>;

template <typename ValueT1, typename IntervalT, typename QuantityT, typename ValueT2>
constexpr auto operator* // (4)
(
    units_t<ValueT1, IntervalT, QuantityT> aUnits,
    ValueT2 aScalar
)-> units_t<std::common_type_t<ValueT1, ValueT2>, IntervalT, QuantityT>;

template <typename ValueT1, typename IntervalT, typename QuantityT, typename ValueT2>
constexpr auto operator* // (5)
(
    ValueT2 aScalar,
    units_t<ValueT1, IntervalT, QuantityT> aUnits
)-> units_t<std::common_type_t<ValueT1, ValueT2>, IntervalT, QuantityT>;

template <typename ValueT1, typename IntervalT, typename QuantityT, typename ValueT2>
constexpr auto operator/ // (6)
(
    units_t<ValueT1, IntervalT, QuantityT> aUnits,
    ValueT2 aScalar
)-> units_t<std::common_type_t<ValueT1, ValueT2>, IntervalT, QuantityT>;

template <typename ValueT1, typename IntervalT, typename QuantityT, typename ValueT2>
constexpr auto operator/ // (7)
(
    ValueT2 aScalar,
    units_t<ValueT1, IntervalT, QuantityT> aUnits
)-> units_t<std::common_type_t<ValueT1, ValueT2>, /*see note 7*/>;

template<typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr auto operator/ // (8)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)-> [std::common_type_t](common_type.md)<decltype(aLHS), decltype(aRHS)>;

template<typename QuantityT1, typename ValueT1, typename IntervalT1, typename QuantityT2, typename ValueT2, typename IntervalT2>
constexpr auto operator/ // (9)
(
    units_t<ValueT1, IntervalT1, QuantityT1> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT2> aRHS
)-> units_t<std::common_type_t<ValueT1, ValueT2>, /*see note 9*/>;

template <typename ValueT1, typename IntervalT, typename QuantityT, typename ValueT2>
constexpr auto operator% // (10)
(
    units_t<ValueT1, IntervalT, QuantityT> aUnits,
    ValueT2 aScalar
)-> units_t<std::common_type_t<ValueT1, ValueT2>, IntervalT, QuantityT>;

template<typename QuantityT, typename ValueT1, typename IntervalT1, typename ValueT2, typename IntervalT2>
constexpr auto operator% // (11)
(
    units_t<ValueT1, IntervalT1, QuantityT> aLHS,
    units_t<ValueT2, IntervalT2, QuantityT> aRHS
)-> [std::common_type_t](common_type.md)<decltype(aLHS), decltype(aRHS)>;
```

Performs basic arithmetic operations between two `units_t`s or between a `units_t` and a unitless value.
1. Converts `aRHS` and `aLHS` to their common type and creates a `units_t` whose value is the sum of their values after conversion.
2. Converts `aRHS` and `aLHS` to their common type and creates a `units_t` whose value is `aRhs.value()` subtracted from `aLHS.value()` after conversion.
3. Converts `aRHS` and `aLHS` to a new `units_t` type having `interval_t` equal to `std::ratio_multiply<IntervalT1, IntervalT2>` and `quantity_t` equal to `si::multiply_quantity<QuantityT1, QuantityT2>` and whose value is the product of the values of `aRHS` and `aLHS` after conversion.
4. Converts the `units_t` aUnits to one whose `value_t` is the common type of `ValueT1` and `ValueT2`, and multiples the value after conversion by `aScalar`.
5. Same as 4.
6. Converts `aUnits` to one whose `value_t` is the common type of `ValueT1` and `ValueT2`, and divides the value after conversion by aScalar.
7. Converts `aUnits` to one whose `value_t` is the common type of `ValueT1` and `ValueT2`, `interval_t` is equal to `std::ratio_divide<std::ratio<1>, IntervalT>` and `quantity_t` is equal to `si::reciprocal_quantity<QuantityT>`, and divides aScalar by the value after conversion.
8. Converts `aRHS` and `aLHS` to their common type and creates a `units_t` whose value is `aLHS.value()` divided by `aRhs.value()` after conversion.
9. Returns a `units_t` whose `value_t` is the common type of `ValueT1` and `ValueT2`, `interval_t` is equal to `std::ratio_divide<IntervalT1, IntervalT2>` and `quantity_t` is equal to `si::divide_quantity<QuantityT2, QuantityT2>` and whose value is `aLHS.value()` divided by `aRhs.value()`.
10. Converts `aUnits` to one whose `value_t` is the common type of `ValueT1` and `ValueT2`, and creates a `units_t` whose value is the remainder of the division of the value, after conversion, by `aScalar`.
11. Converts `aRHS` and `aLHS` to their common type and creates a `units_t` whose value is the remainder of the value after conversion.

## Parameters
aLHS - `units_t` on the left-hand side of the operator
aRHS - `units_t` on the right-hand side of the operator
aUnits - the `units_t` argument for mixed-argument operators
aScalar - the unitless argument for mixed-argument operators

## Return value
Assuming CU is the return type and `CT<A,B> = std::common_type_t<A,B>`, then:
1. `CU{CU{aLHS}.value() + CU{aRHS}.value()}`
1. `CU{CU{aLHS}.value() - CU{aRHS}.value()}`
1. `CU{static_cast<CU::value_t>(aLHS.value()) * static_cast<CU::value_t>(aRHS.value())}`
