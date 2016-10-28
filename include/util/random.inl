// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class Generator>
typename RandomNumber<Generator>::result_type RandomNumber<Generator>::operator()()
{
    return instance()();
}

template<class Generator>
typename RandomNumber<Generator>::result_type RandomNumber<Generator>::min()
{
    return instance().min();
}

template<class Generator>
typename RandomNumber<Generator>::result_type RandomNumber<Generator>::max()
{
    return instance().max();
}

template<class Generator>
Generator& RandomNumber<Generator>::instance()
{
    static Generator gen;
    return gen;
}
