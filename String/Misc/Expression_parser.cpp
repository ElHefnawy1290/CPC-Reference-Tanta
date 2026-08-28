/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: EXPRESSION PARSER
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Evaluate expression", "Calculator", "Infix to Postfix".
 * - Classic Scenarios: You are given a raw string like "3 + 5 * (2 - 8)" and
 *   need the exact mathematical result.
 * - The Magic: Uses Recursive Descent to strictly enforce PEMDAS (Parentheses,
 *   Exponents, Multiplication/Division, Addition/Subtraction) in pure O(N) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       string expr = "10 + 2 * (6 - 3)";
 *       int pos = 0;
 *       long long result = parse_expr(expr, pos);
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Division by Zero: Currently, it performs integer division. If a problem
 *   tests division by zero, add a check inside `parse_term`.
 * - Modulo: If the expression must be evaluated modulo M, wrap all additions,
 *   subtractions, and multiplications in the respective functions with `% MOD`.
 */

using ll = long long;

ll parse_expr(const string &s, int &i);
ll parse_term(const string &s, int &i);
ll parse_factor(const string &s, int &i);
ll parse_number(const string &s, int &i);

void skip_spaces(const string &s, int &i)
{
    while (i < s.length() && isspace(s[i]))
        i++;
}

ll parse_expr(const string &s, int &i)
{
    ll res = parse_term(s, i);
    while (i < s.length())
    {
        skip_spaces(s, i);
        if (i < s.length() && (s[i] == '+' || s[i] == '-'))
        {
            char op = s[i++];
            ll next_term = parse_term(s, i);
            if (op == '+')
                res += next_term;
            else
                res -= next_term;
        }
        else
        {
            break;
        }
    }
    return res;
}

ll parse_term(const string &s, int &i)
{
    ll res = parse_factor(s, i);
    while (i < s.length())
    {
        skip_spaces(s, i);
        if (i < s.length() && (s[i] == '*' || s[i] == '/'))
        {
            char op = s[i++];
            ll next_factor = parse_factor(s, i);
            if (op == '*')
                res *= next_factor;
            else
                res /= next_factor;
        }
        else
        {
            break;
        }
    }
    return res;
}

ll parse_factor(const string &s, int &i)
{
    skip_spaces(s, i);
    if (i < s.length() && s[i] == '(')
    {
        i++;
        ll res = parse_expr(s, i);
        skip_spaces(s, i);
        if (i < s.length() && s[i] == ')')
            i++;
        return res;
    }
    return parse_number(s, i);
}

ll parse_number(const string &s, int &i)
{
    skip_spaces(s, i);
    ll res = 0;
    while (i < s.length() && isdigit(s[i]))
    {
        res = res * 10 + (s[i++] - '0');
    }
    return res;
}