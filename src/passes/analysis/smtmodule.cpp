#include "coreir.h"
#include "coreir-passes/analysis/smtmodule.hpp"
#include "coreir-passes/analysis/smtoperators.hpp"

#include <iostream>
using namespace CoreIR;
using namespace Passes;

typedef void (*voidFunctionType)(void);

string SMTModule::toString() {
  vector<string> iports;
	vector<string> odims;
  for (auto pmap : ports) {
    auto port = pmap.second;
		cout << port.dirstr() << " " << port.getName() << " " << port.dimstr() << endl;
		if (port.dirstr() == "input") {
			iports.push_back(port.getName() + " () " + "(_ BitVec " + port.dimstr() + ")");
		}
		else odims.push_back(port.dimstr());
  }
  ostringstream o;
  string tab = "  ";

  o << endl << "(define-fun " << modname << "\n" << tab << join(iports.begin(), iports.end(), string("\n  ")) << endl;

	cout << "odims size: " << odims.size() << endl;
	cout << "iports size: " << iports.size() << endl;

	if (odims.size() > 0) {
		o << "(_ BitVec " << odims[0] << "))" << endl;
	}

  //Param declaraions
  // for (auto p : params) {
  //   o << tab << "parameter " << p;
  //   if (paramDefaults.count(p)) {
  //     o << " = " << paramDefaults[p];
  //   }
  //   o << ";" << endl;
  // }
  o << endl;
  
  for (auto s : stmts) o << s << endl;
  o << ")" << endl;
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

string SMTModule::toInstanceString(Instance* inst) {
  string instname = inst->getInstname();
  cout << "instname: " << instname << endl;
  Instantiable* iref = inst->getInstantiableRef();
  if (this->gen) {
    ASSERT(inst->isGen(),"DEBUG ME:");
  }
  ostringstream o;
  string tab = "  ";
  string mname;
  unordered_map<string,SmtBVVar> iports;
  Args args;
  if (gen) {
    args = inst->getGenArgs();
    Type2Ports(gen->getTypeGen()->getType(inst->getGenArgs()),iports);
    mname = gen->getNamespace()->getName() + "_" + gen->getName(args);
  }
  else {
    mname = modname;
    iports = ports;
  }

  for (auto amap : inst->getConfigArgs()) {
    ASSERT(args.count(amap.first)==0,"NYI Alisaaed config/genargs");
    args[amap.first] = amap.second;
  }
  vector<string> params;
  const json& jmeta = iref->getMetaData();
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
    ASSERT(args.count(param),"Missing parameter " + param + " from " + Args2Str(args));
    string astr = "." + param + "(" + args[param]->toString() + ")";
    paramstrs.push_back(astr);
  }
  //Assume names are <instname>_port
  unordered_map<string, std::pair <string, SmtBVVar>> portstrs;
  for (auto port : iports) {
    pair<string, SmtBVVar> pstr = std::make_pair(instname, port.second);
    portstrs.emplace(port.first, pstr);
  }

  if (mname == "coreir_neg")
    o << SMTNot(portstrs.find("in")->second, portstrs.find("out")->second);
  else if (mname == "coreir_const")
    o << SMTConst(portstrs.find("out")->second, getSMTbits(stoi(args["width"]->toString()), stoi(args["value"]->toString())));
  else if (mname == "coreir_add")
    o << SMTAdd(portstrs.find("in0")->second, portstrs.find("in1")->second, portstrs.find("out")->second);
  else if (mname == "coreir_reg_PE")
    o << SMTRegPE(portstrs.find("in")->second, portstrs.find("clk")->second, portstrs.find("out")->second, portstrs.find("en")->second);
  else if (mname == "counter") {
    o << SMTCounter(portstrs.find("clk")->second, portstrs.find("en")->second, portstrs.find("out")->second);
  }
  else if (mname == "coreir_concat")
    o << SMTConcat(portstrs.find("in0")->second, portstrs.find("in1")->second, portstrs.find("out")->second);
  else if (mname == "coreir_slice") {
    int lo = stoi(args["lo"]->toString());
    int hi = stoi(args["hi"]->toString())-1;
    o << SMTSlice(portstrs.find("in")->second, portstrs.find("out")->second,
		  std::to_string(lo), std::to_string(hi)) << endl;
  }
  else if (mname == "coreir_term"); // do nothing in terminate case
  else {
    o << "!!! UNMATCHED: " << mname << " !!!" << endl;
    //    o << mname << "(\n" << tab << tab << join(portstrs.begin(),portstrs.end(),",\n"+tab+tab) << "\n  );" << endl;
  }
              
  return o.str();
}

