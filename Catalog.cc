#include "Catalog.h"


Catalog::Catalog() {
    // Constructor implementation
}
Catalog::Catalog(const Catalog& other) {
    // Copy constructor implementation
    data = other.data;
}  
Catalog& Catalog::operator=(const Catalog& other) {
    // Assignment operator implementation
    if (this != &other) {
        data = other.data;
    }
    return *this;
}
Catalog::~Catalog() {
    // Destructor implementation
}

bool Catalog::add(const Resource& r) {
    if (!r.getId().empty() && findById(r.getId()).getId().empty()) {
        data.push_back(r);
        return true;
    }
    return false;
}
bool Catalog::removeById(const string& id) {
   for (int i = 0; i < data.size(); ++i) {
        if (data[i].getId() == id) {
            data.erase(data.begin() + i);
            return true;
        }
    }
    return false;
}
Resource Catalog::findById(const string& id) const {
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].getId() == id) {
            return data[i];
        }
    }
    return Resource(); // Return an empty Resource if not found
}

vector<Resource> Catalog::findByTag(const string& tag) const {
    vector<Resource> result;
    for (int i = 0; i < data.size(); ++i) {
        if (data[i].hasTag(tag)) {
            result.push_back(data[i]);
        }
    }
    return result;
}
int Catalog::size() const {
    return data.size();
}
bool Catalog::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false; // Failed to open the file
    }

    // Se abrió correctamente: borramos los datos actuales
    data.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // La línea debe tener EXACTAMENTE 3 separadores '|' (4 campos)
        int sepCount = count(line.begin(), line.end(), '|');
        if (sepCount != 3) continue; // línea inválida, se ignora

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        string idStr   = line.substr(0, p1);
        string yearStr = line.substr(p1 + 1, p2 - p1 - 1);
        string title   = line.substr(p2 + 1, p3 - p2 - 1);
        string tagsStr = line.substr(p3 + 1); // puede quedar vacío

        if (idStr.empty()) continue; // id vacío -> se ignora

        int year = stoi(yearStr); // el enunciado dice que no hace falta validarlo

        Resource r(idStr, year, title);

        // Separar tags por comas y añadirlas con addTag (aplica sus propias
        // reglas de validación/normalización/orden)
        stringstream tagsStream(tagsStr);
        string tag;
        while (getline(tagsStream, tag, ',')) {
            r.addTag(tag);
        }

        // add() ya rechaza el recurso si el id ya existe en el catálogo,
        // así que "solo se guarda el primero" se cumple automáticamente
        add(r);
    }

    file.close();
    return true;
}  