
echo Testing JOIN in channel

nc 0 8081 -w 1 << EOF
PASS 123
NICK paula
USER paula 0 * :realname
JOIN #g3
PRIVMSG #g3 :oi grupo
PART #g3 :xau!
QUIT :xau!cau!




EOF

nc 0 8081 -w 1 << EOF
PASS 123
NICK tata
USER tata 0 * :realname
JOIN #g3
QUIT :Leaving
QUIT



EOF