#include "coreir.h"
#include "coreir/passes/analysis/smtmodule.hpp"
#include "coreir/passes/analysis/smtoperators.hpp"

#include <iostream>
using namespace CoreIR;
using namespace Passes;

typedef void (*voidFunctionType)(void);

string SMTModule::toString() {
  ostringstream o;
  for (auto s : stmts) o << s << endl;
  return o.str();
}

string SMTModule::toVarDecString() {
  ostringstream o;
  for (auto vd : vardecs) o << vd << endl;
  return o.str();
}

string SMTModule::toNextVarDecString() {
  ostringstream o;
  for (auto vd : nextvardecs) o << vd << endl;
  return o.str();
}

string SMTModule::toInitVarDecString() {
  ostringstream o;
  for (auto vd : initvardecs) o << vd << endl;
  return o.str();
}

string SMTModule::toInstanceString(Instance* inst, string path) {
  string instname = inst->getInstname();
  Module* mref = inst->getModuleRef();
  ostringstream o;
  string tab = "  ";
  string mname;
  Values args;
  if (gen) {
    addPortsFromGen(inst);
    mname = modname; //gen->getNamespace()->getName() + "_" + gen->getName(args);
  }
  else {
    mname = modname;
  }

  // it appears that arguments are kept in GenArgs even if it's a module
  // We want all arguments available
  if (mref->isGenerated()) {
    for (auto amap : mref->getGenArgs()) {
      ASSERT(args.count(amap.first)==0, "NYI Aliased config/genargs");
      args[amap.first] = amap.second;
    }
  }

  for (auto amap : inst->getModArgs()) {
    ASSERT(args.count(amap.first)==0,"NYI Alisaaed config/genargs");
    args[amap.first] = amap.second;
  }
  vector<string> params;
  const json& jmeta = mref->getMetaData();
  if (jmeta.count("verilog") && jmeta["verilog"].count("parameters")) {
    params = jmeta["verilog"]["parameters"].get<vector<string>>();
  }
  else {
    for (auto amap : args) {
      params.push_back(amap.first);
    }
  }
  vector<string> paramstrs;
  for (auto param : params) {
    ASSERT(args.count(param),"Missing parameter " + param + " from " + ::CoreIR::toString(args));
    string astr = "." + param + "(" + args[param]->toString() + ")";
    paramstrs.push_back(astr);
  }
  //Assume names are <instname>_port
  unordered_map<string, SmtBVVar> portstrs;
  for (auto port : ports) {
    portstrs.emplace(port.getPortName(), port);
  }

  string context = path+"$";
  string pre = "";

  enum operation {neg_op = 1,
                  eq_op,
                  const_op,
                  add_op,
                  sub_op,
                  and_op,
                  or_op,
                  xor_op,
                  reg_op,
                  regPE_op,
                  concat_op,
                  slice_op,
                  term_op,
                  mux_op,
                  mul_op,
                  lshr_op,
                  ashr_op,
                  andr_op,
                  orr_op,
                  zext_op,
                  ult_op,
                  ule_op,
                  ugt_op,
                  uge_op,
                  slt_op,
                  sle_op,
                  sgt_op,
                  sge_op
  };

  unordered_map<string, operation> opmap;

  opmap.emplace(pre+"neg", neg_op);
  opmap.emplace(pre+"not", neg_op);
  opmap.emplace(pre+"eq", eq_op);
  opmap.emplace(pre+"not", neg_op);
  opmap.emplace(pre+"const", const_op);
  opmap.emplace(pre+"add", add_op);
  opmap.emplace(pre+"sub", sub_op);
  opmap.emplace(pre+"and", and_op);
  opmap.emplace(pre+"or", or_op);
  opmap.emplace(pre+"eq", eq_op);
  opmap.emplace(pre+"xor", xor_op);
  opmap.emplace(pre+"reg", reg_op);
  opmap.emplace(pre+"reg_PE", regPE_op);
  opmap.emplace(pre+"concat", concat_op);
  opmap.emplace(pre+"slice", slice_op);
  opmap.emplace(pre+"term", term_op);
  opmap.emplace(pre+"mux", mux_op);
  opmap.emplace(pre+"lshr", lshr_op);
  opmap.emplace(pre+"ashr", ashr_op);
  opmap.emplace(pre+"mul", mul_op);
  opmap.emplace(pre+"orr", orr_op);
  opmap.emplace(pre+"andr", andr_op);
  opmap.emplace(pre+"ult", ult_op);
  opmap.emplace(pre+"ule", ule_op);
  opmap.emplace(pre+"ugt", ugt_op);
  opmap.emplace(pre+"uge", uge_op);
  opmap.emplace(pre+"slt", slt_op);
  opmap.emplace(pre+"sle", sle_op);
  opmap.emplace(pre+"sgt", sgt_op);
  opmap.emplace(pre+"sge", sge_op);
  opmap.emplace(pre+"zext", zext_op);

#define var_assign(var, name) if (portstrs.find(name) != portstrs.end()) var = portstrs.find(name)->second

  SmtBVVar out; var_assign(out, "out");
  SmtBVVar in; var_assign(in, "in");
  SmtBVVar in0; var_assign(in0, "in0");
  SmtBVVar in1; var_assign(in1, "in1");
  SmtBVVar clk; var_assign(clk, "clk");
  SmtBVVar en; var_assign(en, "en");
  SmtBVVar sel; var_assign(sel, "sel");
  SmtBVVar clr; var_assign(clr, "clr");
  SmtBVVar rst; var_assign(rst, "rst");

  switch (opmap[mname]) {
  case term_op:
    break;
  case neg_op:
    o << SMTNot(context, in, out);
    break;
  case eq_op:
    o << SMTEq(context, in0, in1, out);
    break;
  case add_op:
    o << SMTAdd(context, in0, in1, out);
    break;
  case sub_op:
    o << SMTSub(context, in0, in1, out);
    break;
  case and_op:
    o << SMTAnd(context, in0, in1, out);
    break;
  case or_op:
    o << SMTOr(context, in0, in1, out);
    break;
  case xor_op:
    o << SMTXor(context, in0, in1, out);
    break;
  case concat_op:
    o << SMTConcat(context, in0, in1, out);
    break;
  case reg_op:
    o << SMTReg(context, in, clk, out);
    break;
  case regPE_op:
    o << SMTRegPE(context, in, clk, out, en);
    break;
  case const_op:
    o << SMTConst(context, out, args["value"]->toString());
    break;
  case mux_op:
    o << SMTMux(context, in0, in1, sel, out);
    break;
  case lshr_op:
    o << SMTLshr(context, in0, in1, out);
    break;
  case ashr_op:
    o << SMTAshr(context, in0, in1, out);
    break;
  case mul_op:
    o << SMTMul(context, in0, in1, out);
    break;
  case orr_op:
    o << SMTOrr(context, in, out);
    break;
  case andr_op:
    o << SMTAndr(context, in, out);
    break;
  case zext_op:
    o << SMTZext(context, in, out);
    break;
  case slice_op:
    int lo; lo = stoi(args["lo"]->toString());
    int hi; hi = stoi(args["hi"]->toString());
    o << SMTSlice(context, in, out, lo, hi-1);
    break;
  case ult_op:
    o << SMTUlt(context, in0, in1, out);
    break;
  case ule_op:
    o << SMTUle(context, in0, in1, out);
    break;
  case ugt_op:
    o << SMTUgt(context, in0, in1, out);
    break;
  case uge_op:
    o << SMTUge(context, in0, in1, out);
    break;
  case slt_op:
    o << SMTSlt(context, in0, in1, out);
    break;
  case sle_op:
    o << SMTSle(context, in0, in1, out);
    break;
  case sgt_op:
    o << SMTSgt(context, in0, in1, out);
    break;
  case sge_op:
    o << SMTSge(context, in0, in1, out);
    break;
  default:
    o << "!!! UNMATCHED: " << mname << " !!!";
  }

  o << endl;
  return o.str();
}
