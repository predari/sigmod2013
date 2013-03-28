#ifndef INDEX_HASH_TABLE_H
#define INDEX_HASH_TABLE_H

#define BITS_PER_UNIT (sizeof(unit)*8)

using namespace std;

class IndexHashTable
{
    typedef unsigned unit;

    unit*               units;          //TODO: Maybe if it becomes stl::vector it will have better performance ?
    unsigned            numUnits;
    bool                keepIndexVec;
    unsigned            capacity;
    unsigned            mSize;
    pthread_mutex_t     mutex;

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }

public:
    vector<unsigned> indexVec;

    IndexHashTable (unsigned _capacity, bool _keepIndexVec=true) :
        keepIndexVec(_keepIndexVec),
        capacity(_capacity),
        mSize(0)
    {
        pthread_mutex_init(&mutex,   NULL);
        numUnits = capacity/BITS_PER_UNIT;
        if (capacity%BITS_PER_UNIT) numUnits++;     // An to capacity den einai akeraio pollaplasio tou BITS_PER_UNIT, tote theloume allo ena unit.
        units = (unit*) malloc (numUnits*sizeof(unit));     // Allocate space with capacity bits. (NOT BYTES, BITS!)
        if (units==0) {fprintf(stderr, "Could not allocate memory for IndexHashTable"); exit(-1);}
        for (unsigned i=0 ; i<numUnits ; i++) units[i]=0;
    }

    IndexHashTable (const IndexHashTable &from) :
        numUnits(from.numUnits),
        keepIndexVec(from.keepIndexVec),
        capacity(from.capacity),
        mSize(from.mSize),
        indexVec(from.indexVec)
    {
        pthread_mutex_init(&mutex,   NULL);
        units = (unit*) malloc (numUnits*sizeof(unit));
        if (units==0) {fprintf(stderr, "Could not allocate memory for IndexHashTable"); exit(-1);}
        for (unsigned i=0 ; i<numUnits ; i++) units[i]=from.units[i];
    }

    ~IndexHashTable () {
        free(units);
    }

    bool insert (int index) {
        //~ lock();
        unsigned unit_offs = index / BITS_PER_UNIT;
        unsigned unit_bit  = index % BITS_PER_UNIT;
        unit mask = 1 << unit_bit;
        if (units[unit_offs] & mask) {
            //~ unlock();
            return false;
        }
        else {
            units[unit_offs] |= mask;
            mSize++;
            if (keepIndexVec) indexVec.push_back(index);
            //~ unlock();
            return true;
        }
    }

    bool exists (int index) {
        unsigned unit_offs = index / BITS_PER_UNIT;
        unsigned unit_bit  = index % BITS_PER_UNIT;
        unit mask = 1 << unit_bit;
        if (units[unit_offs] & mask)
            return true;
        else
            return false;
    }

    unsigned size() const {
        return mSize;
    }

    void clear () {
        for (unsigned i=0 ; i<numUnits ; i++) units[i]=0;
        indexVec.clear();
        mSize=0;
    }

    static bool equals (const IndexHashTable &h1, const IndexHashTable &h2) {
        unsigned max_units = h1.numUnits>h2.numUnits ? h1.numUnits : h2.numUnits;
        for (int unsigned i=0; i< max_units ; i++)
            if (h1.units[i]!=h2.units[i]) return false;
        return true;
    }
};

#endif
