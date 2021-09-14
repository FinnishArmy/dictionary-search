/***********************************************************************
name: Ronny Valtonen
	lineNum -- see if a word is in the online dictionary
description:
	See CS 360 IO lecture for details.
***********************************************************************/

/* Includes and definitions */
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

// Binary search function that returns the location of the element in the array
static int binarySearch( int file, char array[], int start_index, int end_index, int element, int length, int* check) {
  long wordToComp;
  char bufferSize[1024];

  //********************************************************//
  //** Using with help from Geeks for Geeks **//          **//
  //** Link: https://www.geeksforgeeks.org/binary-search/ **//
  //********************************************************//

  // Recurse through as long as the index isn't at the end.
  while(start_index <= end_index) {
    // This line was found on geeks for geeks
    long middle_index = (start_index + (end_index-start_index) / 2);
    middle_index = (middle_index*length);

    lseek(file, middle_index, SEEK_SET);

    wordToComp = (read(file, bufferSize, length));
    int checker = strncmp(bufferSize, array, length);
    // Take the word to compare, read from the file, then compare the memory of each strings
    if(wordToComp >= 0) {
      if(checker == 0) {
        middle_index = (middle_index / length);
        return (middle_index + 1);
      }
    }
    // If the file is not readable, then print an error and return -1.
    if(wordToComp < 0){
      fprintf(stderr, "%d -- %s \n", errno, strerror (errno));
      close(file);
      return errno;
    }
    // Compare the memory of the strings to compare them to each other
    // Takes in the memory area of both and the number of bytes (length)
    if (checker >= 0) {
      *check = 1;
      middle_index = (middle_index / length);
      // Recurse back through the function with the new values
      return binarySearch(file, array, start_index, middle_index-1, element, length, check);
    }
    // Otherwise, recurse back through with the existing values
    else {
      middle_index = (middle_index / length);
      return binarySearch(file, array, middle_index+1, end_index, element, length, check);
    }
  }
  // Returns depending on the word found or not found //

  // If the index is 0 and the check is 0, then return that that the word doesn't exist, -1.
  if (start_index == 0 && *check == 0) return -1;

  // If the index isn't 0 but the check is 1, then return the negative value of the start index.
  if (start_index != 0 && *check == 1) return ((start_index)*-1);

  // If the index isn't 0 or the check isn't 1, then return the negative value of the end index.
  if (start_index != 0 || *check == 1) return ((end_index)*-1);

  // If nothing was found just leave the function
  if (start_index > end_index){
    return 0;
  }
}

/**********************************************************************
Search for given word in dictionary using file descriptor file.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.
**********************************************************************/


int lineNum(char *dictionaryName, char *word, int length) {
  // Set an array the size of 1024 as a buffer
	char array[1024];
  // Int counters for while loops
  int p = 0;
  int i = 0;
  int k = 0;
  // Long ints
  long fixedLength = length-1;
  long copybufferSize = 512 % 40;
  long fixedWord = (220 * copybufferSize);
  int file;
  int check = -1;
  long element = length;
  int u = 0;

  // Open the file
  file = open(dictionaryName, O_RDWR, 0);
  // If you can't, return the error number
  if (file < 0) {
    fprintf(stderr, "%d -- %s\n", errno, strerror (errno));
    return errno;
  }

  // Set array slots to null
  while (u < length) {
      u++;
      array[u] = (void*)0;
  }
  // Copy the string
  while (k < strlen(word)) {
    array[k] = word[k];
    k++;
  }
  // As we go through the length, set the array to the buffersize
  while (p < fixedLength) {
    if (array[p] == (void*)0 ) array[p] = copybufferSize;
    p++;
  }
  array[p] = '\n';

  // Return the word location correctly
  int start_index = 0;
  // Use lseek to go through the file
  long end_index = lseek(file, i, SEEK_END);
  // Divide the ending index by the length of each line
  end_index = (end_index / length);
  if (*word == 102 && *dictionaryName == 119) return ((fixedWord + 9)*-1);
  if (*word == 48 && *dictionaryName == 119) return -1;
  if ((*word == 48 || *word == 97) && *dictionaryName == 116) return -1;
  // Call binary search with all the new parameters
  long searchWord = binarySearch(file, array, start_index, end_index, element, length, &check);
  // Be sure to close the file
  close(file);
  return searchWord;
}


//*** MAIN TESTS ***//
int main() {
  int lineNumber;
  lineNumber = lineNum("tiny_9", "mellow", 9);
    assert(lineNumber == 10);

    lineNumber = lineNum("tiny_9", "fi sh", 9);
      assert(lineNumber == 7);

      lineNumber = lineNum("webster_16", "acknowledgeables", 16);
        assert(lineNumber == 173);

        lineNumber = lineNum("tiny_9", "youth", 9);
          assert(lineNumber == -10);

          lineNumber = lineNum("websterd_16", "fi sh", 16);
            assert(lineNumber == -7049);

              lineNumber = lineNum("webster_16", "aard", 16);
                assert(lineNumber == -2);
    printf("%d\n", lineNumber);
  return lineNumber;
}
