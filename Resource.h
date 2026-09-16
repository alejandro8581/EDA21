#ifndef RESOURCE_H
#define RESOURCE_H

// ============================================================
// "Header guard" (#ifndef / #define / #endif al final del archivo)
// Evita que el contenido de este .h se incluya más de una vez
// si varios .cc lo hacen #include, lo que provocaría errores de
// "redefinición". SIEMPRE pon esto en tus .h.
// ============================================================

#include <string>
#include <vector>
#include <iostream>

using namespace std;
// Nota: usar "using namespace std;" en un .h no es la mejor
// práctica en proyectos grandes (contamina a quien haga #include),
// pero el enunciado de la práctica lo usa así (string, vector<string>
// sin std::), así que lo mantenemos por consistencia con las plantillas.

class Resource {
private:
    // --------------------------------------------------------
    // Atributos privados (encapsulación): nadie fuera de la clase
    // puede tocarlos directamente. Se accede a ellos mediante
    // getters/setters o métodos públicos.
    // --------------------------------------------------------
    string id;
    string title;
    int year;
    vector<string> tags; // siempre en minúsculas, sin duplicados,
                          // y mantenido en orden lexicográfico

    // --------------------------------------------------------
    // Métodos privados auxiliares (el enunciado permite añadirlos).
    // Son un buen sitio para poner lógica reutilizable que NO
    // forma parte de la interfaz pública del TAD.
    // --------------------------------------------------------

    // Convierte una cadena a minúsculas y devuelve una copia nueva.
    // Se pasa por valor (const string&) porque solo la LEEMOS.
    static string toLower(const string& s);

    // Comprueba si un caracter es uno de los permitidos en una tag:
    // A-Z, a-z, 0-9. Útil dentro de addTag.
    static bool isValidTagChar(char c);

    // Elimina espacios en blanco al principio y al final de la cadena.
    // (Solo se usa en addTag; removeTag/hasTag NO deben recortar espacios,
    // según el enunciado).
    static string trim(const string& s);

public:
    // ==========================================================
    // 3.2 FORMA CANÓNICA
    // Toda clase que gestiona recursos (aquí, un vector interno)
    // debería declarar estos 5 miembros. Es el "Rule of Three/Five"
    // de C++: si defines destructor, copy ctor o operator=,
    // probablemente necesitas los tres.
    // ==========================================================

    // Constructor por defecto: id="" , title="", year=0, tags vacío.
    Resource();

    // Constructor con parámetros. Nota el orden EXACTO que pide el
    // enunciado: (id, year, title) -- ¡year va antes que title!
    // Los parámetros de tipo objeto (string) se pasan por
    // "const referencia" (const string&) para evitar copias
    // innecesarias, sin permitir que la función los modifique.
    Resource(const string& id, int year, const string& title);

    // Constructor de copia: se llama cuando haces
    //   Resource b(a);   o   Resource b = a; (en la declaración)
    Resource(const Resource& other);

    // Operador de asignación: se llama cuando ya existen ambos objetos
    //   Resource b;
    //   b = a;
    // Devuelve Resource& para poder encadenar: a = b = c;
    Resource& operator=(const Resource& other);

    // Destructor: aquí no hace falta liberar nada a mano porque
    // string y vector gestionan su propia memoria (RAII), pero
    // se declara igualmente por completitud de la forma canónica.
    ~Resource();

    // ==========================================================
    // 3.3 GETTERS
    // Todos son "const" porque NO modifican el objeto: esto permite
    // llamarlos también sobre objetos "const Resource&", algo muy
    // habitual (por ejemplo, dentro de operator<< o en bucles con
    // "const Resource& r : catalogo").
    // ==========================================================
    string getId() const;
    string getTitle() const;
    int getYear() const;
    vector<string> getTags() const;

    // ==========================================================
    // 3.4 GESTIÓN DE ETIQUETAS
    // ==========================================================

    // Añade una etiqueta validándola y normalizándola.
    // Devuelve true si se añadió, false si no (inválida, vacía o duplicada).
    bool addTag(const string& tag);

    // Elimina una etiqueta (normalizada a minúsculas, SIN recortar espacios).
    bool removeTag(const string& tag);

    // Comprueba si existe una etiqueta (misma normalización que removeTag).
    bool hasTag(const string& tag) const;

    // ==========================================================
    // 3.5 OPERADORES
    // Sobrecargar operadores permite que tus objetos se comporten
    // como tipos "nativos": comparar con ==, ordenar con <, o
    // imprimir con cout << objeto.
    // ==========================================================

    // Dos recursos son iguales si tienen el mismo id.
    bool operator==(const Resource& other) const;

    // Orden por id (para poder meter Resource en, p.ej., std::sort
    // o en un std::set).
    bool operator<(const Resource& other) const;

    // "friend": permite que esta función NO miembro acceda a los
    // atributos PRIVADOS de Resource. Se declara dentro de la clase
    // pero se implementa fuera, como una función normal (no lleva
    // Resource:: delante en el .cc).
    // Debe imprimir SIN salto de línea final:
    //   <id> (<year>) <title> [tag1,tag2,tag3]
    friend ostream& operator<<(ostream& os, const Resource& r);
};

#endif // RESOURCE_H