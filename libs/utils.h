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
 * @brief escape \n from strings
 * 
 * @param str string
 * 
 * @param size taille de la string
 * 
 * @param escape char a escape
 * 
 * @param replace char qui remplace le char escape
 * 
 * @return la fonction n'a pas de valeur de retour
 **/

void escape_string(char *str, int size, char escape, char replace);

#endif