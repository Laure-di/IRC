/*This requires that clients send a PASS command before sending the NICK / USER combination.

The password supplied must match the one defined in the server configuration. It is possible to send multiple PASS commands before registering but only the last one sent is used for verification and it may not be changed once the client has been registered.

If the password supplied does not match the password expected by the server, then the server SHOULD send ERR_PASSWDMISMATCH (464) and MAY then close the connection with ERROR. Servers MUST send at least one of these two messages.*/

/*Numeric replies:

ERR_NEEDMOREPARAMS (461)
ERR_ALREADYREGISTERED (462)
ERR_PASSWDMISMATCH (464)*/
