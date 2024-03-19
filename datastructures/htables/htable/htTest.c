/* 
  Some tests for the string-string hash table
  Author: David De Potter
*/

#include "sshtable.h" // use the string-string hash table

int main (){
  
  ssHt *ht = ssHtNew(CASE_INSENSITIVE);
  
    // makes the hash table copy the keys and values
    // if set: total number of frees is 95
    // if not set: total number of frees is 72
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

    // deletes the key "three"
  ssHtDelKey(ht, "three");

    // removes values from the key "two"
  ssHtDelVal(ht, "two", "due");
    // after this, the key "two" is removed
    // because it has no values left
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

  ssHtFree(ht);  
}