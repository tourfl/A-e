/*
 *  @author: profs, Saïd & Raph
 *  @date: 23/10/15
 *
 */

 #include "mem/memory_v2.h"
 #include "com/notify.h" // Pour mettre des messages de warning et de debugs, inclut stdio (utile pour fp)
 #include "com/interpreteur.h"
 #include "com/command.h" // pour execute_command
 #include <string.h> // Pour utiliser strcmp notamment


int main ( int argc, char *argv[] ) {
    // On initialise la mémoire
    
    Memory *mem = NULL;

    mem = malloc(sizeof(Memory));

    if(mem == NULL)
    {
        ERROR_MSG("unable to allocate Memory");
        return 1;
    }

    DEBUG_MSG("Memory initialized");

    interpreteur inter=init_inter(); /* structure gardant les infos et états de l'interpreteur*/

    FILE *fp = NULL; /* le flux dans lequel les commande seront lues : stdin (mode shell) ou un fichier */

    if ( argc > 2 ) {
        usage_ERROR_MSG( argv[0] );
        exit( EXIT_FAILURE );
    }
    if(argc == 2 && strcmp(argv[1], "-h") == 0) {
        usage_ERROR_MSG( argv[0] );
        exit( EXIT_SUCCESS );
    }

    /*par defaut : mode shell interactif */
    fp = stdin;
    inter->mode = INTERACTIF;
    if(argc == 2) {
        /* mode fichier de commandes */
        fp = fopen( argv[1], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
        inter->mode = SCRIPT;
    }

    /* boucle infinie : lit puis execute une cmd en boucle */
    while ( 1 ) {



        if (acquire_line( fp,  inter)  == 0 ) {
            /* Une nouvelle ligne a ete acquise dans le flux fp*/

            int res = execute_cmd(inter, mem); /* execution de la commande */

            // traitement des erreurs
            switch(res) {
            case CMD_OK_RETURN_VALUE:
                break;
            case CMD_EXIT_RETURN_VALUE:
                /* sortie propre du programme */
                if ( inter->mode == SCRIPT ) {
                    fclose( fp );
                }
                del_inter(inter);
                exit(EXIT_SUCCESS);
                break;
            default:
                /* erreur durant l'execution de la commande */
                /* En mode "fichier" toute erreur implique la fin du programme ! */
                if (inter->mode == SCRIPT) {
                    fclose( fp );
                    del_inter(inter);
                    /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                    ERROR_MSG("ERREUR DETECTEE. Aborts");
                }
                break;
            }
        }
        if( inter->mode == SCRIPT && feof(fp) ) {
            /* mode fichier, fin de fichier => sortie propre du programme */
            DEBUG_MSG("FIN DE FICHIER");
            fclose( fp );
            del_inter(inter);
            exit(EXIT_SUCCESS);
        }
    }
    /* tous les cas de sortie du programme sont gérés plus haut*/
    ERROR_MSG("SHOULD NEVER BE HERE\n");
}

