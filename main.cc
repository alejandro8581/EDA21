// Práctica 1 — Catálogo de Recursos
// Aplicación de línea de comandos (Sección 6 del enunciado).
//
// Lee comandos de la entrada estándar y escribe el resultado
// en la salida estándar, sin prompts ni mensajes adicionales.

#include <iostream>
#include <sstream>
#include <string>

#include "Catalog.h"

using namespace std;

namespace {

// Sustituye todos los '_' de una cadena por espacios (formato del título en ADD).
string underscoresToSpaces(const string& s) {
    string out = s;
    for (size_t i = 0; i < out.size(); i++) {
        if (out[i] == '_') out[i] = ' ';
    }
    return out;
}

// Separa una cadena de tags "tag1,tag2,tag3" en sus componentes.
// Si la entrada es "-" (sin tags) devuelve un vector vacío.
vector<string> splitTags(const string& s) {
    vector<string> result;
    if (s == "-") return result;

    string current;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == ',') {
            result.push_back(current);
            current.clear();
        } else {
            current += s[i];
        }
    }
    result.push_back(current);
    return result;
}

// Procesa el comando LOAD <file>.
void doLoad(istringstream& iss, Catalog& catalog) {
    string filename;
    if (!(iss >> filename)) {
        cout << "INVALIDCMD" << endl;
        return;
    }
    if (catalog.loadFromFile(filename)) {
        cout << "OK " << catalog.size() << endl;
    } else {
        cout << "ERROR" << endl;
    }
}

// Procesa el comando ADD <id> <year> <title> <tags>.
void doAdd(istringstream& iss, Catalog& catalog) {
    string id, yearStr, titleToken, tagsToken;
    if (!(iss >> id >> yearStr >> titleToken >> tagsToken)) {
        cout << "INVALIDCMD" << endl;
        return;
    }

    // El año debe ser un entero válido.
    istringstream yearStream(yearStr);
    int year;
    if (!(yearStream >> year)) {
        cout << "INVALIDCMD" << endl;
        return;
    }

    string title = underscoresToSpaces(titleToken);
    Resource r(id, year, title);

    vector<string> tags = splitTags(tagsToken);
    for (size_t i = 0; i < tags.size(); i++) {
        r.addTag(tags[i]);
    }

    if (catalog.add(r)) {
        cout << "OK" << endl;
    } else {
        cout << "DUPLICATE" << endl;
    }
}

// Procesa el comando DEL <id>.
void doDel(istringstream& iss, Catalog& catalog) {
    string id;
    if (!(iss >> id)) {
        cout << "INVALIDCMD" << endl;
        return;
    }
    if (catalog.removeById(id)) {
        cout << "OK" << endl;
    } else {
        cout << "NOTFOUND" << endl;
    }
}

// Procesa el comando GET <id>.
void doGet(istringstream& iss, const Catalog& catalog) {
    string id;
    if (!(iss >> id)) {
        cout << "INVALIDCMD" << endl;
        return;
    }
    Resource r = catalog.findById(id);
    // findById devuelve un Resource() por defecto (id vacío) si no existe;
    // como add() nunca admite id vacío, esto identifica de forma segura el NOTFOUND.
    if (r.getId().empty()) {
        cout << "NOTFOUND" << endl;
    } else {
        cout << r << endl;
    }
}

// Procesa el comando FINDTAG <tag>.
void doFindTag(istringstream& iss, const Catalog& catalog) {
    string tag;
    if (!(iss >> tag)) {
        cout << "INVALIDCMD" << endl;
        return;
    }
    vector<Resource> found = catalog.findByTag(tag);
    cout << "COUNT " << found.size() << endl;
    for (size_t i = 0; i < found.size(); i++) {
        cout << found[i] << endl;
    }
}

}  // namespace

int main() {
    Catalog catalog;
    string line;

    while (getline(cin, line)) {
        istringstream iss(line);
        string command;
        if (!(iss >> command)) {
            continue;   // línea vacía
        }

        if (command == "EXIT") {
            break;
        } else if (command == "LOAD") {
            doLoad(iss, catalog);
        } else if (command == "ADD") {
            doAdd(iss, catalog);
        } else if (command == "DEL") {
            doDel(iss, catalog);
        } else if (command == "GET") {
            doGet(iss, catalog);
        } else if (command == "FINDTAG") {
            doFindTag(iss, catalog);
        } else {
            cout << "INVALIDCMD" << endl;
        }
    }

    return 0;
}