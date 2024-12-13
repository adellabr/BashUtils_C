SUCCESS_COUNT=0
FAIL_COUNT=0
COUNT=0
DIFF=""

declare -a flags=(
"-i"
"-v"
"-c"
"-l"
"-n"
"-h"
"-s" 
"-o"
)

declare -a files=(
#"s21_greptest1.txt"
"text_file.txt s21_greptest2.txt"
)

file1="text_file.txt"
file2="test2.txt"
file3="doesntexist.txt"

declare -a commline=(
"OPT FILE"
)

function testing()
{
    str=$(echo $@ | sed "s/OPT/$options/")
    str=$(echo $str | sed -e "s/FILE/$file/g")
    ./s21_grep $str > s21_grep_testing.log
    grep $str > system_grep_testing.log
    DIFF="$(diff -s s21_grep_testing.log system_grep_testing.log)"
    (( COUNT++ ))
    if [ "$DIFF" == "Files s21_grep_testing.log and system_grep_testing.log are identical" ]
    then
        (( SUCCESS_COUNT++ ))
        echo "grep $str SUCCESS $COUNTER"
    else
        (( FAIL_COUNT++ ))
        echo "grep $str FAIL $COUNTER"
    fi
    rm s21_grep_testing.log system_grep_testing.log
}

testing_f()
{
    let "COUNT+=1"
grep "-f" $file2 $flag1 $flag2 $flag3 $file1 > result_grep.txt
./s21_grep "-f" $file2 $flag1 $flag2 $flag3 $file1 > result_s21_grep.txt
DIFF="$(diff -s result_grep.txt result_s21_grep.txt)"
if [[ "$DIFF" == "Files result_grep.txt and result_s21_grep.txt are identical" ]]
        then
            let "SUCCESS_COUNT+=1"
            echo "$COUNT test: "-f" $file2 $flag1 $flag2 $flag3 $file1 $file3 SUCCESS"
        else
            let "FAIL_COUNT+=1"
            echo "$COUNT test: "-f" $file2 $flag1 $flag2 $flag3 $file1 $file3 FAIL"
        fi
        rm result_grep.txt result_s21_grep.txt
}

for opt1 in "${flags[@]}"
do
    for opt2 in "${flags[@]}"
    do
        for opt3 in "${flags[@]}"
        do
            for opt4 in "${flags[@]}"
            do
                if [ $opt1 != $opt2 ] && [ $opt1 != $opt3 ] \
                && [ $opt1 != $opt4 ] && [ $opt2 != $opt3 ] \
                && [ $opt2 != $opt4 ] && [ $opt3 != $opt4 ]
                then
                    for file in "${files[@]}"
                    do
                        options="-e little -e escape $opt1 $opt2 $opt3 $opt4"
                        testing $commline
                    done
                fi
            done
        done
    done
done

for flag1 in "${flags[@]}"
do
    for flag2 in "${flags[@]}"
    do
        for flag3 in "${flags[@]}"
        do
            if [ $flag1 != $flag2 ] && [ $flag1 != $flag3 ] && [ $flag2 != $flag3 ]
            then
                testing_f "-f" $file2 $flag1 $flag2 $flag3 $file1 $file3
            fi
        done
    done
done

echo "FAIL: $FAIL_COUNT"
echo "SUCCESS: $SUCCESS_COUNT"
echo "ALL: $COUNT"