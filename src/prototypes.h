// abs.cpp
void eval_abs(void);
void absval(void);
void absval_tensor(void);
void test_abs(void);

// add.cpp
void eval_add(void);
void add_terms(int n);
int cmp_terms(const void *q1, const void *q2);
int combine_terms(U **s, int n);
void push_terms(U *p);
void add();
void add_all(int k);
void subtract(void);

// adj.cpp
void eval_adj(void);
void adj(void);
void test_adj(void);

// alloc.cpp
U * alloc(void);
U * alloc_tensor(int nelem);
void gc(void);
void untag(U *p);
void alloc_mem(void);
void print_mem_info(void);

// append.cpp
void append(void);

// arccos.cpp
void eval_arccos(void);
void arccos(void);
void test_arccos(void);

// arccosh.cpp
void eval_arccosh(void);
void arccosh(void);
void test_arccosh(void);

// arcsin.cpp
void eval_arcsin(void);
void arcsin(void);
void test_arcsin(void);

// arcsinh.cpp
void eval_arcsinh(void);
void arcsinh(void);
void test_arcsinh(void);

// arctan.cpp
void eval_arctan(void);
void arctan(void);
void test_arctan(void);

// arctanh.cpp
void eval_arctanh(void);
void arctanh(void);
void test_arctanh(void);

// arg.cpp
void eval_arg(void);
void arg(void);
void yyarg(void);
void test_arg(void);

// atomize.cpp
void eval_atomize(void);
void atomize(void);

// bake.cpp
void bake(void);
void polyform(void);
void bake_poly();
void bake_poly_term(int k);
void test_bake(void);

// besselj.cpp
void eval_besselj(void);
void besselj(void);
void yybesselj(void);
void test_besselj(void);

// bessely.cpp
void eval_bessely(void);
void bessely(void);
void yybessely(void);
void test_bessely(void);

// bignum.cpp
unsigned int * mnew(int n);
void mfree(unsigned int *p);
unsigned int * mint(int n);
unsigned int * mcopy(unsigned int *a);
int ge(unsigned int *a, unsigned int *b, int len);
void add_numbers(void);
void subtract_numbers(void);
void multiply_numbers(void);
void divide_numbers(void);
void invert_number(void);
int compare_rationals(U *a, U *b);
int compare_numbers(U *a, U *b);
void negate_number(void);
void bignum_truncate(void);
void mp_numerator(void);
void mp_denominator(void);
void bignum_power_number(int expo);
double convert_bignum_to_double(unsigned int *p);
double convert_rational_to_double(U *p);
void push_integer(int n);
void push_double(double d);
void push_rational(int a, int b);
int pop_integer(void);
void print_double(U *p, int flag);
void bignum_scan_integer(char *s);
void bignum_scan_float(char *s);
void print_number(U *p);
void gcd_numbers(void);
double pop_double(void);
void bignum_float(void);
void bignum_factorial(int n);
void mp_set_bit(unsigned int *x, unsigned int k);
void mp_clr_bit(unsigned int *x, unsigned int k);
void mshiftright(unsigned int *a);

// binomial.cpp
void eval_binomial(void);
void binomial(void);
void test_binomial(void);

// ceiling.cpp
void eval_ceiling(void);
void ceiling(void);
void yyceiling(void);
void test_ceiling(void);

// choose.cpp
void eval_choose(void);
void choose(void);
int choose_check_args(void);
void test_choose(void);

// circexp.cpp
void eval_circexp(void);
void circexp(void);
void test_circexp(void);

// clear.cpp
void eval_clear(void);
void clear(void);

// clock.cpp
void eval_clock(void);
void clockform(void);
void test_clock(void);

// cmdisplay.cpp
void eval_display(void);
void cmdisplay(void);
void test_cmdisplay(void);

// coeff.cpp
void eval_coeff(void);
int coeff(void);
void test_coeff(void);

// cofactor.cpp
void eval_cofactor(void);
void cofactor(U *p, int n, int row, int col);
void test_cofactor(void);

// condense.cpp
void eval_condense(void);
void Condense(void);
void yycondense(void);
void test_condense(void);

// conj.cpp
void eval_conj(void);
void conjugate(void);

// cons.cpp
void cons(void);

// contract.cpp
void eval_contract(void);
void contract(void);
void yycontract(void);
void test_contract(void);

// cos.cpp
void eval_cos(void);
void cosine(void);
void cosine_of_angle_sum(void);
void cosine_of_angle(void);
void test_cos(void);

// cosh.cpp
void eval_cosh(void);
void ycosh(void);
void yycosh(void);
void test_cosh(void);

// data.cpp

// decomp.cpp
void eval_decomp(void);
void decomp(void);
void decomp_sum(void);
void decomp_product(void);

// define.cpp
void define_user_function(void);

// defint.cpp
void eval_defint(void);
void test_defint(void);

// degree.cpp
void eval_degree(void);
void degree(void);
void yydegree(U *p);

// denominator.cpp
void eval_denominator(void);
void denominator(void);
void test_denominator(void);

// derivative.cpp
void eval_derivative(void);
void derivative(void);
void d_scalar_scalar(void);
void d_scalar_scalar_1(void);
void dsum(void);
void dproduct(void);
void dpower(void);
void dlog(void);
void dd(void);
void dfunction(void);
void dsin(void);
void dcos(void);
void dtan(void);
void darcsin(void);
void darccos(void);
void darctan(void);
void dsinh(void);
void dcosh(void);
void dtanh(void);
void darcsinh(void);
void darccosh(void);
void darctanh(void);
void dabs(void);
void dsgn(void);
void dhermite(void);
void derf(void);
void derfc(void);
void dbesselj0(void);
void dbesseljn(void);
void dbessely0(void);
void dbesselyn(void);
void derivative_of_integral(void);
void test_derivative(void);

// det.cpp
void det(void);
void determinant(int n);
void detg(void);
void yydetg(void);
void lu_decomp(int n);

// dirac.cpp
void eval_dirac(void);
void dirac(void);
void test_dirac(void);

// display.cpp
void display(U *p);
void fixup_fraction(int x, int k1, int k2);
void displaychar(int c);
void emit_number(U *p, int emit_sign);
char * getdisplaystr(void);
void test_display(void);

// distill.cpp
void distill(void);
void distill_sum(void);
void distill_product(void);

// divisors.cpp
void divisors(void);
void divisors_onstack(void);
void test_divisors(void);

// dpow.cpp
void dpow(void);

// draw.cpp
void eval_draw(void);
void draw_main(void);
void check_for_parametric_draw(void);
void create_point_set(void);
void new_point(double t);
void get_xy(double t);
void eval_f(double t);
void fill(int i, int k, int level);
void setup_trange(void);
void setup_trange_f(void);
void setup_xrange(void);
void setup_xrange_f(void);
void setup_yrange(void);
void setup_yrange_f(void);
void emit_graph(void);

// dsolve.cpp
void dsolve(void);
 int distilly();

// eigen.cpp
void eval_eigen(void);
void eval_eigenval(void);
void eval_eigenvec(void);
void test_eigen(void);

// erf.cpp
void eval_erf(void);
void yerf(void);
void test_erf(void);

// erfc.cpp
void eval_erfc(void);
void yerfc(void);
double erfc(double x);
void test_erfc(void);

// eval.cpp
void eval(void);
void eval_sym(void);
void eval_cons(void);
void eval_binding(void);
void eval_check(void);
void eval_det(void);
void eval_dim(void);
void eval_divisors(void);
void eval_do(void);
void eval_dsolve(void);
void eval_eval(void);
void eval_exp(void);
void eval_factorial(void);
void eval_factorpoly(void);
void eval_hermite(void);
void eval_hilbert(void);
void eval_index(void);
void eval_inv(void);
void eval_invg(void);
void eval_isinteger(void);
void eval_multiply(void);
void eval_number(void);
void eval_operator(void);
void eval_print(void);
void eval_quote(void);
void eval_rank(void);
void setq_indexed(void);
void eval_setq(void);
void eval_sqrt(void);
void eval_stop(void);
void eval_subst(void);
void eval_unit(void);
void eval_noexpand(void);
void eval_predicate(void);

// expand.cpp
void eval_expand(void);
void expand(void);
void expand_tensor(void);
void remove_negative_exponents(void);
void expand_get_C(void);
void expand_get_CF(void);
void trivial_divide(void);
void expand_get_B(void);
void expand_get_A(void);
void expand_get_AF(void);
void test_expand(void);

// expcos.cpp
void eval_expcos(void);
void expcos(void);
void test_expcos(void);

// expsin.cpp
void eval_expsin(void);
void expsin(void);
void test_expsin(void);

// factor.cpp
void eval_factor(void);
void factor_again(void);
void factor_term(void);
void factor(void);
void factor_small_number(void);
void test_factor_number(void);

// factorial.cpp
void factorial(void);
void simplifyfactorials(void);
void sfac_product(void);
void sfac_product_f(U **s, int a, int b);

// factorpoly.cpp
void factorpoly(void);
void yyfactorpoly(void);
void test_factorpoly(void);

// factors.cpp
int factors(U *p);
void push_term_factors(U *p);

// filter.cpp
void eval_filter(void);
void filter(void);
void filter_main(void);
void filter_sum(void);
void filter_tensor(void);

// find.cpp
int find(U *p, U *q);

// float.cpp
void eval_float(void);
void yyfloat(void);
void test_float(void);

// floor.cpp
void eval_floor(void);
void yfloor(void);
void yyfloor(void);
void test_floor(void);

// for.cpp
void eval_for(void);

// gamma.cpp
void eval_gamma(void);
void gamma(void);
void test_gamma(void);

// gcd.cpp
void eval_gcd(void);
void gcd(void);
void gcd_main(void);
void gcd_expr_expr(void);
void gcd_expr(U *p);
void gcd_term_term(void);
void gcd_term_factor(void);
void gcd_factor_term(void);
void test_gcd(void);

// guess.cpp
void guess(void);

// hermite.cpp
void hermite(void);
void yyhermite(void);
void yyhermite2(int n);
void test_hermite(void);

// hilbert.cpp
void hilbert(void);

// history.cpp
void update_cmd_history(char *s);
void do_up_arrow(void);
void do_down_arrow(void);
char * get_cmd_history(void);

// imag.cpp
void eval_imag(void);
void imag(void);
void test_imag(void);

// index.cpp
void index_function(int n);
void set_component(int n);
void test_index(void);

// init.cpp
void init(void);
void defn(void);

// inner.cpp
void eval_inner(void);
void inner(void);
void test_inner(void);

// integral.cpp
void eval_integral(void);
void integral(void);
void integral_of_sum(void);
void integral_of_product(void);
void integral_of_form(void);

// inv.cpp
void inv(void);
void invg(void);
void yyinvg(void);
void decomp(int n);

// is.cpp
int iszero(U *p);
int isnegativenumber(U *p);
int isplusone(U *p);
int isminusone(U *p);
int isinteger(U *p);
int isnonnegativeinteger(U *p);
int isposint(U *p);
int ispoly(U *p, U *x);
int ispoly_expr(U *p, U *x);
int ispoly_term(U *p, U *x);
int ispoly_factor(U *p, U *x);
int isnegativeterm(U *p);
int isimaginarynumber(U *p);
int iscomplexnumber(U *p);
int iseveninteger(U *p);
int isnegative(U *p);
int issymbolic(U *p);
int isintegerfactor(U *p);
int isoneover(U *p);
int isfraction(U *p);
int equaln(U *p, int n);
int equalq(U *p, int a, int b);
int isoneoversqrttwo(U *p);
int isminusoneoversqrttwo(U *p);
int isfloating(U *p);
int isimaginaryunit(U *p);
int isquarterturn(U *p);
int isnpi(U *p);

// isprime.cpp
void eval_isprime(void);
void test_isprime(void);

// itab.cpp

// itest.cpp
void test_integral(void);

// laguerre.cpp
void eval_laguerre(void);
void laguerre(void);
void laguerre2(int n);
void test_laguerre(void);

// laplace.cpp
void eval_laplace(void);
void laplace(void);
void laplace_main(void);
void test_laplace(void);

// lcm.cpp
void eval_lcm(void);
void lcm(void);
void yylcm(void);
void test_lcm(void);

// leading.cpp
void eval_leading(void);
void leading(void);

// legendre.cpp
void eval_legendre(void);
void legendre(void);
void test_legendre(void);

// list.cpp
void list(int n);

// log.cpp
void eval_log(void);
void logarithm(void);
void yylog(void);
void test_log(void);

// madd.cpp
unsigned int * madd(unsigned int *a, unsigned int *b);
unsigned int * msub(unsigned int *a, unsigned int *b);
void test_madd(void);
void test_maddf(int na, int nb, int nc);
void test_msub(void);
void test_msubf(int na, int nb, int nc);

// mag.cpp
void eval_mag(void);
void mag(void);
void yymag(void);
void test_mag(void);

// main.cpp
int main();
void clear_term();
void eval_display(void);
void printstr(char *s);
void printchar(int c);
void printchar_nowrap(int c);
void eval_draw(void);
void eval_sample(void);

// mcmp.cpp
int mcmp(unsigned int *a, unsigned int *b);
int mcmpint(unsigned int *a, int n);
void test_mcmp(void);

// mfactor.cpp
unsigned int * mfactor(unsigned int *n);
void test_mfactor(void);

// mgcd.cpp
unsigned int * mgcd(unsigned int *u, unsigned int *v);
void test_mgcd(void);

// mini-test.cpp
void mini_test(void);

// misc.cpp
void new_string(char *s);
void out_of_memory(void);
void push_zero_matrix(int i, int j);
void push_identity_matrix(int n);
void push_cars(U *p);
void peek(void);
void peek2(void);
int equal(U *p1, U *p2);
int lessp(U *p1, U *p2);
int sign(int n);
int cmp_expr(U *p1, U *p2);
int length(U *p);
U * unique(U *p);
void check_endianess(void);
void ssqrt(void);
void yyexpand(void);
void exponential(void);
void square(void);
void sort_stack(int n);

// mmodpow.cpp
unsigned int * mmodpow(unsigned int *x, unsigned int *n, unsigned int *m);
void test_mmodpow(void);

// mmul.cpp
unsigned int * mmul(unsigned int *a, unsigned int *b);
unsigned int * mdiv(unsigned int *a, unsigned int *b);
unsigned int * mmod(unsigned int *a, unsigned int *b);
void mdivrem(unsigned int **q, unsigned int **r, unsigned int *a, unsigned int *b);
void test_mmul(void);
void test_mdiv(void);
void test_mmod(void);

// mod.cpp
void eval_mod(void);
void mod(void);
void test_mod(void);

// mpow.cpp
unsigned int * mpow(unsigned int *a, unsigned int n);
void test_mpow(void);

// mprime.cpp
int mprime(unsigned int *n);
void test_mprime(void);

// mroot.cpp
unsigned int * mroot(unsigned int *n, unsigned int index);
void test_mroot(void);

// mscan.cpp
unsigned int * mscan(char *s);

// msqrt.cpp
unsigned int * msqrt(unsigned int *n);
void test_msqrt(void);

// mstr.cpp
char * mstr(unsigned int *a);

// multiply.cpp
void multiply(void);
void yymultiply(void);
void combine_factors(int h);
void multiply_noexpand(void);
void multiply_all(int n);
void multiply_all_noexpand(int n);
void divide(void);
void inverse(void);
void reciprocate(void);
void negate(void);
void negate_expand(void);
void negate_noexpand(void);
void test_multiply(void);

// nroots.cpp
void eval_nroots(void);
void monic(int n);
void findroot(int n);
void compute_fa(int n);
void divpoly(int n);
void test_nroots(void);

// numerator.cpp
void eval_numerator(void);
void numerator(void);
void test_numerator(void);

// outer.cpp
void eval_outer(void);
void outer(void);
void yyouter(void);
void test_outer(void);

// partition.cpp
void partition(void);

// polar.cpp
void eval_polar(void);
void polar(void);
void test_polar(void);

// pollard.cpp
void factor_number(void);
void factor_a(void);
void try_kth_prime(int k);
int factor_b(void);
void push_factor(unsigned int *d, int count);

// power.cpp
void eval_power(void);
void power(void);
void yypower(void);
void power_sum(int n);
void multinomial_sum(int k, int n, int *a, int i, int m);
int simplify_polar(void);
void test_power(void);

// prime.cpp
void eval_prime(void);
void prime(void);

// primetab.cpp

// print.cpp
void print(U *p);
void printline(U *p);
void print_subexpr(U *p);
void print_expr(U *p);
int sign_of_term(U *p);
void print_a_over_b(U *p);
void print_term(U *p);
void print_denom(U *p, int d);
void print_factor(U *p);
void print_index_function(U *p);
void print_factorial_function(U *p);
void print_tensor(U *p);
void print_tensor_inner(U *p, int j, int *k);
void print_str(char *s);
void print_char(int c);
void print_function_definition(U *p);
void print_arg_list(U *p);
void print_lisp(U *p);
void print1(U *p);
void print_multiply_sign(void);
int is_denominator(U *p);
int any_denominators(U *p);

// product.cpp
void eval_product(void);

// qadd.cpp
void qadd(void);

// qdiv.cpp
void qdiv(void);

// qmul.cpp
void qmul(void);

// qpow.cpp
void qpow();

// qsub.cpp
void qsub(void);

// quickfactor.cpp
void quickfactor(void);
void test_quickfactor(void);

// quotient.cpp
void eval_quotient(void);
void divpoly(void);
void test_quotient(void);

// rationalize.cpp
void eval_rationalize(void);
void rationalize(void);
void yyrationalize(void);
void test_rationalize(void);

// real.cpp
void eval_real(void);
void real(void);
void test_real(void);

// rect.cpp
void eval_rect(void);
void rect(void);
void test_rect(void);

// rewrite.cpp
void rewrite(void);
void rewrite_tensor(void);

// roots.cpp
void eval_roots(void);
void roots(void);
void test_roots(void);

// run.cpp
void stop(char *s);
void run(char *s);
void check_stack(void);
void echo_input(char *s);
void top_level_eval(void);
void check_esc_flag(void);

// scan.cpp
int scan(char *s);
int scan_meta(char *s);
void scan_stmt(void);
void scan_relation(void);
void scan_expression(void);
int is_factor(void);
void scan_term(void);
void scan_power(void);
void scan_factor(void);
void scan_symbol(void);
void scan_string(void);
void scan_function_call(void);
void scan_subexpr(void);
void error(char *errmsg);
void build_tensor(int n);
void get_next_token();
void get_token(void);
void update_token_buf(char *a, char *b);
void test_scan(void);

// selftest.cpp
void selftest(void);
void logout(char *s);
void errout(void);
void test(char *file, char **s, int n);
void test_low_level(void);

// sgn.cpp
void eval_sgn(void);
void sgn(void);
void yysgn(void);
void test_sgn(void);

// simfac.cpp
void eval_simfac(void);
void simfac(void);
void simfac(void);

// simplify.cpp
void eval_simplify(void);
void simplify(void);
void simplify_main(void);
void simplify_trig(void);
void test_simplify(void);

// sin.cpp
void eval_sin(void);
void sine(void);
void sine_of_angle_sum(void);
void sine_of_angle(void);
void test_sin(void);

// sinh.cpp
void eval_sinh(void);
void ysinh(void);
void yysinh(void);
void test_sinh(void);

// stack.cpp
void push(U *p);
U * pop();
void push_frame(int n);
void pop_frame(int n);
void save(void);
void restore(void);
void swap(void);
void dupl(void);

// subst.cpp
void subst(void);

// sum.cpp
void eval_sum(void);

// symbol.cpp
void std_symbol(char *s, int n);
U * usr_symbol(char *s);
char * get_printname(U *p);
void set_binding(U *p, U *q);
U * get_binding(U *p);
void set_binding_and_arglist(U *p, U *q, U *r);
U * get_arglist(U *p);
int symnum(U *p);
void push_symbol(int k);
void clear_symbols(void);

// tan.cpp
void eval_tan(void);
void tangent(void);
void yytangent(void);
void test_tan(void);

// tanh.cpp
void eval_tanh(void);
void test_tanh(void);

// taylor.cpp
void eval_taylor(void);
void taylor(void);
void test_taylor(void);

// tensor.cpp
void eval_tensor(void);
void tensor_plus_tensor(void);
void tensor_times_scalar(void);
void scalar_times_tensor(void);
int is_square_matrix(U *p);
void d_tensor_tensor(void);
void d_scalar_tensor(void);
void d_tensor_scalar(void);
int compare_tensors(U *p1, U *p2);
void power_tensor(void);
void copy_tensor(void);
void test_tensor(void);

// test.cpp
void eval_test(void);
void eval_testeq(void);
void eval_testge(void);
void eval_testgt(void);
void eval_testle(void);
void eval_testlt(void);
void eval_not(void);
void eval_and(void);
void eval_or(void);
int cmp_args(void);
void test_test(void);

// transform.cpp
void transform(char **s);
int f_equals_a(int h);

// transpose.cpp
void eval_transpose(void);
void transpose(void);
void test_transpose(void);

// userfunc.cpp
void eval_user_function(void);
int rewrite_args(void);
int rewrite_args_tensor(void);

// variables.cpp
void variables(void);

// vectorize.cpp
void vectorize(int n);

// window.cpp
void printchar_nowrap(int c);
void printchar(int c);
void printstr(char *s);
void clear_term(void);
void shipout(unsigned char *d, int w, int h);
void draw_display(void);
char * get_tty_buf(void);

// zero.cpp
void eval_zero(void);
void test_zero(void);

// plot.cpp
void eval_plot(void);
void eplot(void);
