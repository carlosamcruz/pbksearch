//https://gist.github.com/LaurentMazare/6745649
// tonelli_shanks.cpp

//utilizamos a pow_mod3
cpp_int pow_mod(cpp_int base, cpp_int exponent, cpp_int modulus)
{
	cpp_int result = 1;
	base = base % modulus;
	while (exponent > 0)
	{
		if (exponent % 2 == 1)
			result = (result * base)% modulus;
		exponent = exponent >> 1;
		base = (base * base) % modulus;
	}
	return result;
}

//ORIGINAL DO CODIGO
//https://gist.github.com/LaurentMazare/6745649
cpp_int pow_mod1(cpp_int x, cpp_int n, cpp_int p) 
{


  	if (n == 0) return 1;
  	if (n & 1)
    	return (pow_mod(x, n-1, p) * x) % p;
  	x = pow_mod(x, n/2, p);

  	return (x * x) % p;
}

/// https://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method
cpp_int pow_mod2(cpp_int b, cpp_int e, cpp_int n) {
    if (n == 1) return 0;
    cpp_int result = 1;
    b = b % n;
    while (e > 0) {
        if (e % 2 == 1) {
            result = (result * b) % n;
        }
        e >>= 1;
        b = (b*b) % n;
    }
    return result;
}

// https://www.geeksforgeeks.org/find-square-root-modulo-p-set-2-shanks-tonelli-algorithm/
//  utility function to find pow(base, exponent) % modulus
cpp_int pow_mod3(cpp_int base, cpp_int exponent, cpp_int modulus)
{
    cpp_int result = 1;
    base = base % modulus;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
           result = (result * base)% modulus;
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}


/* Takes as input an odd prime p and n < p and returns r
 * such that r * r = n [mod p]. */
cpp_int tonelli_shanks(cpp_int n, cpp_int p) 
{
	cpp_int s = 0;
	cpp_int q = p - 1;
  	while ((q & 1) == 0) 
	{ 
		q /= 2; 
		++s; 
	}
  	if (s == 1) 
	{
	    cpp_int r = pow_mod(n, (p+1)/4, p);
	    if ((r * r) % p == n) return r;
    	    return 0;
  	}
  	// Find the first quadratic non-residue z by brute-force search
  	cpp_int z = 1;
	while (pow_mod(++z, (p-1)/2, p) != p - 1); // ORIGINAL
	//while (1) if (pow_mod(++z, (p-1)/2, p) == p - 1) break;  //ALTERNATIVA

  	cpp_int c = pow_mod(z, q, p);
  	cpp_int r = pow_mod(n, (q+1)/2, p);
  	cpp_int t = pow_mod(n, q, p);
  	cpp_int m = s;
  	while (t != 1) 
	{
	    cpp_int tt = t;
	    cpp_int i = 0;
	    while (tt != 1) 
		{
	      tt = (tt * tt) % p;
	      ++i;
	      if (i == m) return 0;
	    }
    	cpp_int b = pow_mod(c, pow_mod(2, m-i-1, p-1), p);
    	cpp_int b2 = (b * b) % p;
    	r = (r * b) % p;
    	t = (t * b2) % p;
    	c = b2;
    	m = i;
  	}
  	if ((r * r) % p == n) return r;
  	return 0;
}

class RootsMP
{
	public:

	// Constantes da curva eliptica
	cpp_int p, Gx, Gy, n_order;
	cpp_int A, B;

	cpp_int sqrINV3;
	cpp_int inv2;
	cpp_int inv3;
	cpp_int inv9;
	cpp_int e1;
	cpp_int e2;
	cpp_int pexp;
	cpp_int pexp2;

	void initGvar(cpp_int p1)
	{
		//p = 115792089237316195423570985008687907853269984665640564039457584007908834671663;
		//p = hextoint ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
		p = p1;
		sqrINV3 = tonelli_shanks((p-3), p);
		inv2 = inverse3(2, p);
		inv3 = inverse3(3, p);
		inv9 = inverse3(9, p);
		e1 = modp((- 1 + sqrINV3)*inv2, p);
		e2 = modp((- 1 - sqrINV3)*inv2, p);
		pexp = modp((p - 1)*inv3 , p);
		pexp2 = modp((p + 2)*inv9 , p);

	}

	cpp_int modp(cpp_int n, cpp_int p1)
	{
		if(n < 0) return (p1 + (n%p1));
		return (n%p1);
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


	//https://gist.github.com/LaurentMazare/6745649
	// tonelli_shanks.cpp

	//utilizamos a pow_mod3
	cpp_int pow_mod(cpp_int base, cpp_int exponent, cpp_int modulus)
	{
		cpp_int result = 1;
		base = base % modulus;
		while (exponent > 0)
		{
			if (exponent % 2 == 1)
				result = (result * base)% modulus;
			exponent = exponent >> 1;
			base = (base * base) % modulus;
		}
		return result;
	}

	/* Takes as input an odd prime p and n < p and returns r
	 * such that r * r = n [mod p]. */
	cpp_int tonelli_shanks(cpp_int n, cpp_int p) 
	{
		cpp_int s = 0;
		cpp_int q = p - 1;
	  	while ((q & 1) == 0) 
		{ 
			q /= 2; 
			++s; 
		}
	  	if (s == 1) 
		{
		    cpp_int r = pow_mod(n, (p+1)/4, p);
		    if ((r * r) % p == n) return r;
	    	    return 0;
	  	}
	  	// Find the first quadratic non-residue z by brute-force search
	  	cpp_int z = 1;
		while (pow_mod(++z, (p-1)/2, p) != p - 1); // ORIGINAL
		//while (1) if (pow_mod(++z, (p-1)/2, p) == p - 1) break;  //ALTERNATIVA

	  	cpp_int c = pow_mod(z, q, p);
	  	cpp_int r = pow_mod(n, (q+1)/2, p);
	  	cpp_int t = pow_mod(n, q, p);
	  	cpp_int m = s;
	  	while (t != 1) 
		{
		    cpp_int tt = t;
		    cpp_int i = 0;
		    while (tt != 1) 
			{
		      tt = (tt * tt) % p;
		      ++i;
		      if (i == m) return 0;
		    }
	    	cpp_int b = pow_mod(c, pow_mod(2, m-i-1, p-1), p);
	    	cpp_int b2 = (b * b) % p;
	    	r = (r * b) % p;
	    	t = (t * b2) % p;
	    	c = b2;
	    	m = i;
	  	}
	  	if ((r * r) % p == n) return r;
	  	return 0;
	}

	void cbrmp(cpp_int a, cpp_int p, cpp_int cbra[]) 
	{
		//cpp_int symap = modp(pow_mod(a, pexp, p), p);
		if(modp(pow_mod(a, pexp, p), p) == 1)
		{
  			cbra[0] = modp(pow_mod(a, pexp2, p), p);
			cbra[1] = modp(cbra[0] * e1, p);
			cbra[2] = modp(cbra[0] * e2, p);        
			return;
		}

		cbra[0] = 0;
		cbra[1] = 0;
		cbra[2] = 0;        
		return;
	}
};




