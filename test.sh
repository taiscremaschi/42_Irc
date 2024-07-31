cho Testing JOIN in channel

nc 0 8081 << EOF
PASS 123
NICK paula
USER paula 0 * :realname
JOIN #g3
PRIVMSG #g3 :paula passou por aqui!
QUIT :Leaving
QUIT




EOF

nc 0 8081 << EOF
PASS 123
NICK tata
USER tata 0 * :realname
JOIN #g3
PRIVMSG #g3 :tata tambem!
QUIT :Leaving
QUIT



EOF