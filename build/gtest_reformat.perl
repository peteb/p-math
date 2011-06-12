#s/(^.*:[0-9]*: )Failure\nValue of: (.*)\n  Actual: (.*)\nExpected: (.*)\nWhich is: (.*)\n/\1Expected \2 to be \4 (\5) but was \3\n/m

s/(^.*:[0-9]*: )Failure\nValue of: (.*)\n  Actual: (.*)\nExpected: (.*)\n/\1Value of \2 was \3 but expected \4\n/mg;

s/(^.*:[0-9]*: )Failure\nValue of: (.*)\n  Actual: (.*)\nExpected: (.*)\nWhich is: (.*)\n/\1Value of \2 was \3 but expected \4 (\5)\n/mg;