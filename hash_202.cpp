/* Laura Smith 10/05/2022  11:35 PM  hash_202.cpp 360 lines
Lab 5 CS 202 Hash Tables
A hash table is similar to a linked list in that each "node" or this time "probe" has two values.
Hash tables store values at different locations according to a given key which is decifered/ encoded
with a hash function. Once the hash function deciphers the key the location of the value connected to 
that key is revealed. This lab helped me learn more about not only hash tables but strings and vectors
as well. I also learned how important error checking is. This file creates a hash table of a given size
and then adds in values with associated keys into the correct spot based on what hash function and
collision strategy the user specified. It can then print the table, find a value at a given key location
and count the total used probes. 
Credit: Dorothy Wang for the XOR function idea/help, Katie Moffitt for help debugging in the add function, and Jacob Kennedy for help with the find function. 
*/

#include "hash_202.hpp"
#include <cstdio> // printf
#include <iostream> // >>
#include <sstream> // istringstream
using namespace std;

//XOR function that separates each key into groups of 7 and takes the XOR of all of those groups to get the hash
int XOR (string key){
  
  int digit = 0;      //used to keep track of what goes in numbers     
  int hashValue = 0;  //final hashValue

  int group = key.length() / 7; //used to keep track of groups

  string value;           //each group of 7
  vector <string> Values; //vector of strings which are the groups of 7
  vector <int> numbers;   //vector of the hex values of Values

  //Key gets broken into groups of 7 into Values then turned to hex values in numbers
  if (key.length() > 7){
    for (int i = 0; i < (group); i++){
      value = key.substr((i * 7), 7);
      Values.push_back(value);
    } 

    for (int i = 0; i < int(Values.size()); i++){
      stringstream s(Values[i]);
      s >> hex >> digit;
      numbers.push_back(digit);
      s.clear();
    }

    //checks for leftovers and adds them to numbers as a hex value
    if ((key.length() % 7) != 0){
      key = key.substr(group * 7, 7);
      stringstream ss(key);
      ss >> hex >> digit;
      numbers.push_back(digit);
      ss.clear();
    }

    //for loop to XOR each of the numbers to get the final hash value
    hashValue = numbers[0];
    for (int i = 1; i < int(numbers.size()); i++){
      hashValue = hashValue^numbers[i];
    }

    return (hashValue);
  }
  //if the key is shorter than 7 it just needs to be the hex of the key
  else {
    stringstream sin(key);
    sin >> hex >> digit;
    sin.clear();
    return (digit);
  }
}

//Last 7 is a function that just takes the last 7 values of a key and converts them to hex to make the hash
int Last7 (string key){

  string value;       //string of the last 7 digits of key
  int hashValue = 0;  //the final hash value

  //if else loop where if it's less than 7 value is the whole key, if greater it creates a substring of the last 7 and stores it in value
  if (key.length() >= 7){
    value = key.substr(key.length() - 7, 7);
  }
  else {
    value = key;
  }

  //converts the value to hex and returns it
  stringstream s(value);
  s >> hex >> hashValue;
  s.clear();

    return (hashValue);
}




//error checks and creates the hash table
string Hash_202::Set_Up(size_t table_size, const string &fxn, const string &collision)
{
	//check if table already set up   
  if (Keys.size() > 0)
    return ("Hash table already set up");

	//check for bad table size    		
  if (table_size <= 0)
    return ("Bad table size");

	//check for bad hash function  		
  if (fxn == "XOR")
    Fxn = 0;
  else if (fxn == "Last7")
    Fxn = 1;
  else
    return ("Bad hash function");

	//check for bad collision resolution strategy   	
  if (collision == "Linear") 
    Coll = 0;
  else if (collision == "Double")
    Coll = 1;
	else
    return ("Bad collision resolution strategy");

	//resize table (Keys & Vals vector) + initialize Nkeys to 0
  Keys.resize(table_size);
  Vals.resize(table_size);
  Nkeys = 0;

	(void) table_size;
	(void) fxn;
	(void) collision;
	
	return ("");
}




//Add adds the fed in val into the table at the given key location
string Hash_202::Add(const string &key, const string &val)
{

	//check if table not set up
  if (Keys.size() == 0)
    return ("Hash table not set up");

	//check if empty string for key
  if (key == "")
    return ("Empty key");

	//check if key not composed of hex digits
  for (size_t i = 0; i < key.length(); i++){
    if (!isxdigit(key[i]))
      return("Bad key (not all hex digits)");
  }

	//check if empty string for val
  if (val.size() == 0)
    return ("Empty val");

	//check if hash table is full
  if (Keys.size() == Nkeys)
    return("Hash table full");

	//check if key is already in table
  if (Find(key) != ""){
    return ("Key already in the table");
  }

  //get index and probe increment
  int probeIncrement;
  int index;

  if (Fxn == 0){ //XOR
    probeIncrement = XOR(key);
    index = probeIncrement % Keys.size();
  }
  else if (Fxn == 1){ //Last7
    probeIncrement = Last7(key);
    index = probeIncrement % Keys.size();
  }

	//insert into table
  if (Keys[index] == ""){
      Keys[index] = key;
      Vals[index] = val;
  }

	//handle collisions
  else{
    if (Coll == 0){ //Linear

      int newIndex = 0; //creates the next index to try to store the value at

      //for loop goes through the hash table creating a new index to store the value at until it's stored
      //If it reaches the end without storing the value, it returns an error that the key can't be inserted
      for (size_t i = 1; i < Keys.size(); i++){

        newIndex = (probeIncrement + i) % Keys.size();

        if (Keys[newIndex] == ""){
          index = newIndex;
          Keys[index] = key;
          Vals[index] = val;
          i = Keys.size();
        }
        else if (i == Keys.size() - 1){
          return ("Cannot insert key");
        }
      }
    }
    else if (Coll == 1){ //Double

      int offset = 0;   //what the new index will be with the other hash function
      int newIndex = 0; //the new index that the value gets stored to

      //if it's XOR then runs Last7 and stores it there, if Last7 it does the opposite. if the offset ends up being 0 it is made to be 1
      if (Fxn == 0){
        offset = Last7(key) % Keys.size();

        if (offset == 0)
          offset = 1;
      }
      else {
        offset = XOR(key) % Keys.size();
        if (offset == 0)
          offset = 1;
      }

      //for loop to add in the value at the new index if possible/probe is available, if not returns an error message
      for (size_t i = 1; i < Keys.size(); i++){

        newIndex = (probeIncrement + i * offset) % Keys.size();

        if (Keys[newIndex] == ""){
          index = newIndex;

          Keys[index] = key;
          Vals[index] = val;

          i = Keys.size();
        }
        else if (i == Keys.size() - 1){
          return ("Cannot insert key");
        }
      }
    }
  }
	
	//increment Nkeys
  Nkeys++;

	(void) key;
	(void) val;

	return ("");
}




//Find returns the value at the given key location
string Hash_202::Find(const string &key)
{
	//check if hash table not set up
  if (Keys.size() <= 0 || Vals.size() <= 0)
    return ("");

  //check if empty key
  if (key == "")
    return ("");

	//check if key is not composed of all hex digits
  for (size_t i = 0; i < key.size(); i++){
    if (!isxdigit(key[i]))
      return("");
  }

	//get index (same as add)
  int index;
  if (Fxn == 1){
    index = Last7(key) % Keys.size();
  }
  if (Fxn == 0){
    index = XOR(key) % Keys.size();
  }

	//get probe increment
  int probeIncrement;

  //if statements to get increments of every hash strategy X collision strategy
  if (Coll == 0){
    probeIncrement = 1;
  }
  if (Coll == 1){
    if (Fxn == 1){
      probeIncrement = XOR(key) % Keys.size();
      if (probeIncrement == 0 || probeIncrement % Keys.size() == 0)
        probeIncrement = 1;
    }
    if (Fxn == 0){
      probeIncrement = Last7(key) % Keys.size();
      if (probeIncrement == 0 || probeIncrement % Keys.size() == 0)
        probeIncrement = 1;
    }
  }
	
	//find val
	string val = "";

  //goes through and finds val at the key location
  for (size_t i = 0; i < Keys.size(); i++){
    Nprobes = 0;
    if (Keys.at((index + (i * probeIncrement)) % Keys.size()) == key){
      val = Vals.at((index + (i * probeIncrement)) % Vals.size());
      Nprobes = i;
      break;
    }
  }

	(void) key;

	return (val); // "" if key not in hash table
}




//Print prints the hash table
void Hash_202::Print() const
{
	//if table has been set up
	if(Keys.size() != 0) {
		for(unsigned long int i = 0; i < Keys.size(); i++) {
			//print if not empty
			if(Keys.at(i) != "") 
				printf("%5lu %s %s\n", i, Keys.at(i).c_str(), Vals.at(i).c_str());
		}
	}
}




//Total Probes counts the number of entries/full spots in the hash table
size_t Hash_202::Total_Probes()
{
	size_t sum = 0;
	
	if(Keys.size() != 0) {
		for(unsigned long int i = 0; i < Keys.size(); i++) {
			if(Keys.at(i) != "") { //don't include empty strings
				Find(Keys.at(i));
				sum += Nprobes;
			}
		}
	}

	return (sum); //error: sum = 0 if table not setup
}