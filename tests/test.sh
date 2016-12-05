#!/bin/sh

#=================================================
#                            TEST_SUITE FOR IFJ
#-------------------------------------------------
# Name: test.sh
# Version: 2.1
#=================================================
# We create two a complete test_suite in shellscript that will thoroughly
# test majority of our functions and their implementation

# First we need to explain some basics
# Our test suite [test.sh] will execute various test files
# This way we can create invididual files for each and every test



# Let's make the tests colorful, so they are easy to look through
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

# We set a counter for succesfull and failed tests
passed=0
failed=0
totalamount=0
returned=0
expected=0


# Inform the user about the start of testing (visual only)
echo "================= START OF TESTING ================="
echo "*****************************************************"
# We can begin with testing
echo "Testing of [${YELLOW} LEXICAL ANALYZATOR ${RESET}] has been launched"
echo "---"
for file in ./testy/lexika/*.test; do
    returned=0;
    expected=0;
    echo "Currently testing [${YELLOW} $file ${RESET}]. With following results: "
    # 2&>1 takes STDERR into equation
    ./ifj16 "$file" > /dev/null 2>&1
    returned=$?
    returned=${returned#0}

    file=${file%.*}

    expected=$(cat "$file".output)
    expected=${expected#0}

    if [ "$returned" -eq "$expected" ]; then
        passed=$((passed + 1))
        echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${GREEN}successful${RESET}"
        echo "Returned value [${GREEN} $returned ${RESET}] which matches the expected result"
    else
        failed=$((failed + 1))
        echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${RED}unsuccesful${RESET}."
        echo "Returned value [${RED} $returned ${RESET}] which does not match the expected result: $expected"
    fi
    echo ""
done

    totalamount=$((passed + failed))
    echo "---"
    echo "Ended testing of [${YELLOW} LEXICAL ANALYZATOR ${RESET}]. We've recieved the followign results:"
    echo "Amount of ${GREEN}successful ${RESET}tests run: ${GREEN} $passed ${RESET}out of $totalamount"
    echo "Amount of ${RED}unsuccessful ${RESET}tests run: ${RED} $failed ${RESET} out of $totalamount"
    echo "*****************************************************"


# We set a counter for succesfull and failed tests
passed=0
failed=0
totalamount=0
returned=0
expected=0


echo "*****************************************************"
# We can begin with testing
echo "Testing of [${YELLOW} SEMANTIC ANALYZATOR ${RESET}] has been launched"
echo "---"
for file in ./testy/semantika/*.test; do
echo "Currently testing [${YELLOW} $file ${RESET}]. With following results: "
    # 2&>1 takes STDERR into equation
    returned=$(./ifj16 "$file")
    file=${file%.*}
    expected=$(cat "$file".output)

    if [ "$returned" -eq "$expected" ]; then
        passed=$((passed + 1))
        echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${GREEN}successful${RESET}"
        echo "Returned value [${GREEN} $returned ${RESET}] which matches the expected result"
    else
        failed=$((failed + 1))
        echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${RED}unsuccesful${RESET}."
        echo "Returned value [${RED} $returned ${RESET}] which does not match the expected result: $expected"
    fi
done

    totalamount=$((passed + failed))
    echo "---"
    echo "Ended testing of [${YELLOW} SEMANTICAL ANALYZATOR ${RESET}]. We've recieved the followign results:"
    echo "Amount of ${GREEN}successful ${RESET}tests run: ${GREEN} $passed ${RESET}out of $totalamount"
    echo "Amount of ${RED}unsuccessful ${RESET}tests run: ${RED} $failed ${RESET} out of $totalamount"
    echo "*****************************************************"


# We set a counter for succesfull and failed tests
passed=0
failed=0
totalamount=0
returned=0
expected=0


echo "*****************************************************"
# We can begin with testing
echo "Testing of [${YELLOW} SYNTAX ANALYZATOR ${RESET}] has been launched"
echo "---"
for file in ./testy/syntax/*.test; do
echo "Currently testing [${YELLOW} $file ${RESET}]. With following results: "
    # 2&>1 takes STDERR into equation
    returned=$(./ifj16 "$file")
    file=${file%.*}
    expected=$(cat "$file".output)

    if [ "$returned" -eq "$expected" ]; then
        passed=$((passed + 1))
        echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${GREEN}successful${RESET}"
        echo "Returned value [${GREEN} $returned ${RESET}] which matches the expected result"
    else
        failed=$((failed + 1))
        echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${RED}unsuccesful${RESET}."
        echo "Returned value [${RED} $returned ${RESET}] which does not match the expected result: $expected"
    fi
done

    totalamount=$((passed + failed))
    echo "---"
    echo "Ended testing of [${YELLOW} SYNTAX ANALYZATOR ${RESET}]. We've recieved the followign results:"
    echo "Amount of ${GREEN}successful ${RESET}tests run: ${GREEN} $passed ${RESET}out of $totalamount"
    echo "Amount of ${RED}unsuccessful ${RESET}tests run: ${RED} $failed ${RESET} out of $totalamount"
    echo "*****************************************************"


    # We set a counter for succesfull and failed tests
    passed=0
    failed=0
    totalamount=0
    returned=0
    expected=0

    echo "*****************************************************"
    # We can begin with testing
    echo "Testing of [${YELLOW} OVERALL FUNCTIONALITY ${RESET}] has been launched"
    echo "---"
    for file in ./testy/interpret/*.test; do
    echo "Currently testing [${YELLOW} $file ${RESET}]. With following results: "
        # 2&>1 takes STDERR into equation
        returned=$(./ifj16 "$file")
        file=${file%.*}
        expected=$(cat "$file".output)

        if [ "$returned" -eq "$expected" ]; then
            passed=$((passed + 1))
            echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${GREEN}successful${RESET}"
            echo "Returned value [${GREEN} $returned ${RESET}] which matches the expected result"
        else
            failed=$((failed + 1))
            echo "Finished testing [${YELLOW} $file ${RESET}] with returned value of [${YELLOW} $returned ${RESET}]. Test seems to be ${RED}unsuccesful${RESET}."
            echo "Returned value [${RED} $returned ${RESET}] which does not match the expected result: $expected"
        fi
    done

        totalamount=$((passed + failed))
        echo "---"
        echo "Ended testing of [${YELLOW} OVERALL FUNCTIONALITY ${RESET}]. We've recieved the followign results:"
        echo "Amount of ${GREEN}successful ${RESET}tests run: ${GREEN} $passed ${RESET}out of $totalamount"
        echo "Amount of ${RED}unsuccessful ${RESET}tests run: ${RED} $failed ${RESET} out of $totalamount"
        echo "*****************************************************"
        echo "================= END OF TESTING ================="
