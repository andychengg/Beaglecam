#ifndef NETWORKING_H_
#define NETWORKING_H_

// size of the client sends to the server this max size
#define RELAY_PACKET_SIZE       32

// the server sends a response with this max size
#define RESPONSE_PACKET_SIZE    256

#define SERVER_PORT             12345


#define STATUS_CODE_OK          "ok"
#define STATUS_CODE_BAD         "bad"

#define CLIENT_REQ_PING         ":ping"
#define CLIENT_REQ_LOGIN        ":login"
#define CLIENT_REQ_MFA          ":mfa"
#define CLIENT_REQ_AUTH         ":auth"
#define CLIENT_REQ_SETPASS      ":setp"
#define CLIENT_REQ_JSETPASS     ":jsetp"

#endif