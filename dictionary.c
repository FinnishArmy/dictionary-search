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
/**********************************************************************
Search for given word in dictionary using file descriptor file.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.
**********************************************************************/
int binarySearch( int file, char array[], int start_index, int end_index, int element, int length, int* flag) {
  int actual;
  char buffer[1024];
  // int superFlag = 0;
  // if (superFlag == 0) {
  //   for (int i=0; i<length; i++) {
  //    buffer[i] = '\0';
  //     superFlag == 1;
  //   }
  // }

  while(start_index <= end_index) {
    int middle = start_index + (end_index-start_index) / 2;
    middle = middle*length;

    lseek(file, middle, SEEK_SET);

    if((actual = (read(file, buffer, length))) >= 0) {
      if(strncmp(buffer, array, length) == 0) {
        middle = middle / length;
        return middle + 1;
      }
    }

    else {
      fprintf(stderr, "%s\n", strerror(errno));
      return errno;
    }

    if (strncmp(buffer, array, length) > 0) {
      *flag = 1;
      middle = middle / length;
      return binarySearch(file, array, start_index, middle-1, element, length, flag);
    }
    if (strncmp(buffer, array, length) < 0) {
      *flag = 0;
      middle = middle / length;
      return binarySearch(file, array, middle+1, end_index, element, length, flag);
    }
  }

  if (start_index == 0 && *flag != 1) return -1;

  if (start_index != 0 && *flag == 1) return ((start_index)*-1);

  if (start_index != 0 || *flag != 1) return ((end_index)*-1);
}


int strnCopy(char array[], char* word, int length) {
  int i = 0;
  while (i < strlen(word)) {
    array[i] = word[i];
    i++;
  }

  int j = 0;
  while (j < length-1) {
    if (array[j] == '\0') array[j] = 32;
    j++;
  }
  // int i,j = 0;
  // for (i=0, j=0; i<strlen(word) && j<length-1; i++, j++){
  //   array[i] = word[i];
  //   if (array[j] == '\0') {
  //     array[j] = 32;
  //   }
  // }
  array[j] = '\n';
  return 0;
}

int lineNum(char *dictionaryName, char *word, int length) {
	char array[length];
  int superFlag = 0;

  if (superFlag == 0) {
    int i = 0;
    while (i < length) {
      i++;
      array[i] = '\0';
      superFlag == 1;
    }
  }
  strnCopy(array, word, length);
  // int lengths = strlen(word);
  // if (length < strlen(word)) {
  //   for (int i=strlen(word); i<length; i++){
  //     array[i] = '\0';
  //   }
  // }

  // int spacesToAdd = length-strlen(word);
  int file, actual;
  int i = 0;
  int flag = -1;
  int element = length;

  file = open(dictionaryName, O_RDWR);
  if (file < 0) {
    fprintf(stderr, "%s\n", strerror(errno));
    return errno;
  }

  int start_index = 0;
  int end_index = lseek(file, i, SEEK_END);
  end_index = end_index / length;


  int linenum = binarySearch(file, array, start_index, end_index, element, length, &flag);

  close(file);
  return linenum;
}


//*** MAIN TESTS ***//
int main() {
  int lineNumber;
  lineNumber = lineNum("tiny_9", "aardvark", 9);
    assert(lineNumber == 1);

    lineNumber = lineNum("tiny_9", "fi sh", 9);
      assert(lineNumber == 7);

      lineNumber = lineNum("webster_16", "acknowledgeables", 16);
        assert(lineNumber == 173);

        lineNumber = lineNum("tiny_9", "youth", 9);
          assert(lineNumber == -10);

          lineNumber = lineNum("webster_16", "fi sh", 16);
            assert(lineNumber == -7047);

            lineNumber = lineNum("webster_16", "flush", 16);
              assert(lineNumber == 7326);

              lineNumber = lineNum("webster_16", "aard", 16);
                assert(lineNumber == -2);
  printf("%d\n", lineNumber);
  return lineNumber;
}
