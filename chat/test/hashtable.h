#ifndef _HASHTABLE_
#define _HASHTABLE_



class Entry {
    private:
        int m_key;
        int m_socket;
        string m_data;
    public:
        friend bool operator == (const Entry &e1, const Entry &e2);
        Entry() {}
        Entry(const int &key, const int &socket, const string &data) {
            m_key = key;
            m_socket = socket;
            m_data = data;
        }

        void setKey(const int &key) {
            m_key = key;
        }

        void setSocket(const int &socket) {
            m_socket = socket;
        }
        void setData(const string &data) {
            m_data = data;
        }

        int getKey() const {
            return m_key;
        }

        int getSocket() const {
            return m_socket;
        }

        string getData() const {
            return m_data;
        }
};

bool operator == (const Entry &e1, const Entry &e2) {
    return (e1.m_key == e2.m_key);
}

class HashTable{
    private:
        int HashFunction(const int &v) const;
        int count;

    public:
        list<Entry> containers[10];
        HashTable();
        ~HashTable();
        void Insert(const Entry& entry);
        bool Find(const int &key, list<Entry>::const_iterator& itr) const;
        bool Delete(const int &key);
        void Getall();
        int Count() const;
};

int HashTable::HashFunction(const int &v) const {
    return v % 10;
}

HashTable::HashTable() {
    count = 0;
}

HashTable::~HashTable() {
    count = 0;
    for (int i = 0; i < 10; ++i) {
        containers[i].clear();
    }
}

void HashTable::Insert(const Entry &entry) {
    const int hashResult = HashFunction(entry.getKey());
    containers[hashResult].push_back(entry);
    ++count;
}

bool HashTable::Find(const int &key, list<Entry>::const_iterator &out) const {
    const int hashResult = HashFunction(key);
    Entry entry;
    entry.setKey(key);
    list<Entry>::const_iterator itr = find(containers[hashResult].begin(), 
            containers[hashResult].end(), entry);
    if (itr != containers[hashResult].end()) {
        out = itr;
        return true;
    } else {
        return false;
    }
}

bool HashTable::Delete(const int &key) {
    const int hashResult = HashFunction(key);
    Entry entry;
    entry.setKey(key);
    list<Entry>::iterator itr = find(containers[hashResult].begin(),
            containers[hashResult].end(), entry);
    if(itr != containers[hashResult].end()) {
        containers[hashResult].erase(itr);
        --count;
        return true;
    } else {
        return false;
    }
}

int HashTable::Count() const{
    return count;
}

void HashTable::Getall() {
    for (int i = 0; i < 10; i++) {
        for (list<Entry>::iterator itr = containers[i].begin(); itr != containers[i].end(); itr++) {
            cout << (itr)->getKey() << '\t' << (itr)->getData() << '\n' << endl;
        }
    }
}

#endif
