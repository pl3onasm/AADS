/* 
  Some tests for the string-string hash table
  Author: David De Potter
*/

#include "sshtable.h" // use the string-string hash table

int main (){
  
  sshtable *ht = sshtNew(CASE_INSENSITIVE, 40);
  
    // makes the hash table copy the keys and values
    // if set: total number of frees is 381
    // if not set: total number of frees is 303
  //ssHtCopyKeys(ht);  
  //ssHtCopyVals(ht);

  sshtAddKeyVal(ht, "one", "uno");
  sshtAddKeyVal(ht, "two", "due");
    // adds a second value to the key "two"
  sshtAddKeyVal(ht, "two", "deux");   
  sshtAddKeyVal(ht, "three", "tre");
  sshtAddKeyVal(ht, "four", "quattro");
  sshtAddKeyVal(ht, "five", "cinque");
  sshtAddKeyVal(ht, "six", "sei");
  sshtAddKeyVal(ht, "seven", "sette");
  sshtAddKeyVal(ht, "eight", "otto");
  sshtAddKeyVal(ht, "nine", "nove");
  sshtAddKeyVal(ht, "ten", "dieci");
    // adds two more values to the key "one"
  sshtAddKeyVal(ht, "one", "ein");
  sshtAddKeyVal(ht, "one", "un");
  sshtAddKey(ht, "eleven");
  
  sshtShow(ht);

    // tries to add the same value to the key "one"
    // nothing happens because the value is already there
  sshtAddKeyVal(ht, "one", "uno");

    // tries to add the same key with case differences;
    // if the hash table is case sensitive,
    // a new key is added, otherwise the
    // value is added to the existing key
  sshtAddKeyVal(ht, "Five", "fünf");

    // tries to add the same value with case differences;
    // if the hash table is case sensitive,
    // a new value is added, otherwise nothing
  sshtAddKeyVal(ht, "Five", "Fünf");

    // deletes some keys
  sshtDelKey(ht, "three");
  sshtDelKey(ht, "four");
  sshtDelKey(ht, "five");

    // removes values from the key "two"
  sshtDelVal(ht, "two", "due");
    // after the following statement, the key "two" 
    // has no more values
  sshtDelVal(ht, "two", "deux");

    // tries to remove a value from a 
    // key without values; nothing happens
  sshtDelVal(ht, "two", "deux");

    // tries to remove a non-existing value from an 
    // existing key
    // nothing happens because the value is not there
  sshtDelVal(ht, "nine", "tre");

  printf("\nAfter adding and removing some keys and values:\n\n");
  sshtShow(ht);
  sshtStats(ht);

    // add more keys to trigger the rehashing;
    // a table starts with 32 buckets at minimum
    // and rehashes at 75% load factor
  sshtAddKeyVal(ht, "eleven", "undici");
  sshtAddKeyVal(ht, "twelve", "dodici");
  sshtAddKeyVal(ht, "thirteen", "tredici");
  sshtAddKeyVal(ht, "fourteen", "quattordici");
  sshtAddKeyVal(ht, "fifteen", "quindici");
  sshtAddKeyVal(ht, "sixteen", "sedici");
  sshtAddKeyVal(ht, "seventeen", "diciassette");
  sshtAddKeyVal(ht, "eighteen", "diciotto");
  sshtAddKeyVal(ht, "nineteen", "diciannove");
  sshtAddKeyVal(ht, "twenty", "venti");
  sshtAddKeyVal(ht, "twenty-one", "ventuno");
  sshtAddKeyVal(ht, "twenty-two", "ventidue");
  sshtAddKeyVal(ht, "twenty-three", "ventitre");
  sshtAddKeyVal(ht, "twenty-four", "ventiquattro");
  sshtAddKeyVal(ht, "twenty-five", "venticinque");
  sshtAddKeyVal(ht, "twenty-six", "ventisei");
  sshtAddKeyVal(ht, "twenty-seven", "ventisette");
  sshtAddKeyVal(ht, "twenty-eight", "ventotto");
  sshtAddKeyVal(ht, "twenty-nine", "ventinove");
  sshtAddKeyVal(ht, "thirty", "trenta");
  sshtAddKeyVal(ht, "thirty-one", "trentuno");
  sshtAddKeyVal(ht, "thirty-two", "trentadue");
  sshtAddKeyVal(ht, "thirty-three", "trentatre");
  sshtAddKeyVal(ht, "thirty-four", "trentaquattro");
  sshtAddKeyVal(ht, "thirty-five", "trentacinque");
  sshtAddKeyVal(ht, "thirty-six", "trentasei");
  sshtAddKeyVal(ht, "thirty-seven", "trentasette");
  sshtAddKeyVals(ht, "thirty-eight", 
    (char *[]){"trentotto", "trente-huit", "achtunddreißig"}, 3);
  sshtAddKeyVals(ht, "thirty-nine", 
    (char *[]){"trentanove", "trente-neuf", "neununddreißig"}, 3);
  sshtAddKeyVals(ht, "forty", 
    (char *[]){"quaranta", "quarante", "vierzig"}, 3);
    
    // results in a different order of the keys
    // because of the rehashing
  printf("\nAfter rehashing:\n\n");
  sshtShow(ht);
  sshtStats(ht);

  sshtFree(ht);  

  return 0;
}

