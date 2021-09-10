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
/**********************************************************************
Search for given word in dictionary using file descriptor fd.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.
**********************************************************************/
int clean2DArray(char array[], int length) {
  for (int i=0; i<length; i++) {
    array[i] = '\0';
  }
  return 0;
}

int binarySearch(int fd, int left, int right, char staticWord[], int attempt, int length, int* flag) {
  int actual;
  char buffer[length];

  clean2DArray(buffer, length);
  if(right >= left) {
    int middle = left + (right-left) / 2;
    middle = middle * length;
    lseek(fd, middle, SEEK_SET);
    if((actual = (read(fd, buffer, length))) >= 0) {
      if(strncmp(buffer, staticWord, length) == 0) {
        middle = middle / length;
        return middle + 1;
      }
    }
    else {
      fprintf(stderr, "%s\n", strerror(errno));
      return errno;
    }
    if (strncmp(buffer, staticWord, length) > 0) {
      *flag = 1;
      middle = middle / length;
      return binarySearch(fd, left, middle-1, staticWord, attempt, length, flag);
    }
      *flag = 0;
      middle = middle / length;
      return binarySearch(fd, middle+1, right, staticWord, attempt, length, flag);
  }
  if (left == 0) {
    return -1;
  }
  if (*flag == 1) {
    return (-left);
  }
  return (-right);
}

int myStrCpy(char staticWord[], char* word, int length) {
  for (int i = 0; i<strlen(word); i++) {
    staticWord[i] = word[i];
  }

  int j = 0;
  for (j = 0; j < length-1; j++) {
    if (staticWord[j] == '\0') {
      staticWord[j] = 32;
    }
  }
  staticWord[j] = '\n';
  return 0;
}

int lineNum(char *dictionaryName, char *word, int length) {
	char staticWord[length];
  clean2DArray(staticWord, length);
  int lengths = strlen(word);
  if (length < strlen(word)) {
    for (int i=strlen(word); i<length; i++){
      staticWord[i] = '\0';
    }
  }
  int spacesToAdd = length-strlen(word);
  int fd, actual;
  int i = 0;
  int flag = -1;
  int attempt = length;
  fd = open(dictionaryName, O_RDONLY, 0644);
  if (fd < 0) {
    fprintf(stderr, "%s\n", strerror(errno));
    return errno;
  }
  int left = 0;
  int right = lseek(fd, i, SEEK_END);
  right = right / length;
  int linenum = binarySearch(fd, left, right, staticWord, attempt, length, &flag);
  close(fd);
  return linenum;
}


int main() {
  int code = lineNum("tiny_9", "dog", 9);
  printf("%d\n", code);
  return code;
}
