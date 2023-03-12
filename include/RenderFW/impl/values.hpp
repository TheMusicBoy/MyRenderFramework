#ifndef _TMB_VALUES_HPP_
#define _TMB_VALUES_HPP_

#if __cplusplus >= 201703L
    #define TMB_NODISCARD [[nodiscard]]
#else
    #define TMB_NODISCARD
#endif

#if __cplusplus >= 201100L
    #define TMB_NOEXCEPT noexcept
#else
    #define TMB_NOEXCEPT
#endif

#if __cplusplus >= 201100L
    #define TMB_CONSTEXPR constexpr
#else
    #define TMB_CONSTEXPR
#endif

#endif