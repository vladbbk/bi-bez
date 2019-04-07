#include <cstdio>
#include <openssl/evp.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#define ALPHABET_NUM 63
#define TEXT_LEN 20

using namespace std;


int main(int argc, char *argv[])
{
  char hashFunction[] = "sha256"; // zvolena hashovaci funkce ("sha1", "md5" ...)

  unsigned int sameBytes         =  0;
	unsigned int 											hexLen;
	string 														hexChars, randomString;
	vector < int > 										byteArray;
	char textToHash [ ALPHABET_NUM + 1 ];
	const unsigned char alfaNum [] = 	"0123456789"
																	 	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
																		"abcdefghijklmnopqrstuvwxyz";
	long long int num               = 0;

	// -----------------------------------------------------------------------------------	
	// len values 
	cout << "# of chars: ";
	cin >> hexLen;
	if ( hexLen % 2 == 1 )
	{
		cout << "number of values must be dividable by 2!" << endl;
		return 1;
	}
	// -----------------------------------------------------------------------------------	
	
	// into string
	cout << "chars: ";
	cin >> hexChars;
	cout << hexChars << endl;
	if ( hexChars . length () != hexLen ) 
	{
		cout << "wrong length of hex!" << endl;
		return 1;
	}
  hexLen = hexLen / 2;
	// -----------------------------------------------------------------------------------	

	// parse it into two-values and push back the hex value 
	for ( size_t i = 0; i < hexChars . length(); i += 2 )
	{
					string a = "";
					a += hexChars[i];
					a += hexChars[i+1];
					byteArray . push_back ( stoi ( a, nullptr, 16 ) );
	}

	// -----------------------------------------------------------------------------------	
  /*
	// printing my vector -- checking if correct
	for ( size_t i = 0; i < byteArray . size (); i++ )
					cout << dec <<  byteArray[i] << hex << byteArray[i] <<  " ";
	cout << endl;
  */
	// -----------------------------------------------------------------------------------	

  EVP_MD_CTX *ctx;  // struktura kontextu
  const EVP_MD *type; 	// typ pouzite hashovaci funkce
  unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
  int length;  // vysledna delka hashe

  /* Inicializace OpenSSL hash funkci */
  OpenSSL_add_all_digests();

  /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
  type = EVP_get_digestbyname(hashFunction);

  /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
  if ( ! type ) 
	{
    printf ( "Hash %s neexistuje.\n", hashFunction );
    exit ( 1 );
  }

  // HASH while true
  while ( true )
  {
    sameBytes = 0;
    num++;

    // generate the random string
    for ( unsigned int i = 0; i < TEXT_LEN; i++ )
      textToHash[i] = alfaNum [ rand () % ( sizeof ( alfaNum ) - 1 )];
		textToHash [ TEXT_LEN ] = '\0';
    ctx = EVP_MD_CTX_create ();

    if ( ctx == NULL )
      exit ( 9 );

    if ( EVP_DigestInit_ex(ctx, type, NULL) != 1 )
      exit ( 6 );

    if ( EVP_DigestUpdate(ctx, textToHash, strlen(textToHash)) != 1 )
      exit ( 7 );

    if ( EVP_DigestFinal_ex(ctx, hash, (unsigned int *) &length) != 1 )
      exit ( 8 );

    EVP_MD_CTX_destroy(ctx);

    // awful comparrison - please don't burn me
    for ( unsigned int i = 0; i < hexLen ; i++ )
    {
      //printf("[%02x][%02x] ---- ", hash[i], byteArray[i] );
      //cout << (bitset<8>)hash[i] << "\t" << (bitset<8>) byteArray[i] << endl;
      if ( hash[i] == (unsigned char)byteArray[i] )
        sameBytes++;
    }


    if ( sameBytes == hexLen )
    {
      printf("FOUND TEXT: %s\nHASH: ", textToHash );
      for ( int i = 0; i < length; i++ )
      {
        if ( (unsigned int ) i < hexLen )
          printf("[%02x]", hash[i] );
        else
          printf("%02x", hash[i] );
      }
      printf("\n");

      break;
    }

  }

  cout << "# of comparrisons: " << dec <<  num << endl;
  exit ( 0 );
}

