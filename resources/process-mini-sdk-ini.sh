#!/bin/bash
TEMPFILE=`mktemp`
TEMPFILE2=`mktemp`
INCLUDEDIR=../include
SOURCEDIR=../src/syscalls

sed -n -e '/\[vleSyscalls\]/,/\[vleTools\]/p'  $1 |sed -e '$d' -e '1d' -e 's/\r//' > $TEMPFILE

eval `grep -e 'header' $TEMPFILE |sed -e 's/;.*@//g' -e 's/@//g'`

grep -vG -e header -e LIBRARY $TEMPFILE > $TEMPFILE2


mkdir -p $INCLUDEDIR
mkdir -p $SOURCEDIR

old_IFS=$IFS      # save the field separator    
IFS=$'\n'     # new field separator, the end of line    
for line in $(cat $TEMPFILE2)   
do 

    eval "HEADER=`echo \"${line}\" | awk -F\; '{ gsub(/\\(/, \"{\", \$2); gsub(/\\)/, \"}\", \$2); print \$2}'`"
    EQUATE=`echo ${line}|head -c 4`
    SYSCALL=`echo ${line} | awk -F\( '{ sub(/\(.+?/,"", \$1); sub(/.+?(\*|[[:space:]])/,"", \$1); print  \$1 }'`

    echo ${line} | awk -F\@ '{ gsub(/;.*/, "", $2); print $2";" }' >> $INCLUDEDIR/$HEADER
    sort -u -o $INCLUDEDIR/$HEADER $INCLUDEDIR/$HEADER

    echo "#include <asm.h>

SYSCALL(_${SYSCALL}, 0x${EQUATE})
">$SOURCEDIR/${SYSCALL}.S

done   
IFS=$old_IFS


