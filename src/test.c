#include "test.h"
#include "cegex.h"

int main() {
    Tests({
        Describe("Kleene Star (*)", {
            Test("a*b*c* matches 'aaabbbccc'", {
                -1 != cegex_match("a*b*c*", "aaabbbccc").index;
            });
            Test("a*b*c* matches 'abc'", {
                -1 != cegex_match("a*b*c*", "abc").index;
            });
            Test("a*b*c* matches ''", {
                -1 != cegex_match("a*b*c*", "").index;
            });
        });
        Describe("Plus (+)", {
            Test("a+b+c+ matches 'aaabbbccc'", {
                -1 != cegex_match("a+b+c+", "aaabbbccc").index;
            });
            Test("a+b+c+ matches 'abc'", {
                -1 != cegex_match("a+b+c+", "abc").index;
            });
            Test("a+b+c+ does not match ''", {
                -1 == cegex_match("a+b+c+", "").index;
            });
        });
        Describe("Dot (.)", {
            Test("a.c matches 'abc'", {
                -1 != cegex_match("a.c", "abc").index;
            });
            Test("a.c matches 'aXc'", {
                -1 != cegex_match("a.c", "aXc").index;
            });
            Test("a.c does not match 'ac'", {
                -1 == cegex_match("a.c", "ac").index;
            });
        });
        Describe("Question Mark (?)", {
            Test("colou?r matches 'color'", {
                -1 != cegex_match("colou?r", "color").index;
            });
            Test("colou?r matches 'colour'", {
                -1 != cegex_match("colou?r", "colour").index;
            });
            Test("colou?r does not match 'colouur'", {
                -1 == cegex_match("colou?r", "colouur").index;
            });
        });
        Describe("Pharentesis (())", {
            Test("(abc) matches 'abc'", {
                -1 != cegex_match("(abc)", "abc").index;
            });
            Test("(abc) does not match '123abc456'", {
                -1 == cegex_match("(abc)", "123abc456").index;
            });
            Test("(abc) matches 'abc123'", {
                -1 != cegex_match("(abc)", "abc123").index;
            });
        });
        Describe("Alternation (|)", {
            Test("a|b matches 'a'", {
                -1 != cegex_match("a|b", "a").index;
            });
            Test("a|b matches 'b'", {
                -1 != cegex_match("a|b", "b").index;
            });
            Test("a|b does not match 'c'", {
                -1 == cegex_match("a|b", "c").index;
            });
            Test("(a|b)c matches 'ac'", {
                -1 != cegex_match("(a|b)c", "ac").index;
            });
            Test("(a|b)c matches 'bc'", {
                -1 != cegex_match("(a|b)c", "bc").index;
            });
            Test("(a|b)c does not match 'cc'", {
                -1 == cegex_match("(a|b)c", "cc").index;
            });
        });
        Describe("Complex Regex Patterns", {
            Test("Pattern with wildcard . and zero or more *", {
                -1 != cegex_match("a.*b", "axxxb").index;
            });

            Test("Pattern with one or more + and zero or one ?", {
                -1 != cegex_match("a+b?", "aa").index;
            });

            Test("Pattern with alternation | and group ()", {
                -1 != cegex_match("(a|b)c", "ac").index;
            });

            Test("Pattern with escaped characters \\+ and \\*", {
                -1 != cegex_match("a\\+b\\*c", "a+b*c").index;
            });

            Test("Complex pattern with multiple special characters", {
                -1 != cegex_match("a(b|c)*d.e?f", "abccdf").index;
            });

            Test("Pattern with nested groups and alternation", {
                -1 != cegex_match("((a|b)c)d", "acd").index;
            });

            Test("Pattern with start ^ and end $ anchors", {
                -1 != cegex_match("^a.*b$", "axxxb").index;
            });

            Test("Pattern with escaped parentheses \\( and \\)", {
                -1 != cegex_match("a\\(b\\)c", "a(b)c").index;
            });

            Test("Combination of all special characters", {
                -1 != cegex_match("(a|b)+c*d.e?f", "abbcdef").index;
            });
         });
    });
    return failed_test;
}
