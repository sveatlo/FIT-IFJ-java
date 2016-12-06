#!/bin/sh

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

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

    echo "Testing of [${YELLOW} $test_name ${RESET}] has been launched"
    echo "---"

    for file in $(find "$test_dir" -mindepth 1 -maxdepth 1 -name "*.test"); do
        echo "Currently testing [${YELLOW} $file ${RESET}]. With following results: "
        # 2&>1 takes STDERR into equation
        "$intepret" "$file" > /dev/null 2>&1
        returned=$?
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
        echo ""
    done


    totalamount=$((passed + failed))
    echo "---"
    echo "Ended testing of [${YELLOW} $test_name ${RESET}]. We've recieved the followign results:"
    echo "Amount of ${GREEN}successful ${RESET}tests run: ${GREEN} $passed ${RESET}out of $totalamount"
    echo "Amount of ${RED}unsuccessful ${RESET}tests run: ${RED} $failed ${RESET} out of $totalamount"
    echo "*****************************************************"
}

# parse arguments
while getopts :h FLAG; do
  case $FLAG in
    h) #show help
        echo "Usage: ./test INTERPRET_BINARY"
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


run_tests "$interpret" "LEXICAL ANALYZATOR" "./testy/lexika"
run_tests "$interpret" "SEMANTIC ANALYZATOR" "./testy/semantika"
run_tests "$interpret" "SYNTAX ANALYZATOR" "./testy/syntax"
run_tests "$interpret" "OVERALL ANALYZATOR" "./testy/interpret"
