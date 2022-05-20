#include <iostream>
#include <cstring>
using namespace std;
//introduceti aici formula pentru care se vor face calculele
#define formula (char*) "(A -> (B -> C)) & (A -> B) & (A ~ C)"
//care sunt conectorii logici acceptati
#define logical_connectors (char*) ">&v!~"
//care sunt variabilele acceptate
#define variables (char*) "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

int imp(int first_value, int second_value){
	if(first_value == 1 && second_value == 0)
		return 0;
	return 1;
}
int cnj(int first_value, int second_value){
	if(first_value == 1 && second_value == 1)
		return 1;
	return 0;
}
int dsj(int first_value, int second_value){
	if(first_value == 1 || second_value == 1)
		return 1;
	return 0;
}
int neg(int value){
	if(value == 0)
		return 1;
	return 0;
}
int eqv(int first_value, int second_value){
	if(first_value == second_value)
		return 1;
	return 0;
}

int lclen(){
	return strlen(logical_connectors);
}
int vlen(){
	return strlen(variables);
}
int flen(){
	return strlen(formula);
}
char* lccpy(){
	char* logc_chars = new char[lclen() + 1];
	strcpy(logc_chars, logical_connectors);
	return logc_chars;
}
char* vcpy(){
	char* var_chars = new char[vlen() + 1];
	strcpy(var_chars, variables);
	return var_chars;
}
char* fcpy(){
	char* f_chars = new char[flen() + 1];
	strcpy(f_chars, formula);
	return f_chars;
}
char* sfcpy(){
	char* subf_chars = new char[lclen() + vlen() + 1];
	strcpy(subf_chars, vcpy());
	strcat(subf_chars, lccpy());
	return subf_chars;
}
int strchrcmp(char string[]){
	int strchrcmp_number = 0;
	char* first_char = strpbrk(fcpy(), string);
	while(first_char != NULL){
		first_char = strpbrk(first_char + 1, string);
		++strchrcmp_number;
	}
	return strchrcmp_number;
}
int chrcmp(char character){
	int chrcmp_number = 0;
	char* first_char = strchr(fcpy(), character);
	while(first_char != NULL){
		first_char = strchr(first_char + 1, character);
		++chrcmp_number;
	}
	return chrcmp_number;
}

int numberOfParenthesis(){
	return strchrcmp((char*) "()");
}
int numberOfSubformulas(){
	return strchrcmp(sfcpy());
}
int numberOfEncodedCharacters(){
	return numberOfSubformulas() + numberOfParenthesis();
}
int numberOfDistinctVariables(){
	int dvar_number = 0;
	for(int i = 0; i < vlen(); ++i)
		if(strchr(fcpy(), vcpy()[i]) != NULL)
			++dvar_number;
	return dvar_number;
}
char* distinctVariables(){
	char* dvars = new char[numberOfDistinctVariables()];
	int j = 0;
	for(int i = 0; i < vlen(); ++i)
		if(strchr(fcpy(), vcpy()[i]) != NULL)
			dvars[j++] = vcpy()[i];
	return dvars;
}
void displayDistinctVariables(){
	for(int i = 0; i < numberOfDistinctVariables(); ++i)
		cout << " " << distinctVariables()[i];
}
void displayNumberOfVariableOccurrences(){
	for(int i = 0; i < vlen(); ++i)
		if(strchr(fcpy(), vcpy()[i]) != NULL){
			if(vcpy()[i] != vcpy()[0])
				cout << ",";
			cout << " " << vcpy()[i] << "-" << chrcmp(vcpy()[i]);
		}
}
int formulaOrder(){
	return strchrcmp(lccpy());
}

int numberOfLines(){
	int number_of_lines = 1;
	for(int i = 0; i < numberOfDistinctVariables(); ++i)
		number_of_lines *= 2;
	return number_of_lines;
}
int* bin(int counter, int bin_length){
    int* binary_number = new int[bin_length];
	for(int i = 0; i < bin_length; ++i)
		binary_number[i] = 0;
	
    int i = 0;
    while(counter){
    	binary_number[i++] = counter % 2;
        counter /= 2;
    }
	return binary_number;
}
int** defineTruthTable(){
	int** truth_table = new int*[numberOfLines()];
	for(int i = 0; i < numberOfLines(); ++i)
    	truth_table[i] = new int[numberOfDistinctVariables() + 1];

	int* table_values;
	int counter = 0;
	for(int i = 0; i < numberOfLines(); ++i){
		table_values = bin(counter++, numberOfLines() / 2);
		for(int j = 0; j < numberOfDistinctVariables(); ++j){
				truth_table[i][j] = table_values[j];
		}
	}
	return truth_table;
}

int doOperation(char lc, int first_value, int second_value){
	switch(lc){
		case 14:
			return imp(first_value, second_value);
		case 12:
			return cnj(first_value, second_value);
		case 13:
			return dsj(first_value, second_value);
		case 11:
			return neg(second_value);
		case 15:
			return eqv(first_value, second_value);
	}
	return -1;
}
int* encodeFormula(int** truth_table, int i){
	int* formula_array = new int[numberOfEncodedCharacters()];
	for(int j = 0; j < numberOfEncodedCharacters(); ++j)
		formula_array[j] = -1;

	int k = 0;
	for(int j = 0; j < numberOfEncodedCharacters(); ++j){
		while(formula_array[j] == -1){
			for(int l = 0; l < numberOfDistinctVariables(); ++l)
				if(fcpy()[k] == distinctVariables()[l])
					formula_array[j] = truth_table[i][l];
			switch(fcpy()[k]){
				case '(':
					formula_array[j] = 6;
					break;
				case ')':
					formula_array[j] = 9;
					break;
				case '>':
					formula_array[j] = 14;
					break;
				case '&':
					formula_array[j] = 12;
					break;
				case 'v':
					formula_array[j] = 13;
					break;
				case '!':
					formula_array[j] = 11;
					break;
				case '~':
					formula_array[j] = 15;
			}
			++k;
		}
	}

	return formula_array;
}
int* chooseOperationOrder(int* encoded_formula){
	int lc, first_value, second_value;
	for(int i = 0; i < numberOfEncodedCharacters(); ++i)
		if(encoded_formula[i] == 9){
			for(int j = i - 1; j >= 0; --j){
				if(encoded_formula[j] == 6){
					for(int m = 11; m <= 15; ++m){
						for(int k = j + 1; k < i; ++k){
							int l = k - 1, n = k + 1;
							while(encoded_formula[l] == -1)
								--l;
							while(encoded_formula[n] == -1)
								++n;
							if(encoded_formula[k] == m){
								lc = encoded_formula[k];
								first_value = encoded_formula[l];
								second_value = encoded_formula[n];

								encoded_formula[n] = doOperation(lc, first_value, second_value);
								if(encoded_formula[l] < 11 || encoded_formula[l] > 15)
									encoded_formula[l] = -1;
								encoded_formula[k] = -1;
							}
						}
					}
					encoded_formula[j] = -1;
					int p = i;
					while(encoded_formula[p] != 9)
						++p;
					encoded_formula[p] = -1;;
				}
			}
		}
	for(int j = 11; j <= 15; ++j){
		for(int i = 0; i < numberOfEncodedCharacters(); ++i){
			int k = i - 1, l = i + 1;
			while(encoded_formula[k] == -1)
				--k;
			while(encoded_formula[l] == -1)
				++l;
			if(encoded_formula[i] == j){
				lc = encoded_formula[i];
				first_value = encoded_formula[k];
				second_value = encoded_formula[l];

				encoded_formula[l] = doOperation(lc, first_value, second_value);
				if(encoded_formula[k] < 11 || encoded_formula[k] > 15)
					encoded_formula[k] = -1;
				encoded_formula[i] = -1;
			}
		}
	}
	return encoded_formula;
}
int** tableEvaluation(){
	int** truth_table = defineTruthTable();
	int* encoded_formula;
	for(int i = 0; i < numberOfLines(); ++i){
		encoded_formula = encodeFormula(truth_table, i);
		int j = numberOfEncodedCharacters() - 1;
		while(chooseOperationOrder(encoded_formula)[j] == -1)
			--j;
		truth_table[i][numberOfDistinctVariables()] = chooseOperationOrder(encoded_formula)[j];
	}
	return truth_table;
}

void displayTruthTable(){
	int** truth_table = tableEvaluation();
	for(int i = 0; i < numberOfLines(); ++i){
		for(int j = 0; j < numberOfDistinctVariables(); ++j)
			cout << " " << truth_table[i][j];
		cout << "  " << truth_table[i][numberOfDistinctVariables()];
		cout << endl;

	}
}
void displayLine(){
	for(int i = 0; i < numberOfDistinctVariables() + 1; ++i)
		cout << "--";
}

int main(){
	cout << " " << '"' << fcpy() << '"' << endl;
	cout << endl;
	cout << " Date initiale:" << endl;
	cout << " ---" << endl;
	cout << " Formula = " << fcpy() << endl;
	cout << endl;
	cout << " unde:" << endl;
	cout << "   -> : implicatie" << endl;
	cout << "   &  : conjunctie" << endl;
	cout << "   v  : disjunctie" << endl;
	cout << "   !  : negatie" << endl;
	cout << "   ~  : echivalenta" << endl;
	cout << " ---";
	cout << endl;
	cout << endl;
	cout << " Solutii:" << endl;
	cout << " ---" << endl;
	cout << " a. # subformule = " << numberOfSubformulas() << endl;
	cout << " b. # variabile distincte = " << numberOfDistinctVariables() << endl;
	cout << " c. # aparitii variabile:"; displayNumberOfVariableOccurrences(); cout << endl;
	cout << " d. ordinul formulei (# conectori logici) = " << formulaOrder() << endl;
	cout << " e. Tabelul de adevar pentru formula: " << fcpy() << endl;
	cout << endl;
	displayDistinctVariables(); cout << "  F" << endl;
	cout << " "; displayLine(); cout << endl;
	displayTruthTable();
	cout << " "; displayLine(); cout << endl;

	system("pause");
}