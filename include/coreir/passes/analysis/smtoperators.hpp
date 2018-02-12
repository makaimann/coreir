#ifndef SMTOPERATORS_HPP_
#define SMTOPERATORS_HPP_

#include "coreir.h"
#include "coreir/passes/analysis/smtmodule.hpp"
#include <ostream>

using namespace CoreIR;
using namespace std;
namespace CoreIR {
  namespace Passes {
    typedef std::pair<string, SmtBVVar> named_var;

    string SmtBVVarDec(SmtBVVar w);
    string SMTgetCurr(string var);
    string SMTgetNext(string var);
    SmtBVVar SmtBVVarGetInit(SmtBVVar var);
    SmtBVVar SmtBVVarGetNext(SmtBVVar var);
    SmtBVVar SmtBVVarGetCurr(SmtBVVar var);
    string getSMTbits(unsigned, int);
    string SMTAssign(SmtBVVar vleft, SmtBVVar vright);
    string SMTAnd(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTOr(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTXor(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTNot(string context, SmtBVVar in, SmtBVVar out);
    string SMTConst(string context, SmtBVVar out, string val);
    string SMTAdd(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTSub(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTEq(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTConcat(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTReg(string context, SmtBVVar in, SmtBVVar clk, SmtBVVar out);
    string SMTRegPE(string context, SmtBVVar in, SmtBVVar clk, SmtBVVar out, SmtBVVar en);
    string SMTCounter(string context, SmtBVVar clk, SmtBVVar en, SmtBVVar out);
    string SMTSlice(string context, SmtBVVar in, SmtBVVar out, int low, int high);
    string SMTClock(string context, SmtBVVar clk_p);
    string SMTMux(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar sel, SmtBVVar out);
    string SMTLshr(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTAshr(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTMul(string context, SmtBVVar in1, SmtBVVar in2, SmtBVVar out);
    string SMTOrr(string context, SmtBVVar in, SmtBVVar out);
    string SMTAndr(string context, SmtBVVar in, SmtBVVar out);
    string SMTZext(string context, SmtBVVar in, SmtBVVar out);
    string SMTUlt(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);
    string SMTUle(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);
    string SMTUgt(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);
    string SMTUge(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);
    string SMTSlt(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);
    string SMTSle(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);
    string SMTSgt(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);
    string SMTSge(string context, SmtBVVar in0, SmtBVVar in1, SmtBVVar out);

>>>>>>> Added comparators
  }
}
#endif
