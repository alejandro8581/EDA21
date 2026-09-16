#include "Resource.h"
#include <algorithm> // transform, lower_bound, find
#include <cctype>    // tolower, isalnum

// ==============================================================
// MÉTODOS PRIVADOS AUXILIARES
// Los definimos "static" porque no dependen del estado (this) de
// un objeto concreto: son funciones de utilidad ligadas a la clase.
// ==============================================================

string Resource::toLower(const string& s) {
    string result = s; // copiamos: no queremos modificar el original
    // transform aplica una función a cada elemento del rango [begin, end)
    // y guarda el resultado a partir de result.begin(). Aquí usamos
    // ::tolower (la versión global de <cctype>) para pasar cada char
    // a minúscula.
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return tolower(c); });
    return result;
}

bool Resource::isValidTagChar(char c) {
    // A-Z, a-z, 0-9. isalnum() ya cubre exactamente eso para ASCII
    // (letras y dígitos), así que es más corto que comparar rangos
    // a mano. Se le pasa (unsigned char) por seguridad: isalnum con
    // un char negativo (tildes, etc.) es comportamiento indefinido.
    return isalnum(static_cast<unsigned char>(c)) != 0;
}

string Resource::trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return ""; // la cadena es solo espacios
    size_t end = s.find_last_not_of(" \t\n\r");
    // substr(pos, count): extrae "count" caracteres desde "pos".
    return s.substr(start, end - start + 1);
}

// ==============================================================
// FORMA CANÓNICA
// ==============================================================

// Lista de inicialización (tras los ":"): es la forma preferida en
// C++ de inicializar atributos, más eficiente que asignarlos dentro
// del cuerpo del constructor.
Resource::Resource() : id(""), title(""), year(0) {
    // tags se inicializa vacío automáticamente (vector por defecto).
    // Cuerpo vacío: no hay nada más que hacer.
}

Resource::Resource(const string& id, int year, const string& title)
    : id(id), title(title), year(year) {
    // Aquí "id" y "title" (parámetros) tienen el mismo nombre que los
    // atributos; dentro de la lista de inicialización esto es válido
    // y el compilador sabe distinguirlos: id(id) significa
    // "atributo id = parámetro id".
}

// Constructor de copia: como id, title, year y tags son tipos que
// ya saben copiarse a sí mismos (string y vector tienen su propio
// operator=/copy ctor), basta con inicializarlos a partir de "other".
Resource::Resource(const Resource& other)
    : id(other.id), title(other.title), year(other.year), tags(other.tags) {
}

Resource& Resource::operator=(const Resource& other) {
    // Protección contra auto-asignación (a = a). No es estrictamente
    // necesaria aquí porque string/vector la soportan bien, pero es
    // una buena costumbre para cuando gestiones memoria a mano (punteros).
    if (this != &other) {
        id = other.id;
        title = other.title;
        year = other.year;
        tags = other.tags;
    }
    return *this; // "*this" es el objeto actual; se devuelve por
                  // referencia para permitir encadenar asignaciones.
}

Resource::~Resource() {
    // Nada que liberar manualmente: string y vector son "RAII"
    // (liberan su propia memoria interna automáticamente al destruirse).
}

// ==============================================================
// GETTERS
// ==============================================================

string Resource::getId() const { return id; }
string Resource::getTitle() const { return title; }
int Resource::getYear() const { return year; }
vector<string> Resource::getTags() const { return tags; }
// Nota: devolver "vector<string>" (por valor) hace una copia. Es lo
// correcto aquí porque no queremos que quien llame a getTags() pueda
// modificar el vector interno directamente (rompería el invariante
// de "ordenado y sin duplicados").

// ==============================================================
// GESTIÓN DE ETIQUETAS
// ==============================================================

bool Resource::addTag(const string& tag) {
    string trimmed = trim(tag);

    // Validar: solo caracteres A-Z, a-z, 0-9. Si encontramos uno
    // inválido, rechazamos la tag entera.
    for (char c : trimmed) {
        if (!isValidTagChar(c)) return false;
    }

    string normalized = toLower(trimmed);
    if (normalized.empty()) return false; // no se admiten tags vacías

    // lower_bound busca, en un rango ORDENADO, la primera posición
    // donde se podría insertar "normalized" sin romper el orden.
    // Es la forma eficiente (O(log n) en la búsqueda) de mantener
    // un vector ordenado, en vez de hacer push_back + sort cada vez.
    auto it = lower_bound(tags.begin(), tags.end(), normalized);

    // Si esa posición ya contiene exactamente "normalized", es un
    // duplicado: no lo añadimos.
    if (it != tags.end() && *it == normalized) return false;

    // vector::insert desplaza los elementos siguientes para hacer
    // hueco e inserta "normalized" justo en la posición "it",
    // manteniendo el vector ordenado.
    tags.insert(it, normalized);
    return true;
}

bool Resource::removeTag(const string& tag) {
    // Importante: aquí NO se hace trim(), solo toLower(), tal y como
    // pide el enunciado (removeTag normaliza a minúsculas pero no
    // recorta espacios).
    string normalized = toLower(tag);

    // find recorre el rango buscando un valor igual a "normalized"
    // y devuelve un iterador a él (o a tags.end() si no está).
    auto it = find(tags.begin(), tags.end(), normalized);
    if (it == tags.end()) return false;

    tags.erase(it); // elimina el elemento apuntado por "it"
    return true;
}

bool Resource::hasTag(const string& tag) const {
    string normalized = toLower(tag); // igual que en removeTag: sin trim
    return find(tags.begin(), tags.end(), normalized) != tags.end();
}

// ==============================================================
// OPERADORES
// ==============================================================

bool Resource::operator==(const Resource& other) const {
    return id == other.id;
}

bool Resource::operator<(const Resource& other) const {
    return id < other.id; // string ya sabe compararse lexicográficamente
}

// No es un método de la clase (no lleva Resource::antes... bueno,
// SÍ que aparece "Resource" pero solo como tipo del parámetro).
// Al ser "friend", puede leer r.id, r.year, etc. directamente.
ostream& operator<<(ostream& os, const Resource& r) {
    os << r.id << " (" << r.year << ") " << r.title << " [";
    // Imprimimos las tags separadas por comas, sin coma final.
    for (size_t i = 0; i < r.tags.size(); ++i) {
        if (i > 0) os << ",";
        os << r.tags[i];
    }
    os << "]";
    // OJO: el enunciado pide que NO se imprima salto de línea aquí.
    // Quien llame a operator<< (por ejemplo main.cc) decidirá si
    // añade "\n" después, según lo que necesite cada comando.
    return os; // se devuelve el stream para poder encadenar: cout << r1 << r2;
}