#include "coreir.h"

using namespace CoreIR;

int main() {
  Context* c = newContext();

  Namespace* stdlib = c->getNamespace("stdlib");
 
  //Type of module 
  Type* regtestType = c->Record({
    {"clk",stdlib->getNamedType("clkIn")},
    {"in",c->Array(16,c->BitIn())},
    {"out",c->Array(16,c->Bit())}
  });
  Args w16({{"width",c->argInt(16)}});
  Generator* reg = stdlib->getGenerator("reg");
  Module* regtest = c->getGlobal()->newModuleDecl("regtest",regtestType);
  ModuleDef* def = regtest->newModuleDef();
    def->addInstance("r0",reg,w16);
    def->addInstance("r1",reg,{{"width",c->argInt(16)},{"en",c->argBool(true)}});
    
    def->connect("self.in","r0.in");
    def->connect("r0.out","r1.in");
    def->connect("self.out","r1.out");
  regtest->setDef(def);

  regtest->print();
  
  deleteContext(c);
  return 0;
}
