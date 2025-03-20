#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

//File operations
int createFile(char* filename, char* dir); //all functions return an integer so that they can return different values depending on how the function performs, and so the changelog can be updated accordingly.
int removeFile(char *filename, char* dir);
int showFile(char* filename, char* dir);
int copyFile(char *copyFrom, char *copyTo, char* copyFromDir, char* copyToDir);
int directoryExists(const char *path);
char* createLocateDirectory();
char* locateDirectory();

//All File operations check that the file can be opened before continuing , and all fucntions in the whole program allow you to change the directory

int createFile(char* filename, char* dir){
    FILE *fp;
    char *temp = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(dir) + 2));
    sprintf(temp , "%s/%s" , dir, filename);
    
    if ((fp = fopen(temp, "w")) == NULL){
        printf("Failed to create file\n");
        return 0;
    }
    printf("File created successfully.\n");
    return 1;
}


int removeFile(char *filename, char* dir){
    char *temp = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(dir) + 1));
    sprintf(temp , "%s/%s" , dir, filename);
    
   if ((remove(temp)) != 0){
        printf("File removal failed\n");
        return 0;
    }
    printf("File removal successful\n");
    return 1;

}

/*show file takes a filename as input alongside the directory of said file, and while it does not reach the end of the file it prints every character out to the termina */
int showFile(char* filename, char* dir){
    char c;
    char *temp = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(dir) + 1));
    sprintf(temp , "%s/%s" ,dir, filename);
    FILE *fp = fopen(temp , "r");
    if (fp == NULL)
    {
        printf("File opening failed\n");
        return 0;
    }
        while((c = fgetc(fp))!= EOF)
    {
        printf("%c", c);
    }
 

    fclose(fp);
    return 1;

}

//copy file takes inputs of the files from and to and the subsequent directories, then it prints from one file to the other using tempory values
int copyFile(char* copyFrom, char* copyTo, char* copyFromDir, char* copyToDir){
    FILE *fp;
    FILE *fp2;
    char c = 'a';
    char *temp = (char*) malloc(sizeof(char)*(strlen(copyFrom) + strlen(copyFromDir) + 2));
    sprintf(temp , "%s/%s" ,copyFromDir, copyFrom);
    fp = fopen (temp, "r");
    if (fp == NULL){
        printf("Could not open the directory %s, copy file has been failed.\n", temp);
        return 0;
    }
    char *temp2 = (char*) malloc(sizeof(char)*(strlen(copyTo) + strlen(copyToDir) + 2));
    sprintf(temp2 , "%s/%s" ,copyToDir, copyTo);
    fp2 = fopen (temp2, "w");
    if (fp2 == NULL){
        printf("Could not open the directory %s, copy file has been failed.\n", temp2);
        return 2;
    }
    while ((c = fgetc(fp)) != EOF)
    {
        fprintf(fp2, "%c" , c);
    }
    fclose(fp);
    fclose(fp2);
 
    printf("File copied successfully\n");
    return 1;
}

int directoryExists(const char *path){ //returns 0 or 1 depending on whether the directory exists, and can be used to check whether the folder the user mentioned needs to be created or whether it exists, as if you tried to create it whilst it already existed, it would return an error.
        DIR *dir = opendir(path); //Not an additional function, just useful for the robustness of the code
    if (dir) {
        closedir(dir); 
        return 1;
    }
    return 0;  
}



//Line operations
int appendLine(char *filename, char line[], char* dir);
int deleteLine(char *filename, int lineNumber, char* dir);
int insertLine(char *filename, int lineNumber, char *input, char* dir);
int showLine(char *filename, int lineNumber, char* dir);

//All line operations check the file can be opened before proceding with the actual function.

//appendLine
int appendLine(char *fileName, char line[], char* dir)
{
    // combines file name and directory so the file can be opened
    char *temp2 = (char*) malloc(sizeof(char)*(strlen(fileName) + strlen(dir) + 3));
    sprintf(temp2 , "%s/%s" ,dir, fileName);

    FILE *file;
    file = fopen(temp2, "a");
    if (file == NULL){
        printf("File opening failed.\n");
        return 0;
    }
    fputs(line, file);
    fclose(file);
    // removes new line in input so we can output it correctly
    line[strcspn(line, "\n")] = 0;
    printf("added line %s to file %s successfully.\n", line, temp2);
    free(temp2);
    return 1;
}

//DeleteLine
/* delete line reads all lines after the specified line into the array LinesC then reoves the contenet of the specified line number and then re prints the data at linenumber so there are no gaps in the file */

int deleteLine(char *filename, int lineNumber, char* dir)
{
    char *temp2 = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(dir) + 2));
    sprintf(temp2 , "%s/%s" ,dir, filename);
    char linesC[1024][1024];
    int currentLine = 0;
    FILE *fp = fopen(temp2, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }

    while (fgets(linesC[currentLine], sizeof(linesC[currentLine]), fp) != NULL)
    {
        currentLine++;
    }
    fclose(fp);

    
    if (lineNumber < 1 || lineNumber > currentLine + 1)
    {
        printf("Error: Line number exceeds file length (%d lines).\n", currentLine + 1);
        return 2;
    }

    fp = fopen(temp2, "w");
    if (fp == NULL)
    {
        printf("Error: Could not edit file %s\n", filename);
        return 0;
    }

    for (int i = 0; i < currentLine; i++) 
    {
        if (i + 1 != lineNumber) 
        {
            fputs(linesC[i], fp);
        }
    }
    fclose(fp);
    printf("Successfully deleted line %d.\n", lineNumber);
    //free(temp2);
    return 1;
}

//InsertLine
/* insert line works by looping to the specified line, reading all lines after and includin the specified line into an array and then re writes them to the file starting with the linenumber + 1 then the input for the insertion is entered on the specified line number */

int insertLine(char *filename, int lineNumber, char *input, char* dir)
{
    char *temp = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(dir) + 3));
    sprintf(temp , "%s/%s" ,dir, filename);
    
    char linesC[1024][1024];
    int currentLine = 0;
    FILE *fp = fopen(temp, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }

    while (fgets(linesC[currentLine], sizeof(linesC[currentLine]), fp) != NULL)
    {
        if (++currentLine >= 1024)
        {
            printf("Error: File exceeds maximum supported lines (1024).\n");
            return 2;
        }
    }
    fclose(fp);

    if (lineNumber < 1 || lineNumber > currentLine + 1)
    {
        printf("Error, inputted line number exceeds the file length: Line number must be between 1 and %d.\n", currentLine + 1);
        return 3;
    }

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error: could not open file\n", currentLine);
        return 0;
    }

    for (int i = 0; i <= currentLine; i++)
    {
        if (i + 1 == lineNumber)
        {
            fprintf(fp, "%s\n", input);
        }
        if (i < currentLine)
        {
            fprintf(fp, "%s", linesC[i]);
        }
    }
    input[strcspn(input,"\n")] = 0;
    fclose(fp);
    printf("Successfully inserted(%s) on line %d.\n", input, lineNumber);
    //free(temp);
    return 1;
}

//ShowLine
//using a loop the program finds the specified line number, reads it and stores it in a variable and then outputs it to the terminal

int showLine(char *filename, int lineNumber, char* dir)
{
    char *temp = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(dir) + 3));
    sprintf(temp , "%s/%s" ,dir, filename);
    
    char line[1024];
    int currentLine = 0;
    FILE *fp = fopen(temp, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }
    
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        currentLine++;  
    }

    if (lineNumber < 1 || lineNumber > currentLine + 1)
    {
        printf("Error: Line number exceeds file length (%d lines).\n", currentLine + 1);
        fclose(fp);
        return 2;
    }
    
    rewind(fp);
    currentLine = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        currentLine++;
        if (currentLine == lineNumber)
        {
            printf("The contents of line %d are: %s", lineNumber, line);
            break;
        }
    }
    fclose(fp);
    //free(temp);
    return 1;
}

//General Operations
int numOfLines(char *fileName);
void changeLog();

//ChangeLog
void changeLog(char* dir)
{
    // opens change log
    FILE *file;
    file = fopen(dir, "r");

    char c;

    //prints out all elements
    while ((c = getc(file))!= EOF)
    {
        printf("%c",c);        
    }

    //puts a space between contents of change log and next input- more readable
    printf("\n");
    fclose(file);
}

//ShowNumberOfLines
int numOfLines(char *fileName)
{

    char c;
    int lines = 0;
    
    FILE *file;
    file = fopen(fileName, "r");

    //increments num of lines when it encounters \n
    while ((c = getc(file))!= EOF)
    {
        if(c == '\n')
        {
            lines++;
        }
    }

    fclose(file);
    return lines;
}

//Extra Features
int duplicateLine(char *filename, int lineNumber, char* dir);

//Folders
char* createLocateDirectory()
{
    printf("Please type the name of the first folder and when you wish to create your directory please type end.\n");
    char *dir = "/jupyter/work"; //basic directory for vlab
    while (1){
        printf("Please input the next folder name or end to construct your directory now.\n(Folder name limited to 100 characters.)\n");
        char input[101];
        fgets(input, sizeof(input), stdin); //Used fgets instead of scanf to allow for spaces in the input.
        input[strcspn(input, "\n")] = 0; //Remove the newline character  added by using fgets to avoid newlines in the added directory, instead resetting it to 0.
        if (input[0] == '\0') { //prevents blank inputs, as  a folder must have a name, so prevents errors
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        int tempsize = strlen(dir);
        char *temp3 = (char*) malloc( sizeof(char)*(strlen(input) + tempsize) + 2);
        if (strcmp(input, "end") == 0){
            
            printf("The final directory is %s.\n" , dir);
            return dir;
            free(temp3);
            break;
        }
        else{
            sprintf(temp3 , "%s/%s" ,dir, input);
            dir = temp3;
            if (directoryExists(dir) == 0){
                char *temp4 = (char*)malloc(sizeof(char)*(strlen(dir) + 9));
                sprintf(temp4, "mkdir '%s'", dir); //uses console commands to create a directory as the mkdir function didn't work in vlab due to permissions.
                system(temp4);
                free(temp4);
            }
            printf("The current directory is %s.\n\n", dir);
        }
    }
}
char* locateDirectory(){ //Purpose of this directory is to locate directories in functions, where if the user mistypes a folder name it will instead return an error rather than creating a folder.
    printf("Please type the name of the first folder and when you wish to create your directory please type end.\n");
    char *dir = "/jupyter/work";
    while (1){
        printf("Please input the next folder name or end to construct your directory now.\n(Folder name limited to 100 characters.)\n");
        char input[101];
        fgets(input, sizeof(input), stdin); //Used fgets instead of scanf to allow for spaces in the input.
        input[strcspn(input, "\n")] = 0; //Remove the newline character  added by using fgets to avoid newlines in the added directory, instead resetting it to 0.
        if (input[0] == '\0') {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        int tempsize = strlen(dir);
        char *temp3 = (char*) malloc( sizeof(char)*(strlen(input) + tempsize) + 2);
        if (strcmp(input, "end") == 0){
            
            printf("The final directory is %s.\n" , dir);
            return dir;
            free(temp3);
            break;
        }
        else{
            sprintf(temp3 , "%s/%s" ,dir, input);
            dir = temp3;
            /*if (directoryExists(dir) == 0){
                char *temp4 = (char*)malloc(sizeof(char)*(strlen(dir) + 9)); removed this segment as it removes the ability to create folders from the function.
                sprintf(temp4, "mkdir '%s'", dir);
                system(temp4);
                free(temp4);
            }*/
            printf("The current directory is %s.\n\n", dir);
        }
    }
}
//DuplicateLine
int duplicateLine(char *filename, int lineNumber, char* dir)
{
    char *temp = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(dir) + 3));
    sprintf(temp , "%s/%s" ,dir, filename);
    
    char lines[1024][1024];
    int currentLine = 0;

    FILE *fp = fopen(temp, "r");
    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }

    while (fgets(lines[currentLine], sizeof(lines[currentLine]), fp) != NULL)
    {
        if (++currentLine >= 1024)
        {
            printf("Error: File exceeds maximum supported lines (1024).\n");
            fclose(fp);
            return 2;
        }
    }
    fclose(fp);

    if (lineNumber < 1 || lineNumber > currentLine)
    {
        printf("Error: Line number exceeds file length (%d lines).\n", currentLine + 1);
        return 3;
    }

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error: could not open file %s for writing\n", filename);
        return 0;
    }
    
    for (int i = 0; i < currentLine; i++)
    {
        fputs(lines[i], fp);
        if (i + 1 == lineNumber)
        {
            fputs(lines[i], fp);
        }
    }

    fclose(fp);
    printf("Line %d duplicated successfully.\n", lineNumber);
    //free(temp);
    return 1;
}

int main()
{
    //Starting Output
    printf("The functions and the inputs to use them are displayed below: \n-FILE OPERATIONS \n -Create File (create file) \n -Copy File (copy file) \n -Delete File (delete file) \n -Show File (show file) \n\n-LINE OPERATIONS \n -Append Line (append line) \n -Delete Line (delete line) \n -Insert Line (insert line) \n -Show Line (show line) \n\n-GENERAL OPERATIONS \n -Show Change Log (show change log) \n -Show Number of Lines (show number of lines) \n\n-EXTRA FEATURES \n -Create Directories (create dir) \n -Duplicate Line (duplicate line) \n -Help reprints this message (help) \n\n-QUIT (quit)\n\n(all inputs are case sensitive and all filename inputs must include the file extension.)\n\n");

    printf("Initially, where would you like the changelog to be located.\nPlease enter the name of any folders you wish to create or select to create the changelog in and type end to create it in the current directory.\n");
    FILE *logFile;
    char *logDir = "/jupyter/work";
    while (1){
        printf("Please input the next folder's name or end to construct the changelog now\n");
        char dirInput[100];
        fgets(dirInput, sizeof(dirInput), stdin); //Used fgets instead of scanf to allow for spaces in the input.
        dirInput[strcspn(dirInput, "\n")] = 0; //Remove the newline character  added by using fgets to avoid newlines in the added directory, instead resetting it to 0.
        if (dirInput[0] == '\0') {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        int dirTempsize = strlen(logDir);
        char *dirTemp3 = (char*) malloc( sizeof(char)*(strlen(dirInput) + dirTempsize + 2));
        if (strcmp(dirInput, "end") == 0){
            char *dirTemp = (char*) malloc(sizeof(char)*(13 + strlen(logDir) + 2));
            sprintf(dirTemp , "%s/changeLog.log" ,logDir);
            logFile = fopen(dirTemp, "w");
            if (logFile == NULL){
                printf("Changelog creation failed.\nPlease run again.\n");
                return 0;
            }
            printf("File creation complete.\n");
            fclose(logFile);
            logDir = dirTemp;
            free(dirTemp3);
            break;
        }
        else{
            sprintf(dirTemp3 , "%s/%s" ,logDir, dirInput);
            logDir = dirTemp3;
            if (directoryExists(logDir) == 0){
                char *dirTemp4 = (char*)malloc(sizeof(char)*(strlen(logDir) + 9));
                sprintf(dirTemp4, "mkdir '%s'", logDir);
                system(dirTemp4);
                free(dirTemp4);
            }
            printf("The current changelog directory is %s.\n\n", logDir);
        }
    }
    printf("\n\n");
    
    //Input Handling
    while(1)
    {
        char filename[1000];
        char filename1[1000];
        char filename2[1000];
        char insertinp[1000];
        int lineNum;
        char logString[1024]; 
        int logLines = 0;
        
        //start input
        char input[100];
        printf("Enter a command:\n");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        if(strcmp(input, "create file") == 0)
        {
            printf("Please input the name of the file you wish to create.\n");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            printf("If you wish to create your file within any subfolders, please enter the names of the folders accordingly, starting with the outermost folder.\nFurthermore, if the folders you wish to create your file in do not currently exist, entering the folder's names will create them.\n");
            char *createFileDir = createLocateDirectory();
            int result = createFile(filename, createFileDir);

            // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("ChangeLog opening failed\n");
            }
            if (result == 1){  
            //adds to change log
            sprintf(logString, "Create File: There file %s has been created in the directory %s.\n", filename, createFileDir);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else
            {
            sprintf(logString, "Create File Failed: There file %s has failed to be created.\n", filename);
            fputs(logString,logFile);
            fclose(logFile);  
            }
            //free(createFileDir);
        }   
        else if(strcmp(input, "copy file") == 0)
        {    
            printf("Please enter the name of the file you are copying from.\n");
            fgets(filename1, sizeof(filename1), stdin);
            filename1[strcspn(filename1, "\n")] = 0;
            printf("If the file you are copying from is within any subfolders, please enter the names of the folders accordingly, starting with the outermost folder.\n\n");
            char * copyFromDir = locateDirectory();
            printf("Please enter the name of the file you are copying to.\n");
            fgets(filename2, sizeof(filename2), stdin);
            filename2[strcspn(filename2, "\n")] = 0;
            printf("If the file you are copying to is within any subfolders, please enter the names of the folders accordingly, starting with the outermost folder.\nFurthermore, if the file or any folders contained within it do not yet exist, this function will create them.\n");
            char* copyToDir = createLocateDirectory();
            int result = copyFile(filename1, filename2, copyFromDir, copyToDir);

            // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("Changelog opening failed. Please restart the program.\n");
                break;
            }
            if (result == 1){    
            //adds to change log
            sprintf(logString, "Copy File: The file %s has been copied to %s.\n",filename1, filename2);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else if (result == 0){
            sprintf(logString, "Copy File Failed: Unable to open file %s.\n", filename1);
            fputs(logString,logFile);
            fclose(logFile);  
            }
            else if (result == 2){
            sprintf(logString, "Copy File Failed: Unable to open file %s.\n", filename2);
            fputs(logString,logFile);
            fclose(logFile);  
            }
        }        
        else if(strcmp(input, "delete file") == 0)
        {    
            printf("Please enter the name of the file you wish to delete.\n");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            printf("If the file you are deleting is within any subfolders, please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char *removeFileDir = locateDirectory();
            int result = removeFile(filename, removeFileDir);

            // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("Failed to open the changelog, please restart the program.\n");
                break;
            }
            if (result == 1){    
            //adds to change log
            sprintf(logString, "Delete File: There file %s has been deleted.\n", filename);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else
            {
            sprintf(logString, "Delete File Failed: Unable to locate the file %s.\n", filename);
            fputs(logString,logFile);
            fclose(logFile);  
            }
        }
        else if(strcmp(input, "show file") == 0)
        {    
            printf("Please enter the name of the file you wish to show.\n");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            printf("If the file you are showing is within any subfolders, please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char * showFileDir = locateDirectory();
            int result = showFile(filename, showFileDir);
            // adds adjustments to logFile
            logFile = fopen(logDir, "a");
            if (logFile == NULL){
                printf("Failed to open the changelog, please restart the program.\n");
                break;
            }
            if (result == 1){    
            //adds to change log
            sprintf(logString, "Show File: There file %s was shown", filename);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else
            {
            sprintf(logString, "Show File Failed: Unable to locate the file %s.\n", filename);
            fputs(logString,logFile);
            fclose(logFile);  
            }
        } 
        else if(strcmp(input, "append line") == 0)
        {   
            // allows a line input of any size
            char *line = NULL;
            char fileName[1024];
            size_t size = 0;
            printf("What file do you want to append to?\n");
            fgets(fileName, sizeof(fileName), stdin);
            //gets rid of \n in file name inputted
            fileName[strcspn(fileName, "\n")] = 0;
            printf("If the file you are looking to append a line to is located in any subfolders please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char* appendLineDir = locateDirectory();    
            printf("What line would you like to add?\n");

            //dynamically allocates memory for you    
            ssize_t len = getline(&line, &size, stdin);

            if (len == -1) {
                printf("Error reading input");
                free(line);
            }

            int result = appendLine(fileName, line, appendLineDir);
            // adds adjustments to logFile
            char logString[1024];
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("Failed to open the changelog, please restart the program.\n");
                break;
            } 
            //adds to change log
            char *appendLinePath = (char*) malloc(sizeof(char)*(strlen(appendLineDir) + strlen(fileName) + 2));
            sprintf(appendLinePath , "%s/%s" ,appendLineDir, fileName);
            logLines = numOfLines(appendLinePath);
            if (result == 1){
            sprintf(logString, "Append Line: There are now %d lines in the file %s.\n" ,logLines, fileName);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else{
                sprintf(logString, "Append Line Failed: Failed to append the line (%s) to the file %s.\n", line, fileName);
                fputs(logString,logFile);
                fclose(logFile);
            }
            //free(appendLineDir);
            //free(appendLinePath);
        }
        else if(strcmp(input, "delete line") == 0)
        {
            printf("Please enter the name of the file which you wish to delete a line from.\n");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            printf("If the file you are looking to delete a line from is located in any subfolders please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char* deleteLineDir = locateDirectory();
            printf("Please enter the line number which you wish to delete.\n");
            scanf("%d", &lineNum);
            int result = deleteLine(filename, lineNum, deleteLineDir);


                        // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("Failed to open changelog, please restart the program.\n");
                break;
            }
            
            char *deleteLinePath = (char*) malloc(sizeof(char)*(strlen(deleteLineDir) + strlen(filename) + 2));
            sprintf(deleteLinePath , "%s/%s" ,deleteLineDir, filename);
            logLines = numOfLines(deleteLinePath);    
            //adds to change log, different additions depending on whether or not the function failed.
            if (result == 1){
            sprintf(logString, "Delete Line: There are now %d lines in the file %s.\n",logLines, filename);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else if (result == 0){
                sprintf(logString, "Delete Line Failed: Unable to open the file %s.\n", filename);
                fputs(logString,logFile);
                fclose(logFile);
            }
            else if (result == 2){
                sprintf(logString, "Delete Line Failed: There line number inputted (%d) exceeded the total number of lines in the file (%d)", lineNum ,logLines);
                fputs(logString,logFile);
                fclose(logFile);
            }
            //free(deleteLineDir);
            //free(deleteLinePath);
            while (getchar() != '\n');
        }
       else if(strcmp(input, "insert line") == 0)
        {
            printf("Please enter the name of the file in which you wish to insert a line.\n");
            fgets(filename, sizeof(filename), stdin);
            printf("If the file in which you wish to insert a line into is located in any subfolders please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char* insertLineDir = locateDirectory();
            filename[strcspn(filename, "\n")] = 0;
            printf("Please enter the text which you wish to insert into the file.\n");
            fgets(insertinp, sizeof(insertinp), stdin);
            insertinp[strcspn(insertinp, "\n")] = 0;
            printf("Please enter the line at which you wish to insert this line.\n");
            scanf("%d", &lineNum);
            int result = insertLine(filename, lineNum, insertinp, insertLineDir);
            
            // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("Failed to open changelog, please restart the program.\n");
                break;
            }
                
            //adds to change log
            char *insertLinePath = (char*) malloc(sizeof(char)*(strlen(insertLineDir) + strlen(filename) + 2));
            sprintf(insertLinePath , "%s/%s" ,insertLineDir, filename);
            logLines = numOfLines(insertLinePath);    
            if (result == 1){
            sprintf(logString, "Insert line: There are now %d lines in the file %s.\n",logLines, filename);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else if (result == 0){
                sprintf(logString, "Insert line failed: Failed to open the file %s.\n", filename);
                fputs(logString,logFile);
                fclose(logFile);
            }
            else if (result == 2){
                sprintf(logString, "Insert line failed: The file %s contained more lines (%d) than the supported number(1024).\n", filename, logLines);
                fputs(logString,logFile);
                fclose(logFile);
            }
            else if (result == 3){
                sprintf(logString, "Insert line failed: The number of lines inputted (%d) was more than 1 greater than the number of lines in the file (%d)", lineNum, logLines);
                fputs(logString,logFile);
                fclose(logFile);
            }
            //free(insertLineDir);
            //free(insertLinePath);
            while (getchar() != '\n');
        }
        else if(strcmp(input, "show line") == 0)
        {
            printf("Please enter the name of the file from which you wish to show a line.\n");
            fgets(filename, sizeof(filename), stdin);
            printf("If the file in which you wish to show a line from is located in any subfolders please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char* showLineDir = locateDirectory();
            filename[strcspn(filename, "\n")] = 0;
            printf("Please enter the number of the line which you wish to show.\n");
            scanf("%d", &lineNum);
            int result = showLine(filename, lineNum, showLineDir);
            
            // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("Failed to open changelog, please restart the program.\n");
                break;
            }
                
            //adds to change log
            char *showLinePath = (char*) malloc(sizeof(char)*(strlen(showLineDir) + strlen(filename) + 2));
            sprintf(showLinePath , "%s/%s" ,showLineDir, filename);
            logLines = numOfLines(showLinePath); 
            if (result == 1){
            sprintf(logString, "Show Line: There are now %d lines in the file %s",logLines, filename);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else if (result == 0){
                sprintf(logString, "Show line failed: Failed to open the file %s.\n", filename);
                fputs(logString,logFile);
                fclose(logFile);
            }
            else if (result == 2){
                sprintf(logString, "Show line failed: The file %s contained more lines (%d) than the supported number(1024).\n", filename, logLines);
                fputs(logString,logFile);
                fclose(logFile);
            }
            //free(showLineDir);
            //free(showLinePath);
            while (getchar() != '\n');
        }     
        else if(strcmp(input, "show change log") == 0)
        {
            changeLog(logDir);
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("Failed to open changelog.\n");
                continue;
            }
                
            //adds to change log
            logLines = numOfLines(logDir);
            // the line after will add another line to change log
            logLines++;
            sprintf(logString, "Change Log: There are now %d lines in the file ChangeLog.log\n",logLines);
            fputs(logString,logFile);
            fclose(logFile);               
        }
        else if(strcmp(input, "show number of lines") == 0)
        {
            int lines = 0;
                
            printf("What file do you want to inspect?\n");
            fgets(filename, sizeof(filename), stdin); 
            filename[strcspn(filename, "\n")] = 0;
            printf("If the file in which you wish to show the number of lines from is located in any subfolders please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char *numOfLinesDir = locateDirectory();
            char *numOfLinesPath = (char*) malloc(sizeof(char)*(strlen(filename) + strlen(numOfLinesDir) + 3));
            sprintf(numOfLinesPath , "%s/%s" ,numOfLinesDir, filename);
            lines = numOfLines(numOfLinesPath);
            printf("The number of lines in the file is %d.\n",lines);
                
            // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("File opening failed\n");
                sprintf(logString, "Number of lines in a file failed: Unable to open file %s.\n", filename);
                fputs(logString,logFile);
                fclose(logFile);
            }
                
            //adds to change log
            logLines = numOfLines(numOfLinesPath);
            sprintf(logString, "Number of lines in a file: There are now %d lines in the file %s",logLines, filename);
            fputs(logString,logFile);
            fclose(logFile);
            //free(numOfLinesPath);
            //free(numOfLinesDir);
        } 
        else if(strcmp(input, "create dir") == 0)
        {
            createLocateDirectory();
        }
        else if(strcmp(input, "duplicate line") == 0)
        {
            printf("Please enter the name of the file from which you wish to duplicate a line.\n");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            printf("If the file in which you wish to duplicate a line from is located in any subfolders please enter the names of the folders accordingly, starting with the outermost folder.\n");
            char* duplicateLineDir = locateDirectory();
            printf("Please enter the line number which you wish to duplicate.\n");
            scanf("%d", &lineNum);
            int result = duplicateLine(filename, lineNum, duplicateLineDir);
            
            // adds adjustments to logFile
            logFile = fopen(logDir, "a");

            if (logFile == NULL){
                printf("File opening failed\n");
            }
                
            //adds to change log
            char *duplicateLinePath = (char*) malloc(sizeof(char)*(strlen(duplicateLineDir) + strlen(filename) + 2));
            sprintf(duplicateLinePath , "%s/%s" ,duplicateLineDir, filename);
            logLines = numOfLines(duplicateLinePath); 
            
            if (result == 1){
            sprintf(logString, "Duplicate line: There are now %d lines in the file %s.\n",logLines, filename);
            fputs(logString,logFile);
            fclose(logFile);
            }
            else if (result == 0){
                sprintf(logString, "Duplicate line failed: Failed to open the file %s.\n", filename);
                fputs(logString,logFile);
                fclose(logFile);
            }
            else if (result == 2){
                sprintf(logString, "Duplicate line failed: The file %s contained more lines (%d) than the supported number(1024).\n", filename, logLines);
                fputs(logString,logFile);
                fclose(logFile);
            }
            else if (result == 3){
                sprintf(logString, "Duplicate line failed: The number of lines inputted (%d) was more than 1 greater than the number of lines in the file (%d)", lineNum, logLines);
                fputs(logString,logFile);
                fclose(logFile);
            }
            while(getchar()!= '\n');
            //free(duplicateLineDir);
            //free(duplicateLinePath);
            
        }   
        else if(strcmp(input, "help") == 0)
        {
            printf("The functions and the inputs to use them are displayed below: \n-FILE OPERATIONS \n -Create File (create file) \n -Copy File (copy file) \n -Delete File (delete file) \n -Show File (show file) \n\n-LINE OPERATIONS \n -Append Line (append line) \n -Delete Line (delete line) \n -Insert Line (insert line) \n -Show Line (show line) \n\n-GENERAL OPERATIONS \n -Show Change Log (show change log) \n -Show Number of Lines (show number of lines) \n\n-EXTRA FEATURES \n -Create Directories (create dir) \n -Duplicate Line (duplicate line) \n -Help reprints this message (help) \n\n-QUIT (quit)\n\n(all inputs are case sensitive and all filename inputs must include the file extension.)\n\n");
        }
        else if (strcmp(input, "quit") == 0)
        {
            printf("Thank you for using the program.\n");
            return 0;
        }
        else
        {
            printf("Invalid Command!! Try again!!\n");
        }
    }
}
