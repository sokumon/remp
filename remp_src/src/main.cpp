#include <iostream>
#include <getopt.h>
#include "request.hpp"
#include "helper.hpp"

int main(int argc, char *argv[]){
   int option;
   std::string repo_name;
   std::string file_name;
   if(argc == 1){
      std::cout << "Run 'remp -h'" << std::endl;
   }
   while((option = getopt(argc, argv, "hac:r:")) != -1){ //get option from the getopt() method
      switch(option){
         //For option i, r, l, print that these are options
         case 'h':
            std::cout << "Authencticate 'remp -a'" << std::endl;
            std::cout << "Create a new repo with 'remp -c {reponame}'" << std::endl;
            std::cout << "Create a new repo and supply your own readme file with 'remp -c {reponame} -r {README.md}'" << std::endl;
            break;
         case 'a':
            std::cout << "Authenticate mode\n";
            auth();
            break;
         case 'c': //here f is used for some file name
            printf("New Repo Name : %s\n", optarg);
            repo_name = optarg;
            if(argc == 3){
               create_new_repo(repo_name);
               default_readme_file(whoami(),repo_name);
            }else{
               create_new_repo(repo_name);
            }
            break;
         case 'r': //here f is used for some file name
            printf("File name : %s\n", optarg);
            if(argc == 3){
               std::cout << "Invalid usage" << std::endl;
               std::cout << "Run 'remp -c {reponame} -r {README.md}'" << std::endl;
            }else{
               file_name = optarg;
               if (fileExists(file_name)) {
                  user_readme_file(whoami(),repo_name,file_name);
               } else {
                  std::cout << "File  does not exist in the current working directory." << std::endl;
               }
               // std::cout << "Hello " << whoami() << std::endl;
            }
            break;
         case ':':
            printf("option needs a value\n");
            break;
         // case '?': //used for some unknown options
         //    printf("unknown option: %c\n", optopt);
         //    break;
      }
   }


   return 0;
}