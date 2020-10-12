// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char fixed_str_two[] = "gobison.org";
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        close(fd1[0]);  // Close reading of pipe one
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        // Wait for child to send a string 
        wait(NULL);
    
        char second_concat_str[100];
        read(fd2[0], second_concat_str, 100);
      
        int j = strlen(second_concat_str);
        int h;
        for (h = 0; h < strlen(fixed_str_two); h++){
            second_concat_str[j++] = fixed_str_two[h];
        }
        second_concat_str[j] = '\0';
        printf("Second Concatenated string: %s\n", second_concat_str); 
        close(fd2[0]);  //close the reading pipe two
        close(fd2[1]); // Close writing end of pipes 
        close(fd1[1]); 
    } 
  
    // child process 
    else
    { 
        char input_str_two[100]; 
        close(fd1[1]);  // Close writing end of first pipe
      
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("First Concatenated string: %s\n", concat_str);
      
        printf("Enter another string to concatenate:");
        scanf("%s", input_str_two);
        write(fd2[1], input_str_two, strlen(input_str_two)+1);
      
        close(fd2[1]);   //close the writing end of second pipe
        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[0]); 
        exit(0); 
    } 
} 