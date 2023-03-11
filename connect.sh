#!/bin/bash

if [ "$#" -ne 3 ]; then
	echo "use: ./connect.sh <nick> <user> <pass>"
	exit
fi

TMPFILE=$(mktemp -p /tmp)

printf "PASS ${3}\nUSER ${2} 0 * :${USER}\nNICK ${1}\n" >> ${TMPFILE}

cat ${TMPFILE} - | nc localhost 6667 -C

rm -f ${TMPFILE}
