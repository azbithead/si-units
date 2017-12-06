# std::common_type([si::units_t](units_t.md))

`template<typename QUANTITY, typename VALUE1, typename INTERVAL1, typename VALUE2, typename INTERVAL2>`<br>`struct std::common_type<si::units_t<VALUE1, INTERVAL1, QUANTITY>, si::units_t<VALUE2, INTERVAL2, QUANTITY>>`<br>`{`<br>`    using type = si::units_t`<br>`    <`<br>`        std::common_type_t<VALUE1, VALUE2>,`<br>`        /*see note*/,`<br>`        QUANTITY`<br>`    >;`<br>`};`

Exposes the type named `type`, which is the common type of two `units_t`s.

## Note
The interval of the resulting `units_t` is the greatest common divisor of `INTERVAL1` and `INTERVAL2`.

## See also
[`std::common_type`](http://en.cppreference.com/w/cpp/types/common_type)
