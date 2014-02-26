echo "USERNAME:Password (x = encrypted in /etc/shadow:User ID (UID):Primary Group ID (GID):User Info:HOME DIR:DEFAULT SHELL" | cat - /etc/passwd | sed "s/::/: :/g;" | column -s : -t
