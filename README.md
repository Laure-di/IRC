# FT_IRC

## Commands implemented

### Connexion Registration
- PASS
- NICK
- USER
- OPER
- MODE
- SERVICE
- CAP
- QUIT
- SQUIT

### Channel Operations
- JOIN
- PART
- MODE
- TOPIC
- NAMES
- LIST
- INVITE
- KICK

### Sending Messages
- PRIVMSG
- NOTICE

### Server queries and commands
- WHO
- WHOIS
- KILL
- PING
- PONG

### Optional features
- AWAY
- WALLOPS

## Implemented modes
All the modes listed in the irssi documentation https://irssi.org/documentation/help/mode/

### User Server Modes
- i : Invisble
- r : Restricted
- w : Wallops

### User Channel Modes
- o : Operator
- O : Creator
- v : Voice

### Channel Modes without Parameters
- i : Invitation
- m : Moderated
- n : Outside
- p : Private
- s : Secret
- t : Topic

### Channel Modes with Parameters
- b : Ban masks
- e : Exception ban masks
- I : Invitation masks
- k : Key
- l : Limit
