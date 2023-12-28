#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct histo {
    int content;
    struct histo *next;
} histo;

typedef struct {
    histo *head, *tail;
} operation;

operation *queue_new(void) {
    operation *q;
    q = malloc(sizeof(operation));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

int histo_add(operation *q, int op) {
    histo *e;
    e = malloc(sizeof(histo));
    if (!e)
        return -1;
    e->content = op;
    e->next = NULL;
    if (q->tail)
        q->tail->next = e;
    else
        q->head = e;
    q->tail = e;
    return 0;
}

int *queue_front(operation *q) {
    if (!(q->head))
        return NULL;
    return &q->head->content;
}

typedef struct compte {
    char type_compte[30];
    operation *hist;
    float solde;
} compte;

typedef struct {
    char id[7];
    char nom[50];
    char prenom[50];
    char code[8];
    compte *cpmt;
} client;

typedef struct tableclient {
    client *cl;
    struct tableclient *suiv;
} tableclient;

tableclient *chercher_client(tableclient *tcl, char *idcl) {
    tableclient *iterator;
    iterator = tcl;
    while (iterator->cl != NULL) {
        if (strcmp(iterator->cl->id, idcl) == 0) {
            return iterator;
        } else {
            iterator = iterator->suiv;
        }
    }
    return NULL;
}

void ajouter_client(tableclient *tcl, client *cli) {
    tableclient *tl;
    tl = malloc(sizeof(tableclient));
    tl = tcl;
    if (tcl != NULL) {
        tcl->suiv = tl;
    }
    printf("Tapez votre nom: ");
    scanf("%s", &*cli->nom);
    printf("Tapez votre prénom: ");
    scanf("%s", &*cli->prenom);
    printf("Tapez votre ID: ");
    scanf("%s", &*cli->id);
    strcpy(cli->code, "");
    cli->cpmt->solde = 0;
    cli->cpmt->hist = NULL;
    cli->cpmt->hist = queue_new();
    tcl->cl = cli;
}

void versement_Compte(char *idclient, float versement, tableclient *tcl) {
    tableclient *p;
    p = chercher_client(tcl, idclient);
    if (p != NULL) {
        p->cl->cpmt->solde += versement;
    }
    histo_add(p->cl->cpmt->hist, versement);
}

void retrait_Compte(char *idclient, float retrait, tableclient *tcl) {
    tableclient *p;
    p = chercher_client(tcl, idclient);
    if (p != NULL) {
        p->cl->cpmt->solde -= retrait;
    }
    histo_add(p->cl->cpmt->hist, -retrait);
}

void historique_compte(char *idclient, tableclient *tcl) {
    tableclient *p;
    p = chercher_client(tcl, idclient);
    
    histo *current = p->cl->cpmt->hist->head;
    
    while (current != NULL) {
        printf("%d\n", current->content);
        current = current->next;
    }
}

void supprimerCompte(char *idclient, tableclient *tcl, char *codesecret) {
    tableclient *p;
    client *t;
    p = chercher_client(tcl, idclient);
    t = p->cl;
    while (p->suiv->cl->id != idclient) {
        p = p->suiv;
    }
    if (strcmp(p->cl->code, codesecret) == 0) {
        free(p->cl->cpmt->hist);
        p->suiv = p->suiv->suiv;
        tcl = p;
        t = p->suiv->cl;
        free(t);
    } else {
        printf("Code invalide. Répéter l'opération de nouveau.\n");
    }
}

void afficherDetailsCompte(char *idc, tableclient *tbcl) {
    tableclient *p;
    p = chercher_client(tbcl, idc);
    printf("ID: %s\n", p->cl->id);
    printf("Type de compte: %s\n", p->cl->cpmt->type_compte);
    printf("Solde: %.2f\n", p->cl->cpmt->solde);
}

void afficherListeClients(tableclient *cl) {
    tableclient *p;
    p = cl;
    while (p->suiv != NULL) {
        printf("Nom: %s\n", p->cl->nom);
        printf("Prénom: %s\n", p->cl->prenom);
        printf("\n");
        p = p->suiv;
    }
}

void creerCompte(client *c, tableclient *tcl) {
    printf("Création d'un nouveau compte.\n");
    printf("Choix de type de compte : ");
    scanf("%s", &c->cpmt->type_compte);
    printf("Entrez le solde initial : ");
    scanf("%f", &c->cpmt->solde);
    histo_add(c->cpmt->hist, c->cpmt->solde);
    printf("Compte créé avec succès.\n");
}

void Menu() {
    printf("\n\t\t ____ Bienvenue Chez E-BANK _____  \n");
    printf("\t\t========WELCOME========\n\n");
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

int main() {
    int choix;
    char id_client[7];
    float montant;
    tableclient *table_client = malloc(sizeof(tableclient));
    client *clien_t = malloc(sizeof(client));
    char code_secret[8];
    while(choix!=-1){
    do {
        // Display the menu
        Menu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouter_client(table_client,clien_t);
                creerCompte(clien_t, table_client);
            case 2:
                printf("Saisir votre ID svp: ");
                scanf("%s", id_client);
                printf("Saisir le montant à verser: ");
                scanf("%f", &montant);
                versement_Compte(id_client, montant, table_client);
                
            case 3:
                printf("Saisir votre ID svp: ");
                scanf("%s", id_client);
                printf("Saisir le montant à retirer: ");
                scanf("%f", &montant);
                retrait_Compte(id_client, montant, table_client);
                
            case 4:
                printf("Saisir votre ID svp: ");
                scanf("%s", id_client);
                printf("Saisir votre code secret svp: ");
                scanf("%s", code_secret);
                supprimerCompte(id_client, table_client, code_secret);
                
            case 5:
                printf("Saisir votre ID svp: ");
                scanf("%s", id_client);
                afficherDetailsCompte(id_client, table_client);
                
            case 6:
                afficherListeClients(table_client);
                
            case 7:
                printf("Saisir votre ID svp: ");
                scanf("%s", id_client);
                historique_compte(id_client, table_client);
                
            case 0:
                printf("Au revoir!\n");
                
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);}

    return 0;
}
