// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

namespace std
{

template<class A, class B>
struct hash<std::pair<A, B>>
{
    using argument_type = std::pair<A, B>;
    using result_type = std::size_t;

    ///
    /// \brief Specialisation of std to hash
    /// \return Result of the hash
    ///
    /// \param p Pair to hash
    ///

    result_type operator()(argument_type const& p) const
    {
        result_type seed {};
        Nz::HashCombine(seed, p.first);
        Nz::HashCombine(seed, p.second);
        return seed;
    }
};

}
