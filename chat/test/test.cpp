#include "hashtable.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    HashTable h;
    h.Insert(Entry(234, "one"));
    h.Insert(Entry(567, "two"));
    h.Insert(Entry(987, "three"));
    h.Insert(Entry(222, "four"));
    h.Insert(Entry(564, "five"));
    h.Insert(Entry(111, "six"));

    h.Delete(234);
    list<Entry>::const_iterator itr;

    //if (h.Find(234, itr)) {
        //cout << "The data with key 234:" << itr->getData() << endl;
    //} else {
        //cout << "Can not find the data with key 234" << endl;
    //}

    //if (h.Find(111, itr)) {
        //cout << "The data with key 111:" << itr->getData() <<endl;
    //} else {
        //cout << "Can not find the data with key 111" << endl;
    //}


    //cout << "Total count:" << h.Count() << endl;

    h.Getall();
    return 0;
}
