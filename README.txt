The basic construction of the language is complete:

to use variables, you must declare them first (constants, then vars) and then you may “begin” into a compound statement. The compound statement can’t properly use variables that we’re not declared. 
functions like add, subtract, multiply, divide, and modulo all work properly. then on the final statement in the compound statement, a semicolon is not used and instead “end.” is used.

To view the process of the lexical analyzer and the symbol table, prompts are given to the user. The lexical analyzer outputs all read lexemes, their value, and their classification. The Symbol table is where a user can view if arithmetic has been performed properly. You can see the address location and value of that particular symbol. For example, if you assign A := 3 + 5, then on the symbol table, there will be a line that reads “Variable - A: 8, @ address: (some address)”. 

Included in the repository is a test input “program.txt” to show how the input is formatted to properly be read and computed, and to give examples about how the numbs arithmetic operations are functional. 
