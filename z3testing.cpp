#include "getVariables.hh"

int main() {

    z3::context c;

    z3::expr t0_0 = c.bv_const("t0_0", 8);
    z3::expr t0_1 = c.bv_const("t0_1", 8);
    z3::expr t1_0 = c.bv_const("t1_0", 8);
    z3::expr t1_1 = c.bv_const("t1_1", 8);
    z3::expr key0_0 = c.bv_const("key0_0", 8);
    z3::expr key0_1 = c.bv_const("key0_1", 8);
    z3::expr a_0 = c.bv_const("a_0", 8);
    z3::expr a_1 = c.bv_const("a_1", 8);
    z3::expr in0_0 = c.bv_const("in0_0", 8);
    z3::expr in0_1 = c.bv_const("in0_1", 8);

    z3::expr ind_a0 = c.bv_const("ind_a0_0", 8);
    z3::expr ind_a0 = c.bv_const("ind_a0_1", 8);

    std::cout << "variables declared\n";

    z3::solver s(c);

    s.add(a_0 == a_1);
    s.add(in0_0 == in0_1);

    s.add((t0_0) == (in0_0^key0_0));
    s.add((t0_1) == (in0_1^key0_1));
    s.add((t1_0) == (t0_0^a_0));
    s.add((t1_1) == (t0_1^a_1));

    int pow2 = 1 << 2;

    s.add(((key0_0).extract(0,0)) == 0);
    s.add(((key0_1).extract(0,0)) == 1);

    // s.add( ((key0_0&pow2)/(pow2)) == 0);
    // s.add( ((key0_1&pow2)/(pow2)) == 1);

    std::cout << s << "stmt assertions done...\n";

    s.push();
    s.add((t0_0) == (t0_1));
    std::cout << s.assertions() << "\n";
    std::cout << s.check() << "\n\n";
    s.pop();

    s.push();
    s.add((t1_0) == (t1_1));
    std::cout << s.assertions() << "\n";
    std::cout << s.check() << "\n\n";
    s.pop();

    // std::cout << "bitvector example 2\n";
    // z3::context c;
    // z3::expr t0_0 = c.bv_const("t0_0", 32);
    // z3::expr in0_0 = c.bv_const("in0_0", 32);
    // z3::expr key0_0 = c.bv_const("key0_0", 32);
    // z3::solver s(c);
    // // In C++, the operator == has higher precedence than ^.
    // s.add((t0_0 ^ in0_0) == t0_0 * in0_0);
    // std::cout << s << "\n";
    // std::cout << s.check() << "\n";
    // std::cout << s.get_model() << "\n";

    return 0;
}