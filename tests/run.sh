#!/bin/sh

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

tmp_dir="/tmp/ifj_test"

# main test function
run_tests() {
    passed=0
    failed=0
    totalamount=0
    returned=0
    expected=0
    intepret="$1"
    test_name="$2"
    test_dir="$3"
    test_tmp="$tmp_dir/$(basename $test_dir)"

    mkdir -p $test_tmp


    echo "Testing of [${YELLOW} $test_name ${RESET}] has been launched"
    echo "============================================================"

    for file in $(find "$test_dir" -mindepth 1 -maxdepth 1 -name "*.test" | sort -n); do
        echo -n "Running [${YELLOW} $file ${RESET}] "
        # 2&>1 takes STDERR into equation
        file=${file%.*}

        if [ -e "$file.input" ]; then
            cat "$file.input" | "$intepret" "$file.test" > "$test_tmp/$(basename $file).test.out" 2>&1
        else
            "$intepret" "$file.test" > "$test_tmp/$(basename $file).test.out" 2>&1
        fi
        returned=$?
        expected=$(cat "$file".code)

        diiff=""
        if [ -e "$file.output" ]; then
            diiff=$(diff "$file.output" "$test_tmp/$(basename $file).test.out")
        fi

        if [ "$returned" -eq "$expected" ] && [ "$diiff" = "" ]; then
            passed=$((passed + 1))
            echo "(${GREEN}successful${RESET})"
        else
            failed=$((failed + 1))
            echo "(${RED}failed${RESET})"
            if ! [ "$returned" -eq "$expected" ]; then
                echo "    EXPECTED: ${GREEN} $expected ${RESET}"
                echo "    RETURNED: ${RED} $returned ${RESET}"
            fi
            diff "$file.output" "$test_tmp/$(basename $file).test.out"
        fi
    done


    totalamount=$((passed + failed))
    echo "============================================================"
    echo "Ended testing of [${YELLOW} $test_name ${RESET}]. We've recieved the followign results:"
    echo "Amount of ${GREEN}successful ${RESET}tests run: ${GREEN} $passed ${RESET}out of $totalamount"
    echo "Amount of ${RED}unsuccessful ${RESET}tests run: ${RED} $failed ${RESET} out of $totalamount"
    echo "*****************************************************"
    echo ""
    echo ""
    echo ""
}

testgroup="all"

# parse arguments
while getopts :t:h FLAG; do
    case $FLAG in
        t) # run only one group of tests
            testgroup=$OPTARG
            ;;
        h) # show help
            echo "Usage: ./test [-t lex|syntax|sem|overall|complex|all] INTERPRET_BINARY"
            exit 0
            ;;
        \?) # unrecognized option - show help
            echo "Option -${BOLD}$OPTARG${NORM} not allowed. Use -h to see usage." 1>&2
            exit 1
            ;;
    esac
done

shift $((OPTIND-1))
interpret="$1"
if ! [[ -f "$interpret" &&  -x "$interpret" ]]; then
    echo "Interpret binary not an executable file" 1>&2
    exit 1
fi

interpret=$(readlink -f "$interpret")


# cd to the directory of test script
cd $(dirname $(readlink -f "$0"))

case $testgroup in
    "all")
        run_tests "$interpret" "LEXICAL ANALYZATOR" "./testy/lexika"
        run_tests "$interpret" "SEMANTIC ANALYZATOR" "./testy/semantika"
        run_tests "$interpret" "SYNTAX ANALYZATOR" "./testy/syntax"
        run_tests "$interpret" "OVERALL ANALYZATOR" "./testy/interpret"
        run_tests "$interpret" "COMPLEX TESTS" "./testy/complex"
        ;;
    "lex")
        run_tests "$interpret" "LEXICAL ANALYZATOR" "./testy/lexika"
        ;;
    "semantic")
        run_tests "$interpret" "SEMANTIC ANALYZATOR" "./testy/semantika"
        ;;
    "syntax")
        run_tests "$interpret" "SYNTAX ANALYZATOR" "./testy/syntax"
        ;;
    "overall")
        run_tests "$interpret" "OVERALL ANALYZATOR" "./testy/interpret"
        ;;
    "complex")
        run_tests "$interpret" "COMPLEX TESTS" "./testy/complex"
        ;;
    \?)
        echo "Test group not recognized" 1>&2
        ;;

esac

rm -rf "$tmp_dir"
