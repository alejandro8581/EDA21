// Práctica 1 — Catálogo de Recursos

#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Resource {
    friend ostream& operator<<(ostream& os, const Resource& r);

  private:
    string id;
    string title;
    int year;
    vector<string> tags;

    // Métodos auxiliares privados usados internamente por addTag/hasTag/removeTag.
    static string toLower(const string& s);
    static bool isValidTagChar(char c);
    static string trim(const string& s);

  public:
    Resource();
    Resource(const string& id, int year, const string& title);
    Resource(const Resource& other);
    Resource& operator=(const Resource& other);
    ~Resource();
    string getId() const;
    string getTitle() const;
    int getYear() const;
    vector<string> getTags() const;
    bool addTag(const string& tag);
    bool hasTag(const string& tag) const;
    bool removeTag(const string& tag);
    bool operator==(const Resource& other) const;
    bool operator<(const Resource& other) const;
};

#endif // RESOURCE_H