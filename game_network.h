#ifndef GAME_NETWORK_H
#define GAME_NETWORK_H

int setup_listen_socket();

int setup_client_socket();

void listen_on_socket(int, clientData[], int*);

int listen_on_fdset(fd_set, int, char []);

void sent_to_fdset(fd_set, int, char []);

#endif