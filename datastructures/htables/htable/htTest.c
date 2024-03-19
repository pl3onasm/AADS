/* 
  Some tests for the string-string hash table
  Author: David De Potter
*/

#include "sshtable.h" // use the string-string hash table

int main (){
  
  ssHt *ht = ssHtNew(CASE_INSENSITIVE);
  
    // makes the hash table copy the keys and values
    // if set: total number of frees is 211
    // if not set: total number of frees is 159
  ssHtCopyKeys(ht);  
  ssHtCopyVals(ht);

  ssHtAddKey(ht, "one", "uno");
  ssHtAddKey(ht, "two", "due");
    // adds a second value to the key "two"
  ssHtAddKey(ht, "two", "deux");   
  ssHtAddKey(ht, "three", "tre");
  ssHtAddKey(ht, "four", "quattro");
  ssHtAddKey(ht, "five", "cinque");
  ssHtAddKey(ht, "six", "sei");
  ssHtAddKey(ht, "seven", "sette");
  ssHtAddKey(ht, "eight", "otto");
  ssHtAddKey(ht, "nine", "nove");
  ssHtAddKey(ht, "ten", "dieci");
    // adds two more values to the key "one"
  ssHtAddKey(ht, "one", "ein");
  ssHtAddKey(ht, "one", "un");

    // adds the same value to the key "one"
    // nothing happens because the value is already there
  ssHtAddKey(ht, "one", "uno");

    // adds a key with case differences
    // if the hash table is case sensitive,
    // a new key is added, otherwise the
    // value is added to the existing key
  ssHtAddKey(ht, "Five", "fünf");

    // adds a value with case differences
    // if the hash table is case sensitive,
    // a new value is added, otherwise nothing
  ssHtAddKey(ht, "Five", "Fünf");

    // deletes the key "three"
  ssHtDelKey(ht, "three");

    // removes values from the key "two"
  ssHtDelVal(ht, "two", "due");
    // after the following statement, the key "two" 
    // is removed because it has no values left
  ssHtDelVal(ht, "two", "deux");

    // tries to remove a value from a 
    // non-existing key
    // nothing happens because the key is not there
  ssHtDelVal(ht, "two", "deux");

    // tries to remove a non-existing value from an 
    // existing key
    // nothing happens because the value is not there
  ssHtDelVal(ht, "nine", "tre");

  ssHtSetLabel(ht, "Test HT");
  ssHtShow(ht);

    // add more keys to test the rehashing
    // a table starts with 32 buckets, and  
    // rehashes at 75% load factor
  ssHtAddKey(ht, "eleven", "undici");
  ssHtAddKey(ht, "twelve", "dodici");
  ssHtAddKey(ht, "thirteen", "tredici");
  ssHtAddKey(ht, "fourteen", "quattordici");
  ssHtAddKey(ht, "fifteen", "quindici");
  ssHtAddKey(ht, "sixteen", "sedici");
  ssHtAddKey(ht, "seventeen", "diciassette");
  ssHtAddKey(ht, "eighteen", "diciotto");
  ssHtAddKey(ht, "nineteen", "diciannove");
  ssHtAddKey(ht, "twenty", "venti");
  ssHtAddKey(ht, "twentyone", "ventuno");
  ssHtAddKey(ht, "twentytwo", "ventidue");
  ssHtAddKey(ht, "twentythree", "ventitre");
  ssHtAddKey(ht, "twentyfour", "ventiquattro");

    // results in a different order of the keys
    // because of the rehashing
  ssHtShow(ht);

  ssHtFree(ht);  
}