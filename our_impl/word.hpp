#ifndef WORD_H
#define WORD_H

#include <core.h>
#include <set>

using namespace std;

struct Word
{
    int         length;                         ///< Strlen(txt);
    unsigned    letterBits;                     ///< 1 bit for every char [a-z]
    set<QueryID> querySet[3];                   ///< Sets of queries matching this word. One set for each MatchType
    char        txt[MAX_WORD_LENGTH+1];         ///< The actual word :P

    /** Store the new word and populate the data structures */
    Word (const char *c1, const char *c2) {
        letterBits=0;
        int i=0;
        while (c1!=c2) {
            letterBits |= 1 << (*c1-'a');
            txt[i++] = *c1++;
        }
        length = i;
        while (i<MAX_WORD_LENGTH+1) txt[i++] = 0;
    }

    bool equals(const char *c1, const char *c2) const {
        const char *_txt = txt;
        while (c1!=c2 && *_txt) if (*_txt++ != *c1++) return false;
        if (c1==c2 && !*_txt) return true;
        else return false;
    }

    bool equals(Word* w) const {
        if (this==w) return true;
        const int *i1= (int*) txt;
        const int *i2= (int*) w->txt;
        while ( i1!=(int*)(txt+(MAX_WORD_LENGTH+1)) && *i1==*i2) {++i1; ++i2;}
        if (i1!=(int*)(txt+(MAX_WORD_LENGTH+1))) return true;
        else return false;
    }

    int letterDiff(Word *w ) {
        return __builtin_popcount(this->letterBits ^ w->letterBits);
    }

    int EditDist(Word *w) {
        char* a = w->txt;
        int na = w->length;
        char* b = this->txt;
        int nb = this->length;

        int oo=0x7FFFFFFF;

        int T[2][MAX_WORD_LENGTH+1];

        int ia, ib;

        int cur=0;
        ia=0;

        for(ib=0;ib<=nb;ib++)
            T[cur][ib]=ib;

        cur=1-cur;

        for(ia=1;ia<=na;ia++)
        {
            for(ib=0;ib<=nb;ib++)
                T[cur][ib]=oo;

            int ib_st=0;
            int ib_en=nb;

            if(ib_st==0)
            {
                ib=0;
                T[cur][ib]=ia;
                ib_st++;
            }

            for(ib=ib_st;ib<=ib_en;ib++)
            {
                int ret=oo;

                int d1=T[1-cur][ib]+1;
                int d2=T[cur][ib-1]+1;
                int d3=T[1-cur][ib-1]; if(a[ia-1]!=b[ib-1]) d3++;

                if(d1<ret) ret=d1;
                if(d2<ret) ret=d2;
                if(d3<ret) ret=d3;

                T[cur][ib]=ret;
            }

            cur=1-cur;
        }

        int ret=T[1-cur][nb];

        return ret;
    }

    int HammingDist(Word *w) {
        char* a = w->txt;
        int na = w->length;
        char* b = this->txt;
        int nb = this->length;

        int j, oo=0x7FFFFFFF;
        if(na!=nb) return oo;

        unsigned int num_mismatches=0;
        for(j=0;j<na;j++) if(a[j]!=b[j]) num_mismatches++;

        return num_mismatches;
    }

};

#endif