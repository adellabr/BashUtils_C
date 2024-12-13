SUCCESS_COUNT=0
FAIL_COUNT=0
COUNT=0
IDENT=""

declare -a flags=("-b" "-e" "-n" "-s" "-t" "-v")

declare -a files=(
"s21_cattest1.txt"
"s21_cattest2.txt"
"s21_cattest3.txt"
)

function testing()
{
        let "COUNT+=1"
        cat $options $file > result_cat.txt
        ./s21_cat $options $file > result_s21_cat.txt
        IDENT="$(diff -s result_cat.txt result_s21_cat.txt)"
        if [[ "$IDENT" == "Files result_cat.txt and result_s21_cat.txt are identical" ]]
        then
            let "SUCCESS_COUNT+=1"
            echo "$COUNT test:   $options $file SUCCESS"
        else
            let "FAIL_COUNT+=1"
            echo "$COUNT test:   $options $file FAIL"
        fi
        rm result_cat.txt result_s21_cat.txt
}

for flag1 in "${flags[@]}"
do
    for flag2 in "${flags[@]}"
    do
        for flag3 in "${flags[@]}"
        do
            for flag4 in "${flags[@]}"
            do
                for file in "${files[@]}"
                do
                    if [ $flag1 != $flag2 ] && [ $flag1 != $flag3 ] && [ $flag1 != $flag4 ] \
                    && [ $flag2 != $flag3 ] && [ $flag2 != $flag4 ] && [ $flag3 != $flag4 ]
                    then
                    options="$flag1 $flag2 $flag3 $flag4"
                    testing $options $file
                    fi
                done
            done
        done
    done
done

echo "TOTAL: $COUNT"
echo "FAIL: $FAIL_COUNT"
echo "SUCCESS: $SUCCESS_COUNT"