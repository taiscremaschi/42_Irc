GREEN='\033[32m'
RED='\033[31m'
END='\033[0m'
MAG='\001\e[0;35m\002'

WHO=$1
WHERE=$2
PORT=$3
echo $GREEN Testing MODE in channel $END
echo "\n"

nc 0 $PORT << EOF
PASS 123
NICK $WHO
USER $WHO 0 * :realname
PRIVMSG tata :testando 23
PRIVMSG tata :mais
PRIVMSG tata :hahahahaa
JOIN #CU
EOF