#include "cola.h"

void inicie_cola(eCola* c)
{
    c->entrada=0;
    c->salida=0;
    c->peso=0;
}

/*
#define TAM 10
typedef struct eCola {
   DATOCOLA data[TAM];
   int entrada;//pos de entrada en en arreglo
   int salida;// pos de salida en el arreglo
   int peso;
} eCola;
*/

char cola_add(eCola* c, DATOCOLA dato)
{
    // si estoy en un sistema con interrupciones
    // lo más seguro sería apagarlas acá!

    /* ZONA CRÍTICA */
    if (!cola_llena(c)) {
        c->data[c->entrada] = dato;
        c->entrada++;
        c->peso++;
    }
    else {
        return ERROR;
    }
    /* FIN DE ZONA CRÍTICA */

    // si llego hasta acá es porque la cola no estaba
    // llena y se pudo meter el dato en la cola
    if (c->entrada >= TAM)
        c->entrada = 0;
    return BIEN;

    // prender de nuevo las interrupciones
}

DATOCOLA cola_get(eCola* c)
{
    // si estoy en un sistema con interrupciones
    // lo más seguro sería apagarlas acá!
    // cli();//arduinito

    DATOCOLA temp;

    /* ZONA CRÍTICA */
    if (!cola_vacia(c)) {
        temp = c->data[c->salida];
        c->salida++;
        c->peso--;
    }
    else {
        return ERROR; // esto dependería de la aplicación reservar un caracter para el error. También se puede usar una bandera en la estructura cola
    }
    /* FIN DE ZONA CRÍTICA */

    // si llego hasta acá es porque la cola no estaba
    // vacía y se pudo obtener el dato de la cola
    if (c->salida >= TAM)
        c->salida = 0;
    return temp;

    // prender de nuevo las interrupciones
}

int tam_cola(eCola* c) {
    return (c->peso);
}

int cola_llena(eCola* c) {
    if (c->peso >= TAM) {
        return SI;
    }
    else {
        return NO;
    }
}

int cola_vacia(eCola* c) {
    if (c->peso <= 0) {
        return SI;
    }
    else {
        return NO;
    }
}
