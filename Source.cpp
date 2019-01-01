#include "PARSER+TRANSLATOR.h"

int main(int __argc, char** __argv){

	File=new char[100];
	
	if(__argc==1){
		cout<<"Enter Input File Name: ";
		cin.getline(File,99);
		//strcpy(File,"sc.txt");
	}
	else
		strcpy(File,__argv[1]);
		
/*#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#*/

	run_LEX(File);
	cout<<"\n\t* LEX Output file Generated\n";

/*#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#*/
	
	run_PARSER();
	cout<<"\n\t* Parse Tree file Generated\n";

/*#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#*/

	run_TRANSLATOR();
	cout<<"\n\t* Translator Output file Generated\n";

/*#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#*/

	populate_SymbolTable();
	cout<<"\n\t* Symbol Table file Generated\n";

/*#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#* *#*/


	cout<<"\n\n\n";
	system("pause");

}