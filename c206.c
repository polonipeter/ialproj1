
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {

    //inicializacia zoznamu
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    DLLElementPtr elementptr = list->firstElement;//prvy prvok v zozname
    while(elementptr!=list->lastElement){//iterujem kym prvy prvok nie je zaroven posledny
        list->firstElement = list->firstElement->nextElement; //druhy element sa stal prvym
        free(elementptr);//odstranenie prveho elementu
        elementptr = list->firstElement;//kopia prveho element

    }
    elementptr = list->firstElement;
    free(elementptr);//vymazanie posledneho elmenty
    //inicializacia zoznamu
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    DLLElementPtr newElement =  (DLLElementPtr)malloc(sizeof(struct DLLElement));//alokacia miesta pre novy element
    if(newElement == NULL){DLL_Error();return;}//chybna alokacia
    newElement->data = data;
    newElement->previousElement = NULL;//prvy element pred sebou iny element nema
    newElement->nextElement = list->firstElement;//novy element bude ukazovat na druhy element
    if(list->firstElement!=NULL){list->firstElement->previousElement = newElement;}//stary prvy element sa stane druhym
    else{list->lastElement=newElement;}//pokial je jedinym elementom je zaroven aj poslednym
    list->firstElement = newElement;//prvy element je novy element
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    DLLElementPtr newElement =  (DLLElementPtr) malloc(sizeof(struct DLLElement));//alokacia miesta pre novy element
    if(newElement == NULL){DLL_Error();}//neuspesna alokacia
    newElement->data = data;
    newElement->previousElement = list->lastElement;//pred novym elementom bude posledny
    newElement->nextElement = NULL;//novy element bude posledny teda nema nasledovnika
    if(list->lastElement!=NULL){list->lastElement->nextElement = newElement;}//posledny element ukazuje na novy
    else{list->firstElement=newElement;}//pokial je jedinym elementom je aj prvym
    list->lastElement=newElement;//posledny element je novy element
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    list->activeElement = list->firstElement;//akitvny prvok je prvy
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
   list->activeElement = list->lastElement;//aktivny prvok je posledny
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    if(list->firstElement==NULL){DLL_Error();}//pokial je zoznam prazdny
    else{*dataPtr = list->firstElement->data;}// ak nie je prazdny ziska hodnotu prveho elementu
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    if(list->lastElement==NULL){DLL_Error();}//pokial je prazdny
    else{*dataPtr = list->lastElement->data;}//pokial nie je prazdny ziska hodnotu posledneho elementu
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    DLLElementPtr elementptr;
    if(list->firstElement!=NULL){//pokial zoznam nie je prazdny
        elementptr = list->firstElement;
        if(list->activeElement == list->firstElement){list->activeElement = NULL;}//strata aktivity
        if(list->firstElement == list->lastElement){list->firstElement = NULL; list->lastElement = NULL;}//pokial bol jedinym elementom
        else{list->firstElement = list->firstElement->nextElement; list->firstElement->previousElement = NULL;}//prvym elementom sa stava druhy element
        free(elementptr);}//odstranenie zo zoznamu
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    DLLElementPtr elementptr;
    if(list->lastElement!=NULL){//pokial zoznam nie je prazdny
         elementptr = list->lastElement;
         if(list->activeElement==list->lastElement){list->activeElement = NULL;}//strata aktivity
         if(list->firstElement == list->lastElement){list->firstElement = NULL; list->lastElement = NULL;}//pokial bol jedinym elementom
         else{list->lastElement = list->lastElement->previousElement; list->lastElement->nextElement = NULL;}//poslednym elementom sa stava predposledny
         free(elementptr);} //odstranenie zo zoznamu
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    if(list->activeElement!=NULL){//pokial zoznam nie je prazdny
        if(list->activeElement->nextElement!=NULL){//pokial nie je aktivny prvok
            DLLElementPtr elementptr;
            elementptr = list->activeElement->nextElement;
            list->activeElement->nextElement = elementptr->nextElement;//zarucenie ze aktivny prvok bude ukazovat na suseda ruseneho prvku
            if(elementptr == list->lastElement){list->lastElement = list->activeElement;}//ak je ruseny posledny prvok poslednym sa stava aktivny
            else{elementptr->nextElement->previousElement = list->activeElement;}//zarucenie ze na aktivny prvok bude ukazovat sused ruseneho
            free(elementptr);//odstranenie ruseneho prvku
        }
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if(list->activeElement!=NULL){//pokial zoznam nie je prazdny
        if(list->activeElement->previousElement!=NULL){//pokial nie je aktivny prvok
            DLLElementPtr elementptr;
            elementptr = list->activeElement->previousElement;
            list->activeElement->previousElement = elementptr->previousElement;//zarucenie ze aktivny prvok bude ukazovat na suseda ruseneho prvku
            if(elementptr == list->firstElement){list->firstElement = list->activeElement;}//ak je ruseny prvy prvok prvym sa stava aktivny
            else{elementptr->previousElement->nextElement = list->activeElement;}//zarucenie ze na aktivny prvok bude ukazovat sused ruseneho
            free(elementptr);//odstranenie ruseneho prvku
        }
    }

}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    if(list->activeElement!=NULL){
        DLLElementPtr newElement =  (DLLElementPtr) malloc(sizeof(struct DLLElement));//alokacia miesta pre novy prvok
        if(newElement == NULL){DLL_Error();}//chybna alokacia
        newElement->data = data;
        newElement->nextElement = list->activeElement->nextElement;//ukazuje na suseda aktivneho prvku
        newElement->previousElement = list->activeElement;//ukazuje na aktivny element
        list->activeElement->nextElement = newElement;//aktivny prvok ukazuje na novy element
        if(list->activeElement == list->lastElement){list->lastElement = newElement;}//pokial je aktivny prvok posledny poslednym sa stane novy
        else{newElement->nextElement->previousElement = newElement;}//element za novym prvkom ukazuje na novy prvok
     }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    if(list->activeElement!=NULL){
        DLLElementPtr newElement =  (DLLElementPtr) malloc(sizeof(DLLElementPtr));//alokacia miesta pre novy prvok
        if(newElement == NULL){DLL_Error();}//chybna alokacia
        newElement->data = data;
        newElement->nextElement = list->activeElement;//ukazuje na aktivny prvok
        newElement->previousElement = list->activeElement->previousElement;//ukazuje na suseda aktivneho prvku
        list->activeElement->previousElement = newElement;//aktivny prvok na neho ukazuje
        if(list->activeElement == list->firstElement){list->firstElement = newElement;}//pokial bol aktivny prvok prvym prvym sa stane novy prvok
        else{newElement->previousElement->nextElement = newElement;}//prvok pred novym prvkom na neho ukazuje
     }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    if(list->activeElement==NULL){DLL_Error();}//pokial je zoznam neaktivny
    else{*dataPtr = list->activeElement->data;}//ziska hodnotu aktivneho prvku
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    if(list->activeElement!=NULL){//pokial je zoznam aktivny
        list->activeElement->data = data;//zmeni obsah aktivneho prvku
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    if(list->activeElement!=NULL){//pokial je zoznam aktivny
        if(list->activeElement == list->lastElement){list->activeElement = NULL;}////ak je aktivny prvok posledny zoznam straca aktivitu
        else{list->activeElement = list->activeElement->nextElement;}//posunie aktivitu na dalsi prvok
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    if(list->activeElement!=NULL){//ak je zoznam aktivny
        if(list->activeElement == list->firstElement){list->activeElement = NULL;}//ak je aktivny prvok prvy zoznam straca aktivitu
        else{list->activeElement = list->activeElement->previousElement;}//posunie aktivitu na predosly prvok
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {

    return list->activeElement!=NULL;//ak aktivny prvok nie je NULL vracia sa 1
}

/* Konec c206.c */
