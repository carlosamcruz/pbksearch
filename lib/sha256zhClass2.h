class SHA256
{
	public:
	//#include <stdio.h>
	//#include <math.h>
	// Shift right
	#define SHR(x,n) ((x & 0xFFFFFFFF) >> n)
	// Rotate right
	#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))
	// S0 and S1
	#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^ SHR(x, 3))
	#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10))
	// Σ0 and Σ1
	#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
	#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))

	// Maj and Ch
	#define MAJ(x,y,z) ((x & y) | (z & (x | y)))
	#define CH(x,y,z) (z ^ (x & (y ^ z)))

	//Importante definir antes nao ocupar tempo de processamento
	//SHA256 constants K t
	unsigned long Kzh[64] =
	{
		0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
		0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
		0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
		0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
		0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
		0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
		0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
		0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
		0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
		0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
		0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
		0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
		0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
		0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
		0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
		0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
	};

	unsigned long H0[8] = 
	{ 
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	// SHA256 geral 
	// Para strings maiores precisamos definir H0 externamente
	// Importante, verificar que o MOD32 eh automatico para long
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	int sha256G (unsigned long W[], unsigned long Hi0[], unsigned long Hi[])
	{
		unsigned long A, B, C, D, E, F, G, H, temp1, temp2;
		int t;

		// Calculating W t values after round 16
/*		for(t=16;t<64;t++)
		{
			W[t] = S1(W[t -	2]) + W[t -	7] + S0(W[t - 15]) + W[t - 16];
		}
*/

		////////////////////////////////////////////////////////////////////////////////////////////
		// Expansao para evitar o for
		////////////////////////////////////////////////////////////////////////////////////////////

//			W[16] = S1(W[16 -	2]) + W[16 -	7] + S0(W[16 - 15]) + W[16 - 16];
			W[16] = S1(W[14]) + W[9] + S0(W[1]) + W[0];
			W[17] = S1(W[15]) + W[10] + S0(W[2]) + W[1];
			W[18] = S1(W[16]) + W[11] + S0(W[3]) + W[2];
			W[19] = S1(W[17]) + W[12] + S0(W[4]) + W[3];
			W[20] = S1(W[18]) + W[13] + S0(W[5]) + W[4];
			W[21] = S1(W[19]) + W[14] + S0(W[6]) + W[5];
			W[22] = S1(W[20]) + W[15] + S0(W[7]) + W[6];
			W[23] = S1(W[21]) + W[16] + S0(W[8]) + W[7];
			W[24] = S1(W[22]) + W[17] + S0(W[9]) + W[8];
			W[25] = S1(W[23]) + W[18] + S0(W[10]) + W[9];

			W[26] = S1(W[24]) + W[19] + S0(W[11]) + W[10];
			W[27] = S1(W[25]) + W[20] + S0(W[12]) + W[11];
			W[28] = S1(W[26]) + W[21] + S0(W[13]) + W[12];
			W[29] = S1(W[27]) + W[22] + S0(W[14]) + W[13];
			W[30] = S1(W[28]) + W[23] + S0(W[15]) + W[14];
			W[31] = S1(W[29]) + W[24] + S0(W[16]) + W[15];
			W[32] = S1(W[30]) + W[25] + S0(W[17]) + W[16];
			W[33] = S1(W[31]) + W[26] + S0(W[18]) + W[17];
			W[34] = S1(W[32]) + W[27] + S0(W[19]) + W[18];
			W[35] = S1(W[33]) + W[28] + S0(W[20]) + W[19];
			W[36] = S1(W[34]) + W[29] + S0(W[21]) + W[20];

			W[37] = S1(W[35]) + W[30] + S0(W[22]) + W[21];
			W[38] = S1(W[36]) + W[31] + S0(W[23]) + W[22];
			W[39] = S1(W[37]) + W[32] + S0(W[24]) + W[23];
			W[40] = S1(W[38]) + W[33] + S0(W[25]) + W[24];
			W[41] = S1(W[39]) + W[34] + S0(W[26]) + W[25];
			W[42] = S1(W[40]) + W[35] + S0(W[27]) + W[26];
			W[43] = S1(W[41]) + W[36] + S0(W[28]) + W[27];
			W[44] = S1(W[42]) + W[37] + S0(W[29]) + W[28];
			W[45] = S1(W[43]) + W[38] + S0(W[30]) + W[29];
			W[46] = S1(W[44]) + W[39] + S0(W[31]) + W[30];

			W[47] = S1(W[45]) + W[40] + S0(W[32]) + W[31];
			W[48] = S1(W[46]) + W[41] + S0(W[33]) + W[32];
			W[49] = S1(W[47]) + W[42] + S0(W[34]) + W[33];
			W[50] = S1(W[48]) + W[43] + S0(W[35]) + W[34];
			W[51] = S1(W[49]) + W[44] + S0(W[36]) + W[35];
			W[52] = S1(W[50]) + W[45] + S0(W[37]) + W[36];
			W[53] = S1(W[51]) + W[46] + S0(W[38]) + W[37];
			W[54] = S1(W[52]) + W[47] + S0(W[39]) + W[38];
			W[55] = S1(W[53]) + W[48] + S0(W[40]) + W[39];
			W[56] = S1(W[54]) + W[49] + S0(W[41]) + W[40];

			W[57] = S1(W[55]) + W[50] + S0(W[42]) + W[41];
			W[58] = S1(W[56]) + W[51] + S0(W[43]) + W[42];
			W[59] = S1(W[57]) + W[52] + S0(W[44]) + W[43];
			W[60] = S1(W[58]) + W[53] + S0(W[45]) + W[44];
			W[61] = S1(W[59]) + W[54] + S0(W[46]) + W[45];
			W[62] = S1(W[60]) + W[55] + S0(W[47]) + W[46];
			W[63] = S1(W[61]) + W[56] + S0(W[48]) + W[47];

		////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////
		// Message compression
		// Para t = 0
		// Faz a primeira iteracao antes do loop 
		// para evitar inicializacao de A, B, C, D, E, F, G, e H	
		///////////////////////////////////////////////////////////
		temp1 = Hi0[7] + S3(Hi0[4]) + CH(Hi0[4],Hi0[5],Hi0[6]) + Kzh[0] + W[0];
		temp2 = S2(Hi0[0]) + MAJ(Hi0[0],Hi0[1],Hi0[2]);
		H = Hi0[6]; 
		G = Hi0[5]; 
		F = Hi0[4]; 
		E = Hi0[3] + temp1;
		D = Hi0[2]; 
		C = Hi0[1]; 
		B = Hi0[0];
		A = temp1 + temp2;

		// Message compression
		// Para t = 1 a t = 62
/*		for(t=1;t<63;t++)
		{
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[t] + W[t];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		}
*/
		//////////////////////////////////////////////////////////////////////////////
		// Expansao para evitar o for
		//////////////////////////////////////////////////////////////////////////////


		//para t=1

			temp1 = H + S3(E) + CH(E,F,G) + Kzh[1] + W[1];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=2
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[2] + W[2];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=3
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[3] + W[3];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=4
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[4] + W[4];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=5
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[5] + W[5];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=6
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[6] + W[6];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=7
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[7] + W[7];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=8
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[8] + W[8];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=9
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[9] + W[9];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=10
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[10] + W[10];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		
	// 11>t>20

		//para t=11
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[11] + W[11];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=12
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[12] + W[12];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=13
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[13] + W[13];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=14
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[14] + W[14];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=15
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[15] + W[15];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=16
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[16] + W[16];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=17
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[17] + W[17];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=18
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[18] + W[18];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=19
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[19] + W[19];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=20
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[20] + W[20];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

	//    21>t>30

		//para t=21
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[21] + W[21];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=22
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[22] + W[22];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=23
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[23] + W[23];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=24
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[24] + W[24];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=25
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[25] + W[25];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=26
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[26] + W[26];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=27
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[27] + W[27];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=28
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[28] + W[28];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=29
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[29] + W[29];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=30
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[30] + W[30];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

	//   31>t>40

		//para t=31
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[31] + W[31];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=32
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[32] + W[32];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=33
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[33] + W[33];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=34
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[34] + W[34];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=35
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[35] + W[35];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=36
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[36] + W[36];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=37
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[37] + W[37];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=38
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[38] + W[38];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=39
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[39] + W[39];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=40
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[40] + W[40];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

	//   41>t>50

		//para t=41
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[41] + W[41];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=42
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[42] + W[42];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=43
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[43] + W[43];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=44
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[44] + W[44];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=45
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[45] + W[45];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=46
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[46] + W[46];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=47
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[47] + W[47];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=48
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[48] + W[48];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=49
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[49] + W[49];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=50
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[50] + W[50];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

	//   51>t>60

		//para t=51
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[51] + W[51];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=52
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[52] + W[52];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=53
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[53] + W[53];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=54
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[54] + W[54];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=55
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[55] + W[55];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=56
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[56] + W[56];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=57
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[57] + W[57];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=58
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[58] + W[58];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=59
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[59] + W[59];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//para t=60
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[60] + W[60];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

	//  61>t>62

		//para t=61
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[61] + W[61];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;
		//para t=62
			temp1 = H + S3(E) + CH(E,F,G) + Kzh[62] + W[62];
			temp2 = S2(A) + MAJ(A,B,C);
			H = G; 
			G = F; 
			F = E; 
			E = D + temp1;
			D = C; 
			C = B; B = A;
			A = temp1 + temp2;

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////
		// Para t = 63
		// Faz a ultima iteracao depois do loop 
		// para evitar uma operacao extra
		///////////////////////////////////////////////////////////

		temp1 = H + S3(E) + CH(E,F,G) + Kzh[63] + W[63];
		temp2 = S2(A) + MAJ(A,B,C);
		Hi[0] = (temp1 + temp2) + Hi0[0]; 
		Hi[1] = A + Hi0[1]; 
		Hi[2] = B + Hi0[2]; 
		Hi[3] = C + Hi0[3];
		Hi[4] = (D + temp1) + Hi0[4]; 
		Hi[5] = E + Hi0[5]; 
		Hi[6] = F + Hi0[6]; 
		Hi[7] = G + Hi0[7];	 

		return (0);
	}
};

