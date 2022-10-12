
//https://www.boost.org/doc/libs/1_54_0/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/egs/factorials.html

#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/multiprecision/cpp_intNEW.hpp>
//#include "boost/cpp_int.hpp"

// atencao com #   include "include_preprocessed.hpp"
// #   include BOOST_PP_STRINGIZE(boost/mpl/aux_/preprocessed/AUX778076_PREPROCESSED_HEADER)

// DISSECAR A BIBLIOTECA cpp_int.hpp eh um processo muito complexo, 
// pois existe um numero gigantesco de interdependenicas dos arquivos
// da biblioteca BOOST melhor utlizar cpp_int.hpp ou construir uma biblioteca propria

using boost::multiprecision::cpp_int;

class Secp256k1
{
	public:

	// Constantes da curva eliptica
	cpp_int p, Gx, Gy, n_order, Gy_comp;
	cpp_int A, B;

	////////////////////////////////////////////////////////////////////////////////////////////////
	cpp_int r, t, aux, newr, newt; // variaveis para o calculo do inverso de um numero no campo p
	////////////////////////////////////////////////////////////////////////////////////////////////


	//Verificar se nao sera necessario estas variaveis static nas funcoes membro 
	cpp_int point[2], pointNULL[2] = {0, 0};

	//Transforma uma string hexadecimal em um inteiro
	cpp_int hextoint(std::string s)
	{
		cpp_int n;
		std::stringstream ss;
		ss << std::hex << s;
		ss >> n;		
		return (n);
	}

        cpp_int dectoint(std::string s)
	{
		cpp_int n;
		std::stringstream ss;
		ss << std::dec << s;
		ss >> n;		
		return (n);
	}

	//Retorna o resto da divisao de numeros naturais
	cpp_int modp(cpp_int n, cpp_int p1)
	{
		//if(n < 0) return (p1 + (n%p1));
		//return (n%p1);
		if((n = (n%p1)) < 0) return (p1 + n);
		return (n);



		//(n<0)?return(p1 + (n%p1)):return(n%p1)
		//return((n<0)?(p1 + (n%p1)):(n%p1));
	}

	int pointVerify(cpp_int Px, cpp_int Py)
	{

		if (modp((Py*Py), p) != modp((Px*Px*Px + A*Px + B), p))
			return (0);
		else
			return (1);
		
	}

	// Esta funcao inicializa os parametros da curva eliptica: 
	// Precisa ser chamada na funcao main antes de comecar a utilizar a curva eliptica
	// p = base prima da curva elipitica (p^n onde n = 1)
	// G = (Gx, Gy) ponto da curva eliptica de ordem n_order, onde P = n_order * G = (0, 0) ou o ponto no infinito
	// A e B Constantes que definem a cuva eliptica secp256k1 (y^2 = x^3 + Ax + B, onde 4A^2 + 27B^3 != 0)

	void initGvar()
	{
		//p = 115792089237316195423570985008687907853269984665640564039457584007908834671663;
		p = hextoint ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");

		//Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240;
		Gx = hextoint ("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");

		//Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424;
		Gy = hextoint ("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
		
		//n_order = 115792089237316195423570985008687907852837564279074904382605163141518161494337;
		n_order = hextoint ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

		A = 0;
		B = 7;
	}

	void initGvarNewPoint(cpp_int Px, cpp_int Py)
	{
		//p = 115792089237316195423570985008687907853269984665640564039457584007908834671663;
		p = hextoint ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");

		//Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240;
		//Gx = hextoint ("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
		Gx = Px;

		//Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424;
		//Gy = hextoint ("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
		Gy = Py;
		
		//n_order = 115792089237316195423570985008687907852837564279074904382605163141518161494337;
		n_order = hextoint ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

		A = 0;
		B = 7;
	}


	// Esta funcao devolve o inverso de um numero 1/a na base p 
	// E eh um dos gargalos do codigo, e deve ser executada da forma mais rapida o possivel
	// Esta versao aparentemente estah super-otimizada
	// foi implementada baseada no Algoritmo Euclidiano Estendido
	//	https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
	//		Computing multiplicative inverses in modular structures
	// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
	// Outras fontes:https://math.stackexchange.com/questions/25390/how-to-find-the-inverse-modulo-m
	// Outras relacoes: https://en.wikipedia.org/wiki/Pollard's_rho_algorithm
	// ALGO para Versao Especifica estritamente para P primo a < P e a > 0
	// Versao Super Compacta

	cpp_int inverse(cpp_int a, cpp_int p)
	{
		cpp_int newr, r, t, newt, quotient, aux;

		newr = modp(p, a);
		r = a;
		t = 1;
		newt = (0 - (p / a) * 1);

		while (newr != 0)
		{
			quotient = ( r / newr );
			aux = r;
			r = newr;
			newr = modp(aux, newr);
				
			aux = t;
			t = newt;
			newt = (aux - (quotient * newt));
		}

		if (t < 0) return (t + p);
		return (t);
	}

	cpp_int inverse2(cpp_int r, cpp_int p, cpp_int t, cpp_int aux, cpp_int newr, cpp_int newt)
	{
		newt = -(aux/r);
		while (newr = (aux % r))
		{
			aux = t;
			t = newt;
			newt = (aux - (r/newr)*newt);
			
			aux = r;
			r = newr;
		}

		if (t < 0) return (t + p);
		return (t);
	}

	cpp_int inverse3(cpp_int r, cpp_int p)
	{
		cpp_int t = 1;
		cpp_int aux = p;
		cpp_int newr = r;
		cpp_int newt = -(aux/r);
		while (newr = (aux % (r = newr)))
		{
			aux = t;

			newt = (aux - (r/newr)*(t = newt));
			
			aux = r;
		}

		if (t < 0) return (t + p);
		return (t);
	}



	// Esta funcao dobra o ponto P, executa 2*P(x,y)
	// Verificar uma forma de retorno mais adequada
	cpp_int *doublep (cpp_int x, cpp_int y)
	{
		cpp_int m; 
		//static cpp_int point[2];

		//m = modp (((3*(x*x) + A)*inverse ((2*y), p)), p);


		//cpp_int inverse3(cpp_int r, cpp_int p)
		{
			//cpp_int r, t, aux, newr, newt;
			t = 1;//cpp_int t = 1;
			aux = p;//cpp_int aux = p;
			newr = r = (2*y); //cpp_int newr = r;
			newt = -(aux/r);//cpp_int newt = -(aux/r);

			while (newr = (aux % (r = newr)))
			{
				aux = t;

				newt = (aux - (r/newr)*(t = newt));
			
				aux = r;
			}
			//if (t < 0) return (t + p);
			//return (t);
			if (t < 0) t = (t + p);
		}

		//m = modp (((3*(x*x) + A)*inverse2 ((2*y), p, 1, p, 0, 0)), p);
		m = modp (((3*(x*x) + A)*t), p);

		point[0] = modp ((((m*m)) - (2*x)), p);
		point[1] = modp ((-( (m * modp ((m*m), p)) - (m*(2*x)) + (y - (m*x)))), p);

		return (point);
	}

	// Esta funcao soma dois pontos da curva eliptica P1 + P2 = (x1, y1) + (x2, y2)
	// UTILIZADO PARA SOMAR PONTO A PONTO COM BTCCOLLIDER
	cpp_int *addPPG (cpp_int x1, cpp_int y1, cpp_int x2, cpp_int y2)
	{
		//static cpp_int point[2];//, pointNULL[2] = {0, 0};
		cpp_int m, d;
	
		//if (x1 == x2)
		//{
		//	if (y1 == y2) return (doublep (x1, y1));
	
		//	else return (pointNULL);
		//}
		
		//m = modp (((y2 - y1)*inverse (modp ((x2 - x1), p), p)), p);



		//cpp_int inverse3(cpp_int r, cpp_int p)
		{
			//cpp_int r, t, aux, newr, newt;
			t = 1;//cpp_int t = 1;
			aux = p;//cpp_int aux = p;
			newr = r = modp ((x2 - x1), p); //cpp_int newr = r;
			newt = -(aux/r);//cpp_int newt = -(aux/r);

			while (newr = (aux % (r = newr)))
			{
				aux = t;

				newt = (aux - (r/newr)*(t = newt));
			
				aux = r;
			}
			//if (t < 0) return (t + p);
			//return (t);
			if (t < 0) t = (t + p);
		}


		//m = modp (((y2 - y1)*inverse2 (modp ((x2 - x1), p), p, 1, p, 0, 0)), p);
		m = modp (((y2 - y1)*t), p);

		point[0] = modp((m*m - x1 - x2), p);		
		point[1] = modp((-(( m * modp((m*m), p)) - m*(x1 + x2) + (y1 - m*x1))), p);

		return point;
	}


	// Esta funcao soma dois pontos da curva eliptica P1 + P2 = (x1, y1) + (x2, y2)
	// A curva pode ter pontos com X = 0, mas nao com Y == 0
	cpp_int *addp (cpp_int x1, cpp_int y1, cpp_int x2, cpp_int y2)
	{
		//static cpp_int point[2], pointNULL[2] = {0, 0};
		cpp_int m, d;
	
		if (x1 == x2)
		{
			// Verificar a condicao de que y1 = y2 = p/2 
			// equivalente a y1 = y2 = 0 nao acontecerah
			if (y1 == y2) return (doublep (x1, y1));
	
			else return (pointNULL);
		}
		
		//m = modp (((y2 - y1)*inverse (modp ((x2 - x1), p), p)), p);


		//cpp_int inverse3(cpp_int r, cpp_int p)
		{
			//cpp_int r, t, aux, newr, newt;
			t = 1;//cpp_int t = 1;
			aux = p;//cpp_int aux = p;
			newr = r = modp ((x2 - x1), p); //cpp_int newr = r;
			newt = -(aux/r);//cpp_int newt = -(aux/r);

			while (newr = (aux % (r = newr)))
			{
				aux = t;

				newt = (aux - (r/newr)*(t = newt));
			
				aux = r;
			}
			//if (t < 0) return (t + p);
			//return (t);
			if (t < 0) t = (t + p);
		}

		//m = modp (((y2 - y1)*inverse2 (modp ((x2 - x1), p), p, 1, p, 0, 0     )), p);
		m = modp (((y2 - y1)*t), p);

		point[0] = modp((m*m - x1 - x2), p);		
		point[1] = modp((-(( m * modp((m*m), p)) - m*(x1 + x2) + (y1 - m*x1))), p);

		return point;
	}

	// Esta funcao soma dois pontos da curva eliptica P1 + P2 = (x1, y1) + (x2, y2)
	// A curva pode ter pontos com X = 0, mas nao com Y == 0
	cpp_int *addpColl (cpp_int x1, cpp_int y1, cpp_int x2, cpp_int y2)
	{
		//static cpp_int point[2], pointNULL[2] = {0, 0};
		cpp_int m, d;
	
		if (x1 == x2)
		{
			// Verificar a condicao de que y1 = y2 = p/2 
			// equivalente a y1 = y2 = 0 nao acontecerah

			//Jah verficado que eh necessario na busca por colisoes
			if (y1 == y2) return (doublep (x1, y1));
	
			else return (pointNULL);
			//return (pointNULL);
		}

		// verificado por Tonneli-Shanks x tal que x * x == 7 mod p: 0
		// Nao existe raiz de 7 neste capo p, logo se x == 0 entao y == 0
		// Se x == 0, so exite um y == 0 
		// y^2 = x^3 + Ax + B
		// A == 0 e B = 7 -> y^2 = x^3 + 7 (Verificar raiz de 7 e raiz curbica de -7 no campo primo P)
		// se y == 0 entao x =! 0; 
		//if (y2 == 0)
		if (x2 == 0)
		{
			// Verificar a condicao de que y1 = y2 = p/2 
			// equivalente a y1 = y2 = 0 nao acontecerah

			//Nao ocorrerah o caso y1 == y1 pois eh a volta completa na curva
			//if (y1 == y2) return (doublep (x1, y1));
	
			//else return (pointNULL);

			point[0] = x1;		
			point[1] = y1;

			return point;
		}
		
		//m = modp (((y2 - y1)*inverse (modp ((x2 - x1), p), p)), p);


		//cpp_int inverse3(cpp_int r, cpp_int p)
		{
			//cpp_int r, t, aux, newr, newt;
			t = 1;//cpp_int t = 1;
			aux = p;//cpp_int aux = p;
			newr = r = modp ((x2 - x1), p); //cpp_int newr = r;
			newt = -(aux/r);//cpp_int newt = -(aux/r);

			while (newr = (aux % (r = newr)))
			{
				aux = t;

				newt = (aux - (r/newr)*(t = newt));
			
				aux = r;
			}
			//if (t < 0) return (t + p);
			//return (t);
			if (t < 0) t = (t + p);
		}

		//m = modp (((y2 - y1)*inverse2 (modp ((x2 - x1), p), p, 1, p, 0, 0     )), p);
		m = modp (((y2 - y1)*t), p);

		point[0] = modp((m*m - x1 - x2), p);		
		point[1] = modp((-(( m * modp((m*m), p)) - m*(x1 + x2) + (y1 - m*x1))), p);

		return point;
	}

	// Funcao HASH, executa a operacao irreversivel de n.P = K
	// Onde P = (x, y) eh um ponto especifico da curva eliptica utilizada,
	// n eh a Chave Privada que deve ser mantida em sigilo,
	// e K eh o resultado da fucao HASH, Chave Publica.
	// Codigo eh generico para n >= 0 ou n <= 0

	cpp_int * secp256k1 (cpp_int n, cpp_int x, cpp_int y)
	{
		//static cpp_int point[2], pointNULL[2] = {0, 0};
		//cpp_int point[2], pointNULL[2] = {0, 0};
		cpp_int x3, y3, n3, bittest, *pointR;

		if (modp(n, n_order) == 0) return (pointNULL);
		if (n < 0)
		{
			n = -n;
			// como y estah em (mod p) -(yp) mod p = p - yp
			y = p - y;
		}

		// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX	
		// OTIMIZACAO: Divide o Ciclo de Hash em duas partes;
		// se n for da segunda metade da curva, aplicamos esta modificacao para economizar processamento
		// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		// Remover esta otimizacao caso utilize n > n_order sem tirar o mod n_order  antes da chamada da funcao

		if (n > (n_order - 1)/2)
		{
			n = n_order - n;
			y = p - y;
		}

		// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		n3 = 3*n;
		bittest = 1;
	
		while ((bittest) <= n3)
		{ 
			bittest = 2 * bittest;
		}

		// deslocamento de dois bits para a direita
		bittest = bittest / 4;

		x3 = x;
		y3 = y;

		// Executa a operacao n*P
		while (bittest > 1)
		{
			// O primeiramente dobramos o valor atual do ponto P = (x3, y3) atraves da operacao P + P = 2*P
			pointR = doublep (x3, y3);
			x3 = pointR[0];
			y3 = pointR[1];

			// Verificar se demvemos acrescentar o valor de G = (x, y) ao valor atual de P = (x3, y3)
			if ( ((n3 & bittest) != 0) && ((n & bittest) == 0) )
			{			
				pointR = addp (x3, y3, x, y);
				x3 = pointR[0];
				y3 = pointR[1];
			}

			// Verificar se demvemos subtrair o valor de G = (x, y) ao valor atual de P = (x3, y3)
			// Neste caso a subtracao de G = (x, y) eh equivalente a somar -G = (x, -y) = (x, p - y)
			if ( ((n3 & bittest) == 0) && ((n & bittest) != 0) )
			{ 
				pointR = addp (x3, y3, x, (p - y));
				x3 = pointR[0];
				y3 = pointR[1];
			}

			bittest = bittest / 2;
		}

		point[0] = x3;
		point[1] = y3;

		return point;
	}
};
