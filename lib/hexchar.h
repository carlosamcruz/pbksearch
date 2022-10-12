
char hexpp (char a)
{
	switch (a)
	{
		case '0': return ('1');
		case '1': return ('2');
		case '2': return ('3');
		case '3': return ('4');
		case '4': return ('5');
		case '5': return ('6');
		case '6': return ('7');
		case '7': return ('8');
		case '8': return ('9');
		case '9': return ('A');
		case 'a': return ('B');
		case 'A': return ('B');
		case 'b': return ('C');
		case 'B': return ('C');
		case 'c': return ('D');
		case 'C': return ('D');
		case 'd': return ('E');
		case 'D': return ('E');
		case 'e': return ('F');
		case 'E': return ('F');
		case 'f': return ('0');
		case 'F': return ('0');
     }
}

int hexint (char a)
{
	switch (a)
	{
		case '0': return (0);
		case '1': return (1);
		case '2': return (2);
		case '3': return (3);
		case '4': return (4);
		case '5': return (5);
		case '6': return (6);
		case '7': return (7);
		case '8': return (8);
		case '9': return (9);
		case 'a': return (10);
		case 'A': return (10);
		case 'b': return (11);
		case 'B': return (11);
		case 'c': return (12);
		case 'C': return (12);
		case 'd': return (13);
		case 'D': return (13);
		case 'e': return (14);
		case 'E': return (14);
		case 'f': return (15);
		case 'F': return (15);
     }
}
