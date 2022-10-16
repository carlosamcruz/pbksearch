#include <iostream>

using namespace std;

///////////////////////////////
//CRIPTO HEADERS
///////////////////////////////
#include "lib/secp256k1Class3.h" 	// depende de <boost/multiprecision/cpp_int.hpp>
#include "lib/hexchar.h"

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <vector>


#include <graphics.h>
#include <iostream>

///////////////////////////////
///////////////////////////////

////////////////////////////////////////////////////////////////////
//CONVERTE AS CHAVE PRIVADA DE HEX PARA BIGINT
////////////////////////////////////////////////////////////////////

cpp_int chBN(unsigned char chA[])
{
	//unsigned char chA[64], cSHA[33], cSHA2[65];
	cpp_int ichA, tA;

	//converte a chave A em inteiro
	tA = 1;
	ichA = 0;
	for (int i = 63; i>= 0; i--)
	{
		ichA = ichA + tA*hexint (chA[i]);
		tA = tA*16;
	}
	return (ichA);
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

//const int nOfBPBKFiles = 22;
const int nOfBPBKFiles = 20;
cpp_int pointsPBKderiva[33000][nOfBPBKFiles];
//const int nKeys = 12;
const int nKeys = 144;

////////////////////////////////////////////////////////////////////
//CLASSE PARA BUSCA POR COLISOES
////////////////////////////////////////////////////////////////////

class pbkColl
{
	public:

	int numberOfKeys;
	cpp_int ntries;
	cpp_int pvtKeyTest[nKeys], keyOffSet;

	//Nao se desfazer desta variavel de teste
	//cpp_int testFactor;

	///////////////////////////////////////////////////////////////////////////////////////////
	//Metodo para Busca por colisoes
	///////////////////////////////////////////////////////////////////////////////////////////

	int pbkSearch()
	{
		Secp256k1 EC;

		cpp_int *g, gn[2][nKeys], gn_anth[2][nKeys];

		///////////////////////////////////////////////////////////////////////////////////////////
		//Inicializacao do DB de pontos Px derivados
		///////////////////////////////////////////////////////////////////////////////////////////

		for(int j = 0; j < nOfBPBKFiles; j++)
		{
			for(int i = 0; i < 33000; i++)
			{
			    //pode acontecer de de 0 ser uma solucao de colisao
				pointsPBKderiva[i][j] = -1;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Determina a quantidade de chaves que serao derivadas a partir da chave publica original
		// A curva tem simetrica em relação ao ponto central de inicio e de meio
		//
		// Px(PVTKey) onde a chave priva PVTKey eh qualquer numero entre 1 e (n_order - 1)
		// Px(1) = Px(n_order - 1)
		// Px(n_order/2) = Px((n_order/2) + 1)
		//
		// Px(0) = Px(n_order) = (0, 0)
		//
		// se slices == 4 teremos 3 chaves: 1; n_order/4; 2(n_order/4)
		//	1 => 1 e n_order/2
		//	2 => 1; n_order/4; 2(n_order/4)
		//	4 => 1; n_order/8; 2(n_order/8); 3(n_order/8); 4(n_order/8)
		//	8 => 1; n_order/16; 2(n_order/16); 3(n_order/16); 4(n_order/16); 5(n_order/16); 6(n_order/16); 7(n_order/16); 8(n_order/16)
		///////////////////////////////////////////////////////////////////////////////////////////

		//int  slices = 8; //4
		//int  slices = 16; //8
		//int slices = 512; //256
	//	int  slices = 1024; //512
		//int  slices = 4096; // 2048
		//int  slices = 2*2*4096; // 8k
		//int  slices = 2*2*2*4096; // 16k
		int  slices = 2*2*2*2*4096; // 32k

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Sequencia de arquivos com as derivacoes das chaves publicas a serem testadas
		///////////////////////////////////////////////////////////////////////////////////////////
		//const int nOfBPBKFiles = 22; //String precisa ir para um ponto anterior no codigo
		//const int nOfBPBKFiles = 21;
		std::string fileNames[nOfBPBKFiles];
		fileNames[0] = "PBK01_32kKEYS.txt";
		fileNames[1] = "PBK02_32kKEYS.txt";
		fileNames[2] = "PBK03_32kKEYS.txt";
		fileNames[3] = "PBK04_32kKEYS.txt";
		fileNames[4] = "PBK05_32kKEYS.txt";
		fileNames[5] = "PBK06_32kKEYS.txt";
		fileNames[6] = "PBK07_32kKEYS.txt";
		fileNames[7] = "PBK08_32kKEYS.txt";
		fileNames[8] = "PBK09_32kKEYS.txt";
		fileNames[9] = "PBK10_32kKEYS.txt";
		fileNames[10] = "PBK11_32kKEYS.txt";
		fileNames[11] = "PBK12_32kKEYS.txt";
		fileNames[12] = "PBK13_32kKEYS.txt";
		fileNames[13] = "PBK14_32kKEYS.txt";
		fileNames[14] = "PBK15_32kKEYS.txt";
		fileNames[15] = "PBK16_32kKEYS.txt";
		fileNames[16] = "PBK17_32kKEYS.txt";
		fileNames[17] = "PBK18_32kKEYS.txt";
		fileNames[18] = "PBK19_32kKEYS.txt";
		fileNames[19] = "PBK20_32kKEYS.txt";
		//fileNames[20] = "PBK21_TKEY12.txt";
		//fileNames[21] = "PBK22_TKEY07.txt";

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		//std::cout << "Key offset: "<< keyOffSet << std::endl;
		//std::cout << "Number of Keys: " << numberOfKeys << std::endl;

		///////////////////////////////////////////////////////////////////////////////////////////
		// Carrega na memoria de processamento os pontos indicados nos arquivos de chaves publicas
		///////////////////////////////////////////////////////////////////////////////////////////

		for(int iFKey = 0; iFKey < nOfBPBKFiles; iFKey ++ )
		{
			////////////////////////////////////////////////////////////////////////
			// Leiture de Arquivo
			////////////////////////////////////////////////////////////////////////
			//std::ifstream       file("PBK01_32kKEYS.txt");
			std::ifstream       file;
			//file.open("PBK01_32kKEYS.txt");
			file.open(fileNames[iFKey]);

			file.seekg(0,std::ios::end);
			std::streampos          length = file.tellg();
			file.seekg(0,std::ios::beg);

			std::vector<char>       buffer(length);
			file.read(&buffer[0],length);

			file.close();
			////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			// Devolver Dados para o Arquivo e Fechar
			////////////////////////////////////////////////////////////////////////
		  	std::ofstream pXfile;
		  	//pXfile.open ("PBK01_32kKEYS.txt");
			pXfile.open (fileNames[iFKey]);


		    	if (!pXfile) {
				std::cout << "Unable to open file";
				exit(1); // terminate with error
		    	}
			//https://www.techiedelight.com/convert-vector-chars-std-string/
			std::string s;
			//std::string s(buffer.begin(), buffer.end());
			//std::string s(&buffer[0], length);
			//std::string s(buffer.begin(), length);
			int iT = 0;
			do
			{
				s += buffer[iT];
				iT++;

			}while(buffer[iT] != ';');
			s += buffer[iT];

			pXfile << s;
			pXfile.close();

			////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////

			//std::string pxString [(slices/2) + 1];

			int j = 0;

			//Buscar a posição da primeira chave
			do
			{
				j++;

			}while(buffer[j] != '"');
			j++;

			std::cout << "Loading " << (slices/2) + 1 + 1 << " Keys group " << iFKey + 1 << " of 20 ... WAIT!!!" << std::endl;

			for(int i = 0; i <= (slices/2) + 1; i++)
			//for(int i = 0; i <= 20; i++)
			{
				std::string pxString;

				do
				{
					pxString += buffer[j];
					j++;

				}while(buffer[j] != '"');
				j = j + 4;

				//pointsPBKderiva[i][0] = EC.dectoint(Xpoints01[i]);
				pointsPBKderiva[i][iFKey] = EC.dectoint(pxString);
				//std::cout << "PxM: "<< std::endl << pointsPBKderiva[i][0] << std::endl;
				//Xpoints01[i] = "";//esvazia a variável
			}

			//pointsPBKderiva[(slices/2) + 1][iFKey] = 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////
		// Inciailizacao da Curva Elitipica com os pontos padroes
		// secp256k1
		///////////////////////////////////////////////////////////////////////////////////////////

		EC.initGvar();

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////
		// Ajuste do ponto de partida da busca por colisao a partir da chave privada raiz
		// e inicializacao da fucao de hash para derivacao das demais chaves de busca
		///////////////////////////////////////////////////////////////////////////////////////////

		pvtKeyTest[0] = EC.modp((pvtKeyTest[0]), EC.n_order);

		//std::cout << "key1: "<< std::endl << pvtKeyTest[0] << std::endl;

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		//Geracao de numberOfKeys de chaves derivadas a partir da chave privada inicial
		///////////////////////////////////////////////////////////////////////////////////////////


		if(pvtKeyTest[0] > (EC.n_order/2) )
		{
            //pvtKeyTest[0] = pvtKeyTest[0] - (EC.n_order/2);
            pvtKeyTest[0] = EC.n_order - pvtKeyTest[0];
		}

		//for(int i = 1; i < nKeys ; i++)
		for(int i = 1; i < numberOfKeys ; i++)
		{

		    //pvtKeyTest[i] = EC.modp(pvtKeyTest[0] + i*((EC.n_order/2)/(numberOfKeys)) , EC.n_order/2);
		    //pvtKeyTest[i] = pvtKeyTest[0] + i*((EC.n_order/2)/(numberOfKeys));
		    pvtKeyTest[i] = pvtKeyTest[i-1] + (EC.n_order/(2*(numberOfKeys)));

		 	if(pvtKeyTest[i] > (EC.n_order/2) )
		    {

		       pvtKeyTest[i] = pvtKeyTest[i] - (EC.n_order/2);
		    }

			//std::cout << "key1: "<< std::endl << pvtKeyTest[0] << std::endl;
			//std::cout << "key"<< i+1 << ": "<< std::endl << pvtKeyTest[i] << std::endl;
		}

		//pvtKeyTest[11] = EC.modp((pvtKeyTest[11]* 1*(EC.n_order/slices)), EC.n_order);

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Teste de conceito para Gerar uma Colisao com um ponto derivado de uma Chave Publica
		// da qual conhecemos a chave privada -> PBK1(PVTK1);
		// A colisao acontecerah com o ponto da curva cuja chave privada eh:
		// (pvtKeyTest[X] * n*(EC.n_order/slices))
		// onde n indica um dos pontos derivados da chave publica PBK1(PVTK1)
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		//A operacao (EC.n_order/slices) nao funcionou anteriormente nesta posicao deste metodo da classe
		// pois o fileNames[20] estava declarado com 20 posicoes e estava usando 21
		// por isso tivemos que usar a variavel testFactor
		// mas depois do problema ser verificado, o codigo rodou normal novamente
		////////////////////////////////////////////////////////////////////////////////////////////

//		cpp_int nSlices = 512;
//		pvtKeyTest[numberOfKeys - 1] = EC.modp((pvtKeyTest[numberOfKeys - 1]* nSlices*(EC.n_order/slices)), EC.n_order);
		//pvtKeyTest[numberOfKeys - 1 - 5] = EC.modp((pvtKeyTest[numberOfKeys - 1 - 5]* 5*(EC.n_order/slices)), EC.n_order);

		//pvtKeyTest[numberOfKeys - 1] = pvtKeyTest[numberOfKeys - 1] * 3 * testFactor;
		//pvtKeyTest[numberOfKeys - 1 - 5 ] = EC.modp((pvtKeyTest[numberOfKeys - 1 - 5]), EC.n_order);
//		pvtKeyTest[numberOfKeys - 1] = EC.modp((pvtKeyTest[numberOfKeys - 1]), EC.n_order);

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////
		//Teste de colisao a uma distancai X do ponto de partida
		//	tanto no sentido horario, quanto no sentido anti-horario
		///////////////////////////////////////////////////////////////////////////////////////////


//		pvtKeyTest[numberOfKeys - 1] = pvtKeyTest[numberOfKeys - 1] - 20;
		//pvtKeyTest[numberOfKeys - 1 - 5] = pvtKeyTest[numberOfKeys - 1 -5] + 21;
		//pvtKeyTest[numberOfKeys - 1] = pvtKeyTest[numberOfKeys - 1] + 1;

		//std::cout << "n*key - 20: "<< std::endl << pvtKeyTest[numberOfKeys - 1] << std::endl;

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Gera o ponto de partida de verificacao
		// a partir de uma chave privada conhecida pvtKeyTest
		///////////////////////////////////////////////////////////////////////////////////////////

		//std::cout << "Generating 12 Starting Points ... WAIT!!!" << std::endl;
		std::cout << "Generating "<< numberOfKeys << " Starting Points ... WAIT!!!" << std::endl;

		//keyOffSet = 10;
		//keyOffSet = 0;

		//g = EC.secp256k1 (pvtKeyTest, EC.Gx, EC.Gy); //busca no sentido horario
		//g = EC.secp256k1 (EC.n_order - pvtKeyTest, EC.Gx, EC.Gy); //busca no sentido anti-horario
		g = EC.secp256k1 (EC.n_order - 1, EC.Gx, EC.Gy); //busca no sentido antihorario
		gn[0][0] = g[0];
		gn[1][0] = g[1];

		EC.Gy_comp = gn[1][0];//Determinação do Ponto Py Complementar

		///////////////////////////////////////////////////////////////////////////////////////////
		//Inicializacao dos nKeys pontos de testes a paritir da chave privada e suas derivadas
		///////////////////////////////////////////////////////////////////////////////////////////

		//for(int i = 0; i < nKeys ; i++)
		for(int i = 0; i < numberOfKeys ; i++)
		{
			g = EC.secp256k1 (pvtKeyTest[i] + keyOffSet, EC.Gx, EC.Gy); //para busca no sentido horario
			gn[0][i] = g[0];
			gn[1][i] = g[1];

			//Se keyOffSet == 0 as duas chaves sao iguais
			//g = EC.secp256k1 (pvtKeyTest[i] - keyOffSet, EC.Gx, EC.Gy); //para busca no sentido anti-horario
			//EXTRA -1 at this point
			g = EC.secp256k1 (pvtKeyTest[i] - 1 - keyOffSet, EC.Gx, EC.Gy); //para busca no sentido anti-horario
			gn_anth[0][i] = g[0];
			gn_anth[1][i] = g[1];
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Testa os Pontos Px Derivados da Chave Publica Desconhecida contra
		// o Ponto de Partida (com chave privada conhecida) se seus incrementos
		// ateh ser atingido o numero de tentativas ntries
		///////////////////////////////////////////////////////////////////////////////////////////

		int ntries = 1000000000;
		//int ntries = 10;
		//int ntries = 30;

		cpp_int numberOfTests = 0;
		cpp_int numberOfTests1 = 0;

		//Numero de testes realizados a cada rodada = (numero de PVTKEYS)*(Numero de Arquivos)*(PBK em cada arquivo)*(Dois sentidos de busca)
		//numberOfTests1 = (cpp_int)(12*20*32769*2);
		numberOfTests1 = (cpp_int)(numberOfKeys*nOfBPBKFiles*(1 + (slices/2) + 1)*2);

		///////////////////////////////////////////////////////////////////////////////////////////
		//nonce da tentativa em relacao a chave inicial
		///////////////////////////////////////////////////////////////////////////////////////////

		for(int j = 0; j < ntries; j++)
		{
			///////////////////////////////////////////////////////////////////////////////////////////
			//Quantidade de arquivos de grupos de chaves
			///////////////////////////////////////////////////////////////////////////////////////////

			for(int k = 0; k < nOfBPBKFiles; k++)
			{

				///////////////////////////////////////////////////////////////////////////////////////////
				//Teste da Coordenada Px da chave dentro do Grupo k
				///////////////////////////////////////////////////////////////////////////////////////////
				//for(int i = 0; pointsPBKderiva[i][k] != 0; i++)
				for(int i = 0; pointsPBKderiva[i][k] != -1; i++)
				{

					///////////////////////////////////////////////////////////////////////////////////////////
					//Teste da Coordenada Px contra cada incremento das chaves iK
					///////////////////////////////////////////////////////////////////////////////////////////
					for(int iK = 0; iK < numberOfKeys ; iK++)
					//for(int iK = 0; iK < nKeys ; iK++)
					{

						///////////////////////////////////////////////////////////////////////////////////////////
						//Teste da Coordenada Px contra cada incremento das chaves iK no sentido horario
						///////////////////////////////////////////////////////////////////////////////////////////

						if(pointsPBKderiva[i][k] == gn[0][iK])
						{

							std::cout << std::endl << "//////////////////////////////////////////////////////////" << std::endl;
							std::cout << "//////////////////////////////////////////////////////////" << std::endl << std::endl;
							std::cout << "Collison ClockWise" << std::endl;
							std::cout << "Nonce: " << j << std::endl;
							std::cout << "Key OffSet: " << keyOffSet << std::endl;
							std::cout << "BPK Key Group: " << k + 1 << std::endl;
							std::cout << "EC X Point: " << gn[0][iK] << std::endl;
							std::cout << "BPK Derivative Slice Index: " << i << std::endl;
							std::cout << "Index PVT Key: " << iK + 1 << std::endl;
							std::cout << std::endl << "//////////////////////////////////////////////////////////" << std::endl;
							std::cout << "//////////////////////////////////////////////////////////" << std::endl << std::endl;
							return(0);
						}

						///////////////////////////////////////////////////////////////////////////////////////////
						///////////////////////////////////////////////////////////////////////////////////////////

						///////////////////////////////////////////////////////////////////////////////////////////
						//Teste da Coordenada Px contra cada incremento das chaves iK no sentido anti-horario
						///////////////////////////////////////////////////////////////////////////////////////////

						if(pointsPBKderiva[i][k] == gn_anth[0][iK])
						{
							std::cout << std::endl << "//////////////////////////////////////////////////////////" << std::endl;
							std::cout << "//////////////////////////////////////////////////////////" << std::endl << std::endl;
							std::cout << "Collison Counter ClockWise" << std::endl;
							std::cout << "Nonce: " << j << " (put extra -1) " << std::endl;
							std::cout << "Key OffSet: " << keyOffSet << std::endl;
							std::cout << "BPK Key Group: " << k + 1 << std::endl;
							std::cout << "EC X Point: " << gn_anth[0][iK] << std::endl;
							std::cout << "BPK Derivative Slice Index: " << i << std::endl;
							std::cout << "Index PVT Key: " << iK + 1 << std::endl;
							std::cout << std::endl << "//////////////////////////////////////////////////////////" << std::endl;
							std::cout << "//////////////////////////////////////////////////////////" << std::endl << std::endl;
							return(0);
						}
						///////////////////////////////////////////////////////////////////////////////////////////
						///////////////////////////////////////////////////////////////////////////////////////////
					}
					///////////////////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////////////////
				}
				///////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////
			}
			///////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////


			numberOfTests = numberOfTests1 * (j+1);
			//numberOfTests = ((j+1)*(12*13*32769*2));
			//std::cout << "Nonce: " << j << " Tests done: " << (cpp_int)((j+1)*12*13*32769*2) << std::endl;
			//std::cout << "Nonce: " << j << " Tests done: " << numberOfTests + numberOfTestsOffSet << std::endl;
			std::cout << "Nonce: " << j << " Tests done: " << numberOfTests << std::endl;
			//std::cout << "PX: " << std::endl << gn[0][0] << std::endl;
			//std::cout << "Py: " << std::endl << gn[1][0] << std::endl;

			///////////////////////////////////////////////////////////////////////////////////////////
			// Gera o proximo ponto de verificacao de cada uma das nKeys chaves
			///////////////////////////////////////////////////////////////////////////////////////////

			//for(int i = 0; i < nKeys ; i++)
			for(int i = 0; i < numberOfKeys ; i++)
			{
				//g = EC.addPPG (EC.Gx, EC.Gy, gn[0][i], gn[1][i]); //determina o proximo ponto a partir do anterior
				//g = EC.addp (EC.Gx, EC.Gy, gn[0][i], gn[1][i]); //determina o proximo ponto a partir do anterior
				g = EC.addpColl (EC.Gx, EC.Gy, gn[0][i], gn[1][i]); //determina o proximo ponto a partir do anterior
				gn[0][i] = g[0];
				gn[1][i] = g[1];

				//g = EC.addPPG (EC.Gx, EC.Gy_comp, gn_anth[0][i], gn_anth[1][i]); //determina o proximo ponto a partir do anterior no sentido anti-horario
                //g = EC.addp (EC.Gx, EC.Gy_comp, gn_anth[0][i], gn_anth[1][i]); //determina o proximo ponto a partir do anterior no sentido anti-horario
                g = EC.addpColl (EC.Gx, EC.Gy_comp, gn_anth[0][i], gn_anth[1][i]); //determina o proximo ponto a partir do anterior no sentido anti-horario
				gn_anth[0][i] = g[0];
				gn_anth[1][i] = g[1];
			}
			///////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		std::cout << "No Collison" << std::endl;
		std::cin.get();
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
};



////////////////////////////////////////////////////////////////////
//Thread Unica 2022
////////////////////////////////////////////////////////////////////

unsigned int singleTHRD(void* data)
{
	//threadSTC *tspec = (threadSTC*) data;
	pbkColl *tspec = (pbkColl*) data;
	//tspec->mT();
	//tspec->pbkDeriva();
	tspec->pbkSearch();
	return (0);
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


int main()
{
	Secp256k1 EC;  // Variavel necessaria para utilizar parametros da secp256k1
	EC.initGvar(); // Inicia a variaveis da curva eliptica
	//int  slices = 1024; //512

	//Formato SEC precisa ter 64 bytes cada coordenada ttem 32 bytes
	//Verificar o Formato de Minimo 32 bytes na carteira
	//Format SEC 64x2 + 2 = 130 characteres

	//Test key: 7265ED413FFAFBEC04DE374B2D1BE4161EB036E7C9FE1BD1E27EA5D2B05E1CFE
	//Test key - E: 7265ED413FFAFBEC04DE374B2D1BE4161EB036E7C9FE1BD1E27EA5D2B05E1CF0
	//PK TESTE    "049F3D6E36E39CBE628AF9027516D4969C8834F42D5E8E84EA944DFAA9D4F5B1CBD8AFC41DE82C6B1EA4F7EA682BE1DF3D64BFC51399489AC3BBD11DBE0146C51C"

	unsigned char PUBKEY[] = "0411db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3";
	unsigned char Px[] = "11db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5c";
	unsigned char Py[] = "b2e0eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3";

	pbkColl tspec;
	//tspec.testFactor = EC.n_order/slices;

	//std::cout << "Teste: " << EC.n_order/slices << std::endl;
	//std::cout << "Teste: " << tspec.testFactor << std::endl;

	unsigned char a[] = "038109007313a5807b2eccc082c8c3fbb988a973cacf1a7df9ce725c31b14776";

	std::cout << "Input an string up to 64 characteres (0-9 and A-F) (DO NOT INPUT MORE OR LESS THAN 64 CHAR): ";
	for (int j = 0; j < 64; j++) std::cin >> a[j];

	std::cout << "Key OffSet Number: "; std::cin >> tspec.keyOffSet;
	std::cout << "Number of Keys (1 a 144): "; std::cin >> tspec.numberOfKeys;

	//tspec.numberOfKeys = tspec.numberOfKeys % 13;
	tspec.numberOfKeys = tspec.numberOfKeys % 145;
	if(tspec.numberOfKeys == 0)
		tspec.numberOfKeys = 1;
	if(tspec.numberOfKeys < 0)
		tspec.numberOfKeys = 1;

	if(tspec.keyOffSet < 0)
		tspec.keyOffSet = 0;

	tspec.pvtKeyTest[0] = chBN(a);

	singleTHRD((void*)&tspec);

	std::cin.get();
//	std::cin.get();

	return (0);
}
