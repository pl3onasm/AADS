/* 
  Some tests for the string-string hash table
  Author: David De Potter
*/

#include "sshtable.h" // use the string-string hash table

int main (){
  
  ssHt *ht = ssHtNew(CASE_INSENSITIVE, 40);
  
    // makes the hash table copy the keys and values
    // if set: total number of frees is 381
    // if not set: total number of frees is 303
  ssHtCopyKeys(ht);  
  ssHtCopyVals(ht);

  ssHtAddKeyVal(ht, "one", "uno");
  ssHtAddKeyVal(ht, "two", "due");
    // adds a second value to the key "two"
  ssHtAddKeyVal(ht, "two", "deux");   
  ssHtAddKeyVal(ht, "three", "tre");
  ssHtAddKeyVal(ht, "four", "quattro");
  ssHtAddKeyVal(ht, "five", "cinque");
  ssHtAddKeyVal(ht, "six", "sei");
  ssHtAddKeyVal(ht, "seven", "sette");
  ssHtAddKeyVal(ht, "eight", "otto");
  ssHtAddKeyVal(ht, "nine", "nove");
  ssHtAddKeyVal(ht, "ten", "dieci");
    // adds two more values to the key "one"
  ssHtAddKeyVal(ht, "one", "ein");
  ssHtAddKeyVal(ht, "one", "un");
  ssHtAddKey(ht, "eleven");
  

    // adds the same value to the key "one"
    // nothing happens because the value is already there
  ssHtAddKeyVal(ht, "one", "uno");

    // adds the same key with case differences;
    // if the hash table is case sensitive,
    // a new key is added, otherwise the
    // value is added to the existing key
  ssHtAddKeyVal(ht, "Five", "fünf");

    // adds the same value with case differences;
    // if the hash table is case sensitive,
    // a new value is added, otherwise nothing
  ssHtAddKeyVal(ht, "Five", "Fünf");

    // deletes the key "three"
  ssHtDelKey(ht, "three");

    // removes values from the key "two"
  ssHtDelVal(ht, "two", "due");
    // after the following statement, the key "two" 
    // has no more values
  ssHtDelVal(ht, "two", "deux");

    // tries to remove a value from a 
    // key without values; nothing happens
  ssHtDelVal(ht, "two", "deux");

    // tries to remove a non-existing value from an 
    // existing key
    // nothing happens because the value is not there
  ssHtDelVal(ht, "nine", "tre");

  ssHtSetLabel(ht, "Test HT");
  ssHtShow(ht);
  ssHtStats(ht);

    // add more keys to test the rehashing
    // a table starts with 32 buckets, and  
    // rehashes at 75% load factor
  ssHtAddKeyVal(ht, "eleven", "undici");
  ssHtAddKeyVal(ht, "twelve", "dodici");
  ssHtAddKeyVal(ht, "thirteen", "tredici");
  ssHtAddKeyVal(ht, "fourteen", "quattordici");
  ssHtAddKeyVal(ht, "fifteen", "quindici");
  ssHtAddKeyVal(ht, "sixteen", "sedici");
  ssHtAddKeyVal(ht, "seventeen", "diciassette");
  ssHtAddKeyVal(ht, "eighteen", "diciotto");
  ssHtAddKeyVal(ht, "nineteen", "diciannove");
  ssHtAddKeyVal(ht, "twenty", "venti");
  ssHtAddKeyVal(ht, "twenty-one", "ventuno");
  ssHtAddKeyVal(ht, "twenty-two", "ventidue");
  ssHtAddKeyVal(ht, "twenty-three", "ventitre");
  ssHtAddKeyVal(ht, "twenty-four", "ventiquattro");
  ssHtAddKeyVal(ht, "twenty-five", "venticinque");
  ssHtAddKeyVal(ht, "twenty-six", "ventisei");
  ssHtAddKeyVal(ht, "twenty-seven", "ventisette");
  ssHtAddKeyVal(ht, "twenty-eight", "ventotto");
  ssHtAddKeyVal(ht, "twenty-nine", "ventinove");
  ssHtAddKeyVal(ht, "thirty", "trenta");
  ssHtAddKeyVal(ht, "thirty-one", "trentuno");
  ssHtAddKeyVal(ht, "thirty-two", "trentadue");
  ssHtAddKeyVal(ht, "thirty-three", "trentatre");
  ssHtAddKeyVal(ht, "thirty-four", "trentaquattro");
  ssHtAddKeyVal(ht, "thirty-five", "trentacinque");
  ssHtAddKeyVal(ht, "thirty-six", "trentasei");
  ssHtAddKeyVal(ht, "thirty-seven", "trentasette");
  ssHtAddKeyVals(ht, "thirty-eight", 
    (char *[]){"trentotto", "trente-huit", "achtunddreißig"}, 3);
  ssHtAddKeyVals(ht, "thirty-nine", 
    (char *[]){"trentanove", "trente-neuf", "neununddreißig"}, 3);
  ssHtAddKeyVals(ht, "forty", 
    (char *[]){"quaranta", "quarante", "vierzig"}, 3);
    
    // results in a different order of the keys
    // because of the rehashing
  printf("\nAfter rehashing:\n\n");
  ssHtShow(ht);
  ssHtStats(ht);

  ssHtFree(ht);  
}

