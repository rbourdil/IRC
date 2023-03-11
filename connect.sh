NICK=${1}
USER=${2}
PASSWD=${3}

if [ "$#" -ne 3 ]; then
	echo "use: ./connect.sh <nickname> <username> <password>"
	exit
fi

nc localhost 6667 <<- BLOCK
PASS ${PASSWD}
USER ${USER} 0 * :${USER}
NICK ${NICK}
BLOCK
