#ifndef _UTILS_H
#define _UTILS_H

#define MAX_MESSAGE_LEN 128
#define PORT 8096

/**
 * @brief Users structure
 **/

struct user {
    unsigned int user_id; // User identifier
    char username[MAX_MESSAGE_LEN];

};

/**
 * @brief Message structure
 **/

struct message {
    unsigned int sender; // user_id de l'utilisateur qui envoie le message
    unsigned int receiver; // user_id de l'utilisateur qui doit recevoir le message (-1 permet de broadcast à tous les utilisateurs)
    unsigned int data_len; 
    char *data; // message
};

/**
 * @brief Exit printing error prefixed by `prefix` if `condition` is true (non zero)
 */

void exit_if(int condition, const char *prefix);

/**
 * @brief Fonction lisant l'entrée standard de maniére non bloquante
 * 
 * 
 * @param buffer Pointeur vers un Tableau de char de longeur `MAX_MESSAGE_LEN`
 * 
 * @return La fonction retourne TRUE si l'utilisateur rentre le caractére entrée ou la taille maximale est atteinte.
 */

int read_stdin(char *buffer);

/**
 * @brief Fonction permettant de remplir un tableau de zero
 * 
 * @param str tableau à remplir
 * 
 * @param size taille du tableau
 * 
 * @return La fonction ne retourne rien.
 * */

void fill_array(char *str, int size);

/**
 * @brief overwrite stdout
 **/

void str_overwrite_stdout();


/**
 * @brief remove \n char at the end of a new message
 * 
 * @param arr string
 * 
 * @param length size of string
 * 
 **/

void str_trim_lf (char* arr, int length);

/**
 * @brief change printf colour to black
 **/

void stdout_black();

/**
 * @brief change printf colour to red
 **/

void stdout_red();

/**
 * @brief change printf colour to green
 **/

void stdout_green();

/**
 * @brief change printf colour to yellow
 **/

void stdout_yellow();

/**
 * @brief change printf colour to blue
 **/

void stdout_blue();

/**
 * @brief change printf colour to purple
 **/

void stdout_purple();

/**
 * @brief change printf colour to cyan
 **/

void stdout_cyan();

/**
 * @brief change printf colour to white
 **/

void stdout_white();



#endif