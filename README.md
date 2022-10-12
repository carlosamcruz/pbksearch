"# pbksearch" 
Para compilar o projeto é preciso colocar a biblioteca boost na pasta include do MinGW, para o uso de Big Integers

O código está configurado para ler 20 arquivos de chaves com nomes especificos, que são definidos manualmente no código e devem estar na mesma pasta do arquivo executável.
Se quiser complilar com mais ou menos chaves é só alterar as seguintes porções do código:

          const int nOfBPBKFiles = 20;
          
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

Os arquivos de chaves devem criados previamente com o projeot PBKDERIVA;

OBS: o numero de chaves por arquivos deve ser o mesmo em ambos os projeto PBKDERIVA e PBKSEARCH. A variável slices configura este número.

		int  slices = 2*2*2*2*4096; // 32k





