#include "coreir.h"
#include "coreir-passes/common.h"

using namespace CoreIR;

int main() {
  // New context
  Context* c = newContext();
  
  Namespace* g = c->getGlobal();
  Namespace* prj = c->newNamespace("prj");
  
  Namespace* sl = c->getNamespace("coreir");

  Args wargs({{"width",c->argInt(19)}});
  
  // Define random module with subs
  Module* ms = prj->newModuleDecl("SNRTestSub",c->Any());
  ModuleDef* def = ms->newModuleDef();
    def->addInstance("c0",sl->getGenerator("const"),wargs,{{"value",c->argInt(5)}});
    def->addInstance("c1",sl->getGenerator("const"),wargs,{{"value",c->argInt(5)}});
    def->addInstance("c2",sl->getGenerator("const"),wargs,{{"value",c->argInt(2)}});
    def->addInstance("s0",sl->getGenerator("sub"),wargs);
    def->addInstance("s1",sl->getGenerator("sub"),wargs);
    def->addInstance("m0",sl->getGenerator("mul"),wargs);
    def->connect("c0.out","s0.in0");
    def->connect("c1.out","s0.in1");
    def->connect("s0.out","s1.in0");
    def->connect("c2.out","s1.in1");
    def->connect("s1.out","m0.in0");
    def->connect("s0.out","m0.in1");
  ms->setDef(def);
  ms->print();
  
  // Define same random module with adds instead of subs and operands switched
  Module* ma = g->newModuleDecl("SNRTestAdd",c->Any());
  def = ma->newModuleDef();
    def->addInstance("c0",sl->getGenerator("const"),wargs,{{"value",c->argInt(0)}});
    def->addInstance("c1",sl->getGenerator("const"),wargs,{{"value",c->argInt(1)}});
    def->addInstance("c2",sl->getGenerator("const"),wargs,{{"value",c->argInt(2)}});
    def->addInstance("a0",sl->getGenerator("add"),wargs);
    def->addInstance("a1",sl->getGenerator("add"),wargs);
    def->addInstance("m0",sl->getGenerator("mul"),wargs);
    def->connect("c0.out","a0.in1");
    def->connect("c1.out","a0.in0");
    def->connect("a0.out","a1.in1");
    def->connect("c2.out","a1.in0");
    def->connect("a1.out","m0.in0");
    def->connect("a0.out","m0.in1");
  ma->setDef(def);
  ma->print();

  //Now try to do a search and replace on ms to replace all subs with adds
  
  PassManager* pm = new PassManager(prj);

  //Create all the search and replace patterns. 
  Namespace* patns = c->newNamespace("mapperpatterns");

  //Create the match pattern and the replace pattern for Sub2Add
  Type* subType = sl->getTypeGen("binary")->getType(wargs);
  Module* patternSub = patns->newModuleDecl("sub",subType);
  ModuleDef* pdef = patternSub->newModuleDef();
    pdef->addInstance("inst",sl->getGenerator("sub"),wargs);
    pdef->connect("self.in0","inst.in1");
    pdef->connect("self.in1","inst.in0");
    pdef->connect("self.out","inst.out");
  patternSub->setDef(pdef);

  MatchAndReplacePass::Opts opts;
  opts.genargs = wargs;
  MatchAndReplacePass* pSub2Add = new MatchAndReplacePass(
      patternSub,
      sl->getGenerator("add"),
      opts
  );
  pm->addPass(pSub2Add,0);
  
  //match const -> add.in1  change to neg
  Type* cType = sl->getTypeGen("unary")->getType(wargs);
  Module* patternC = patns->newModuleDecl("cpat",cType);
  pdef = patternC->newModuleDef();
    pdef->addInstance("a0",sl->getGenerator("add"),wargs);
    pdef->addInstance("c0",sl->getGenerator("const"),wargs,{{"value",c->argInt(31)}});
    pdef->connect("self.in","a0.in1");
    pdef->connect("c0.out","a0.in0");
    pdef->connect("self.out","a0.out");
  patternC->setDef(pdef);

  MatchAndReplacePass* pC = new MatchAndReplacePass(
      patternC,
      sl->getGenerator("neg"),
      opts
  );
  //(void) pC;
  pm->addPass(pC,1);
  
  cout << "Running all the passes!" << endl;
  pm->run();
  cout << "Printing the (hopefully) modified graph" << endl;
  ms->print();
  
  //TODO finish isEqual function.
  //assert(Module::isEqual(ma,ms));

  deleteContext(c);
  
  return 0;
}
