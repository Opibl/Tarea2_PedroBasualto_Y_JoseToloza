/*
    Nombres:Pedro Basualto
            Jose Toloza
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include <ctype.h>
/*
    estructura usada
*/
typedef struct
{

    char *nombre;
    char *artista;
    char *duracion;
    char *album;
    char *fechaL;
}cancion;

/*
    para separar las lineas
*/
char *get_csv_field (char * tmp, int i)
{
    char * line = _strdup (tmp);
    char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n")) {
        if (!--i) {
            return tok;
        }
    }
    return NULL;
}

char * _strdup (const char *s) {
    size_t len = strlen (s) + 1;
    void *new = malloc (len);

    if (new == NULL)
    	return NULL;

    return (char *) memcpy (new, s, len);
}
/*
    esto es para poder crear mapas (funcion hash)
*/
long long stringHash(const void * key) {
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++) {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}

/*
    para comparar las key
*/

int stringEqual(const void * key1, const void * key2) {
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}

/*
    crea un data de tipo cancion
*/
cancion *crear_cancion (char *nombre, char *artista, char *duracion,char *album,char *fechaL)
{
    cancion *nueva_cancion = malloc(sizeof(cancion));
    nueva_cancion->nombre=nombre;
    nueva_cancion->artista = artista;
    nueva_cancion->duracion = duracion;
    nueva_cancion->album = album;
    nueva_cancion->fechaL=fechaL;
    return nueva_cancion;

}
//1
/*
    la funcion de la opcion 1: ingresa las canciones a sus respectivos mapas
*/
void ingresar_musica(Map *mapa_canciones,Map* Mapa_album, Map* mapa_artistas)
{
    FILE *entrada_musica = fopen("canciones.csv","r"); // se crea una entrada del archivo csv
    char linea[100];
    char *nombre;
    char *artista;
    char *duracion;
    char *album;
    char *fechaL;

    if (entrada_musica == NULL) // si la entrada no existe , no deja continuar el programa
    {
        printf("Archivo no se pudo abrir :(");
        exit(0);
    }

    while(fgets(linea,1024,entrada_musica) != NULL) // lee una linea de la entrada
    {
        nombre = get_csv_field(linea, 1);  // las separa hasta que encuentre una coma
        artista = get_csv_field(linea, 2);
        duracion = get_csv_field(linea, 3);
        album = get_csv_field(linea, 4);
        fechaL = get_csv_field(linea, 5);

        cancion *m = crear_cancion (nombre,artista,duracion,album,fechaL); //se crea la cancion

        if (!searchMap(mapa_canciones, m->nombre)) // se ingresa la cancion al mapa_canciones con su clave nombre
        {
            insertMap(mapa_canciones, m->nombre, m);

        }
        if (!searchMap(mapa_artistas, m->artista)) //se ingresa al mapa_artistas con clave artista  y su valor es un mapa con cancions de un artista
        {
            Map * mapa_canciones_aux = createMap(stringHash, stringEqual);
            insertMap(mapa_canciones_aux, m->nombre, m);
            insertMap(mapa_artistas, m->artista, mapa_canciones_aux);
        }
        else
        {
            insertMap(searchMap(mapa_artistas, m->artista), m->nombre, m);
        }

        if(!searchMap(Mapa_album, m->album)) //se ingresa al mapa album
        {
            Map * mapa_canciones_aux = createMap(stringHash, stringEqual);
            insertMap(mapa_canciones_aux, m->nombre, m);
            insertMap(Mapa_album, m->album,mapa_canciones_aux);
        }
        else
        {
            Map * mapa_canciones_aux = searchMap(Mapa_album, m->album);
            insertMap(mapa_canciones_aux, m->nombre, m);
        }

    }


}
//2
void exportar_musica(Map *mapa_canciones)
{
    FILE *salida_musica = fopen("canciones_salida.csv","w"); //se crea la salida del nuevo csv
    cancion *dato = firstMap(mapa_canciones); // se para en en primer dato del mapa
    while(dato != NULL)
    {
        fprintf(salida_musica,"%s, ",dato->nombre); //imprime en el nuevo archivo creado
        fprintf(salida_musica,"%s, ",dato->artista);
        fprintf(salida_musica,"%s, ",dato->duracion);
        fprintf(salida_musica,"%s, ",dato->album);
        fprintf(salida_musica,"%s\n",dato->fechaL);
        dato = nextMap(mapa_canciones); // para recorrer los datos del mapa
    }
}
//3
/*
    se crea un nuevo album y se le ingresan canciones
*/
void agregar_album(Map* Mapa_album,Map* mapa_canciones)
{
    char nombre[100];
    char artista[100];
    char duracion[100];
    char album[100];
    char fechaL[100];
    cancion *m;
    char opcion[100];
    printf("ingrese el album que desea crear\n"); // lee un album
    scanf("%s",album);
    while(1)
    {
        printf("ingrese las canciones // "); // se crean canciones
        printf("ingresar el nombre: ");
        scanf("%s",nombre);
        printf("ingresar el artista\n");
        scanf("%s",artista);
        printf("ingresar la duracion\n");
        scanf("%s",duracion);
        printf("ingresar fecha de lanzamiento\n");
        scanf("%s",fechaL);
        m = crear_cancion(nombre,artista,duracion,album,fechaL);
        insertMap(mapa_canciones,nombre,m);

        if(!searchMap(Mapa_album, m->album))
        {
            Map * mapa_canciones_aux = createMap(stringHash, stringEqual);
            insertMap(mapa_canciones_aux, m->nombre, m);
            insertMap(Mapa_album, m->album,mapa_canciones_aux);
        }
        else
        {
            Map * mapa_canciones_aux = searchMap(Mapa_album, m->album);
            insertMap(mapa_canciones_aux, m->nombre, m);
        }


        printf("desea continuar ingresando canciones(si o no): "); // pregunta si desea continuar
        scanf("%s",opcion);
        if((strcmp(opcion,"NO")==0)||(strcmp(opcion,"no")==0)||(strcmp(opcion,"No")==0))
        {
            break;
        }
    }


}

//4

/*
    se agrega una nueva cancion
*/
void agregar_cancion(Map *mapa_canciones)
{
    char nombre[100];
    char artista[100];
    char duracion[100];
    char album[100];
    char fechaL[100];
    cancion *m;

    printf("ingresar el nombre\n");
    getchar();
    scanf("%[^\n]s",nombre);
    printf("ingresar el artista\n");
    getchar();
    scanf("%[^\n]s",artista);
    printf("ingresar la duracion\n");
    getchar();
    scanf("%[^\n]s",duracion);
    printf("ingesar album\n");
    getchar();
    scanf("%[^\n]s",album);
    printf("ingresar fecha de lanzamiento");
    scanf("%[^\n]s",fechaL);
    m = crear_cancion(nombre,artista,duracion,album,fechaL);
    insertMap(mapa_canciones,nombre,m);

}
//5
/*
    se elimina las canciones de un artista
*/
void eliminar_canciones_artista(Map * mapa_canciones , Map * mapa_artistas, Map * Mapa_album){
    char nombre_artista[100];
    printf("ingrese el artista\n");
    getchar();
    scanf("%[^\n]s",nombre_artista); // se ingresa el artista
    Map * c = searchMap(mapa_artistas, nombre_artista); // se busca el artita y recive su mapa de canciones (c)
    cancion * ptr = firstMap(c); // se para en el primer dato de c
    long cont = mapCount(c); //cuenta la cantidad de datos del mapa
    while(cont != 0){ // lo recorre eliminandolos de los mapas
        eraseKeyMap(mapa_canciones, ptr->nombre);
        if (searchMap(Mapa_album, ptr->album) != NULL){
            removeAllMap(searchMap(Mapa_album, ptr->album));
        }
        ptr = nextMap(c);
        cont--;
    }
    removeAllMap(c);
}
//6
/*
    se busca una cancion
*/
void buscar_cancion(Map *mapa_canciones)
{
    char cancion_buscada[100];
    printf("Ingrese la cancion que quiere buscar\n");
    getchar();
    scanf("%[^\n]s",cancion_buscada);


    cancion *m;
    m = searchMap(mapa_canciones,cancion_buscada);
    if(m != NULL)
    {
        printf("%s,",m->nombre);
        printf("%s,",m->artista);
        printf("%s,",m->duracion);
        printf("%s,",m->album);
        printf("%s\n",m->fechaL);
    }
    else
    {
        printf("cancion no encontrada\n");
    }
}
//7
/*
    se busca un artista , mostrando sus canciones
*/
void buscar_artista(Map *mapa_artistas)
{
    char artista_buscado[100];
    printf("Ingrese el artista que quiere buscar: ");
    getchar();
    scanf("%[^\n]s",artista_buscado);


    Map *m;
    m = searchMap(mapa_artistas,artista_buscado);
    if(m != NULL)
    {
        cancion *dato = firstMap(m);
        while(dato != NULL)
        {
            printf("%s,",dato->nombre);
            printf("%s,",dato->artista);
            printf("%s,",dato->duracion);
            printf("%s,",dato->album);
            printf("%s\n",dato->fechaL);
            dato = nextMap(m);
        }
    }
    else
    {
        printf("artista no encontrado\n");
    }
}

//8
/*
    se busca un album , mostrando sus canciones
*/
void buscar_album(Map *Mapa_album)
{
    char album_buscado[100];
    printf("Ingrese el album que quiere buscar: \n");
    getchar();
    scanf("%[^\n]s",album_buscado);


    Map *m;
    m = searchMap(Mapa_album,album_buscado);
    if(m != NULL)
    {
        cancion *dato1 = firstMap(m);
        while(dato1 != NULL)
        {
            printf("%s,",dato1->nombre);
            printf("%s,",dato1->artista);
            printf("%s,",dato1->duracion);
            printf("%s,",dato1->album);
            printf("%s\n",dato1->fechaL);
            dato1 = nextMap(m);
        }
    }
    else
    {
        printf("album no encontrado\n");
    }
}

int main()
{
    int op;
    Map* mapa_canciones = createMap(stringHash,stringEqual);
    Map* Mapa_album = createMap(stringHash,stringEqual);
    Map* mapa_artistas = createMap(stringHash,stringEqual);

do
{
    printf("ingresar una opcion , (ingresar 0 para terminar)\n"); // las opciones
    printf("1.Importar musica desde un archivo CSV \n");
    printf("2.exportar musica\n");
    printf("3.agregar un album\n");
    printf("4.agregar cancion\n");
    printf("5.eliminar canciones de un artista\n");
    printf("6.buscar una cancion\n");
    printf("7.buscar canciones de un artista\n");
    printf("8.buscar album\n");
    scanf("%d",&op);

    if(op == 1) // las opciones
    {
        ingresar_musica(mapa_canciones,Mapa_album,mapa_artistas);
        printf("\n");
    }

    if(op == 2)
    {
        exportar_musica(mapa_canciones);
        printf("\n");
    }

    if(op == 3)
    {
        agregar_album(Mapa_album,mapa_canciones);
        printf("\n");
    }

    if(op == 4)
    {
        agregar_cancion(mapa_canciones);
        printf("\n");
    }

    if(op == 5)
    {
        eliminar_canciones_artista(mapa_canciones ,mapa_artistas,Mapa_album);
        printf("\n");
    }
    if(op == 6)
    {
        buscar_cancion(mapa_canciones);
        printf("\n");
    }

    if(op == 7)
    {
        buscar_artista(mapa_artistas);
        printf("\n");
    }

    if(op == 8)
    {

        buscar_album(Mapa_album);
        printf("\n");
    }


}while(op != 0);


    return 0;
}
