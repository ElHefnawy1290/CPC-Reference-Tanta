

// Positive Numbers Only: This specific BigInt template design 
// (using vector<int> blocks) does not natively handle negative numbers.
//  If you need to subtract A - B, you must guarantee that A >= B,
//   or write wrapper logic to handle the negative sign separately.

void solve() {
    // 1. Initialization
    // You can initialize from extremely large strings, ints, or long longs
    BigInt a = Integer("1000000000000000000000000000000"); 
    BigInt b = Integer(999999);
    BigInt c = Integer(123456789012345LL);

    // 2. Standard Arithmetic (BigInt with BigInt)
    BigInt sum = a + b;
    BigInt diff = a - b; // Note: Ensure 'a >= b' before subtracting! (very important)
    BigInt prod = a * b;

    // 3. Arithmetic with standard integers (BigInt with int)
    BigInt increased = a + 500;
    BigInt divided = a / 3; 
    int remainder = a % 7; 

    // 4. Comparisons
    if (a > b) {
        cout << "A is strictly strictly greater than B" << endl;
    }
    if (c <= Integer("123456789012345")) {
        cout << "Comparisons work perfectly!" << endl;
    }

    // 5. Input / Output
    cout << "--- Results ---" << endl;
    cout << "A: " << a << endl;
    cout << "B: " << b << endl;
    cout << "A + B: " << sum << endl;
    cout << "A - B: " << diff << endl;
    cout << "A * B: " << prod << endl;
    cout << "A / 3: " << divided << endl;
    cout << "A % 7: " << remainder << endl;
    
    // Taking BigInt input from the console
    // BigInt user_input;
    // cin >> user_input;
    // cout << "You entered: " << user_input << endl;
}