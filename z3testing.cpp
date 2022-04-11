#include "z3++.h"
#include <iostream>

int main() {

    z3::context c;
    z3::solver s(c);

    // z3::expr t0_0 = c.bv_const("t0_0", 8);
    // z3::expr t0_1 = c.bv_const("t0_1", 8);
    // z3::expr t1_0 = c.bv_const("t1_0", 8);
    // z3::expr t1_1 = c.bv_const("t1_1", 8);
    // z3::expr key0_0 = c.bv_const("key0_0", 8);
    // z3::expr key0_1 = c.bv_const("key0_1", 8);
    // z3::expr a_0 = c.bv_const("a_0", 8);
    // z3::expr a_1 = c.bv_const("a_1", 8);
    // z3::expr in0_0 = c.bv_const("in0_0", 8);
    // z3::expr in0_1 = c.bv_const("in0_1", 8);

    // std::cout << "variables declared\n";

    // s.add(t0_0 == in0_0^1);
    // s.add(t0_1 == in0_1^key0_1);
    // s.add(t1_0 == t0_0^a_0);
    // s.add(t1_1 == t0_1^a_1);

    // std::cout << "stmt assertions done...\n";

    // s.add((key0_0^a_0).extract(0,0) == 0);
    // s.add((key0_1^a_1).extract(0,0) == 1);

    // s.push();
    // s.add(t0_0 == t0_1);
    // std::cout << s.assertions() << "\n";
    // std::cout << s.check() << "\n\n";
    // s.pop();

    // s.push();
    // s.add(t1_0 == t1_1);
    // std::cout << s.assertions() << "\n";
    // std::cout << s.check() << "\n\n";
    // s.pop();

    return 0;
}