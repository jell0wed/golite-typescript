/*
 * JOOS is Copyright (C) 1997 Laurie Hendren & Michael I. Schwartzbach
 *
 * Reproduction of all or part of this software is permitted for
 * educational or research use on condition that this copyright notice is
 * included in any copy. This software comes with no warranty of any
 * kind. In no event will the authors be liable for any damages resulting from
 * use of this software.
 *
 * email: hendren@cs.mcgill.ca, mis@brics.dk
 */

/* iload x        iload x        iload x
 * ldc 0          ldc 1          ldc 2
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload x        iload x
 *                               dup
 *                               iadd
 */

int simplify_multiplication_right(CODE **c) {
    int x, k;
    if (is_iload(*c, &x)
        && is_ldc_int(next(*c), &k)
        && is_imul(next(next(*c)))) {
        if (k == 0) return replace(c, 3, makeCODEldc_int(0, NULL));
        if (k == 1) return replace(c, 3, makeCODEiload(x, NULL));
        if (k == 2) return replace(c, 3, makeCODEiload(x, makeCODEdup(makeCODEiadd(NULL))));
    }
    return 0;
}

/* ldc 0          ldc 1          ldc 2
 * iload x        iload x        iload x
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload x        iload x
 *                               dup
 *                               iadd
 */

int simplify_multiplication_left(CODE **c) {
    int x, k;
    if (is_ldc_int(*c, &k)
        && is_iload(next(*c), &x)
        && is_imul(next(next(*c)))) {
        if (k == 0) return replace(c, 3, makeCODEldc_int(0, NULL));
        if (k == 1) return replace(c, 3, makeCODEiload(x, NULL));
        if (k == 2) return replace(c, 3, makeCODEiload(x, makeCODEdup(makeCODEiadd(NULL))));
    }
    return 0;
}

/* iload x
 * ldc 0
 * iadd
 * ------>
 * iload x
 */
int simplify_addition_left(CODE **c) {
    int x, k;
    if (is_iload(*c, &x)
        && is_ldc_int(next(*c), &k)
        && is_iadd(next(next(*c))) && k == 0) {
        return replace(c, 3, makeCODEiload(x, NULL));
    }
    return 0;
}

/* ldc 0
 * iload x
 * iadd
 * ------>
 * iload x
 */
int simplify_addition_right(CODE **c) {
    int x, k;
    if (is_ldc_int(*c, &k)
        && is_iload(next(*c), &x)
        && is_iadd(next(next(*c)))
        && k == 0) {
        return replace(c, 3, makeCODEiload(x, NULL));
    }
    return 0;
}

/* iload x
 * ldc 0
 * isub
 * ------>
 * iload x
 */
int simplify_subtract_left(CODE **c) {
    int x, k;
    if (is_iload(*c, &x)
        && is_ldc_int(next(*c), &k)
        && is_isub(next(next(*c)))
        && k == 0) {
        return replace(c, 3, makeCODEiload(x, NULL));
    }
    return 0;
}

/* iload x
 * iload x
 * isub
 * ------>
 * ldc 0
 */
int simplify_self_subtract(CODE **c) {
    int x, k;
    if (is_iload(*c, &x)
        && is_iload(next(*c), &k)
        && is_isub(next(next(*c)))
        && x == k) {
        return replace(c, 3, makeCODEldc_int(0, NULL));
    }
    return 0;
}

/* iload x
 * ldc 1
 * idiv
 * ------>
 * iload x
 */

int simplify_division_right(CODE **c) {
    int x, k;
    if (is_iload(*c, &x)
        && is_ldc_int(next(*c), &k)
        && is_idiv(next(next(*c)))
        && k == 1) {
        return replace(c, 3, makeCODEiload(x, NULL));
    }
    return 0;
}

/* dup
 * astore x
 * pop
 * -------->
 * astore x
 */
int simplify_astore(CODE **c) {
    int x;
    if (is_dup(*c)
        && is_astore(next(*c), &x)
        && is_pop(next(next(*c)))) {
        return replace(c, 3, makeCODEastore(x, NULL));
    }
    return 0;
}

/* iload x
 * ldc k   (0<=k<=127)
 * iadd
 * istore x
 * --------->
 * iinc x k
 */
int positive_increment(CODE **c) {
    int x, y, k;
    if (is_iload(*c, &x)
        && is_ldc_int(next(*c), &k)
        && is_iadd(next(next(*c)))
        && is_istore(next(next(next(*c))), &y)
        && x == y
        && 0 <= k
        && k <= 127) {
        return replace(c, 4, makeCODEiinc(x, k, NULL));
    }
    return 0;
}

/* iload x
 * ldc k   (-128<=k<0)
 * iadd
 * istore x
 * --------->
 * iinc x k
 */
int negative_increment(CODE **c) {
    int x, y, k;
    if (is_iload(*c, &x)
        && is_ldc_int(next(*c), &k)
        && is_iadd(next(next(*c)))
        && is_istore(next(next(next(*c))), &y)
        && x == y
        && -128 <= k
        && k < 0) {
        return replace(c, 4, makeCODEiinc(x, k, NULL));
    }
    return 0;
}

/* goto L1
 * ...
 * L1:
 * goto L2
 * ...
 * L2:
 * --------->
 * goto L2
 * ...
 * L1:    (reference count reduced by 1)
 * goto L2
 * ...
 * L2:    (reference count increased by 1)  
 */
int simplify_goto_goto(CODE **c) {
    int l1, l2;
    if (is_goto(*c, &l1)
        && is_goto(next(destination(l1)), &l2)
        && l1 > l2) {
        droplabel(l1);
        copylabel(l2);
        return replace(c, 1, makeCODEgoto(l2, NULL));
    }
    return 0;
}

/* iload x
 * ldc 0
 * if_icmpeq
 * --------->
 * iload x
 * if_eq
 */
int branch_eq_left(CODE **c) {
    int x1, x2, x3;
    if (is_iload(*c, &x1)
        && is_ldc_int(next(*c), &x2)
        && is_if_icmpeq(next(next(*c)), &x3)
        && x2 == 0) {
        return replace(c, 3, makeCODEiload(x1, makeCODEifeq(x3, NULL)));
    }
    return 0;
}

/* ldc 0
 * iload x
 * if_icmpeq
 * --------->
 * iload x
 * if_eq
 */
int branch_eq_right(CODE **c) {
    int x1, x2, x3;
    if (is_ldc_int(*c, &x1)
        && is_iload(next(*c), &x2)
        && is_if_icmpeq(next(next(*c)), &x3)
        && x1 == 0) {
        return replace(c, 3, makeCODEiload(x2, makeCODEifeq(x3, NULL)));
    }
    return 0;
}

/* iload x
 * ldc 0
 * if_icmpne
 * --------->
 * iload x
 * if_ne
 */
int branch_ne_left(CODE **c) {
    int x1, x2, x3;
    if (is_iload(*c, &x1)
        && is_ldc_int(next(*c), &x2)
        && is_if_icmpne(next(next(*c)), &x3)
        && x2 == 0) {
        return replace(c, 3, makeCODEiload(x1, makeCODEifne(x3, NULL)));
    }
    return 0;
}

/* ldc 0
 * iload x
 * if_icmpne
 * --------->
 * iload x
 * if_ne
 */
int branch_ne_right(CODE **c) {
    int x1, x2, x3;
    if (is_ldc_int(*c, &x1)
        && is_iload(next(*c), &x2)
        && is_if_icmpne(next(next(*c)), &x3)
        && x1 == 0) {
        return replace(c, 3, makeCODEiload(x2, makeCODEifne(x3, NULL)));
    }
    return 0;
}

/* aload x
 * aconst_null
 * if_acmpeq
 * --------->
 * aload x
 * ifnull
 */
int branch_is_null_left(CODE **c) {
    int x1, x2;
    if (is_aload(*c, &x1)
        && is_aconst_null(next(*c))
        && is_if_acmpeq(next(next(*c)), &x2)) {
        return replace(c, 3, makeCODEaload(x1, makeCODEifnull(x2, NULL)));
    }
    return 0;
}

/* aconst_null
 * aload x
 * if_acmpeq
 * --------->
 * aload x
 * ifnull
 */
int branch_is_null_right(CODE **c) {
    int x1, x2;
    if (is_aconst_null(*c)
        && is_aload(next(*c), &x1)
        && is_if_acmpeq(next(next(*c)), &x2)) {
        return replace(c, 3, makeCODEaload(x1, makeCODEifnull(x2, NULL)));
    }
    return 0;
}

/* aload x
 * aconst_null
 * if_acmpne
 * --------->
 * aload x
 * ifnonnull
 */
int branch_is_not_null_left(CODE **c) {
    int x1, x2;
    if (is_aload(*c, &x1)
        && is_aconst_null(next(*c))
        && is_if_acmpne(next(next(*c)), &x2)) {
        return replace(c, 3, makeCODEaload(x1, makeCODEifnonnull(x2, NULL)));
    }
    return 0;
}

/* aconst_null
 * aload x
 * if_acmpne
 * --------->
 * aload x
 * ifnonnull
 */
int branch_is_not_null_right(CODE **c) {
    int x1, x2;
    if (is_aconst_null(*c)
        && is_aload(next(*c), &x1)
        && is_if_acmpne(next(next(*c)), &x2)) {
        return replace(c, 3, makeCODEaload(x1, makeCODEifnonnull(x2, NULL)));
    }
    return 0;
}

/* iload x
 * istore x
 * --------->
 */
int self_store(CODE **c) {
    int x1, x2;
    if (is_iload(*c, &x1)
        && is_istore(next(*c), &x2)
        && x1 == x2) {
        return replace(c, 2, NULL);
    }
    return 0;
}

/* ldc x
 * istore y
 * ldc x
 * istore y
 * --------->
 * ldc x
 * istore y
 */
int redundant_ldc_store(CODE **c) {
    int x1, x2, x3, x4;
    if (is_ldc_int(*c, &x1)
        && is_istore(next(*c), &x2)
        && is_ldc_int(next(next(*c)), &x3)
        && is_istore(next(next(next(*c))), &x4)
        && x1 == x3
        && x2 == x4) {
        return replace(c, 4, makeCODEldc_int(x1, makeCODEistore(x2, NULL)));
    }
    return 0;
}

/**
 * if_acmpeq true_2
 * iconst_0
 * goto stop_3
 * true_2:
 * iconst_1
 * stop_3:
 * ifeq else_0
 * ------------>
 * if_acmpne else_0
 */
int simplify_if_acmpeq_branch(CODE **c) {
    int x1, x2, x3, x4, x5, x6, x7;
    if (is_ldc_int(next(*c), &x2)
        && x2 == 0
        && is_goto(next(next(*c)), &x3)
        && is_label(next(next(next(*c))), &x4)
        && is_ldc_int(next(next(next(next(*c)))), &x5)
        && x5 == 1
        && is_label(next(next(next(next(next(*c))))), &x6)
        && x3 == x6
        && is_ifeq(next(next(next(next(next(next(*c)))))), &x7)) {
        if(is_if_acmpeq(*c, &x1)) return replace(c, 7, makeCODEif_acmpne(x7, NULL));
        if(is_if_acmpne(*c, &x1)) return replace(c, 7, makeCODEif_acmpeq(x7, NULL));
        if(is_if_icmpeq(*c, &x1)) return replace(c, 7, makeCODEif_icmpne(x7, NULL));
        if(is_if_icmpne(*c, &x1)) return replace(c, 7, makeCODEif_icmpeq(x7, NULL));
        if(is_if_icmplt(*c, &x1)) return replace(c, 7, makeCODEif_icmpge(x7, NULL));
        if(is_if_icmple(*c, &x1)) return replace(c, 7, makeCODEif_icmpgt(x7, NULL));
        if(is_if_icmpgt(*c, &x1)) return replace(c, 7, makeCODEif_icmple(x7, NULL));
        if(is_if_icmpge(*c, &x1)) return replace(c, 7, makeCODEif_icmplt(x7, NULL));
        if(is_if_icmpge(*c, &x1)) return replace(c, 7, makeCODEif_icmplt(x7, NULL));
    }
    return 0;
}

void init_patterns(void) {
    /*Given optimization*/
    ADD_PATTERN(simplify_multiplication_right);
    ADD_PATTERN(simplify_astore);
    ADD_PATTERN(positive_increment);
    ADD_PATTERN(simplify_goto_goto);

    /*Simple optimization*/
    ADD_PATTERN(negative_increment);
    ADD_PATTERN(branch_eq_left);
    ADD_PATTERN(branch_eq_right);
    ADD_PATTERN(branch_ne_left);
    ADD_PATTERN(branch_ne_right);
    ADD_PATTERN(branch_is_null_left);
    ADD_PATTERN(branch_is_null_right);
    ADD_PATTERN(branch_is_not_null_left);
    ADD_PATTERN(branch_is_not_null_right);
    ADD_PATTERN(simplify_division_right);
    ADD_PATTERN(simplify_multiplication_left);
    ADD_PATTERN(simplify_addition_left);
    ADD_PATTERN(simplify_addition_right);
    ADD_PATTERN(simplify_subtract_left);
    ADD_PATTERN(simplify_self_subtract);
    ADD_PATTERN(redundant_ldc_store);
    ADD_PATTERN(self_store);

    /*Medium optimization*/
    ADD_PATTERN(simplify_if_acmpeq_branch);
}