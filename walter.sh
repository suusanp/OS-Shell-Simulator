#!/bin/bash
function setup1 {
    cd ..
    cd .. 
    cd src 
    rm mysh
    make mysh framesize=18 varmemsize=10
    cd .. 
    cd testcases/assignment3 
}

function setup2 {
    cd ..
    cd .. 
    cd src 
    rm mysh
    make mysh framesize=21 varmemsize=10
    cd .. 
    cd testcases/assignment3 
}

function setup3 {
    cd ..
    cd .. 
    cd src 
    rm mysh
    make mysh framesize=6 varmemsize=10
    cd .. 
    cd testcases/assignment3 
}

#testcase 1
rm mysh
make mysh framesize=18 varmemsize=10
cd .. 
cd testcases/assignment3 
../../src/mysh < T_tc1.txt > T_tc1_myoutput.txt

#testcase 2
setup1
../../src/mysh < T_tc2.txt > T_tc2_myoutput.txt

#testcase 3
setup2
../../src/mysh < T_tc3.txt > T_tc3_myoutput.txt

#testcase 4
setup1
../../src/mysh < T_tc4.txt > T_tc4_myoutput.txt

#testcase 5
setup3
../../src/mysh < T_tc5.txt > T_tc5_myoutput.txt

#testcase 6
setup1
../../src/mysh < T_tc6.txt > T_tc6_myoutput.txt

#testcase 7
setup1
../../src/mysh < T_tc7.txt > T_tc7_myoutput.txt

#testcase 8
setup2
../../src/mysh < T_tc8.txt > T_tc8_myoutput.txt

#testcase 9
setup1
../../src/mysh < T_tc9.txt > T_tc9_myoutput.txt

#testcase 1-
setup3
../../src/mysh < T_tc10.txt > T_tc10_myoutput.txt


#compare
echo test 1
diff -w T_tc1_result.txt -w T_tc1_myoutput.txt
echo test 2
diff -w T_tc2_result.txt -w T_tc2_myoutput.txt
echo test 3
diff -w T_tc3_result.txt -w T_tc3_myoutput.txt
echo test 4
diff -w T_tc4_result.txt -w T_tc4_myoutput.txt
echo test 5
diff -w T_tc5_result.txt -w T_tc5_myoutput.txt
echo test 6
diff -w T_tc6_result.txt -w T_tc6_myoutput.txt
echo test 7
diff -w T_tc7_result.txt -w T_tc7_myoutput.txt
echo test 8
diff -w T_tc8_result.txt -w T_tc8_myoutput.txt
echo test 9
diff -w T_tc9_result.txt -w T_tc9_myoutput.txt
echo test 10
diff -w T_tc10_result.txt -w T_tc10_myoutput.txt



