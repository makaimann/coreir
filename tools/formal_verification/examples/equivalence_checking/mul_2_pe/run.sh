WORKINGDIR=tools/formal_verification/examples/equivalence_checking/mul_2_pe/
echo "Generating system"
python tools/formal_verification/equivalence_checking.py -if1 $WORKINGDIR/system_1.smt2 -if2 $WORKINGDIR/system_2.smt2 -m $WORKINGDIR/imap.txt > $WORKINGDIR/equivalence.smt2
echo "Running Equivalence Check"
bash tools/formal_verification/cvc4_check.sh $WORKINGDIR/equivalence.smt2
