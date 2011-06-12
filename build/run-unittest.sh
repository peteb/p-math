#!/bin/sh

# list of unittest binaries
targets=`find $1/bin/ -name unittest_*`

ret=0
# run all unittest
for target in $targets 
do 
  $target --gtest_shuffle > output
  
  if [ $? -ne 0 ]
  then
    ret=$?
  fi
  
  # run a script to convert the GTest output into GCC-similar output
  perl -00 -p gtest_reformat.perl output
done 

exit $ret
