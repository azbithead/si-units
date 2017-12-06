# [si::units_t](../units_t.md)::operator+()\[unary\], si::units_t::operator-()\[unary\]

Function | Description
---------|----------
`constexpr units_t operator+() const;` | (1)
`constexpr units_t operator-() const;` | (2)

Implements unary plus and unary minus for `units_t`.
If value_ is a member variable holding the number of intervals in a `units_t` object, and D is the return type,
1. Equivalent to `return D(*this);`
2. Equivalent to `return D(-value_);`

## Parameters
(none)

## Return value
1. a copy of this `units_t` object
2. a copy of this `units_t` object, with the number of intervals negated
