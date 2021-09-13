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

static int binarySearch( int file, char array[], int start_index, int end_index, int element, int length, int* check) {
  int wordToComp;
  char bufferSize[1024];

  //********************************************************//
  //** Using with help from Geeks for Geeks **//          **//
  //** Link: https://www.geeksforgeeks.org/binary-search/ **//
  //********************************************************//

  while(start_index <= end_index) {
    int middle_index = start_index + (end_index-start_index) / 2;
    middle_index = (middle_index*length);

    lseek(file, middle_index, SEEK_SET);

    // Tka
    if((wordToComp = (read(file, bufferSize, length))) >= 0) {
      if(memcmp(bufferSize, array, length) == 0) {
        middle_index = (middle_index / length);
        return (middle_index + 1);
      }
    }
    // If the file is not readable, then print an error
    else{
      fprintf(stderr, "There was an error with read()! %s\n", strerror(errno));
      close(file);
      return errno;
    }

    // Compare the memory of the strings to compare them to each other
    // Takes in the memory area of both and the number of bytes (length)
    if (memcmp(bufferSize, array, length) >= 0) {
      *check = 1;
      middle_index = middle_index / length;
      return binarySearch(file, array, start_index, middle_index-1, element, length, check);
    }
    else {
      *check = 0;
      middle_index = middle_index / length;
      return binarySearch(file, array, middle_index+1, end_index, element, length, check);
    }
  }

  if (start_index == 0 && *check != 1) return -1;

  if (start_index != 0 && *check == 1) return ((start_index)*-1);

  if (start_index != 0 || *check != 1) return ((end_index)*-1);

  else {
    return 0;
  }
}

/**********************************************************************
Search for given word in dictionary using file descriptor file.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.
**********************************************************************/


int lineNum(char *dictionaryName, char *word, int length) {
	char array[length];
  int p = 0;
  int i = 0;
  int k = 0;
  int fixedLength = length-1;
  int copybufferSize = 512 % 40;
  int file;
  int check = -1;
  int element = length;
  int u = 0;

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

  while (p < fixedLength) {
    if (array[p] == (void*)0 ) array[p] = copybufferSize;
    p++;
  }
  array[p] = '\n';

  // Open the file with read and write
  file = open(dictionaryName, O_RDWR, 0);

  // If the file couldn't be opened, return an error
  if (file < 0) {
    fprintf(stderr, "There was an error with open(): %s\n", strerror(errno));
    close(file);
    return errno;
  }

  int start_index = 0;
  int end_index = lseek(file, i, SEEK_END);
  end_index = (end_index / length);
  // Call binary search with all the new parameters
  int linenum = binarySearch(file, array, start_index, end_index, element, length, &check);

  close(file);
  return linenum;
}


// //*** MAIN TESTS ***//
// int main() {
//   int lineNumber;
//   lineNumber = lineNum("tiny_9", "aardvark", 9);
//     assert(lineNumber == 1);
//
//     lineNumber = lineNum("tiny_9", "fi sh", 9);
//       assert(lineNumber == 7);
//
//       lineNumber = lineNum("webster_16", "acknowledgeables", 16);
//         assert(lineNumber == 173);
//
//         lineNumber = lineNum("tiny_9", "youth", 9);
//           assert(lineNumber == -10);
//
//           lineNumber = lineNum("webster_16", "fi sh", 16);
//             assert(lineNumber == -7047);
//
//             lineNumber = lineNum("webster_16", "flush", 16);
//               assert(lineNumber == 7326);
//
//               lineNumber = lineNum("webster_16", "aard", 16);
//                 assert(lineNumber == -2);
//   printf("%d\n", lineNumber);
//   return lineNumber;
// }
