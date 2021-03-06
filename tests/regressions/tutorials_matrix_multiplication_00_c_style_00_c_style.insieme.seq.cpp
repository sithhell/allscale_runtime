/**
 * ------------------------ Auto-generated Code ------------------------ 
 *           This code was generated by the Insieme Compiler 
 * --------------------------------------------------------------------- 
 */
#include <allscale/runtime.hpp>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
#define INS_INIT(...) __VA_ARGS__
#else
#define INS_INIT(...) (__VA_ARGS__)
#endif
#ifdef __cplusplus
#include <new>
#define INS_INPLACE_INIT(Loc,Type) new(Loc) Type
#else
#define INS_INPLACE_INIT(Loc,Type) *(Loc) = (Type)
#endif
#ifdef __cplusplus
				/** Workaround for libstdc++/libc bug.
				 *  There's an inconsistency between libstdc++ and libc regarding whether
				 *  ::gets is declared or not, which is only evident when using certain
				 *  compilers and language settings
				 *  (tested positively with clang 3.9 --std=c++14 and libc 2.17).
				 */
				#include <initializer_list>  // force libstdc++ to include its config
				#undef _GLIBCXX_HAVE_GETS    // correct broken config
#endif

/* ------- Program Code --------- */

struct __wi_main_name {
    static const char* name() { return "__wi_main"; }
};

struct __wi_main_variant_0;
typedef struct __wi_main_variant_0 __wi_main_variant_0;

struct __wi_main_variant_1;
typedef struct __wi_main_variant_1 __wi_main_variant_1;

using __wi_main_work = allscale::work_item_description<int32_t, __wi_main_name, allscale::no_serialization, __wi_main_variant_0, __wi_main_variant_1 >;

/* ------- Function Definitions --------- */
int32_t main() {
    return allscale::runtime::main_wrapper<__wi_main_work >();
}

ALLSCALE_REGISTER_TREETURE_TYPE(int32_t)
/* ------- Function Definitions --------- */
double** new_arr_fun(uint64_t var_0) {
    uint64_t var_1 = var_0;
    return new double*[var_1];
}
/* ------- Function Definitions --------- */
double* allscale_fun_9(uint64_t var_0) {
    uint64_t var_1 = var_0;
    return new double[var_1];
}
/* ------- Global Variable Declaration ---------- */
const int32_t N = {100};

/* ------- Function Definitions --------- */
double** IMP_createMatrix() {
    double** var_0 = new_arr_fun((uint64_t)((uint64_t)N));
    double* var_1 = allscale_fun_9((uint64_t)((uint64_t)(N * N)));
    {
        int32_t var_2 = 0;
        while (var_2 < N) {
            var_0[var_2] = &var_1[var_2 * N];
            var_2++;
        };
    };
    return var_0;
}
/* ------- Function Definitions --------- */
double** IMP_id() {
    double** var_0 = IMP_createMatrix();
    {
        int32_t var_1 = 0;
        while (var_1 < N) {
            {
                int32_t var_2 = 0;
                while (var_2 < N) {
                    var_0[var_1][var_2] = (double)(var_1 == var_2?1:0);
                    var_2++;
                };
            };
            var_1++;
        };
    };
    return var_0;
}
/* ------- Function Definitions --------- */
double** IMP_mm(double** var_0, double** var_1) {
    double** var_2 = IMP_createMatrix();
    {
        int32_t var_3 = 0;
        while (var_3 < N) {
            {
                int32_t var_4 = 0;
                while (var_4 < N) {
                    {
                        var_2[var_3][var_4] = (double)0;
                        {
                            int32_t var_5 = 0;
                            while (var_5 < N) {
                                var_2[var_3][var_4]+=var_0[var_3][var_5] * var_1[var_5][var_4];
                                ++var_5;
                            };
                        };
                    };
                    ++var_4;
                };
            };
            ++var_3;
        };
    };
    return var_2;
}
/* ------- Function Definitions --------- */
bool IMP_equal(double** var_0, double** var_1) {
    {
        int32_t var_2 = 0;
        while (var_2 < N) {
            {
                int32_t var_3 = 0;
                while (var_3 < N) {
                    if (var_0[var_2][var_3] != var_1[var_2][var_3]) {
                        return (bool)false;
                    };
                    var_3++;
                };
            };
            var_2++;
        };
    };
    return (bool)true;
}
/* ------- Function Definitions --------- */
void IMP_freeMatrix(double** var_0) {
    delete[] var_0[0];
    delete[] var_0;
}
/* ------- Function Definitions --------- */
int32_t IMP_main() {
    double** var_0 = IMP_id();
    double** var_1 = IMP_id();
    double** var_2 = IMP_mm(var_0, var_1);
    bool var_3 = IMP_equal(var_0, var_2);
    IMP_freeMatrix(var_0);
    IMP_freeMatrix(var_1);
    IMP_freeMatrix(var_2);
    return var_3?0:1;
}
/* ------- Function Definitions --------- */
allscale::treeture<int32_t > allscale_fun_1(hpx::util::tuple< > const& var_0) {
    return allscale::treeture<int32_t >(IMP_main());
}
struct __wi_main_variant_0 {
    static allscale::treeture<int32_t > execute(hpx::util::tuple< > const& var_0);
    static constexpr bool valid = true;
};

allscale::treeture<int32_t > __wi_main_variant_0::execute(hpx::util::tuple< > const& var_0) {
    return allscale_fun_1(var_0);
}
struct __wi_main_variant_1 {
    static allscale::treeture<int32_t > execute(hpx::util::tuple< > const& var_0);
    static constexpr bool valid = true;
};

allscale::treeture<int32_t > __wi_main_variant_1::execute(hpx::util::tuple< > const& var_0) {
    return allscale_fun_1(var_0);
}

