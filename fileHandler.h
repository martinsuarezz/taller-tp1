typedef struct{
    FILE* file;
    size_t readbytes;
} filehandler_t;

//Inicializa el handler de archivos, recibe un nombre de archivo a abrir, si es NULL lee de stdin.
//Recibe también la cantidad de bytes a leer en cada llamada a filehandler_read
//PRE: self apunta a un filehandler
//POST: si encuentra un error devuelve 1, sino devuelve 0
int filehandler_create(filehandler_t* self, const char* filename, size_t readbytes);

//Recibe un handler de archivos y copia al buffer la cantidad designada de bytes 
//leyendolos del archivo de lectura.
//PRE: self apunta a un filehandler_t ya inicializado
char* filehandler_read(filehandler_t* self, char* buffer);

//Cierra el archivo utilizado
//PRE: self apunta a un filehandler_t ya inicializado
void filehandler_destroy(filehandler_t* self);