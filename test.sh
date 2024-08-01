echo Testing JOIN in channel

nc 0 8081 << EOF




PASS 123
NICK oioi
USER tata 0 * :realname
JOIN #g3
JOIN #g4

PRIVMSG #g3 :tata tambem!



EOF