//This needs to stay the same as CoreIR::Param
typedef enum {
  COREBoolParam = 0,
  COREUintParam = 1,
  COREIntParam = 2,
  COREStringParam = 3,
  CORETypeParam = 4
} COREParam;


//Get the Type Kind enum
extern COREParam COREGetArgKind(COREArg* arg);

//Create Args
extern COREArg* COREArgInt(COREContext* c,int i);
extern COREArg* COREArgUint(COREContext* c,uint u);
extern COREArg* COREArgString(COREContext* c,char* str);

//Arg Getter functions will assert on wrong arg type
extern int COREArgIntGet(COREArg* a);
extern const char* COREArgStringGet(COREArg* a);
