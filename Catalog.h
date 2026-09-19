#ifndef TADCATALOG_H
#define TADCATALOG_H

#include "Resource.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>   // ifstream
#include <sstream>   // stringstream
#include <algorithm> // count
using namespace std;

class Catalog {
    private:
        vector<Resource> data;

    public:
        Catalog();
        Catalog(const Catalog& other);
        Catalog& operator=(const Catalog& other);
        ~Catalog();

        bool add(const Resource& r);
        bool removeById(const string& id);
        Resource findById(const string& id) const;
        vector<Resource> findByTag(const string& tag) const;
        int size() const;
        bool loadFromFile(const string& filename);
};

#endif // TADCATALOG_H