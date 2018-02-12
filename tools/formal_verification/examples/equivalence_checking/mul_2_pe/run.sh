WORKINGDIR=tools/formal_verification/examples/equivalence_checking/mul_2_pe/
echo " --> Generating Encoding for System 1"
./bin/coreir -i $WORKINGDIR/system_1.json -p 'cullgraph,flattentypes,flatten,verifyflattenedtypes' -o $WORKINGDIR/system_1.smt2
echo " --> Generating Encoding for System 2"
./bin/coreir -i $WORKINGDIR/system_2.json -p 'cullgraph,flattentypes,flatten,verifyflattenedtypes' -o $WORKINGDIR/system_2.smt2
echo " --> Generating Encoding for Combined Systems"
python tools/formal_verification/equivalence_checking.py -if1 $WORKINGDIR/system_1.smt2 -if2 $WORKINGDIR/system_2.smt2 -m $WORKINGDIR/imap.txt > $WORKINGDIR/equivalence.smt2
echo " --> Running Equivalence Check"
bash tools/formal_verification/cvc4_check.sh $WORKINGDIR/equivalence.smt2
