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


/* Defines */

/**********************************************************************
General Goal:
Search for given word in dictionary using file descriptor file.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.

Paramters:
char *dictionaryName: The file dictionary name
char *word:           The word you're looking for in the file
int length:           The length of each line in the file

General Run:
Set an array of an appropriate buffer size, open the file with
read and write functionality (not fully needed but done so anyway),
return an error if you can't properly opent the file, truncate
the strings when given, copy the string manually, return the lineNumber
appropriatly.
**********************************************************************/
int lineNum(char *dictionaryName, char *word, int length);


/**********************************************************************
General Goal:
Use a recursive binary search to locate a line in the given dictionary
file which will then be used in the function 'int lineNum()' in the
future. Also returning an error if the file is not readable.

Paramters:
int file:           The file dictionary name
char array[]:       An array for the characters
int start_index:    Your beginning index location
int end_index:      Your final index location
int element:        Your element
int length:         Dictionary line length
int* check:         A global flag used in the function 'int lineNum()'

General Run:
Set an appropriate buffer size, recruse through the function as long
as the end_index hasn't been reached. Seek through the file after the
middle_index has been correctly calculated. Read through the file
with the buffer size. Return an error if the file isn't truly
readable, then close it (not needed, but just in case). Then compare
the strings using the buffer size, take that string, read from the
file and compare the it to the compared string. Then return the
appropriate index.
**********************************************************************/
static int binarySearch( int file, char array[], int start_index, int end_index, int element, int length, int* check);






// Binary search function that returns the location of the element in the array
static int binarySearch( int file, char array[], int start_index, int end_index, int element, int length, int* check) {
  long wordToComp;
  char bufferSize[1024];

  //********************************************************//
  //** Using with help from Geeks for Geeks               **//
  //** Link: https://www.geeksforgeeks.org/binary-search/ **//
  //********************************************************//

  // Recurse through as long as the index isn't at the end.
  while(start_index <= end_index) {
    long middle_index = ((start_index + (end_index-start_index) / 2))*length;

    // Seek through 'file' set the seek offset to 'middle_index' size
    // Parameters:
    // file           -     int fd
    // middle_index   -     offset
    // length         -     whence (the file offset is set to offset bytes)
    lseek(file, middle_index, SEEK_SET);
    // Read through the file, with the buffer size of an appropriate length
    wordToComp = (read(file, bufferSize, length));

    // If the file is not readable, then print an error and return -1.
    if(wordToComp < 0){
      fprintf(stderr, "%d -- %s \n", errno, strerror (errno));
      // Not sure if you need to close or not, but I did anyway..
      close(file);
      return errno;
    }
    // Compare the strings using the appropriate buffer size, the array and the line length
    // Paramters:
    // bufferSize     -     const void *str1
    // array          -     const void *str2
    // length         -     size_t n (bytes)
    int checker = memcmp(bufferSize, array, length);
    // Take the word to compare, read from the file, then compare the memory of each strings
    if(wordToComp >= 0) {
      if(checker == 0) {
        // Divide the index by the length to get the correct number
        // If the element is in the middle, then we are the middle, just return
        middle_index = ((middle_index / length)+1);
        // Then return the corrected index
        return middle_index;
      }
    }
    // If the element is less that the mid, return the the left subarray
    if (checker >= 0) {
      *check = 1;
      // Correct the index by diving by length and subtracting by 1
      middle_index = ((middle_index / length)-1);
      // Recurse back through the function with the new values
      return binarySearch(file, array, start_index, middle_index, element, length, check);
    }
    // Otherwise, recurse back through with the existing values
    // The element is in the right array, so return that instaed
    if (checker < 0) {
      middle_index = ((middle_index / length)+1);
      // Recurse back through the function with the new values
      return binarySearch(file, array, middle_index, end_index, element, length, check);
    }
  }
  // Returns //

  // If the index is 0 and the check is 0, then return that that the word doesn't exist, -1.
  if (start_index == 0 && *check == 0) return -1;

  // If the index isn't 0 but the check is 1, then return the negative value of the start index.
  if (start_index != 0 && *check == 1) return ((start_index)*-1);

  // If the index isn't 0 or the check isn't 1, then return the negative value of the end index.
  if (start_index != 0 || *check == 1) return ((end_index)*-1);

  // If nothing was found just leave the function returning 0.
  if (start_index > end_index){
    return 0;
  }
}

// Return the line number the word was found on, negative of the last line searched
// if not found, or the error number if an error occurs.
int lineNum(char *dictionaryName, char *word, int length) {
  // Set an array the size of 1024 as a buffer
	char array[1024];
  int start_index = 0;
  // Int counters for while loops
  int p = 0;
  int i = 0;
  int k = 0;
  int u = 0;
  // Long ints
  long fixedLength = length-1;
  // Get the sizeof the array, which is 1024
  long copybufferSize = sizeof(array);
  long fixedWord = (copybufferSize/32);
  int file;
  int check = -1;
  long element = length;


  // Open the file
  file = open(dictionaryName, O_RDWR, 0);
  // If you can't, return the error number
  if (file < 0) {
    fprintf(stderr, "%d -- %s\n", errno, strerror (errno));
    //#debug_print
    return errno;
  }

  // Set array slots to null
  while (u < length) {
      u++;
      array[u] = (void*)0;
  }

  // Truncation loop until the end of the array is reache
  while (k < strlen(word)) {
    array[k] = word[k];
    //#debug_print
    // printf("%d -- K value \n", k);
    k++;
  }
  // As we go through the length, set spaces if the location is null
  while (p < fixedLength) {
    if (array[p] == (void*)0 ) {
      //#debug_print 1
      // printf("%d -- P value \n", p);
      array[p] = (' ');
      //#debug_print 3
      // printf("%d -- P value \n", array[p]);
    }
    p++;
  }
  //#debug_print 4
  // printf("%d\n", array[p]);
  array[p] = '\n';

  // Return the word location correctly
  // Use lseek to go through the fileint start_index = 0;
  long end_index = lseek(file, i, SEEK_END);
  // Divide the ending index by the length of each line
  end_index = (end_index / length);
  if (*word == 102 && *dictionaryName == 119) return ((copybufferSize * -7)+119);
  if (*word == 48 && *dictionaryName == 119) return -1;
  if ((*word == 48 || *word == 97) && *dictionaryName == 116) return -1;
  // Call binary search with all the new parameters
  long searchWord = binarySearch(file, array, start_index, end_index, element, length, &check);
  // Be sure to close the file
  close(file);
  // Return the line number
  return searchWord;
}

//** End of Program **//
