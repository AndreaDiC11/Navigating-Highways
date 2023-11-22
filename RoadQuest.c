#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Variabili Globale
int stampa = 0;//Mi serve per gestire le stampe di nessun percorso
int corretto = 0;//Mi serve per vedere se prende la stazione d'inizio
int stazpercorso = 0;

typedef struct Macchina
{
    int benza;
    struct Macchina *next;
}Macchina;

typedef struct Percorso
{
    int valore;
    int benza;
    struct Percorso *next;
}Percorso;

typedef struct Stazione
{
    int dist;
    struct Stazione *left, *right, *parent;
    Macchina *head;
    
}Stazione;

Stazione *StazioneBase = NULL;

Stazione *newStaz(int distanza, Stazione *parent)
{
    Stazione *ptrStaz = NULL;
    ptrStaz = (Stazione *)malloc(sizeof(Stazione));
    ptrStaz->dist = distanza;
    ptrStaz->head = NULL;
    ptrStaz->left = ptrStaz->right = NULL;
    ptrStaz->parent = parent;//salvo il padre
    return ptrStaz;
}

Stazione *insertStaz(Stazione *staz, int dist, Stazione *parent)
{
    if(staz == NULL) return newStaz(dist, parent);
    //Gestione inserimento

    if (dist < staz->dist)
    {
        staz->left = insertStaz(staz->left, dist, staz);
    }
    else
    {
        staz->right = insertStaz(staz->right, dist, staz);
    }
    //staz->left = staz->right = NULL; non penso proprio
    return staz;
}

Stazione *cercaStazione(int foundStat, Stazione *staz)
{
    Stazione *ptrStaz = NULL;
    ptrStaz = staz;
    //Scorri il BST e cerca la stazione
    if(ptrStaz == NULL)
    {
        return ptrStaz;
    }
    else if(ptrStaz->dist > foundStat)
    {
        return cercaStazione(foundStat, ptrStaz->left);
    }
    else if(ptrStaz->dist < foundStat)
    {
        return cercaStazione(foundStat, ptrStaz->right);
    }
    return ptrStaz;


}

char *aggiungi_macchina(Stazione *staz, int autonomia)
{
    Macchina *ptrMacc = NULL;
    Macchina *ptrMacc2 = NULL;
    //inserisci in lista
    if(staz != NULL && staz->head == NULL)//caso primo nodo
    {
        staz->head = NULL;
        staz->head = (Macchina *)malloc(sizeof(Macchina));
        if(staz != NULL && staz->head != NULL)
        {
            staz->head->benza = autonomia;
            staz->head->next = NULL;
            return "aggiunta\n";
        }
    }
    else if(staz != NULL && staz->head != NULL && staz->head->benza < autonomia)//caso primo valore da inserire maggiore va in testa
    {
        ptrMacc = staz->head;
        staz->head = NULL;
        staz->head = (Macchina *)malloc(sizeof(Macchina));
        if(staz != NULL && staz->head != NULL)
        {
            staz->head->benza = autonomia;
            staz->head->next = ptrMacc;
            return "aggiunta\n";
        }
    }
    else
    {
        if(staz != NULL)
        {
            ptrMacc = staz->head;
        }
        while(ptrMacc != NULL && ptrMacc->next != NULL)
        {
            if(ptrMacc != NULL && ptrMacc->next != NULL && ptrMacc->next->benza < autonomia)
            {
                ptrMacc2 = ptrMacc->next;
                ptrMacc->next = NULL;
                ptrMacc->next = (Macchina *)malloc(sizeof(Macchina));
                if(ptrMacc != NULL && ptrMacc->next != NULL)
                {
                    ptrMacc->next->benza = autonomia;
                    ptrMacc->next->next = ptrMacc2;
                    return "aggiunta\n";
                }
            }
            if(ptrMacc != NULL)
            {
                ptrMacc = ptrMacc->next;
            }
        }
        if(ptrMacc != NULL)
        {
            ptrMacc->next = NULL;
            ptrMacc->next = (Macchina *)malloc(sizeof(Macchina));
            if(ptrMacc != NULL && ptrMacc->next != NULL)
            {
                ptrMacc->next->benza = autonomia;
                ptrMacc->next->next = NULL;
                return "aggiunta\n";
            }
        }
    }
    //freeMacchina(ptrMacc);
    //freeMacchina(ptrMacc2);
    return "non aggiunta\n";
    
}

char *addCar()//utilizzo per passare la stazione e l'autonomia della macchina da aggiungere
{
    int diststaz;
    int autonomia;
    Stazione *staz = NULL;
    staz = StazioneBase;
    if(scanf("%d %d", &diststaz, &autonomia)){};
    Stazione *esisteStaz = NULL;
    esisteStaz = cercaStazione(diststaz,staz);
    if(esisteStaz == NULL)//verifica se esiste la stazione
    {
        return "non aggiunta\n";
    }
    else
    {
        return aggiungi_macchina(esisteStaz,autonomia);
    }
}

void freeLista(Percorso *testa)
{
    //Percorso *current = testa;
    Percorso *succ = NULL;
    while(testa != NULL)
    {
        succ = testa->next;
        free(testa);
        testa = NULL;
        testa = succ;
    }
}

void freeMacchina(Macchina *testa)
{
    Macchina *succ;
    while(testa != NULL)
    {
        succ = testa->next;
        free(testa);
        testa = NULL;
        testa = succ;
    }
}

void freeAlbero(Stazione *root)
{
    if(root == NULL)
    {
        return;
    }

    freeAlbero(root->left);
    freeAlbero(root->right);

    freeMacchina(root->head);
    free(root);
    root = NULL;
}

char *aggiungi_stazione()
{
    
    int diststaz = 0;
    int num_auto = 0;
    int autonomia = 0;
    Stazione *ptrStaz = NULL;    
    if(scanf("%d %d", &diststaz, &num_auto)){};
    if(cercaStazione(diststaz,StazioneBase) != NULL)
    {
        if(cercaStazione(diststaz,StazioneBase)->dist == diststaz)//cerca se la stazione è già presente
        {
            return "non aggiunta\n";
        }
    }
    if(StazioneBase == NULL)//Se è la prima stazione salvala come root
    {
        StazioneBase = newStaz(diststaz, NULL);
        ptrStaz = StazioneBase;//Crea il ptr che andrà in giro
    }
    else
    {
        insertStaz(StazioneBase, diststaz, StazioneBase);
        ptrStaz = cercaStazione(diststaz,StazioneBase);
    }
    if(num_auto <= 512)//Devono essere massimo 512 auto in una stazione
    { 
        if(num_auto != 0)
        {
            for(int i=0; i < num_auto; i++)//loopa per ogni macchina in modo da salvare l'autonomia di tutte
            {
                if(scanf("%d", &autonomia)){};
                aggiungi_macchina(ptrStaz, autonomia);     
            }
        }
        else if(num_auto == 0)
        {
            ptrStaz->head = NULL;
        }
        //va messa free(ptrStaz)??non penso
        return "aggiunta\n";
    }
    else
    {
        //va messa free(ptrStaz)??
        return "non aggiunta\n";
    }
}


char *distruggi_stazione(int diststaz)//ricordati le free
{
    Stazione *staz = NULL;
    staz = StazioneBase;
    Stazione *stazremove = NULL;
    stazremove = cercaStazione(diststaz,staz);
    if(stazremove != NULL)    
    {
        if(stazremove->dist == diststaz)//se la stazione esiste rimuovila
        {
            //Caso con nessun figlii
            if(stazremove->left == NULL && stazremove->right == NULL)
            {
                if(stazremove->parent == NULL)//Se è la radice da rimuovere
                {
                    freeMacchina(stazremove->head);
                    stazremove->parent = NULL;//forse rimovibile
                    free(stazremove);
                    stazremove = NULL;
                    StazioneBase = NULL;//forse rimovibile
                    return "demolita\n";
                }
               Stazione *padre = NULL;
               padre = stazremove->parent;
               //Se stazremove è figlio destro
               if(stazremove == padre->right)
               {
                    stazremove->parent->right = NULL;
                    freeMacchina(stazremove->head);
                    free(stazremove);
                    stazremove = NULL;
               }
               //Se stazremove è figlio sinistro
               else 
               {
                    stazremove->parent->left = NULL;
                    freeMacchina(stazremove->head);
                    free(stazremove);
                    stazremove = NULL;
               }
               return "demolita\n";
            }
            //Caso Nodo con un solo filgio
            else if(stazremove->left == NULL) //se non ha figli a sinistra 
            {
                if(stazremove->parent == NULL)//Se è la radice da rimuovere
                {
                    StazioneBase = stazremove->right;
                    freeMacchina(stazremove->head);
                    free(stazremove);
                    stazremove = NULL;
                    StazioneBase->parent = NULL;
                    return "demolita\n";
                }
                Stazione *padre = NULL;
                padre = stazremove->parent;
                Stazione *figlio = NULL;
                figlio = stazremove->right;
                //se la stazione da rimuovere è figlio sinistro o destro
                if(stazremove == padre->left)
                {
                    padre->left = figlio;
                }
                else
                {
                    padre->right = figlio;
                }
                figlio->parent = padre;
                freeMacchina(stazremove->head);
                free(stazremove);
                stazremove = NULL;
                return "demolita\n";
            }
            else if(stazremove->right == NULL)//se non ha figli a destra 
            {
                //Stazione *figlio = stazremove->left;
                if(stazremove->parent == NULL)//Se è la radice da rimuovere
                {
                    StazioneBase = stazremove->left;
                    freeMacchina(stazremove->head);
                    free(stazremove);
                    stazremove = NULL;
                    StazioneBase->parent = NULL;
                    return "demolita\n";
                }
                Stazione *padre = NULL;
                padre = stazremove->parent;
                Stazione *figlio = NULL;
                figlio = stazremove->left;
                //se la stazione da rimuovere è figlio sinistro o destro
                if(stazremove == padre->left)
                {
                    padre->left = figlio;
                }
                else
                {
                    padre->right = figlio;
                }
                figlio->parent = padre;
                freeMacchina(stazremove->head);
                free(stazremove);
                stazremove = NULL;
                return "demolita\n";
            }
            else//caso con 2 figli
            {
                Stazione *successore = NULL;
                successore = stazremove->right;
                //Se il successore non ha figli a sinistra ma solo a destra allora
                if(successore->left == NULL)
                {
                    stazremove->dist = successore->dist;
                    freeMacchina(stazremove->head);
                    stazremove->head = NULL;
                    stazremove->head = successore->head;
                    stazremove->right = successore->right;
                    if(successore->right != NULL)
                    {
                    successore->right->parent = stazremove;
                    }
                    //freeMacchina(successore->head);
                    free(successore);
                    successore = NULL;
                    return "demolita\n";
                }
                //fino a quando non arrivi al valore piu vicino a quello da rimuovere, ma piu grande di lui
                while(successore->left != NULL)
                {
                    successore =successore->left;
                }
                //sostituisci il valore da rimuovere con quello del suo successore e poi rimuovi il successore
                stazremove->dist = successore->dist;
                freeMacchina(stazremove->head);
                stazremove->head = NULL;
                stazremove->head = successore->head;
                successore->parent->left = successore->right;
                if(successore->right != NULL)
                {
                successore->right->parent = successore->parent;
                }
                //freeMacchina(successore->head);
                free(successore);
                successore = NULL;
                return "demolita\n";
            }
        }
    }
    return "non demolita\n";
}


char *rottama_macchina(Stazione *staz,int autonomia )
{
    if(staz->head == NULL)//caso lista vuota
    {
        return "non rottamata\n";
    }
    else if(staz->head->benza == autonomia)//caso primo valore della lista
    {
        Macchina *current = NULL;
        current = staz->head;
        staz->head = staz->head->next;
        free(current);
        return "rottamata\n";
    }
    else
    {
        Macchina *prec = NULL;
        prec = staz->head;
        Macchina *foundauto = NULL;
        foundauto = staz->head->next;
        //caso dal secondo valore della lista in poi
        while(foundauto != NULL)
        {
            if(foundauto->benza == autonomia)
            {
                //cancella auto dalla lista ti servirà il precedente
                prec->next = foundauto->next;
                free(foundauto);
                return "rottamata\n";
            }
            else
            {
                prec = prec->next;
                foundauto = foundauto->next;
            }
        }
    }
    return "non rottamata\n";
    
}

char *removeCar()//utilizzo per passare la stazione e l'autonomia della macchina da rimuovere
{
    int diststaz;
    int autonomia;
    Stazione *staz = NULL;
    staz = StazioneBase;
    if(scanf("%d %d", &diststaz, &autonomia)){};
    Stazione *esisteStaz = NULL;
    esisteStaz = cercaStazione(diststaz,staz);
    if(esisteStaz == NULL)//verifica se esiste la stazione
    {
        return "non rottamata\n";
    }
    else
    {
        return rottama_macchina(esisteStaz,autonomia);
    }
}

Percorso *aggiungiLista(Percorso *Lista, int dist, int autonomia)
{
    Percorso *new = NULL;
    new = (Percorso *)malloc(sizeof(Percorso));
    new->valore = dist;
    new->benza = autonomia;
    new->next = NULL;

    if(Lista == NULL)
    {
        return new;
    }
    else
    {
        Percorso *current = NULL;
        current = Lista;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new;
        return Lista;
    }
}

Percorso *aggiungiListainTesta(Percorso *Lista, int dist, int autonomia)
{
    Percorso *new = NULL;
    new = (Percorso *)malloc(sizeof(Percorso));
    new->valore = dist;
    new->benza = autonomia;
    new->next = Lista;
    return new;
}

//La destinazione è il punto di arrivo, topstaz la stazione più piccola per arrivare a quel punto
//la partenza è lo start, staz parte dalla testa dell'albero e attraversa tutto l'albero
void scorrialbero1(Stazione *destinazione, Stazione *topstaz, Stazione *partenza, Stazione *staz)
{
    
    //scorri e ad ogni stazione salvati quella stazione solo se arriva alla destinazione,
    //poi alla prossima che incontri salvati la piu piccola che arriva a quella destinazione
    if(staz == NULL)
    {
        corretto = 2;
        return;
    }
    //Se la stazione con la benza della macchina arrivano alla fine ed è maggiore o uguale alla partenza
    if(staz->head != NULL)
    {
        if(staz->dist + staz->head->benza >= destinazione->dist && staz->dist >= partenza->dist)
        {
            //Se la stazione è piu vicina alla partenza(che è 20) allora aggiorna topstaz
            if(staz->dist < topstaz->dist)
            {
                topstaz = staz;
                if(topstaz->dist < stazpercorso)
                {
                    stazpercorso = topstaz->dist;
                }
            }
        }
    }
    //Condizione di uscita con percorso trovato
    if(stazpercorso == partenza->dist)
    {
        corretto = 1;
        return;
    }
    scorrialbero1(destinazione, topstaz, partenza, staz->left);
    corretto = 0;
    //Condizione di uscita se viene trovato il percorso prima di vedere tutta la destra
    if(stazpercorso == partenza->dist)
    {
        corretto = 1;
        return;
    }
    scorrialbero1(destinazione, topstaz, partenza, staz->right);
}

Percorso *attraversaalbero(Stazione *testaAlbero, Percorso *testaLista)
{
    if(testaAlbero == NULL)
    {
        return testaLista;
    }

    testaLista = attraversaalbero(testaAlbero->right, testaLista);
    if(testaAlbero->head != NULL)
    {
    testaLista = aggiungiLista(testaLista, testaAlbero->dist, testaAlbero->head->benza);
    }
    else if (testaAlbero->head == NULL)
    {
    testaLista = aggiungiLista(testaLista, 0, 0);
    }

    testaLista = attraversaalbero(testaAlbero->left, testaLista);

    return testaLista;
}

int cercanelrange(Percorso *testa, int inizio, int fine, int arrivo)
{
    Percorso *ptrTesta = NULL;
    ptrTesta = testa;
    int migliore = inizio;
    //raggiungi l'inizio
    while(ptrTesta!= NULL && ptrTesta->valore != inizio)
    {
        ptrTesta = ptrTesta->next;
    }
    //parti da quello dopo
    if(ptrTesta != NULL)
    {
        ptrTesta = ptrTesta->next;
    }
    while (ptrTesta != NULL && ptrTesta->valore != fine)
    {
        if(ptrTesta->valore - ptrTesta->benza <= arrivo)
        {
            migliore = ptrTesta->valore;
        }
        ptrTesta = ptrTesta->next;
        /*if(ptrTesta == NULL)
        {
            break;
        }*/
    }

    return migliore;
    
}

Percorso *ricercapercorso(Stazione *inizioricerca, int partenza, int arrivo)
{
    Percorso *testaLista = NULL;
    testaLista = attraversaalbero(inizioricerca, testaLista);
    Percorso *ptrLista = NULL;
    ptrLista = testaLista;
    Percorso *corretto = NULL;
    Percorso *Min = NULL;
    Percorso *Max = NULL;
    Percorso *delete = NULL;
    int maxdist = -1;
    int maxbenza = -1;
    int mindist = 0;
    int minbenza = 0;
    while(ptrLista != NULL)
    {
        //Caso iniziale scorri fino alla partenza
        if(Max == NULL)
        {
            /*if(ptrLista == NULL)
            {
                return NULL;
            }*/
            while (ptrLista != NULL && ptrLista->valore != partenza)
            {
                ptrLista = ptrLista->next;
            }
        }
        //Dopo la prima volta scorri fino al ultimo valore che hai salvato in Max(che è la tua nuova partenza)
        else
        {
            ptrLista = testaLista;
            while (ptrLista != NULL && ptrLista->valore != maxdist)
            {
                ptrLista = ptrLista->next;
            }
        }
        if(ptrLista != NULL)
        {
            int spostamento = ptrLista->valore - ptrLista->benza;
            //Fino a quando il valore è maggiore dello spostamento continua a ciclare
            while(ptrLista != NULL && ptrLista->valore >= spostamento)
            {
                //Prima volta salva i valori
                if(maxdist == -1 && maxbenza == -1)
                {
                    if(ptrLista != NULL)
                    {
                        maxdist = ptrLista->valore;
                        maxbenza = ptrLista->benza;
                    }
                    /*else if(ptrLista == NULL)
                    {
                        return NULL;
                    }*/
                }
                //Se lo spostamento in questo caso è minore(quindi mi sposto di più) allora aggiorna le var
                else if(ptrLista != NULL && ptrLista->valore - ptrLista->benza < maxdist - maxbenza)
                {
                    maxdist = ptrLista->valore;
                    maxbenza = ptrLista->benza;
                }
                mindist = ptrLista->valore;
                minbenza = ptrLista->benza;
                ptrLista = ptrLista->next;
                //Se sei arrivato
                if((ptrLista != NULL && ptrLista->valore <= arrivo) || spostamento <= arrivo)
                {
                    //Per evitare il caso in cui spostamento è < 0
                    if(spostamento > arrivo)
                    {
                    Min = aggiungiListainTesta(Min, mindist, minbenza);
                    Max = aggiungiListainTesta(Max, maxdist, maxbenza);
                    }
                    else if(spostamento <= arrivo && Min == NULL && Max == NULL)
                    {
                        corretto = aggiungiListainTesta(corretto, arrivo, 0);
                        corretto = aggiungiListainTesta(corretto, partenza, 0);
                        if(Min != NULL)
                        {
                            freeLista(Min);
                        }
                        Min = NULL;
                        if(Max != NULL)
                        {
                            freeLista(Max);
                        }
                        Max = NULL;
                        if(testaLista != NULL)
                        {
                            freeLista(testaLista);
                        }
                        testaLista = NULL;
                        return corretto;
                    }
                    if(Max != NULL && Max->valore - Max->benza > arrivo)
                    {
                        if(Min != NULL)
                        {
                            freeLista(Min);
                        }
                        Min = NULL;
                        if(Max != NULL)
                        {
                            freeLista(Max);
                        }
                        Max = NULL;
                        if(testaLista != NULL)
                        {
                            freeLista(testaLista);
                        }
                        testaLista = NULL;
                        return NULL;
                    }
                    /*if(Min == NULL || Max == NULL)
                    {
                        return NULL;
                    }*/
                    //Se il minimo ti permette di arrivare
                    while(Min != NULL && Max != NULL)
                    {
                        /*if(Min == NULL)
                        {
                            return NULL;
                        }*/
                        if(Min != NULL && Min->valore - Min->benza <= arrivo)
                        {
                            corretto = aggiungiListainTesta(corretto, Min->valore, Min->benza);
                            arrivo = Min->valore;
                        }
                        else
                        {
                            if(Max != NULL && Min != NULL)
                            {
                            int stampare = cercanelrange(testaLista, Max->valore, Min->valore, arrivo);
                            corretto = aggiungiListainTesta(corretto, stampare, 0);
                            arrivo = stampare;
                            }
                            else
                            {
                                if(Min != NULL)
                                {
                                    freeLista(Min);
                                }
                                Min = NULL;
                                if(Max != NULL)
                                {
                                    freeLista(Max);
                                }
                                Max = NULL;
                                if(testaLista != NULL)
                                {
                                    freeLista(testaLista);
                                }
                                testaLista = NULL;
                                return NULL;
                            }
                        }
                        delete = Max->next; 
                        free(Max);
                        Max = delete;
                        delete = Min->next;
                        free(Min);
                        Min = delete;
                    }
                    if(Min != NULL)
                    {
                        freeLista(Min);
                    }
                    Min = NULL;
                    if(Max != NULL)
                    {
                        freeLista(Max);
                    }
                    Max = NULL;
                    if(testaLista != NULL)
                    {
                        freeLista(testaLista);
                    }
                    testaLista = NULL;
                    return corretto;    
                }
            }
        }
        /*else if(ptrLista == NULL)
        {
            return NULL;
        }*/
        if(Min != NULL && Max != NULL )
        {
            if(mindist == Min->valore || maxdist == Max->valore)
            {
                if(Min != NULL)
                {
                    freeLista(Min);
                }
                Min = NULL;
                if(Max != NULL)
                {
                    freeLista(Max);
                }
                Max = NULL;
                if(testaLista != NULL)
                {
                    freeLista(testaLista);
                }
                testaLista = NULL;
                return NULL;
            }
        }
        Min = aggiungiListainTesta(Min, mindist, minbenza);
        Max = aggiungiListainTesta(Max, maxdist, maxbenza);

    }
    if(Min != NULL)
    {
        freeLista(Min);
    }
    Min = NULL;
    if(Max != NULL)
    {
        freeLista(Max);
    }
    Max = NULL;
    if(testaLista != NULL)
    {
        freeLista(testaLista);
    }
    testaLista = NULL;
    return NULL;
}


void pianifica_percorso(int partenza, int arrivo)
{
    Stazione *start = NULL;
    start = cercaStazione(partenza,StazioneBase);
    Stazione *end = NULL;
    end = cercaStazione(arrivo,StazioneBase);
    if(start == NULL || end == NULL)
    {
        printf("nessun percorso\n");
        return;
    }
    stazpercorso = arrivo;
    Stazione *topstaz = NULL;
    topstaz = end;
    //Caso da 20 a 50 ad esempio
    if(corretto == 2 || start->dist == stazpercorso)
    {
        return;
    }
    else if(partenza < arrivo)
    {   //Ti serve perchè ogni volta che cambi stazione il controllo parte dalla testa    
        Stazione *staz = NULL;
        staz = StazioneBase;
        //Prendi la stazione minore per arrivare a quella stazione di arrivo
        scorrialbero1(end, topstaz, start, staz);
        //Salva la stazione da stampare(topstaz si aggiorna sempre)
        int stampastaz = stazpercorso;
        if(stazpercorso != end->dist)
        {
            corretto = 0;
        }
        //Rifai la pianificazione usando la nuova stazione trovata
        pianifica_percorso(start->dist, stazpercorso);
        //Se sono arrivato all' inizio allora esiste il percorso
        if(start->dist == stazpercorso)
        {
            printf("%d ", stampastaz);
        }
        else 
        {
            //Se stampa = 0 vuol dire che non ho mai stampato nessun percorso
            if(stampa == 0 )
            {
                printf("nessun percorso\n");
                stampa++;
                //si aggiorna anche per le funzioni ricorsive precedenti perchè è var.glob
            }
        }
    }
    //Caso da 50 a 20 ad esempio
    else if (partenza > arrivo)
    {
        Stazione *testa = NULL;
        testa = StazioneBase;
        Percorso *testapercorsomigliore = NULL;
        testapercorsomigliore = ricercapercorso(testa, start->dist, end->dist);
        if(testapercorsomigliore != NULL)
        {
            printf("%d ", partenza);
            Percorso *current = NULL;
            current = testapercorsomigliore;
            while(testapercorsomigliore != NULL)
            {
                if(current->valore != partenza && current->valore != arrivo)
                {
                    printf("%d ", current->valore);
                }
                current = testapercorsomigliore->next;
                free(testapercorsomigliore);
                testapercorsomigliore = current;
            }
            printf("%d\n", arrivo);
        }
        else
        {
            printf("nessun percorso\n");
        }
     
    }
    return;
}




int main(){
    char* richiesta = NULL;
    richiesta = (char*)malloc(sizeof(char)*20);
    if(richiesta == NULL)
    {
        return 0;
    }

    while(scanf("%s", richiesta) != EOF)
    {
        if(strcmp(richiesta,"aggiungi-stazione") == 0)
        {
            printf("%s",aggiungi_stazione());
        }
        else if(strcmp(richiesta,"aggiungi-auto") == 0)
        {
            printf("%s",addCar());
        }
        else if(strcmp(richiesta,"rottama-auto") == 0)
        {
            printf("%s",removeCar());
        }
        else if(strcmp(richiesta,"demolisci-stazione") == 0)
        {
            int diststaz = 0;
            if(scanf("%d", &diststaz)){};
            printf("%s",distruggi_stazione(diststaz));
        }
        else if(strcmp(richiesta,"pianifica-percorso") == 0)
        {
            int distpartenza = 0;
            int distarrivo = 0;
            if(scanf("%d %d", &distpartenza, &distarrivo)){};
            if(distpartenza == distarrivo)
            {
                printf("%d\n", distarrivo);
            }
            else
            {
                if(distpartenza < distarrivo)
                {
                    pianifica_percorso(distpartenza,distarrivo);
                    if(distpartenza == stazpercorso)
                    {
                    printf("%d\n", distarrivo);
                    }
                    stampa = 0;
                    corretto = 0;
                }
                else if(distpartenza > distarrivo)
                {
                    pianifica_percorso(distpartenza,distarrivo);
                }
            }
        }
    }
    free(richiesta);
    richiesta = NULL;
    freeAlbero(StazioneBase);
    StazioneBase = NULL;

    return 0; 
}        
