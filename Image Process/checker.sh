#!/bin/bash

i=1
score=0

write_passed() {
	echo "Test $i ................... PASSED";
}

write_failed() {
	echo "Test $i ................... FAILED";
}

#se compileaza programul
(make && lamboot) &> /dev/null

#daca compilarea a esuat 
if ! [ -f "sets" ] ; then
    echo "Compilarea temei 3 a esuat :("
    exit 1
fi

echo -e "\n============ TESTING ============\n"

#se ruleaza fiecare test in parte si se verifica corectitudinea
for file in `ls test | grep in | cut -d '.' -f 1`; do
	
	mpiexec -np $1 ./sets test/$file.in $file.pgm &> /dev/null
	wait $!

	if ! [ -f $file.pgm ] 
		then write_failed && i=$((i+1)) && continue
	fi

	result=`./imgdiff test/$file.pgm $file.pgm | tr -d '\n'`
	
	if test "$result" = "Images differ!" 
		then write_failed
		else write_passed && score=$((score+10))
	fi

	rm -f $file.pgm
	i=$((i+1))

done;
	
echo -e "\n============" $score "/ 100 ============\n"

make clean &> /dev/null