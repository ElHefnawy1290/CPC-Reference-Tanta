* 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: FAST SUM OF PROPER DIVISORS
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Sum of divisors", "Proper divisors", "Aliquot sum", "N up to 10^16".
 * - Classic Scenarios: You are given a massive number N (up to 10^16) and you need to find 
 *   the sum of all its divisors EXCEPT the number itself.

vector<int> Primes;
	
	void Sieve(int num)
	{
	    vector<bool> is_prime(num + 1, true);
	    is_prime[0], is_prime[1] = false;
	    for (int i = 2; i * i <= num; i++)
	    {
	        if (is_prime[i])
	        {
	            for (int j = i * i; j <= num; j += i)
	            is_prime[j] = false;
	        }
	    }
	    Primes.push_back(2);
	    for (int i = 3; i <= num; i += 2)
	    {
	        if (is_prime[i])
	        Primes.push_back(i);
	    }
	
	}
	
	void Hassan ()
	{
	//one integer between 1 and 1e16 inclusive
	    ll n; cin >> n;
	    ll ans = 1;
	    ll copy = n;
	    for (auto p : Primes)
	    {
	        if (1LL * p * p > n) 
	        break;
	        if (n % p == 0)
	        {
	            ll sum = 1, pow = 1;
	            while (n % p == 0)
	            {
	                pow *= p;
	                sum += pow;
	                n /= p;
	            }
	            ans *= sum;
	        }
	    }
	    if (n > 1) ans *= (n + 1);
	    cout << ans - copy << '\n'; /// note 
	}
	
