#include "LEX.h"

using namespace std;

void BD();
void E();
void AV();
string FC(string);

string current;
LexemeData lexeme;

int curr1=0,spaces=0,tmpCount=0;
bool funcFlag;

ofstream fout_P("Parse Tree.txt");

int n=0,rel=0;
string temp;

vector<string> ALL_EMITS;

string newTmp(){
	string ret;
	ret.clear();
	ret.append("t");

	char tn[10];
	itoa(tmpCount,tn,10);
	ret.append(tn);
	tmpCount++;

	symbolTableData sd;
	sd.ID=ret;
	sd.Type="TMP";
	sd.relAdd=rel;
	SYMBOL_TABLE.push_back(sd);
	st++;

	rel+=4;

	return ret;
}

void EMIT(string str="",int p=-1){

	char cn[10];
	itoa(n,cn,10);

	ALL_EMITS.push_back("");
	ALL_EMITS.at(n).append(cn);
	ALL_EMITS.at(n).append(") ");
	if(str=="")		
		ALL_EMITS.at(n).append(temp);

	else{
		if(p!=-1)
			str.append(itoa(p,cn,10));
		
		ALL_EMITS.at(n).append(str);
	}

	n++;
}

void BACKPATCH(int d){

	char cn[10];
	itoa(n,cn,10);

	ALL_EMITS.at(d).append(cn);
	
}

//void POPULATE(){
//
//	
//	string input;
//	ifstream fin("Lex Output.txt");
//	while(!fin.eof()){
//		
//		getline(fin,input);
//		//fin>>input;
//		if(input.find(",",0)!=string::npos){
//
//			TOKENS.push_back(input.substr(1,input.find(",",2)-1));
//			int i=input.length()-2;
//			LEXEMES.push_back(input.substr(input.find(",",2)+1,input.length()-input.find(",",2)-2));
//		}
//	}
//}

void printSpaces(){

	fout_P<<"\n";
	
	for(int i=0;i<spaces-1;i++)
		fout_P<<"|   ";

	if(spaces!=0)
		fout_P<<"|---";

	spaces++;
}

void ERROR(){

	fout_P<<"Invalid String\n";
}

string nextToken(){

	if(++curr1<TOKENS.size()){
		lexeme=LEXEMES.at(curr1);
		return TOKENS.at(curr1);
	}
	else
		return "FINISH";
}

void match(string token){


	if(token==current){
		printSpaces();
		spaces--;
		fout_P<<token;
		if(lexeme.lexeme!="NULL")
			fout_P<<" ( "<<lexeme.lexeme<<" )";

		//      FOR TRANSLATOR    //
		if(lexeme.lexeme!="NULL"){
			if(lexeme.lexeme=="<-")
				temp.append("=");
			else if(lexeme.lexeme=="=<")
				temp.append("<=");
			else if(lexeme.lexeme=="=>")
				temp.append(">=");
			else
				temp.append(lexeme.lexeme);

		}
		else{
			if(token=="print")
				temp.append("out");
			else if(token=="while")
				temp.append("if");
			else
				temp.append(token);
		}
		temp.append(" ");
		/////////////////////////////////////////

		current=nextToken();
	}
	else
		ERROR();
	
}

void B(){
	printSpaces();
	fout_P<<"B";
	
	if(current=="["){
		match("[");
		match("]");
	}
	else
		;									//NULL


	spaces--;

	
}

void T1(string &type,int &size){
	printSpaces();
	fout_P<<"T1";
	

	if(current=="int"){
		match("int");
		type="INT";
		size=4;
	}
	
	else if(current=="char"){
		match("char");
		type="CHR";
		size=1;
	}
	else 
		ERROR();


	spaces--;
											
}

void T2(){
	printSpaces();
	fout_P<<"T2";
	
	string ts;int ti;
	if(current=="int" ||current=="char")
		T1(ts,ti);
	else if(current=="void")
		match("void");
	else 
		ERROR();


	spaces--;
											
}

void A2(){
	printSpaces();
	fout_P<<"A2";
	
	string ts;int ti;

	if(current==","){
		match(","); 
		T1(ts,ti); 
		B();  
		match("ID");
		A2(); 
	}
	else 
		;						//NULL


	spaces--;
	
}

void A1(){
	printSpaces();
	fout_P<<"A1";
	
	string ts;int ti;		

	if(current!=")"){
		T1(ts,ti); 
		B();  
		match("ID"); 
		A2(); 
	}
	else
		;						//NULL


	spaces--;
}

void FD(){
	printSpaces();
	fout_P<<"FD";
	
	match("def"); 
//A:
	SYMBOL_TABLE.at(lexeme.index).Type="FUN";
	SYMBOL_TABLE.at(lexeme.index).relAdd=rel; 
	rel+=30;

	match("ID"); 
	match("("); 
	A1(); 
	match(")");
	match("{"); 
	BD();
	match("}");

	spaces--;
}

void NI(){
	printSpaces();
	fout_P<<"NI";
	

	if(current=="NUM")
		match("NUM");
	else if(current=="ID")
		match("ID");
	else
		ERROR();	


	spaces--;							
}

void D(){
	printSpaces();
	fout_P<<"D";
	

	if(current=="["){
		match("["); 
		NI();  
		match("]");
	}
	else
		;								//NULL


	spaces--;
}

void VAL(){
	printSpaces();
	fout_P<<"VAL";
	

	if(current=="ID"){
		match("ID");
		D(); 
	}
	else if(current=="NUM")
		match("NUM");
	else if(current=="CHAR")
		match("CHAR");
	else
		ERROR();


	spaces--;
}

void AV(){
	printSpaces();
	fout_P<<"AV";
	

	 if(current=="["){
		 match("[");
		 match("NUM");
		 match("]");  
	 }
	 else if(current=="EQ"){
		match("EQ");
		VAL();
	 }
	 else
		 ;						//NULL


	spaces--;
 }

void M(string type,int size){
	printSpaces();
	fout_P<<"M";
	

	if(current==","){
		match(",");
//A:
	SYMBOL_TABLE.at(lexeme.index).Type=type;
	SYMBOL_TABLE.at(lexeme.index).relAdd=rel; 
	rel+=size;

		match("ID"); 
		AV();  
		M(type,size);
	}
	else
		;						//NULL


	spaces--;
 }

void VD(){
	printSpaces();
	fout_P<<"VD";
	
	string type;int size;
	T1(type,size); 
//A:
	SYMBOL_TABLE.at(lexeme.index).Type=type;
	SYMBOL_TABLE.at(lexeme.index).relAdd=rel; 
	rel+=size;

	match("ID"); 
	AV();  
	M(type,size);
	match(";");


	spaces--;
}

void OP(){
	
	printSpaces();
	fout_P<<"OP";
	

	if(current=="+")
		match("+");
	else if(current=="-")
		match("-");
	else if(current=="*")
		match("*");
	else if(current=="/")
		match("/");
	else
		ERROR();


	spaces--;
}

string Y(){
	printSpaces();
	fout_P<<"Y";
	
	string n="-1";
	if(current=="NUM"){
//A12:
		n=lexeme.lexeme;
		match("NUM");


	}
	else if(current=="ID"){
//A13:
		n=lexeme.lexeme;
		string funcLex=lexeme.lexeme;
		match("ID");
		D(); 
		if(current=="("){
			funcFlag=false;
			n=FC(funcLex);
			
		}
	}
	else
		ERROR();
		

	spaces--;	

	return n;
}

string Z(string i){
	printSpaces();
	fout_P<<"Z";

	string s;
	if(current=="*"){
		match("*");
		string tn=Y();
//A11:
		string i1=newTmp();
		temp.clear();
		temp.append(i1);
		temp.append("=");
		temp.append(i);
		temp.append("*");
		temp.append(tn);
		EMIT(temp);
		////////////////////////

		string s1=Z(i1);
//A12:
		s=s1;
		///////////////////////

	}
	else if(current=="/"){
		match("/");
		string tn=Y();
//A13:
		string i1=newTmp();
		temp.clear();
		temp.append(i1);
		temp.append("=");
		temp.append(i);
		temp.append("/");
		temp.append(tn);
		EMIT(temp);
		////////////////////////

		string s1=Z(i1);
//A14:
		s=s1;
		///////////////////////

	}
	else{
		;
//A15:
		s=i;
		///////////////////////
	}
	spaces--;

	return s;
}

string T(){
	printSpaces();
	fout_P<<"T";

//A9:
	string i=Y();

//A10:
	string n=Z(i);


	spaces--;

	return n;
}

string Q(string i){
	printSpaces();
	fout_P<<"Q";

	string s;
	if(current=="+"){
		match("+");
		string tn=T();
//A4:
		string i1=newTmp();
		temp.clear();
		temp.append(i1);
		temp.append("=");
		temp.append(i);
		temp.append("+");
		temp.append(tn);
		EMIT(temp);
		////////////////////////

		string s1=Q(i1);
//A5:
		s=s1;
		///////////////////////

	}
	else if(current=="-"){
		match("-");
		string tn=T();
//A6:
		string i1=newTmp();
		temp.clear();
		temp.append(i1);
		temp.append("=");
		temp.append(i);
		temp.append("-");
		temp.append(tn);
		EMIT(temp);
		////////////////////////

		string s1=Q(i1);
//A7:
		s=s1;
		///////////////////////

	}
	else{
		;
//A8:
		s=i;
		///////////////////////
	}
	spaces--;

	return s;
}

string G(){
	printSpaces();
	fout_P<<"G";

//A2:
	string i=T();

//A3:
	string n=Q(i);
	
	spaces--;

	return n;
}

void VAO (){
	printSpaces();
	fout_P<<"VAO";
	
	string currtemp=temp;
	D(); 
	match("EQ");
	
	string n=G();

	currtemp.append("=");
	currtemp.append(n);
	EMIT(currtemp);

	match(";");


	spaces--;
}

void W(){
	printSpaces();
	fout_P<<"W";
	
	int t,f;
	temp.clear();

	match("while");
	VAL(); 
	match("RO");  
	VAL(); 

	temp.append("goto ");

		//*****************************************//
//A1:
	EMIT();							
	t=n-1;

	EMIT("goto ");

	f=n-1;
	//*****************************************//

	match("{");

	BACKPATCH(t);
	
	BD();

	EMIT("goto ",t);
	BACKPATCH(f);

	match("}");


	spaces--;
}

void F(){
	printSpaces();
	fout_P<<"F";
	
	int t,f,x;
	temp.clear();

	match("if"); 
	VAL(); 
	match("RO"); 
	VAL();  

	temp.append(" goto ");

//A1:
	EMIT();							
	t=n-1;

	EMIT("goto ");

	f=n-1;

	match("{");
	BACKPATCH(t);
	
	temp.clear();
	BD();
	EMIT("goto ");
	x=n-1;


	match("}");
	BACKPATCH(f);
	E();
	BACKPATCH(x);


	spaces--;
}

void E(){
	printSpaces();
	fout_P<<"E";

	if(current=="else"){
		match("else"); 
		if(current=="{"){
			match("{");
			BD(); 
			match("}");
		}
		else if(current=="if"){
			F();
		}
	}
	else 
		;									//NULL


	spaces--;
}

void R(){
	printSpaces();
	fout_P<<"R";
	

	match("return");
	VAL();
	match(";");


	spaces--;
}

void P(){
	printSpaces();
	fout_P<<"P";
	
	temp.clear();

	match("print");
	match("ID");

	EMIT();

	match(";");


	spaces--;
}

void I(){
	printSpaces();
	fout_P<<"I";
	
	temp.clear();


	match("input");
	match("ID"); 

	EMIT();


	match(";");


	spaces--;
}

void P2(int &c){
	printSpaces();
	fout_P<<"P2";
	
	if(current==","){
		match(",");

		string n=G();
//A:
		c++;
		temp.clear();
		temp.append("arg ");
		temp.append(n);
		EMIT();

		P2(c);
	}
	else 
		;


	spaces--;
}

void P1(int &c){
	printSpaces();
	fout_P<<"P1";
	
	if(current!=")"){
		string n=G();
//A:
		c++;
		temp.clear();
		temp.append("arg ");
		temp.append(n);
		EMIT();

		P2(c);
	}
	else 
		;


	spaces--;
}

string FC (string idLex){
	printSpaces();
	fout_P<<"FC";
	
	int c=0;
	
	match("(");
	P1(c);
	match(")");
	
	if(funcFlag)
		match(";");
	
	string n=newTmp();
	temp.clear();
	temp.append(n);
	temp.append(" = call ");
	temp.append(idLex);
	temp.append(", ");

	char cc[5];
	itoa(c,cc,10);
	temp.append(cc);
	
	EMIT();
	
	spaces--;

	return n;
}

void BD(){
	printSpaces();
	fout_P<<"BD";
	

	if(current=="while"){
		W(); 
		BD();
	}
	else if(current=="if"){
		F(); 
		BD();
	}
	else if(current=="print"){
		P(); 
		BD();
	}
	else if(current=="input"){
		I(); 
		BD();
	}
	else if(current=="return"){
		R();
	}
	else if(current=="char" || current=="int"){
		VD();
		BD();
	}
	else if(current=="ID"){
		temp.clear();
		string funcLex=lexeme.lexeme;
		match("ID");

		if(current=="("){
			funcFlag=true;
			FC(funcLex);
			BD();
		}
		else {
			VAO();
			BD();
		}

	}

	else
		;							//NULL


	spaces--;
	
}

void S(){
	
	printSpaces();
	fout_P<<"S";
	
	if(current=="def"){
		FD();

		if(current!="FINISH")
			S();
	}
	else if(current=="int"||current=="char"){
		VD();
		if(current!="FINISH")
			S();
	}
	else if(current=="FINISH")
		;
	else
		ERROR();

	spaces--;
}

void run_PARSER(){
	//POPULATE();
	current=TOKENS.at(curr1); lexeme=LEXEMES.at(curr1);
	S();

	fout_P.close();
}

void run_TRANSLATOR(){
	
	ofstream fout_T("Translator Output.txt");

	for(int i=0;i<n;i++){
		//cout<<ALL_EMITS.at(i)<<endl;
		fout_T<<ALL_EMITS.at(i)<<endl;
	}

	fout_T.close();

	
}

void populate_SymbolTable(){

	ofstream fout_ST("Symbol Table.txt");
	fout_ST<<"ID,Type,Relative Address\n\n";

	int L,maxL=-1;
	for(int i=0;i<st;i++){
		L=SYMBOL_TABLE.at(i).ID.length();
		if(L>maxL)
			maxL=SYMBOL_TABLE.at(i).ID.length();
	}
	
	for(int i=0;i<st;i++){
		fout_ST<<" "<<SYMBOL_TABLE.at(i).ID;
		
		L=SYMBOL_TABLE.at(i).ID.length();
		for(int j=0;j<maxL-L;j++)
			fout_ST<<" ";
		
		fout_ST<<" | "<<SYMBOL_TABLE.at(i).Type<<" | "<<SYMBOL_TABLE.at(i).relAdd<<endl;

	}
	
	fout_ST.close();

}