#include "../includes/Commands.hpp"

/*
* 3.1 Connection Registration
*/
cmd_func pass;
cmd_func nick;
cmd_func user;
cmd_func oper;
cmd_func mode;
cmd_func service;
cmd_func quit;
cmd_func squit;

/*
* 3.2 Channel operations
*/
cmd_func join;
cmd_func part;
cmd_func topic;
cmd_func names;
cmd_func list;
cmd_func invite;
cmd_func kick;

/*
* 3.3 Sending messages
*/
cmd_func privmsg;
cmd_func notice;

/*
* 3.4 Server queries and commands
*/
cmd_func motd;
cmd_func lusers;
cmd_func version;
cmd_func stats;
cmd_func links;
cmd_func time;
cmd_func connect;
cmd_func trace;
cmd_func admin;
cmd_func info;

/*
* 3.5 Service Query and Commands
*/
cmd_func servlist;
cmd_func squery;

/*
* 3.6 User based queries
*/
cmd_func who;
cmd_func whois;
cmd_func whowas;

/*
* 3.7 Miscellaneous messages
*/
cmd_func kill;
cmd_func ping;
cmd_func pong;
cmd_func error;

/*
* 4.1 Away
*/
cmd_func away;
cmd_func rehash;
cmd_func die;
cmd_func restart;
cmd_func summon;
cmd_func users;
cmd_func wallops;
cmd_func userhost;
cmd_func ison;
