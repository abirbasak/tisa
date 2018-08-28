#ifndef AZUIK_MACRO_HPP
#define AZUIK_MACRO_HPP

#define AZUIK_PP_CAT(x, y) AZUIK_PP_cat_1(x, y)
#define AZUIK_PP_cat_1(x, y) AZUIK_PP_cat_2(x, y)
#define AZUIK_PP_cat_2(x, y) x##y

#define AZUIK_PP_TO_STRING(x) AZUIK_PP_to_string_1(x)
#define AZUIK_PP_to_string_1(x) #x
#define AZUIK_PP_TO_CHAR(x) AZUIK_PP_to_char_1(x)
#define AZUIK_PP_to_char_1(x) 0 [#x]

#define AZUIK_PP_EMPTY()
#define AZUIK_PP_COMMA() ,
#define AZUIK_PP_LPAREN() (
#define AZUIK_PP_RPAREN() )
#define AZUIK_PP_COMMA_IF(cond) AZUIK_PP_IF(cond, AZUIK_PP_COMMA, AZUIK_PP_EMPTY)()
#define AZUIK_PP_LPAREN_IF(cond) AZUIK_PP_IF(cond, AZUIK_PP_LPAREN, AZUIK_PP_EMPTY)()
#define AZUIK_PP_RPAREN_IF(cond) AZUIK_PP_IF(cond, AZUIK_PP_RPAREN, AZUIK_PP_EMPTY)()

#define AZUIK_PP_SEQ(_1, _2, _3, _4_5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define AZUIK_PP_ARG_COUNT(...)                                                                    \
    AZUIK_SEQ(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define AZUIK_PP_FOREACH(MACRO, SEP, ...)                                                          \
    AZUIK_PP_CAT(AZUIK_PP_foreach_, AZUIK_PP_ARG_COUNT(__VA_ARGS__))(MACRO, SEP, __VA_ARGS__)

//bool
#define AZUIK_PP_BOOL(cond) AZUIK_PP_bool_1(cond)
#define AZUIK_PP_bool_1(cond) AZUIK_PP_bool_internal_##cond
//if
#define AZUIK_PP_IFF(cond, true_cond, false_cond) AZUIK_PP_IFF_I(cond, true_cond, false_cond)
#define AZUIK_PP_IFF_I(cond, true_cond, false_cond)                                                \
    AZUIK_PP_IFF_internal_##cond(true_cond, false_cond)
#define AZUIK_PP_IFF_internal_0(true_cond, false_cond) false_cond
#define AZUIK_PP_IFF_internal_1(true_cond, false_cond) true_cond
#define AZUIK_PP_IF(cond, true_cond, false_cond) AZUIK_PP_IFF_I(cond, true_cond, false_cond)

//AZUIK_PP_foreach
#define AZUIK_PP_foreach_1(MACRO, SEP, X0) MACRO(X0)
#define AZUIK_PP_foreach_2(MACRO, SEP, X0, X1) MACRO(X0) SEP() MACRO(X1)
#define AZUIK_PP_foreach_3(MACRO, SEP, X0, X1, X2) MACRO(X0) SEP() MACRO(X1) SEP() MACRO(X2)
#define AZUIK_PP_foreach_4(MACRO, SEP, X0, X1, X2, X3)                                             \
    MACRO(X0) SEP() MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3)
#define AZUIK_PP_foreach_5(MACRO, SEP, X0, X1, X2, X3, X4)                                         \
    MACRO(X0) SEP() MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4)
#define AZUIK_PP_foreach_6(MACRO, SEP, X0, X1, X2, X3, X4, X5)                                     \
    MACRO(X0) SEP() MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5)
#define AZUIK_PP_foreach_7(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6)                                 \
    MACRO(X0)                                                                                      \
    SEP() MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)
#define AZUIK_PP_foreach_8(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7)                             \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7)
#define AZUIK_PP_foreach_9(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8)                         \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8)
#define AZUIK_PP_foreach_10(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9)                    \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8) SEP() MACRO(X9)
#define AZUIK_PP_foreach_11(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10)               \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8) SEP() MACRO(X9) SEP() MACRO(X10)
#define AZUIK_PP_foreach_12(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11)          \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8) SEP() MACRO(X9) SEP() MACRO(X10) SEP() MACRO(X11)
#define AZUIK_PP_foreach_13(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12)     \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8) SEP() MACRO(X9) SEP() MACRO(X10) SEP() MACRO(X11) SEP()    \
            MACRO(X12)
#define AZUIK_PP_foreach_14(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12,     \
                            X13)                                                                   \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8) SEP() MACRO(X9) SEP() MACRO(X10) SEP() MACRO(X11) SEP()    \
            MACRO(X12) SEP() MACRO(X13)
#define AZUIK_PP_foreach_15(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12,     \
                            X13, X14)                                                              \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8) SEP() MACRO(X9) SEP() MACRO(X10) SEP() MACRO(X11) SEP()    \
            MACRO(X12) SEP() MACRO(X13) SEP() MACRO(X14)
#define AZUIK_PP_foreach_16(MACRO, SEP, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12,     \
                            X13, X14, X15)                                                         \
    MACRO(X0)                                                                                      \
    SEP()                                                                                          \
    MACRO(X1) SEP() MACRO(X2) SEP() MACRO(X3) SEP() MACRO(X4) SEP() MACRO(X5) SEP() MACRO(X6)      \
        SEP() MACRO(X7) SEP() MACRO(X8) SEP() MACRO(X9) SEP() MACRO(X10) SEP() MACRO(X11) SEP()    \
            MACRO(X12) SEP() MACRO(X13) SEP() MACRO(X14) SEP() MACRO(X15)

#endif
