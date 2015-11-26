/*
 *  @author: profs, Saïd & Raph
 *  @date: 23/10/15
 *
 */

#include "emulator.h"

#include <string.h> // Pour utiliser strcmp notamment
#include "inter/notify.h" // Messages de contrôle
#include <stdlib.h>
#include "inter/command.h"





int main ( int argc, char *argv[] ) {

    int r = 0;
    FILE *fp = NULL; /* le flux dans lequel les commande seront lues : stdin (mode shell) ou un fichier */
    FILE *fd = NULL; // Pour le mode debug, permet de stocker provisoirement le fichier ouvert en mode script
    Emulator *emul = init_emul();
    interpreteur inter = emul->inter;




    if(emul == NULL) {
        exit(r);
    }

    if (load_dic(emul->dic) != 0 
        || emul->dic->ins32 == NULL 
        || emul->dic->ins32[0].commande == NULL)
    {
        del(emul);
        ERROR_MSG("unable to load dic");
    }


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

            del(emul);
            exit( EXIT_FAILURE );
        }
        inter->mode = SCRIPT;
    }

    /* boucle infinie : lit puis execute une cmd en boucle */
    while ( 1 ) {



        if (acquire_line( fp,  inter)  == 0 ) {
            /* Une nouvelle ligne a ete acquise dans le flux fp*/

            int res = execute_cmd(emul); /* execution de la commande */

            // traitement des erreurs
            switch(res) {
            case CMD_OK_RETURN_VALUE:
                break;

            case CMD_DEBUG_RETURN_VALUE:
                if(inter->mode == SCRIPT)
                {
                    fd = fp; // On stocke provisoirement l'adresse du fichier en cours
                    fp = stdin; // On perd le fichier en cours
                    inter->mode = DEBUG_MODE;
                }
                else
                    WARNING_MSG("Debug command only in script mode");
                break;

            case CMD_RESUME_RETURN_VALUE:
                if(inter->mode == DEBUG_MODE)
                {
                    fp = fd; // On repasse au contenu du fichier
                    fd = NULL; // On remet fd à NULL
                    inter->mode = SCRIPT;
                }
                else
                    WARNING_MSG("Resume command only in debug mode");
                break;


            case CMD_EXIT_RETURN_VALUE:
                /* sortie propre du programme */
                if ( inter->mode == SCRIPT ) {
                    fclose( fp );
                }
                else if(inter->mode == DEBUG_MODE) // fd pointe vers le fichier du mode SCRIPT
                    fclose(fd);

                
                exit(EXIT_SUCCESS);
                break;
            default:
                /* erreur durant l'execution de la commande */
                /* En mode "fichier" toute erreur implique la fin du programme afin de récupérer la valeur de retour de la commande */
                if (inter->mode == SCRIPT) {
                    fclose( fp );
                    del(emul);
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
            del(emul);
            exit(EXIT_SUCCESS);
        }
    }
    /* tous les cas de sortie du programme sont gérés plus haut*/
    ERROR_MSG("SHOULD NEVER BE HERE\n");
}

