#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure de l'historique d'opérations
typedef struct histo {
    int content; // Contenu de l'opération (montant)
    struct histo *next; // Pointeur vers l'opération suivante
} histo;

// Structure de la file d'opérations
typedef struct {
    histo *head, *tail; // Pointeurs vers la tête et la queue de la file
} operation;

// Fonction de création d'une nouvelle file d'opérations
operation *queue_new(void) {
    operation *q = malloc(sizeof(operation));
    if (!q) return NULL;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

// Fonction d'ajout d'une opération à la file
int histo_add(operation *q, int op) {
    histo *e = malloc(sizeof(histo));
    if (!e) return -1;
    e->content = op;
    e->next = NULL;
    if (q->tail) q->tail->next = e;
    else q->head = e;
    q->tail = e;
    return 0;
}

// Structure d'un compte bancaire
typedef struct compte {
    char type_compte[30]; // Type de compte
    operation *hist; // Historique des opérations
    float solde; // Solde du compte
} compte;

// Structure d'un client
typedef struct {
    char id[7]; // Identifiant du client
    char nom[50]; // Nom du client
    char prenom[50]; // Prénom du client
    char code[8]; // Code secret du client
    compte *cpmt; // Compte du client
} client;

// Structure d'un élément dans une table de clients
typedef struct tableclient {
    client *cl; // Client
    struct tableclient *suiv; // Pointeur vers le client suivant dans la table
} tableclient;

// Fonction de recherche d'un client par son identifiant dans une table
tableclient *chercher_client(tableclient *tcl, char *idcl) {
    while (tcl != NULL) {
        if (strcmp(tcl->cl->id, idcl) == 0) {
            return tcl;
        }
        tcl = tcl->suiv;
    }
    return NULL;
}

// Fonction d'ajout d'un nouveau client à la table
void ajouter_client(tableclient **tcl) {
    tableclient *newNode = malloc(sizeof(tableclient));
    if (newNode == NULL) {
        printf("Erreur : Échec d'allocation mémoire pour tableclient.\n");
        exit(EXIT_FAILURE);
    }

    newNode->cl = malloc(sizeof(client));
    if (newNode->cl == NULL) {
        printf("Erreur : Échec d'allocation mémoire pour client.\n");
        free(newNode);
        exit(EXIT_FAILURE);
    }

    printf("Entrez votre identifiant : ");
    scanf("%6s", newNode->cl->id);
    printf("Entrez votre nom : ");
    scanf("%49s", newNode->cl->nom);
    printf("Entrez votre prénom : ");
    scanf("%49s", newNode->cl->prenom);

    newNode->cl->cpmt = malloc(sizeof(compte));
    if (newNode->cl->cpmt == NULL) {
        printf("Erreur : Échec d'allocation mémoire pour compte.\n");
        free(newNode->cl);
        free(newNode);
        exit(EXIT_FAILURE);
    }
    printf("Entrez le type de compte : ");
    scanf("%29s", newNode->cl->cpmt->type_compte );
    newNode->cl->cpmt->hist = queue_new();
    printf("Entrez votre dépôt initial : ");
    float montant;
    scanf("%f", &montant);
    newNode->cl->cpmt->solde = montant;
    histo_add(newNode->cl->cpmt->hist, montant);
    newNode->suiv = *tcl;
    *tcl = newNode;
}

// Fonction d'affichage de l'historique d'un compte
void historique_compte(char *idclient, tableclient *tcl) {
    tableclient *p = chercher_client(tcl, idclient);
    if (p == NULL) {
        printf("Client non trouvé.\n");
        return;
    }

    histo *current = p->cl->cpmt->hist->head;
    while (current != NULL) {
        printf("%d\n", current->content);
        current = current->next;
    }
}

// Fonction de suppression d'un compte
void supprimerCompte(char *idclient, tableclient **tcl, char *codesecret) {
    if (*tcl == NULL) {
        printf("Aucun client à supprimer.\n");
        return;
    }

    tableclient *current = *tcl;
    tableclient *prev = NULL;

    while (current != NULL && strcmp(current->cl->id, idclient) != 0) {
        prev = current;
        current = current->suiv;
    }

    if (current == NULL) {
        printf("Client non trouvé.\n");
        return;
    }

    // Vérifier le code secret
    // ...

    if (prev == NULL) {
        *tcl = current->suiv;
    } else {
        prev->suiv = current->suiv;
    }

    // Libérer la mémoire
    free(current->cl->cpmt->hist);
    free(current->cl->cpmt);
    free(current->cl);
    free(current);
}

// Fonction d'affichage des détails d'un compte
void afficherDetailsCompte(char *idc, tableclient *tbcl) {
    tableclient *p = chercher_client(tbcl, idc);
    if (p == NULL) {
        printf("Client non trouvé.\n");
        return;
    }

    printf("ID : %s\n", p->cl->id);
    printf("Nom : %s %s\n", p->cl->nom, p->cl->prenom);
    printf("Type de compte : %s\n", p->cl->cpmt->type_compte);
    printf("Solde : %.2f\n", p->cl->cpmt->solde);
}

// Fonction d'affichage de la liste des clients
void afficherListeClients(tableclient *cl) {
    if (cl == NULL) {
        printf("Aucun client à afficher.\n");
        return;
    }

    while (cl != NULL) {
        printf("ID : %s, Nom : %s %s\n", cl->cl->id, cl->cl->nom, cl->cl->prenom);
        cl = cl->suiv;
    }
}

// Fonction de dépôt sur un compte client
void versement_Compte(char *idclient, float versement, tableclient *tcl) {
    tableclient *p = chercher_client(tcl, idclient);
    if (p == NULL) {
        printf("Client non trouvé.\n");
        return;
    }
    p->cl->cpmt->solde += versement;
    histo_add(p->cl->cpmt->hist, versement);
}

// Fonction de retrait d'un compte client
void retrait_Compte(char *idclient, float retrait, tableclient *tcl) {
    tableclient *p = chercher_client(tcl, idclient);
    if (p == NULL) {
        printf("Client non trouvé.\n");
        return;
    }
    if (p->cl->cpmt->solde < retrait) {
        printf("Solde insuffisant.\n");
        return;
    }
    p->cl->cpmt->solde -= retrait;
    histo_add(p->cl->cpmt->hist, -retrait);
}

// Fonction d'affichage du menu
void Menu() {
    printf("\n\t\t _ Bienvenue Chez E-BANK __  \n");
    printf("\t\t========BIENVENUE========\n\n");
    printf("\n  ========================\n\t>< MENU ><\n  ========================\n\n");
    printf("[1]. Créer un nouveau compte.\n");
    printf("[2]. Versement.\n");
    printf("[3]. Retrait.\n");
    printf("[4]. Supprimer un compte existant.\n");
    printf("[5]. Afficher les détails d'un compte existant.\n");
    printf("[6]. Afficher la liste des clients.\n");
    printf("[7]. Afficher Mon historique;\n");
    printf("[0]. Quitter\n\n");
    printf("  ========================\n\n");
    printf("Choisissez une option : ");
}

// Fonction principale du programme
int main() {
    tableclient *table_client = NULL;
    int choix;
    char id_client[7];
    float montant;
    char code[8];

    do {
        Menu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouter_client(&table_client);
                break;
            case 2:
                printf("Entrez l'identifiant du client : ");
                scanf("%6s", id_client);
                printf("Entrez le montant du dépôt : ");
                scanf("%f", &montant);
                versement_Compte(id_client, montant, table_client);
                break;
            case 3:
                printf("Entrez l'identifiant du client : ");
                scanf("%6s", id_client);
                printf("Entrez le montant du retrait : ");
                scanf("%f", &montant);
                retrait_Compte(id_client, montant, table_client);
                break;
            case 4:
                printf("Entrez l'identifiant du client : ");
                scanf("%6s", id_client);
                printf("Entrez le code secret : ");
                scanf("%7s", code);
                supprimerCompte(id_client, &table_client, code);
                break;
            case 5:
                printf("Entrez l'identifiant du client : ");
                scanf("%6s", id_client);
                afficherDetailsCompte(id_client, table_client);
                break;
            case 6:
                afficherListeClients(table_client);
                break;
            case 7:
                printf("Entrez l'identifiant du client : ");
                scanf("%6s", id_client);
                historique_compte(id_client, table_client);
                break;
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 0);

    // Libérer la mémoire avant de quitter
    // ...

    return 0;
}

