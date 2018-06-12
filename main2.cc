#include <iostream>
#include <string>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;

constexpr auto alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr auto digits = "0123456789";

string input;
int inputIndex;

inline void advancePointer()
{
  inputIndex++;
}

inline void error()
{
  cout << "Character mismatch!" << endl;
  cout << input << endl;
  for(int i = 0; i < inputIndex; i++)
    cout << " ";
  cout << "^" << endl;
  throw std::invalid_argument("Character mismatch!");
}

inline bool find(const string& s, const char& c)
{
  return s.find_first_of(c) != string::npos;
}

inline void match(const char& c)
{
  if(input.at(inputIndex) == c)
    inputIndex++;
  else error();
}

inline void match(const string& s)
{
  for(const auto& c: s)
    match(c);
}

inline char getPointedChar()
{
  return input.at(inputIndex);
}

inline void printDerivation(const string& s)
{
  cout << s << endl;
}

void ifexpr();
void bexpr();
void bexprPrime();
void bterm();
void btermPrime();
void bfactor();
void bfactorN();
void bfactorNO();
void bfactorNOT();
void value();
void spc();
void spcPrime();
void letter();
void digit();
void letterN();
void letterO();
void letterT();
bool isInFirstOfLetter(const char&);
bool isInFirstOfDigit(const char&);
bool isInFirstOfLetterN(const char&);
bool isInFirstOfLetterO(const char&);
bool isInFirstOfLetterT(const char&);

void ifexpr()
{
  printDerivation("ifexpr => if ( bexpr ) instr");
  match("if (");
  bexpr();
  match(") instr");
}

void bexpr()
{
  printDerivation("bexpr => bterm bexprPrime");
  bterm();
  bexprPrime();
}

void bexprPrime()
{
  const auto& c = getPointedChar();
  if( 'O' == c )
    {
      printDerivation("bexprPrime => OR spc bterm bexprPrime");
      match("OR");
      spc();
      bterm();
      bexprPrime();
    }
  else
    {
      printDerivation("bexprPrime => epsilon");
    }
}

void bterm()
{
  printDerivation("bterm => bfactor btermPrime");
  bfactor();
  btermPrime();
}

void btermPrime()
{
  const auto& c = getPointedChar();
  if( 'A' == c)
    {
      printDerivation("btermPrime => AND spc bfactor btermPrime");
      match("AND");
      spc();
      bfactor();
      btermPrime();
    }
  else
    {
      printDerivation("btermPrime => epsilon");
    }
}

void bfactor()
{
  const auto& c = getPointedChar();
  if( '(' == c )
    {
      printDerivation("bfactor => ( bfactor ) spcPrime");
      match(c);
      bfactor();
      match(')');
      spcPrime();
    }
  else if( isInFirstOfLetterN(c) )
    {
      printDerivation("bfactor => letterN value");
      letterN();
      value();
    }
  else if( isInFirstOfDigit(c) )
    {
      printDerivation("bfactor => digit value");
      digit();
      value();
    }
  else if( 'N' == c )
    {
      printDerivation("bfactor => N bfactorN");
      match(c);
      bfactorN();
    }
  else error();
}

void bfactorN()
{
  const auto& c = getPointedChar();
  if( isInFirstOfLetterO(c) )
    {
      printDerivation("bfactorN => letterO value");
      letterO();
      value();
    }
  else if( isInFirstOfDigit(c) )
    {
      printDerivation("bfactorN => digit value");
      digit();
      value();
    }
  else if( 'O' == c )
    {
      printDerivation("bfactorN => O bfactorNO");
      match(c);
      bfactorNO();
    }
  else
    {
      printDerivation("bfactorN => spc");
      spc();
    }
}

void bfactorNO()
{
  const auto& c = getPointedChar();
  if( isInFirstOfLetterT(c) )
    {
      printDerivation("bfactorNO => letterT value");
      letterT();
      value();
    }
  else if( isInFirstOfDigit(c) )
    {
      printDerivation("bfactorNO => digit value");
      digit();
      value();
    }
  else if( 'T' == c )
    {
      printDerivation("bfactorNO => T bfactorNOT");
      match(c);
      bfactorNOT();
    }
  else
    {
      printDerivation("bfactorNO => spc");
      spc();
    }
}

void bfactorNOT()
{
  const auto& c = getPointedChar();
  if( find(alphabet, c) )
    {
      printDerivation("bfactorNOT => letter value");
      letter();
      value();
    }
  else if( find(digits, c) )
    {
      printDerivation("bfactorNOT => digit value");
      digit();
      value();
    }
  else
    {
      printDerivation("bfactorNOT => spc bfactor");
      spc();
      bfactor();
    }
}

void value()
{
  const auto& c = getPointedChar();
  if( isInFirstOfLetter(c) )
    {
      printDerivation("value => letter value");
      letter();
      value();
    }
  else if( isInFirstOfDigit(c) )
    {
      printDerivation("value => digit value");
      digit();
      value();
    }
  else
    {
      printDerivation("value => spcPrime");
      spcPrime();
    }
}

void spc()
{
  printDerivation("spc => <space> spcPrime");
  match(" ");
  spcPrime();
}

void spcPrime()
{
  if( ' ' == getPointedChar() )
    {
      printDerivation("spcPrime => <space> spcPrime");
      match(" ");
      spcPrime();
    }
  else
    {
      printDerivation("spcPrime => epsilon");
    }
}

void letter()
{
  const auto& c = getPointedChar();
  if( find(alphabet, c ) != std::string::npos )
    {
      printDerivation("letter => " + string(1, c));
      match(c);
    }
  else error();    
}

void digit()
{
  const auto& c = getPointedChar();
  if( find(digits, c) != std::string::npos )
    {
      printDerivation("digit => " + string(1, c));
      match(c);
    }
  else error();
}

void letterN()
{
  const auto& c = getPointedChar();
  if( 'N' == c ) error();
  else if( find(alphabet, c) )
    {
      printDerivation("letterN => " + string(1, c));
      match(c);
    }
  else error();
}

void letterO()
{
  const auto& c = getPointedChar();
  if( 'O' == c ) error();
  else if( find(alphabet, c) )
    {
      printDerivation("letterO => " + string(1, c));
      match(c);
    }
  else error();
}

void letterT()
{
  const auto& c = getPointedChar();
  if( 'T' == c ) error();
  else if( find(alphabet, c) )
    {
      printDerivation("letterT => " + string(1, c));
      match(c);
    }
  else error();
}

bool isInFirstOfLetter(const char& c)
{
  return find(alphabet, c);
}

bool isInFirstOfDigit(const char& c)
{
  return find(digits, c);
}

bool isInFirstOfLetterN(const char& c)
{
  if( 'N' == c ) return false;
  else return find(alphabet, c);
}

bool isInFirstOfLetterO(const char& c)
{
  if( 'O' == c ) return false;
  else return find(alphabet, c);
}

bool isInFirstOfLetterT(const char& c)
{
  if( 'T' == c ) return false;
  else return find(alphabet, c);
}

int main(int argc, char* argv[])
{
  inputIndex = 0;
  std::getline(std::cin, input);

  bool acceptedInput = true;
  cout << "Input: " << input << endl;
  try
    {
      ifexpr();
    }
  catch(std::invalid_argument& e)
    {
      acceptedInput = false;
      cout << e.what() << endl;
    }

  if( acceptedInput )
    {
      cout << "Input accepted!" << endl;
    }
  return 0;
}
