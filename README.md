# TP1_PROG
# réalise par Ben Youssef Nour / Bouzidi Amira

# Question 1
Ce programme affiche des messages en utilisant la fonction write().

# Question 2
Le programme entre dans une boucle infinie, ce qui signifie qu'il continuera à s'exécuter jusqu'à ce qu'une condition de sortie interne soit rencontrée.
À l'intérieur de la boucle, le programme attend que l'utilisateur saisisse une commande via read(), stockant cette commande dans un tampon (buffer) de taille 10. La saisie utilisateur est lue depuis l'entrée standard (0).

pour l'exécution de la commande, un nouveau processus enfant est créé à l'aide de fork().
Dans le processus enfant (pid == 0), la commande saisie est exécutée à l'aide de execlp(), qui remplace le programme courant par celui spécifié dans buffer.
Le processus parent (pid != 0) attend que le processus enfant se termine à l'aide de wait().
-> Ces ajouts permettent au programme d'attendre en permanence les commandes saisies par l'utilisateur, d'exécuter ces commandes via le shell, puis de revenir en attente pour une nouvelle saisie.


#Question 3
Pour la gestion de la sortie du shell avec la commande “exit” : Après l'exécution de chaque commande, il vérifie si la commande saisie est "exit" à l'aide de strcmp(). Si l'utilisateur tape "exit", le programme sort de la boucle infinie (break;), ce qui arrête l'exécution continue des commandes.
Une fois que l'utilisateur tape "exit" et que le programme sort de la boucle, il affiche un message de sortie "Bye bye...".

# Question 4 
Affichage du code de retour (ou du signal) de la commande précédente dans le prompt :
-> Récupération du statut de fin du processus enfant : Il vérifie d'abord si le processus enfant s'est terminé normalement (WIFEXITED(status)).Si oui, il récupère le code de sortie (exit_status = WEXITSTATUS(status)) et met à jour le drapeau ret_ex pour indiquer qu'une sortie s'est produite (ret_ex = 1).
Sinon, s'il a été terminé par un signal (WIFSIGNALED(status)), il récupère le numéro de signal (signal_status = WTERMSIG(status)) et met à jour le drapeau ret_sig pour indiquer qu'un signal s'est produit (ret_sig = 1).

->Vérification et affichage des informations de statut (signal ou sortie) :Il vérifie si un signal (ret_sig) ou une sortie (ret_ex) s'est produit lors de l'exécution de la commande précédente.
    *Si un signal s'est produit (ret_sig == 1), il formate et affiche un message contenant des informations sur le signal à l'aide de sprintf(). Ensuite, il utilise write() pour afficher ce message sur la sortie standard.
    Si une sortie s'est produite (ret_ex == 1), il formate et affiche un message contenant des informations sur la sortie à l'aide de sprintf(). Ensuite, il utilise write() pour afficher ce message sur la sortie standard.
En résumé, ce bloc de code affiche le prompt du shell, vérifie et affiche les informations sur le statut de fin du processus précédent, à savoir s'il s'est terminé par un signal ou une sortie, fournissant ainsi des informations de suivi à l'utilisateur sur les exécutions précédentes.


#Question 5
Dans cette partie on s'intéresse à mesure le temps d’exécution de la commande. Avant l'exécution de chaque commande, clock_gettime(CLOCK_REALTIME, &start); est appelé pour enregistrer le temps de départ.
Après l'exécution de la commande (après wait(&status);), clock_gettime(CLOCK_REALTIME, &stop); est utilisé pour enregistrer le temps d'arrêt.
Ensuite, accum = (stop.tv_nsec - start.tv_nsec)/1000000; calcule la différence de temps en millisecondes entre le début et la fin de l'exécution de la commande.
Les informations sur le statut (signal ou sortie) sont maintenant accompagnées du temps d'exécution de la commande. Ces informations sont ajoutées au message affiché après le prompt du shell à l'aide de sprintf().


#Question 6
une nouvelle logique a été ajoutée pour gérer les commandes avec des arguments. Plutôt que d'exécuter directement la commande entière saisie, le code divise maintenant la commande en tokens (mots) pour identifier la commande et ses arguments distincts.
La fonction strtok() est utilisée pour diviser la chaîne de caractères en tokens séparés par des espaces. Les tokens sont ensuite stockés dans un tableau de pointeurs args.

Une fois que la commande et ses arguments sont identifiés et stockés dans args, la fonction execvp() est appelée pour exécuter la commande avec ses arguments. Cette fonction prend le nom du programme et un tableau d'arguments comme paramètres et remplace le programme en cours par le nouveau programme spécifié.
Si execvp() échoue (retournant une valeur), le processus enfant sort avec exit(EXIT_FAILURE).

Ces ajouts permettent au shell de prendre en charge des commandes plus complexes, permettant à l'utilisateur de saisir des commandes avec des arguments.

#Question 7
Pour la partie de gestion des redirections avec les symboles < et >, ce code permet de rediriger le flux d'entrée (stdin) et de sortie (stdout) pour les commandes saisies dans le shell. 

*1/Détection et traitement des redirections :
Le code utilise strtok() pour séparer la commande saisie en deux parties selon les symboles < et >. Ces symboles indiquent des redirections de flux.
Si un symbole > est détecté, le code sépare la commande en deux parties : la partie gauche avant > et la partie droite après >. La partie droite est considérée comme le nom du fichier de sortie.
Ensuite, le code ouvre ce fichier en écriture (O_CREAT | O_WRONLY | O_TRUNC) et redirige la sortie standard (STDOUT_FILENO) vers ce fichier en utilisant dup2().
        
Si un symbole < est détecté, le code sépare la commande en deux parties : la partie gauche avant < et la partie droite après <. La partie droite est considérée comme le nom du fichier d'entrée.
Ensuite, le code ouvre ce fichier en lecture (O_RDONLY) et redirige l'entrée standard (STDIN_FILENO) depuis ce fichier en utilisant dup2().

*2/Exécution de la commande modifiée :
Après avoir traité les redirections, le code exécute la commande en utilisant execvp() avec les arguments appropriés.
Si la commande nécessite une redirection de fichier, la redirection est établie avant l'exécution de la commande en modifiant les descripteurs de fichiers avec dup2().


#Question 8
La partie du code qui concerne les pipes (|) permet de gérer les redirections de flux entre les commandes, en créant un pipeline où la sortie de la première commande est utilisée comme entrée pour la seconde.

*1/Détection et traitement des pipes (|) :
Le code utilise strtok() pour séparer la commande saisie en deux parties selon le symbole |. Ces symboles indiquent la création d'un pipeline pour la redirection de la sortie d'une commande vers l'entrée d'une autre.
Si un symbole | est détecté, le code sépare la commande en deux parties : la partie gauche avant | et la partie droite après |.

*2/Création du pipeline :
Un tube (pipe) est créé à l'aide de la fonction pipe(). Ce tube permettra la communication entre les deux processus enfants.
Un processus enfant est créé pour exécuter la première partie de la commande (avant |). La sortie de ce processus est redirigée vers l'entrée du tube à l'aide de dup2().
La lecture du tube est redirigée vers l'entrée standard (STDIN_FILENO) du second processus enfant à l'aide de dup2().

*3/Exécution des commandes dans le pipeline :
Le premier processus enfant exécute la première partie de la commande, dont la sortie est redirigée vers l'entrée du tube.
Le deuxième processus enfant exécute la deuxième partie de la commande (après |), utilisant la sortie du tube comme entrée.
Les deux processus enfants exécutent leurs commandes respectives via execvp().

Le processus parent attend la fin de l'exécution des processus enfants via wait().Après chaque exécution, le temps d'exécution est mesuré avec clock_gettime().


